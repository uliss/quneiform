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
            QRect rr = page->mapToPage(r);
            res.addReadRect(cf::Rect(rr.left(), rr.top(), rr.width(), rr.height()));
        }
    }

    switch(page->angle()) {
    case 0:
        break;
    case 90:
        res.setTurnAngle(cf::RecognizeOptions::ANGLE_90);
        break;
    case 180:
        res.setTurnAngle(cf::RecognizeOptions::ANGLE_180);
        break;
    case 270:
        res.setTurnAngle(cf::RecognizeOptions::ANGLE_270);
        break;
    default:
        qWarning() << Q_FUNC_INFO << "unsupported angle: " << page->angle();
        break;
    }

    if(!page->imageURL().isSimple())
        res.setImageNumber(page->imageURL().imageNumber());

    QSettings settings;
    settings.beginGroup("debug");
    cf::Config::instance().setDebug(settings.value("printCuneiformDebug", false).toBool());
    cf::Config::instance().setDebugLevel(100);
    settings.beginGroup("modules");
    res.setDebugCleanupDelayed(settings.value("cimage", false).toBool());

    return res;
}

PageRecognizer::PageRecognizer(QObject * parent)
    : QObject(parent),
    page_(NULL),
    counter_(NULL),
    recog_state_(NULL),
    abort_(false),
    worker_type_(LOCAL)
{
    counter_ = new cf::PercentCounter;
    counter_->setCallback(this, &PageRecognizer::handleRecognitionProgress);

    recog_state_ = new cf::RecognitionState;
    recog_state_->setCallback(this, &PageRecognizer::handleRecognitionState);

    QSettings settings;
#ifndef NDEBUG
    bool process = settings.value("debug/processRecognition", false).toBool();
#else
    bool process = settings.value("debug/processRecognition", true).toBool();
#endif

    if(process)
        worker_type_ = PROCESS;
    else
        worker_type_ = LOCAL;
}

PageRecognizer::~PageRecognizer()
{
    delete counter_;
    delete recog_state_;
}

void PageRecognizer::abort() {
    QMutexLocker l(&lock_);
    abort_ = true;
    emit aborted();
}

void PageRecognizer::exportPageText() {
    Q_CHECK_PTR(page_);
    Q_CHECK_PTR(counter_);

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
    case RecognitionState::LOADED:
        emit loaded();
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
    Q_CHECK_PTR(counter_);
    Q_CHECK_PTR(recog_state_);
    // update counter and state
    counter_->add(9);
    recog_state_->set(cf::RecognitionState::LOADED);
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

static cf::ImageURL makeImageURL(Page * p)
{
    cf::ImageURL res(p->imagePath().toUtf8().constData());
    res.setImageNumber(p->imageURL().imageNumber());
    return res;
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

        FormatOptions fopts = getFormatOptions(page_);
        RecognizeOptions ropts = getRecogOptions(page_);

        PercentCounter recog_counter(counter_);
        recog_counter.setContribution(80);

        ResolutionHistogramCallbackSetter hist_clbk(boost::bind(&PageRecognizer::saveResolutionHeightHistogram, this, _1),
                                                    boost::bind(&PageRecognizer::saveResolutionWidthHistogram, this, _1));

        RecognitionPtr server = makeRecognitionServer(worker_type_, &recog_counter, recog_state_);
        CEDPagePtr cedptr = server->recognize(makeImageURL(page_), ropts, fopts);

        if(!cedptr)
            return false;

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

void PageRecognizer::setConfigOptions() {
    Q_CHECK_PTR(counter_);

    QSettings global_settings;
    cf::Config::instance().setDebug(
            global_settings.value("debug/printCuneiformDebug", false).toBool());

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
