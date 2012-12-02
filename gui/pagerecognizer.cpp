/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <QDebug>
#include <QMutexLocker>
#include <QCoreApplication>
#include <QSettings>
#include <sstream>
#include <boost/bind.hpp>

#include "pagerecognizer.h"
#include "page.h"
#include "quneiform_debug.h"
#include "recognitioninternaldata.h"
#include "rdib/qtimageloader.h"
#include "resolutionhistogramcallbacksetter.h"
#include "puma/recognitionfactory.h"
#include "common/imageurl.h"
#include "common/formatoptions.h"
#include "common/recognizeoptions.h"
#include "common/cifconfig.h"
#include "common/percentcounter.h"
#include "common/recognitionstate.h"
#include "export/rectexporter.h"
#include "export/qtextdocumentexporter.h"
#include "settingskeys.h"
#include "cfutils.h"
#include "imagecache.h"

static language_t languageToType(const Language& lang) {
    if(lang.isValid()) {
        return static_cast<language_t>(lang.code());
    }
    else {
        qDebug() << "[Warning] Language is not valid, using English instead";
        return ::LANGUAGE_ENGLISH;
    }
}

static cf::FormatOptions getFormatOptions(Page * page) {
    Q_CHECK_PTR(page);

    cf::FormatOptions res;
    page->formatSettings().exportTo(res);
    res.setLanguage(languageToType(page->language()));

    return res;
}

static cf::RecognizeOptions getRecogOptions(Page * page)
{
    Q_CHECK_PTR(page);

    cf::RecognizeOptions res;
    RecognitionSettings s = page->recognitionSettings();
    res.setFax(s.fax());
    res.setDotMatrix(s.dotMatrix());
    res.setOneColumn(s.oneColumn());
    res.setPictureSearch(s.picturesSearch());
    res.setSpellCorrection(s.spelling());
    res.setLanguage(languageToType(page->language()));

    if(page->hasReadAreas()) {
        foreach(QRect r, page->readAreas()) {
            r = page->mapToBackend(r);
            res.addReadRect(toRect(r));
        }
    }

    if(!page->imageURL().isSimple())
        res.setImageNumber(page->imageURL().imageNumber());

    res.setManualLayout(page->manualLayout());

    QSettings settings;
    cf::Config::instance().setDebug(settings.value(KEY_PRINT_CUNEIFORM_DEBUG, false).toBool());
    cf::Config::instance().setDebugLevel(100);
    res.setDebugCleanupDelayed(settings.value(KEY_MODULE_CIMAGE_DEBUG, false).toBool());

    return res;
}

static cf::RecognitionPtr makeRecognitionServer(PageRecognizer::WorkerType workerType,
                                                cf::PercentCounter * counter,
                                                cf::RecognitionState * state)
{
    using namespace cf;
    RecognitionServerType type;

    switch(workerType) {
    case PageRecognizer::LOCAL:
        type = SERVER_LOCAL;
        qDebug() << Q_FUNC_INFO << "local recognition server created.";
        break;
    case PageRecognizer::PROCESS:
        type = SERVER_PROCESS;
        qDebug() << Q_FUNC_INFO << "process recognition server created.";
        break;
    default:
        type = SERVER_DUMMY;
        break;
    }

    RecognitionPtr server = RecognitionFactory::instance().make(type);

    if(!server) {
        qCritical() << Q_FUNC_INFO << "recognition server creation failed.";
        throw std::runtime_error("[makeRecognitionServer] recognition server creation failed");
    }

    server->setCounter(counter);
    server->setStateTracker(state);
    return server;
}

PageRecognizer::PageRecognizer(QObject * parent, WorkerType type)
    : QObject(parent),
    page_(NULL),
    abort_(false),
    worker_type_(type)
{
    counter_.reset(new cf::PercentCounter);
    counter_->setCallback(this, &PageRecognizer::handleRecognitionProgress);

    recog_state_.reset(new cf::RecognitionState);
    recog_state_->setCallback(this, &PageRecognizer::handleRecognitionState);

    connect(this, SIGNAL(failed(QString)), SLOT(handleFail(QString)));
}

