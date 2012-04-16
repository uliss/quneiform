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
            res.addReadRect(cf::Rect(r.left(), r.top(), r.width(), r.height()));
        }
    }

    return res;
}

PageRecognizer::PageRecognizer(QObject * parent)
    : QObject(parent),
    page_(NULL),
    counter_(NULL),
    recog_state_(NULL),
    abort_(false)
{
    counter_ = new cf::PercentCounter;
    counter_->setCallback(this, &PageRecognizer::handleRecognitionProgress);

    recog_state_ = new cf::RecognitionState;
    recog_state_->setCallback(this, &PageRecognizer::handleRecognitionState);
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

QImage PageRecognizer::loadImage() {
    Q_CHECK_PTR(page_);
    Q_CHECK_PTR(counter_);
    Q_CHECK_PTR(recog_state_);

    QImage img(page_->imagePath());

    // rotate
    if(page_->angle() != 0) {
        QTransform t;
        t.rotate(page_->angle());
        img = img.transformed(t);
    }

    // update counter and state
    counter_->add(9);
    recog_state_->set(cf::RecognitionState::LOADED);

    return img;
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

        QtImageLoader loader;
        ImagePtr image = loader.load(loadImage());

        FormatOptions fopts = getFormatOptions(page_);
        RecognizeOptions ropts = getRecogOptions(page_);

        PercentCounter recog_counter(counter_);
        recog_counter.setContribution(80);

        ResolutionHistogramCallbackSetter hist_clbk(boost::bind(&PageRecognizer::saveResolutionHeightHistogram, this, _1),
                                                    boost::bind(&PageRecognizer::saveResolutionWidthHistogram, this, _1));

#ifndef NDEBUG
        RecognitionPtr server = RecognitionFactory::instance().make(SERVER_LOCAL);
#else
        RecognitionPtr server = RecognitionFactory::instance().make(SERVER_PROCESS);
#endif

        if(!server)
            throw std::runtime_error("[PageRecognizer::recognize] recognition server creation failed");

        server->setCounter(&recog_counter);
        server->setStateTracker(recog_state_);

        CEDPagePtr cedptr = server->recognize(image, ropts, fopts);

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
