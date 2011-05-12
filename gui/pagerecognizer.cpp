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
#include <ctime>

#include "pagerecognizer.h"
#include "page.h"
#include "cuneiform.h"
#include "rdib/qtimageloader.h"
#include "common/formatoptions.h"
#include "quneiform_debug.h"
#include "rectexporter.h"
#include "qtextdocumentexporter.h"

#ifdef Q_OS_WIN32
#define usleep(t) Sleep((t)/1000)
#endif

language_t languageToType(const Language& lang) {
    if(lang.isValid()) {
        return static_cast<language_t>(lang.code());
    }
    else {
        qDebug() << "[Warning] Language is not valid, using English instead";
        return ::LANGUAGE_ENGLISH;
    }
}

PageRecognizer::PageRecognizer(QObject * parent)
    : QObject(parent),
    page_(NULL),
    abort_(false),
    stage_sleep_(FORMAT + 1, 0)
{
}

void PageRecognizer::abort() {
    QMutexLocker l(&lock_);
    abort_ = true;
    emit aborted();
}

void PageRecognizer::doRecognize() {
    if(abort_) {
        qDebug() << Q_FUNC_INFO << "aborted";
        return;
    }

    stageSleep(RECOGNIZE);

    cf::Puma::instance().recognize();
    emit recognized();
    emit percentsDone(80);
    QCoreApplication::processEvents();
}

void PageRecognizer::formatResult() {
    if(abort_)
        return;

    Q_CHECK_PTR(page_);

    stageSleep(FORMAT);

    cf::Puma::instance().formatResult();
    cf::RectExporter exporter(cf::Puma::instance().cedPage());
    exporter.collect();

    page_->setBlocks(exporter.pictures(), Page::PICTURE);
    page_->setBlocks(exporter.chars(), Page::CHAR);
    page_->setBlocks(exporter.lines(), Page::LINE);
    page_->setBlocks(exporter.paragraphs(), Page::PARAGRAPH);
    page_->setBlocks(exporter.columns(), Page::COLUMN);
    page_->setBlocks(exporter.sections(), Page::SECTION);
    emit formatted();
    emit percentsDone(90);
    QCoreApplication::processEvents();
}

void PageRecognizer::open() {

}

QImage PageRecognizer::loadImage() {
    Q_CHECK_PTR(page_);

    stageSleep(LOAD);

    QImage img(page_->imagePath());

    // rotate
    if(page_->angle() != 0) {
        QTransform t;
        t.rotate(page_->angle());
        img = img.transformed(t);
    }

    // select page area
    if(page_->pageArea().isValid())
        img = img.copy(page_->pageArea());

    emit loaded();
    emit percentsDone(10);
    QCoreApplication::processEvents();
    return img;
}

QString PageRecognizer::pagePath() const {
    return page_ ? page_->imagePath() : QString();
}

bool PageRecognizer::recognize() {
    if(!page_) {
        emit failed("[PageRecognizer::recognize] NULL page pointer given");
        return false;
    }

    try {
        setFormatOptions();
        setRecognizeOptions();
        QImage img = loadImage();

        stageSleep(OPEN);
        cf::QtImageLoader loader;
        cf::ImagePtr image = loader.load(img);
        if (!image)
            throw Page::Exception("[PageRecognizer::recognize] can't load image");
        emit percentsDone(20);

        cf::Puma::instance().open(image);
        emit opened();
        emit percentsDone(30);
        QCoreApplication::processEvents();

        doRecognize();
        formatResult();
        saveOcrText();
        cf::Puma::instance().close();
    }
    catch(std::exception& e) {
        page_->setFlag(Page::RECOGNITION_FAILED);
        page_->unsetFlag(Page::RECOGNIZED);
        emit failed(e.what());
        return false;
    }

    emit done();
    emit percentsDone(100);
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

void PageRecognizer::saveOcrText() {
    if(abort_) {
        qDebug() << Q_FUNC_INFO << "aborted";
        return;
    }

    Q_CHECK_PTR(page_);

    page_->unsetFlag(Page::RECOGNITION_FAILED);
    page_->setFlag(Page::RECOGNIZED);

    page_->document()->clear();

    QTextDocumentExporter exp(NULL, cf::Puma::instance().formatOptions());
    exp.setPage(page_);
    exp.setDocument(page_->document());
    exp.exportPage(*cf::Puma::instance().cedPage());

    emit percentsDone(95);
    QCoreApplication::processEvents();
}

void PageRecognizer::setFormatOptions() {
    Q_CHECK_PTR(page_);

    cf::FormatOptions opts;
    page_->formatSettings().exportTo(opts);
    opts.setLanguage(languageToType(page_->language()));

    cf::Puma::instance().setFormatOptions(opts);
}

void PageRecognizer::setPage(Page * p) {
    page_ = p;
}

void PageRecognizer::setRecognizeOptions() {
    Q_CHECK_PTR(page_);

    cf::RecognizeOptions recognize_opts;

    RecognitionSettings s = page_->recognitionSettings();
    recognize_opts.setFax(s.fax());
    recognize_opts.setDotMatrix(s.dotMatrix());
    recognize_opts.setOneColumn(s.oneColumn());
    recognize_opts.setPictureSearch(s.picturesSearch());
    recognize_opts.setSpellCorrection(s.spelling());
    recognize_opts.setLanguage(languageToType(page_->language()));

    cf::Puma::instance().setRecognizeOptions(recognize_opts);

    QSettings global_settings;
    cf::Config::instance().setDebug(
            global_settings.value("debug/printCuneiformDebug", false).toBool());
}

void PageRecognizer::setStageSleep(StageType t, int msec) {
    stage_sleep_[t] = msec;
}

void PageRecognizer::stageSleep(StageType t) {
#ifndef NDEBUG
    int mcsec = stage_sleep_[t] * 1000;

    if(!mcsec)
        return;

    qDebug() << Q_FUNC_INFO << mcsec << "micro seconds";
    ::usleep(mcsec);
#endif
}