PageRecognizer::~PageRecognizer()
{}

void PageRecognizer::abort()
{
    QMutexLocker l(&lock_);
    abort_ = true;
    emit aborted();
}

bool PageRecognizer::analyze()
{
    using namespace cf;

    if(!page_) {
        emit failed("[PageRecognizer::analyze] NULL page pointer given");
        return false;
    }

    try {
        counter_->reset();
        recog_state_->reset();

        setConfigOptions();

        if(!loadImage()) {
            emit failed("can't load image");
            return false;
        }

        FormatOptions fopts = getFormatOptions(page_);
        RecognizeOptions ropts = getRecogOptions(page_);

        RecognitionPtr server = makeRecognitionServer(workerType(), NULL, NULL);
        server->setRecognizeOptions(ropts);
        server->setFormatOptions(fopts);

        if(!server->open(QtImageLoader::fromQImage(image_))) {
            emit failed("image open failed");
            return false;
        }

        recog_state_->set(RecognitionState::OPENED);

        if(!server->binarize()) {
            emit failed("image binarization failed");
            return false;
        }

        recog_state_->set(RecognitionState::BINARIZED);

        if(!server->analyze()) {
            emit failed("image analyze failed");
            return false;
        }

        recog_state_->set(RecognitionState::ANALYZED);

        bool rc = importPageLayout(server.get());

        if(!rc) {
            emit failed("page analyze failed");
            return false;
        }
    }
    catch(std::exception& e) {
        emit failed(e.what());
        qWarning() << Q_FUNC_INFO << e.what() << "for" << page_->imageURL();
        return false;
    }

    emit percentsDone(100);
    emit done();
    page_->setAnalyzed(true);
    QCoreApplication::processEvents();

    return true;
}

void PageRecognizer::exportPageText() {
    Q_CHECK_PTR(page_);
    Q_ASSERT(counter_);

    page_->updateTextDocument();
    page_->setRecognized();

    counter_->add(10);
}

bool PageRecognizer::exportPageLayout(cf::AbstractRecognitionServer * server)
{
    if(!page_)
        return false;

    foreach(Block block, page_->blocks(BLOCK_LAYOUT_TEXT)) {
        QRect r = page_->mapToBackend(block.rect());
        server->addTextBlock(toRect(r));
    }

    foreach(Block block, page_->blocks(BLOCK_LAYOUT_IMAGE)) {
        QRect r = page_->mapToBackend(block.rect());
        server->addImageBlock(toRect(r));
    }

    return true;
}

bool PageRecognizer::importPageLayout(const cf::AbstractRecognitionServer * server)
{
    if(!page_)
        return false;

    page_->clearBlocks(BLOCK_LAYOUT_TEXT);
    foreach(cf::LayoutBlock b, server->textBlocks()) {
        page_->appendTextBlock(toQRect(b.rect()));
    }

    page_->clearBlocks(BLOCK_LAYOUT_IMAGE);
    foreach(cf::LayoutBlock b, server->imageBlocks()) {
        page_->appendImageBlock(toQRect(b.rect()));
    }

    return true;
}

void PageRecognizer::handleRecognitionProgress(unsigned char perc) {
    emit percentsDone(perc);
    QCoreApplication::processEvents();
}

void PageRecognizer::handleRecognitionState(int state) {
    using namespace cf;

    switch(state) {
    case RecognitionState::BINARIZED:
        break;
    case RecognitionState::OPENED:
        emit opened();
        break;
    case RecognitionState::RECOGNIZED:
        emit recognized();
        break;
    case RecognitionState::FORMATTED:
        emit formatted();
        break;
    case RecognitionState::ANALYZED:
        break;
    case RecognitionState::FAILED:
        emit failed("Unknown error");
        break;
    default:
        qDebug() << "[Error] Unknown recognition state: " << state;
    }

    QCoreApplication::processEvents();
}

bool PageRecognizer::loadImage()
{
    Q_CHECK_PTR(page_);

    if(!ImageCache::load(page_->imageURL(), &image_))
        return false;
    image_ = image_.copy(page_->readBoundingRect()).transformed(QTransform().rotate(page_->angle()));

    counter_->add(9);
    return true;
}

