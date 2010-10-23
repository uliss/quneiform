/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include <iostream>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSignalMapper>
#include <QProgressDialog>
#include <qdebug.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "document.h"
#include "languageselect.h"
#include "page.h"
#include "thumbnailwidget.h"

static const char * ORGANIZATION = "openocr.org";
static const char * APPLICATION = "Cuneiform OCR";
static const char * EMAIL = "serj.poltavski@gmail.com";
static const int VERSION_MAJOR = 0;
static const int VERSION_MINOR = 0;
static const int VERSION_PATCH = 1;
static const char * VERSION_EXTRA = "-alpha";

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui_(new Ui::MainWindow), doc_(new Document(this)), progress_(NULL) {
    setupUi();
    connectActions();
    connectThumbs();
    readSettings();

    qDebug() << pos();
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::about() {
    QMessageBox::about(this,
                       tr("About"),
                       tr("<b>Quneiform OCR</b><br/>"
                          "  Author:  Serj Poltavski<br/>"
                          "  Email:   <a href=\"mailto:%1\">%2</a><br/>"
                          "  Version: %3.%4.%5%6<br/>"
                          "  License: GPL v.3")
                       .arg(EMAIL)
                       .arg(EMAIL)
                       .arg(VERSION_MAJOR)
                       .arg(VERSION_MINOR)
                       .arg(VERSION_PATCH)
                       .arg(VERSION_EXTRA));
}

void MainWindow::changeDocumentLanguage(int lang) {
    qDebug() << "[MainWindow::changeDocumentLanguage(" << lang << ")]";
    doc_->setLanguage(lang);
    selectLanguage(lang);
}

void MainWindow::closeEvent(QCloseEvent * event) {
    if(doc_)  {
        if(doc_->isChanged()) {
            QMessageBox ask(QMessageBox::Question,
                                  tr("Document is not saved"),
                                  tr("Document is not saved!\nDo you want to save it?"),
                                  QMessageBox::Cancel | QMessageBox::Save,
                                  this);
            ask.setDefaultButton(QMessageBox::Save);
            int button = ask.exec();
            if(button == QMessageBox::Save)
                savePacket();
        }
    }

    writeSettings();
    event->accept();
}

void MainWindow::connectActions() {
    Q_CHECK_PTR(ui_);
    Q_CHECK_PTR(doc_);
    connect(doc_, SIGNAL(changed()), SLOT(documentChange()));
    connect(doc_, SIGNAL(saved()), SLOT(documentSave()));
    connect(ui_->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect(ui_->actionOpen, SIGNAL(triggered()), SLOT(openImages()));
    connect(ui_->actionZoom_In, SIGNAL(triggered()), ui_->image_view_, SLOT(zoomIn()));
    connect(ui_->actionZoom_Out, SIGNAL(triggered()), ui_->image_view_, SLOT(zoomOut()));
    connect(ui_->actionFitWidth, SIGNAL(triggered()), ui_->image_view_, SLOT(fitWidth()));
    connect(ui_->actionFitPage, SIGNAL(triggered()), ui_->image_view_, SLOT(fitPage()));
    connect(ui_->actionOriginalSize, SIGNAL(triggered()), ui_->image_view_, SLOT(originalSize()));
    connect(ui_->actionRecognizeAll, SIGNAL(triggered()), SLOT(recognizeAll()));
    connect(ui_->actionRotateLeft, SIGNAL(triggered()), SLOT(rotateLeft()));
    connect(ui_->actionRotateRight, SIGNAL(triggered()), SLOT(rotateRight()));
    connect(ui_->actionOpenPacket, SIGNAL(triggered()), SLOT(openPacket()));
    connect(ui_->actionSavePacket, SIGNAL(triggered()), SLOT(savePacket()));
}

void MainWindow::connectThumbs() {
    Q_CHECK_PTR(ui_);
    connect(ui_->thumbs_, SIGNAL(thumbSelected(Page*)), SLOT(showPageImage(Page*)));
    connect(ui_->thumbs_, SIGNAL(thumbSelected(Page*)), SLOT(showPageText(Page*)));
    connect(ui_->thumbs_, SIGNAL(thumbRecognize(Page*)), SLOT(recognizePage(Page*)));
    connect(ui_->thumbs_, SIGNAL(save(Page*)), SLOT(savePage(Page*)));
}

void MainWindow::documentChange() {
    if(!windowTitle().endsWith('*'))
        setWindowTitle(windowTitle() + "*");
}

void MainWindow::documentSave() {
    QString title = windowTitle();
    if(title.endsWith('*')) {
        title.chop(1);
        setWindowTitle(title);
    }
}

void MainWindow::mapLanguageActions(const QList<QAction*>& actions) {
    Q_CHECK_PTR(lang_mapper_);

    foreach(QAction * act, actions) {
        connect(act, SIGNAL(triggered()), lang_mapper_, SLOT(map()));
        lang_mapper_->setMapping(act, act->data().toInt());
    }
}

void MainWindow::mapLanguageMenuActions() {
    Q_CHECK_PTR(lang_menu_);

    mapLanguageActions(lang_menu_->actions());
}

void MainWindow::mapLanguageToolButtonActions() {
    Q_CHECK_PTR(lang_select_);
    Q_CHECK_PTR(lang_select_->menu());

    mapLanguageActions(lang_select_->menu()->actions());
}

bool MainWindow::openImage(const QString& path) {
    Q_CHECK_PTR(doc_);
    qDebug() << "[MainWindow::openImage]" << path;

    QFileInfo info(path);
    if(!info.exists()) {
        QMessageBox warning(QMessageBox::Critical, tr("Quniform OCR"),
                            tr("Error while opening \"%1\"").arg(path), QMessageBox::Ok, progress_);
        warning.setInformativeText(tr("File not exists"));
        warning.exec();
        return false;
    }

    Page * p = new Page(path);

    if(p->isNull()) {
        QMessageBox warning(QMessageBox::Critical, tr("Quniform OCR"),
                            tr("Error while opening \"%1\"").arg(path), QMessageBox::Ok, progress_);
        warning.setInformativeText(tr("Unable to load image"));
        warning.exec();
        return false;
    }

    doc_->append(p);

    return true;
}

void MainWindow::openImages() {
    QStringList files = QFileDialog::getOpenFileNames(NULL, tr("Open images"), "",
                                                      tr("Images (*.gif *.png *.xpm *.jpg *.tif *.bmp)"));
    openImages(files);
}

void MainWindow::openImages(const QStringList& files) {
    qDebug() << "[openImages()]";
    delete progress_;
    progress_ = new QProgressDialog(this);
    progress_->setWindowTitle(tr("Quneiform OCR - opening images"));
    progress_->setRange(0, files.count());
    progress_->show();


    for(int i = 0, total = files.count(); i < total; i++) {
        progress_->setValue(i);
        QApplication::processEvents();
        if(progress_->wasCanceled())
            break;

        progress_->setLabelText(tr("Opening image \"%1\"").arg(files.at(i)));
        openImage(files.at(i));
    }

    delete progress_;
    progress_ = NULL;
}

void MainWindow::openPacket() {
    QString packet = QFileDialog::getOpenFileName(this, tr("Open Quneiform packet"), "",
                                                      tr("Quneiform packets (*.qfp)"));
    openPacket(packet);
}

void MainWindow::openPacket(const QString& path) {
    Q_CHECK_PTR(doc_);

    if(path.isEmpty())
        return;

    if(!doc_->open(path)) {
        QMessageBox::warning(this,
                             tr("Quneiform OCR"),
                             tr("Can't read file \"%1\"").arg(path));
    }
}

void MainWindow::readSettings() {
    QSettings settings(ORGANIZATION, APPLICATION);
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(800, 600)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void MainWindow::recognizeAll() {
    Q_CHECK_PTR(doc_);

    if(!doc_->countSelected()) {
        QMessageBox::warning(this, tr("Warning"), tr("No page selected"));
        return;
    }

    doc_->recognizeSelected();
}

void MainWindow::recognizePage(Page * page) {
    Q_CHECK_PTR(page);
    Q_CHECK_PTR(lang_select_);

    if(page->isRecognized()) {
        if(QMessageBox::Ok != QMessageBox::question(this,
                                                    tr("Warning"),
                                                    tr("Page already recognized. Do you want do rerecognize it?"))) {
            return;
        }
    }

    page->setLanguage(lang_select_->currentLanguage());
    page->recognize();
    showPageText(page);
}

void MainWindow::rotate(int factor) {
    Page * p = ui_->thumbs_->currentPage();
    if (!p) {
        qDebug() << "No page selected";
        return;
    }

    p->rotate(factor);
}

void MainWindow::rotateLeft() {
    rotate(-90);
}

void MainWindow::rotateRight() {
    rotate(90);
}

void MainWindow::savePacket() {
    Q_CHECK_PTR(doc_);

    QString fname;

    if(!doc_->fileName().isEmpty())
        fname = doc_->fileName();
    else
        fname = QFileDialog::getSaveFileName(this, tr("Save Quneiform packet to"), "",
                                                           tr("Quneiform packets (*.qfp)"));
    savePacket(fname);
}

void MainWindow::savePacket(const QString& path) {
    Q_CHECK_PTR(doc_);

    if(path.isEmpty())
        return;

    if(!doc_->save(path)) {
        QMessageBox::warning(this,
                             tr("Quneiform OCR"),
                             tr("Can't open file \"%1\" for writing. Check file permissions!").arg(path));
    }
}

void MainWindow::savePage(Page * page) {
    Q_CHECK_PTR(page);

    if(!page->isRecognized()) {
        QMessageBox::warning(this,
                             tr("Quneiform OCR"),
                             tr("Page is not recognized yet"));
        return;
    }

    QFileInfo info(page->imagePath());
    QString filename_suggest = info.baseName() + ".html";
    QString filename = QFileDialog::getSaveFileName(this, tr("Save document to"),
                                filename_suggest,
                                tr("HTML documents (*.html *.htm)"));
    page->save(filename);
}

void MainWindow::selectLanguage(int lang) {
    Q_CHECK_PTR(lang_menu_);
    Q_CHECK_PTR(lang_select_);

    foreach(QAction * act, lang_menu_->actions()) {
        if(act->data().toInt() != lang)
            act->setChecked(false);
        else
            act->setChecked(true);
    }

    lang_select_->select(lang);
}

void MainWindow::setupLanguageUi() {
    lang_mapper_ = new QSignalMapper(this);
    lang_select_ = new LanguageSelect(this);
    ui_->mainToolBar->addWidget(lang_select_);
    lang_menu_  = ui_->menuRecognition->addMenu(tr("Language"));
    LanguageSelect::fillLanguageMenu(lang_menu_);

    mapLanguageMenuActions();
    mapLanguageToolButtonActions();

    connect(lang_mapper_, SIGNAL(mapped(int)), this, SLOT(changeDocumentLanguage(int)));
}

void MainWindow::setupUi() {
    setUnifiedTitleAndToolBarOnMac(true);
    ui_->setupUi(this);
    ui_->thumbs_->setDocument(doc_);
    setZoomEnabled(false);
    setupLanguageUi();
}

void MainWindow::setZoomEnabled(bool value) {
    ui_->actionZoom_In->setEnabled(value);
    ui_->actionZoom_Out->setEnabled(value);
    ui_->actionFitWidth->setEnabled(value);
    ui_->actionFitPage->setEnabled(value);
    ui_->actionRotateLeft->setEnabled(value);
    ui_->actionRotateRight->setEnabled(value);
}

void MainWindow::showPageImage(Page * page) {
    qDebug() << "[MainWindow::showPageImage]" << page;
    Q_CHECK_PTR(page);
    statusBar()->showMessage(QFileInfo(page->imagePath()).fileName());
    setZoomEnabled(true);
    ui_->image_view_->showPage(page);
}

void MainWindow::showPageText(Page * page) {
    Q_CHECK_PTR(page);
    Q_CHECK_PTR(ui_->text_view_);
    ui_->text_view_->document()->setHtml(page->ocrText());
}

void MainWindow::writeSettings() {
    QSettings settings(ORGANIZATION, APPLICATION);
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}
