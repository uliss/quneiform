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
#include <QProgressDialog>
#include <QTextEdit>
#include <QSplitter>
#include <QHBoxLayout>
#include <QDebug>
#include <QMenuBar>
#include <QLabel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "document.h"
#include "languagemenu.h"
#include "languageselect.h"
#include "page.h"
#include "imagewidget.h"
#include "thumbnailwidget.h"
#include "thumbnaillist.h"
#include "pagerecognitionqueue.h"
#include "recentmenu.h"
#include "settings.h"

static const char * EMAIL = "serj.poltavski@gmail.com";
static const int VERSION_MAJOR = 0;
static const int VERSION_MINOR = 0;
static const int VERSION_PATCH = 1;
static const char * VERSION_EXTRA = "-alpha";
static const int MAX_RECENT_FILES = 5;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui_(new Ui::MainWindow), doc_(new Document(this)), progress_(NULL) {
    setupUi();
    setupDocument();
    setupShortcuts();
    connectActions();
    connectThumbs();
    readSettings();
    setupRecognitionQueue();
    setupRecent();
    setupOpenProgress();
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::about() {
    QMessageBox::about(this,
                       tr("About"),
                       tr("<b>Quneiform OCR</b><br/>"
                          "Author: Serj Poltavski<br/>"
                          "Email: <a href=\"mailto:%1\">%2</a><br/>"
                          "Version: %3.%4.%5%6<br/>"
                          "License: GPL v.3")
                       .arg(EMAIL)
                       .arg(EMAIL)
                       .arg(VERSION_MAJOR)
                       .arg(VERSION_MINOR)
                       .arg(VERSION_PATCH)
                       .arg(VERSION_EXTRA));
}

void MainWindow::addRecentMenu(QMenu * menu) {
    QMenu * file_menu = menuBar()->findChild<QMenu*>("menuFile");
    if(!file_menu)
        return;

    QAction * separator = NULL;
    foreach(QAction * a, file_menu->actions()) {
        if(a->isSeparator()) {
            separator = a;
            break;
        }
    }

    file_menu->insertMenu(separator, menu);
}

void MainWindow::changeDocumentLanguage(int lang) {
    qDebug() << Q_FUNC_INFO << lang;
    doc_->setLanguage(lang);
    recognition_queue_->setLanguage(lang);
}

void MainWindow::closeEvent(QCloseEvent * event) {
    if(doc_)  {
        if(doc_->isChanged()) {
            QMessageBox ask(QMessageBox::Question,
                                  tr("Recognition packet is not saved"),
                                  tr("Recognition packet is not saved!\nDo you want to save it?"),
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
    connect(ui_->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect(ui_->actionOpen, SIGNAL(triggered()), SLOT(openImages()));
    connect(ui_->actionZoom_In, SIGNAL(triggered()), image_widget_, SLOT(zoomIn()));
    connect(ui_->actionZoom_Out, SIGNAL(triggered()), image_widget_, SLOT(zoomOut()));
    connect(ui_->actionFitWidth, SIGNAL(triggered()), image_widget_, SLOT(fitWidth()));
    connect(ui_->actionFitPage, SIGNAL(triggered()), image_widget_, SLOT(fitPage()));
    connect(ui_->actionOriginalSize, SIGNAL(triggered()), image_widget_, SLOT(originalSize()));
    connect(ui_->actionRecognizeAll, SIGNAL(triggered()), SLOT(recognizeAll()));
    connect(ui_->actionRotateLeft, SIGNAL(triggered()), SLOT(rotateLeft()));
    connect(ui_->actionRotateRight, SIGNAL(triggered()), SLOT(rotateRight()));
    connect(ui_->actionOpenPacket, SIGNAL(triggered()), SLOT(openPacket()));
    connect(ui_->actionSavePacket, SIGNAL(triggered()), SLOT(savePacket()));
    connect(ui_->actionPreferences, SIGNAL(triggered()), SLOT(showSettings()));
}

void MainWindow::connectThumbs() {
    Q_CHECK_PTR(ui_);
    connect(thumbs_, SIGNAL(thumbSelected(Page*)), SLOT(showPageImage(Page*)));
    connect(thumbs_, SIGNAL(thumbSelected(Page*)), SLOT(showPageText(Page*)));
    connect(thumbs_, SIGNAL(thumbRecognize(Page*)), SLOT(recognizePage(Page*)));
    connect(thumbs_, SIGNAL(save(Page*)), SLOT(savePage(Page*)));
    connect(thumbs_, SIGNAL(openDraggedImages(QStringList)), SLOT(openImages(QStringList)));
}

void MainWindow::disableViewActions() {
    enableViewActions(false);
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

void MainWindow::enableViewActions(bool value) {
    ui_->actionZoom_In->setEnabled(value);
    ui_->actionZoom_Out->setEnabled(value);
    ui_->actionFitWidth->setEnabled(value);
    ui_->actionFitPage->setEnabled(value);
    ui_->actionRotateLeft->setEnabled(value);
    ui_->actionRotateRight->setEnabled(value);
    ui_->actionOriginalSize->setEnabled(value);
    ui_->actionRecognizeAll->setEnabled(value);
}

void MainWindow::enableViewActions() {
    enableViewActions(true);
}

void MainWindow::imageDuplication(const QString& path) {
    if(QMessageBox::Ok == QMessageBox::warning(this,
                                               tr("Warning"),
                                               tr("Image \"%1\" already opened. Do you want to add it anyway?").arg(path),
                                               QMessageBox::Ok,
                                               QMessageBox::Cancel))
        openImage(path, true);
}

bool MainWindow::openImage(const QString& path, bool allowDuplication) {
    Q_CHECK_PTR(doc_);
    qDebug() << Q_FUNC_INFO << path;

    QFileInfo info(path);
    if(!info.exists()) {
        QMessageBox warning(QMessageBox::Critical, tr("Quneiform OCR"),
                            tr("Error while opening \"%1\"").arg(path), QMessageBox::Ok, progress_);
        warning.setInformativeText(tr("File not exists"));
        warning.exec();
        return false;
    }

    Page * p = new Page(path);

    if(p->isNull()) {
        QMessageBox warning(QMessageBox::Critical, tr("Quneiform OCR"),
                            tr("Error while opening \"%1\"").arg(path), QMessageBox::Ok, progress_);
        warning.setInformativeText(tr("Unable to open image"));
        warning.exec();
        return false;
    }

    doc_->append(p, allowDuplication);
    recent_images_->add(path);

    return true;
}

void MainWindow::openImages() {
    QStringList files = QFileDialog::getOpenFileNames(NULL, tr("Open images"), "",
                                                      tr("Images (*.gif *.png *.xpm *.jpg *.tif *.bmp)"));
    openImages(files);
}

void MainWindow::openImages(const QStringList& files) {
    Q_CHECK_PTR(progress_);

    progress_->reset();
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

    progress_->hide();
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
                             tr("Can't read packet \"%1\"").arg(path));
    }

    selectLanguage(doc_->language());
    recent_packets_->add(path);
}

void MainWindow::openRecentImage(const QString& path) {
    Q_CHECK_PTR(doc_);
    if(doc_->hasPage(path))
        return;

    openImage(path, false);
}

void MainWindow::readSettings() {
    QSettings settings;
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(800, 600)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void MainWindow::recognizeAll() {
    Q_CHECK_PTR(doc_);

    if(!doc_->countSelected()) {
        QMessageBox warn(QMessageBox::Warning,
                      tr("Warning"),
                      tr("No page selected"),
                      QMessageBox::Cancel | QMessageBox::Ok,
                      this);
        warn.setInformativeText(tr("Do you want to recognize all?"));

        int button = warn.exec();
        if(button == QMessageBox::Cancel)
            return;
        else if(button == QMessageBox::Ok)
            thumbs_->selectAll();
    }

    for(int i = 0; i < doc_->pageCount(); i++) {
        Page * p = doc_->page(i);
        if(p->isSelected())
            recognition_queue_->add(p);
    }

    recognition_queue_->setLanguage(lang_select_->currentLanguage());
    recognition_queue_->start();
}

void MainWindow::recognizePage(Page * page) {
    Q_CHECK_PTR(page);
    Q_CHECK_PTR(lang_select_);

    if(page->isRecognized()) {
        if(QMessageBox::Ok != QMessageBox::question(this,
                                                    tr("Warning"),
                                                    tr("Page already recognized. Do you want to recognize it again?"),
                                                    QMessageBox::Ok,
                                                    QMessageBox::Cancel)) {
            return;
        }
    }

    recognition_queue_->add(page);
    recognition_queue_->start();
}

void MainWindow::rotate(int factor) {
    Page * p = thumbs_->currentPage();
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
    QString filename = QFileDialog::getSaveFileName(this, tr("Saving page"),
                                filename_suggest,
                                tr("HTML documents (*.html *.htm)"));
    if(filename.isEmpty())
        return;

    page->save(filename);
}

void MainWindow::selectLanguage(int lang) {
    Q_CHECK_PTR(lang_select_);
    Q_CHECK_PTR(lang_menu_);

    if(sender() != lang_select_)
        lang_select_->select(lang);

    if(sender() != lang_menu_)
        lang_menu_->select(lang);

    changeDocumentLanguage(lang);
}

void MainWindow::setupDocument() {
    Q_CHECK_PTR(doc_);
    connect(doc_, SIGNAL(changed()), SLOT(documentChange()));
    connect(doc_, SIGNAL(saved()), SLOT(documentSave()));
    connect(doc_, SIGNAL(imageDuplicated(QString)), SLOT(imageDuplication(QString)));
}

void MainWindow::setupImageView() {
    image_widget_ = new ImageWidget(this);
    connect(image_widget_, SIGNAL(pageDeleted()), SLOT(disableViewActions()));
}

void MainWindow::setupLanguageMenu() {
    lang_menu_ = new LanguageMenu;
    ui_->menuRecognition->addMenu(lang_menu_);
    connect(lang_menu_, SIGNAL(languageSelected(int)), SLOT(selectLanguage(int)));
}

void MainWindow::setupLanguageSelect() {
    lang_select_ = new LanguageSelect(this);
    ui_->mainToolBar->addWidget(lang_select_);
    connect(lang_select_, SIGNAL(languageSelected(int)), SLOT(selectLanguage(int)));
}

void MainWindow::setupLanguageUi() {
    setupLanguageMenu();
    setupLanguageSelect();
}

void MainWindow::setupOpenProgress() {
    progress_ = new QProgressDialog(this);
    progress_->setWindowTitle(tr("Quneiform OCR - opening images"));
    QLabel * label = new QLabel(progress_);
    label->setAlignment(Qt::AlignLeft);
    label->setTextFormat(Qt::PlainText);
    progress_->setLabel(label);
    progress_->setFixedWidth(450);
}

void MainWindow::setupRecent() {
    setupRecentImages();
    setupRecentPackets();
}

void MainWindow::setupRecentImages() {
    recent_images_ = new RecentMenu(this, tr("Recent files"), "recent-files");
    addRecentMenu(recent_images_);
    connect(recent_images_, SIGNAL(selected(QString)), SLOT(openRecentImage(QString)));
}

void MainWindow::setupRecentPackets() {
    recent_packets_ = new RecentMenu(this, tr("Recent packets"), "recent-packets");
    addRecentMenu(recent_packets_);
    connect(recent_packets_, SIGNAL(selected(QString)), SLOT(openPacket(QString)));
}

void MainWindow::setupRecognitionQueue() {
    recognition_queue_ = new PageRecognitionQueue(this);
    connect(recognition_queue_, SIGNAL(started()), SLOT(disableViewActions()));
    connect(recognition_queue_, SIGNAL(finished()), SLOT(updateCurrentPage()));
    connect(recognition_queue_, SIGNAL(finished()), SLOT(enableViewActions()));
}

void MainWindow::setupShortcuts() {
    ui_->actionExit->setShortcut(QKeySequence::Quit);
    ui_->actionOpen->setShortcut(QKeySequence::Open);
    ui_->actionZoom_In->setShortcut(QKeySequence::ZoomIn);
    ui_->actionZoom_Out->setShortcut(QKeySequence::ZoomOut);
}

void MainWindow::setupTextView() {
    text_view_ = new QTextEdit(this);
    text_view_->setReadOnly(true);
}

void MainWindow::setupThumbs() {
    thumbs_ = new ThumbnailList(this);
    thumbs_->setDocument(doc_);
}

void MainWindow::setupUi() {
    setUnifiedTitleAndToolBarOnMac(true);
    ui_->setupUi(this);
    enableViewActions(false);
    setupLanguageUi();
    setupThumbs();
    setupImageView();
    setupTextView();
    setupUiLayout();
}

void MainWindow::setupUiLayout() {
    main_layout_ = new QHBoxLayout;
    main_layout_->setSpacing(0);
    main_layout_->setMargin(1);
    main_layout_->setContentsMargins(0, 0, 0, 0);

    main_layout_->addWidget(thumbs_);

    QSplitter * spl = new QSplitter(Qt::Vertical);
    spl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spl->addWidget(image_widget_);
    spl->addWidget(text_view_);

    main_layout_->addWidget(spl);

    ui_->centralWidget->setLayout(main_layout_);
}

void MainWindow::showPageImage(Page * page) {
    qDebug() << Q_FUNC_INFO << page;
    Q_CHECK_PTR(page);

    enableViewActions(true);
    statusBar()->showMessage(QFileInfo(page->imagePath()).fileName());
    image_widget_->showPage(page);
}

void MainWindow::showPageText(Page * page) {
    Q_CHECK_PTR(page);
    Q_CHECK_PTR(text_view_);
    text_view_->document()->setHtml(page->ocrText());
}

void MainWindow::showSettings() {
    Settings s;
    int state = s.exec();

    if(state == QDialog::Accepted)
        image_widget_->updateSettings();
}

void MainWindow::updateCurrentPage() {
    Q_CHECK_PTR(thumbs_);
    Page * current = thumbs_->currentPage();
    if(!current)
        return;

    showPageText(current);
}

void MainWindow::writeSettings() {
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}