QString PageRecognizer::pagePath() const {
    return page_ ? page_->imagePath() : QString();
}

bool PageRecognizer::recognize() {
    using namespace cf;

    if(!page_) {
        emit failed("[PageRecognizer::recognize] NULL page pointer given");
        return false;
    }

    try {
        counter_->reset();
        recog_state_->reset();

        setConfigOptions();

        if(!loadImage()) {
            emit failed("can't load image");
            return false;
        }

        PercentCounter recog_counter(counter_.data());
        recog_counter.setContribution(80);

        ResolutionHistogramCallbackSetter hist_clbk(boost::bind(&PageRecognizer::saveResolutionHeightHistogram, this, _1),
                                                    boost::bind(&PageRecognizer::saveResolutionWidthHistogram, this, _1));

        RecognitionPtr server = makeRecognitionServer(workerType(), &recog_counter, recog_state_.data());


        server->setBinarizeOptions(BinarizeOptions());
        server->setFormatOptions(getFormatOptions(page_));
        RecognizeOptions ropts = getRecogOptions(page_);
        server->setRecognizeOptions(ropts);

        if(!server->open(QtImageLoader::fromQImage(image_))) {
            emit failed("can't open image");
            return false;
        }

        if(!server->binarize()) {
            emit failed("binarization failed");
            return false;
        }

        if(ropts.manualLayout()) {
             if(!server->manualLayout()) {
                 emit failed("manual layout failed");
                 return false;
             }

             exportPageLayout(server.get());
        }
        else {
            if(!server->analyze()) {
                emit failed("auto layout failed");
                return false;
            }
        }

        if(!server->recognize()) {
            emit failed("recognition failed");
            return false;
        }

        importPageLayout(server.get());

        CEDPagePtr cedptr = server->format();

        if(!cedptr) {
            emit failed("formatting failed");
            return false;
        }

        page_->setCEDPage(cedptr);

        exportPageText();
    }
    catch(std::exception& e) {
        page_->setFlag(Page::RECOGNITION_FAILED);
        page_->unsetFlag(Page::RECOGNIZED);
        emit failed(e.what());
        return false;
    }

    emit done();
    QCoreApplication::processEvents();

    {
        QMutexLocker l(&lock_);
        if(abort_)  {
            abort_ = false;
            return false;
        }
        else
            return true;
    }
}

void PageRecognizer::setConfigOptions()
{
    Q_ASSERT(counter_);

    QSettings s;
    cf::Config::instance().setDebug(s.value(KEY_PRINT_CUNEIFORM_DEBUG, false).toBool());

    counter_->add(1);
}

void PageRecognizer::setPage(Page * p) {
    page_ = p;
}

void PageRecognizer::setWorkerType(PageRecognizer::WorkerType t)
{
    worker_type_ = t;
}

PageRecognizer::WorkerType PageRecognizer::workerType() const
{
    switch(worker_type_) {
    case LOCAL:
    case PROCESS:
        return worker_type_;
    case RUNTIME:
    default: {
#ifdef NDEBUG
        bool default_process = false;
#else
        bool default_process = true;
#endif
        return QSettings().value(KEY_PROCESS_RECOGNITION, default_process).toBool() ? PROCESS : LOCAL;
    }
    }

    return worker_type_;
}

void PageRecognizer::saveResolutionHeightHistogram(const std::vector<int>& hist)
{
    assert(page_);

    RecognitionInternal::instance().setComponentHeightHistogram(page_->imagePath(), hist);
}

void PageRecognizer::saveResolutionWidthHistogram(const std::vector<int>& hist)
{
    assert(page_);

    RecognitionInternal::instance().setComponentWidthHistogram(page_->imagePath(), hist);
}


void PageRecognizer::handleFail(const QString& msg)
{
    qWarning() << "Recognition error:     " << msg;

    if(page_) {
        page_->setFlag(Page::RECOGNITION_FAILED);
        page_->unsetFlag(Page::RECOGNIZED);
    }
}
