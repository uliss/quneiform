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

static cf::RecognizeOptions getRecogOptions(Page * page) {
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
            res.addReadRect(cf::Rect(r.left(), r.top(), r.width(), r.height()));
        }
    }

    if(!page->imageURL().isSimple())
        res.setImageNumber(page->imageURL().imageNumber());

    QSettings settings;
    cf::Config::instance().setDebug(settings.value(KEY_PRINT_CUNEIFORM_DEBUG, false).toBool());
    cf::Config::instance().setDebugLevel(100);
    res.setDebugCleanupDelayed(settings.value(KEY_MODULE_CIMAGE_DEBUG, false).toBool());

    return res;
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
}

PageRecognizer::~PageRecognizer()
{}

void PageRecognizer::abort() {
    QMutexLocker l(&lock_);
    abort_ = true;
    emit aborted();
}

void PageRecognizer::exportPageText() {
    Q_CHECK_PTR(page_);
    Q_ASSERT(counter_);

    page_->updateTextDocument();
    page_->setRecognized();

    counter_->add(10);
}

void PageRecognizer::handleRecognitionProgress(unsigned char perc) {
    emit percentsDone(perc);
    QCoreApplication::processEvents();
}

void PageRecognizer::handleRecognitionState(int state) {
    using namespace cf;

    switch(state) {
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
        qDebug() << "[Info] analyzed";
        break;
    case RecognitionState::FAILED:
        emit failed("Unknown error");
        break;
    default:
        qDebug() << "[Error] Unknown recognition state: " << state;
    }

    QCoreApplication::processEvents();
}

void PageRecognizer::loadImage()
{
    Q_ASSERT(counter_);
    Q_ASSERT(recog_state_);
    // update counter and state
    counter_->add(9);
}

QString PageRecognizer::pagePath() const {
    return page_ ? page_->imagePath() : QString();
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
        loadImage();

        QImage image;
        if(!ImageCache::load(page_->imageURL(), &image))
            throw std::runtime_error("[PageRecognizer::recognize] can't load image");
        image = image.copy(page_->readBoundingRect()).transformed(QTransform().rotate(page_->angle()));

        FormatOptions fopts = getFormatOptions(page_);
        RecognizeOptions ropts = getRecogOptions(page_);

        PercentCounter recog_counter(counter_.data());
        recog_counter.setContribution(80);

        ResolutionHistogramCallbackSetter hist_clbk(boost::bind(&PageRecognizer::saveResolutionHeightHistogram, this, _1),
                                                    boost::bind(&PageRecognizer::saveResolutionWidthHistogram, this, _1));

        RecognitionPtr server = makeRecognitionServer(workerType(), &recog_counter, recog_state_.data());

        CEDPagePtr cedptr = server->recognizeImage(QtImageLoader::fromQImage(image), BinarizeOptions(), ropts, fopts);

        if(!cedptr)
            return false;

        page_->setCEDPage(cedptr);

        exportPageText();
    }
    catch(std::exception& e) {
        page_->setFlag(Page::RECOGNITION_FAILED);
        page_->unsetFlag(Page::RECOGNIZED);
        emit failed(e.what());
        qWarning() << Q_FUNC_INFO << e.what() << "for" << page_->imageURL();
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

void PageRecognizer::setConfigOptions() {
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
