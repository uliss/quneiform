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
#include <QTextEdit>
#include <QSplitter>
#include <QHBoxLayout>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "document.h"
#include "languageselect.h"
#include "page.h"
#include "imagewidget.h"
#include "thumbnailwidget.h"
#include "thumbnaillist.h"

static const char * ORGANIZATION = "openocr.org";
static const char * APPLICATION = "Quneiform OCR";
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
    populateRecentFiles();
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

void MainWindow::addRecentFile(const QString& file) {
    if(recent_files_.contains(file))
        recent_files_.removeAll(file);

    recent_files_ << file;

    while(recent_files_.size() > MAX_RECENT_FILES)
        recent_files_.removeFirst();

    populateRecentFiles();
}

void MainWindow::addRecentFileMenuAction(const QString& path) {
    QAction * act = new QAction(path, NULL);
    act->setData(path);
    connect(act, SIGNAL(triggered()), SLOT(openRecent()));
    ui_->menuRecentFiles->addAction(act);
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
    enablePageActions(false);
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

void MainWindow::enablePageActions(bool value) {
    ui_->actionZoom_In->setEnabled(value);
    ui_->actionZoom_Out->setEnabled(value);
    ui_->actionFitWidth->setEnabled(value);
    ui_->actionFitPage->setEnabled(value);
    ui_->actionRotateLeft->setEnabled(value);
    ui_->actionRotateRight->setEnabled(value);
    ui_->actionOriginalSize->setEnabled(value);
    ui_->actionRecognizeAll->setEnabled(value);
}

void MainWindow::imageDuplication(const QString& path) {
    if(QMessageBox::Ok == QMessageBox::warning(this,
                                               tr("Warning"),
                                               tr("Image \"%1\" already opened. Do you want to add it anyway?").arg(path),
                                               QMessageBox::Ok,
                                               QMessageBox::Cancel))
        openImage(path, true);
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

    addRecentFile(path);

    return true;
}

void MainWindow::openImages() {
    QStringList files = QFileDialog::getOpenFileNames(NULL, tr("Open images"), "",
                                                      tr("Images (*.gif *.png *.xpm *.jpg *.tif *.bmp)"));
    openImages(files);
}

void MainWindow::openImages(const QStringList& files) {
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
                             tr("Can't read packet \"%1\"").arg(path));
    }
}

void MainWindow::openRecent() {
    Q_CHECK_PTR(doc_);

    QAction * act = qobject_cast<QAction*>(sender());
    if(!act)
        return;

    QString path = act->data().toString();
    if(path.isEmpty())
        return;

    if(!doc_->hasPage(path))
        openImage(path);
}

void MainWindow::populateRecentFiles() {
    ui_->menuRecentFiles->clear();

    QStringListIterator i(recent_files_);
    for(i.toBack(); i.hasPrevious(); i.previous())
        addRecentFileMenuAction(i.peekPrevious());
}

void MainWindow::readSettings() {
    QSettings settings(ORGANIZATION, APPLICATION);
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(800, 600)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    recent_files_ = settings.value("recent-files", QStringList()).toStringList();
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

    doc_->recognizeSelected();
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

    page->setLanguage(lang_select_->currentLanguage());
    page->recognize();
    showPageText(page);
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

void MainWindow::setupDocument() {
    Q_CHECK_PTR(doc_);
    connect(doc_, SIGNAL(changed()), SLOT(documentChange()));
    connect(doc_, SIGNAL(saved()), SLOT(documentSave()));
    connect(doc_, SIGNAL(imageDuplicated(QString)), SLOT(imageDuplication(QString)));
    connect(doc_, SIGNAL(allPagesRecognized()), SLOT(updateCurrentPage()));
}

void MainWindow::setupImageView() {
    image_widget_ = new ImageWidget(this);
    connect(image_widget_, SIGNAL(pageDeleted()), SLOT(disableViewActions()));
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
    enablePageActions(false);
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

    enablePageActions(true);
    statusBar()->showMessage(QFileInfo(page->imagePath()).fileName());
    image_widget_->showPage(page);
}

void MainWindow::showPageText(Page * page) {
    Q_CHECK_PTR(page);
    Q_CHECK_PTR(text_view_);
    text_view_->document()->setHtml(page->ocrText());
}

void MainWindow::updateCurrentPage() {
    Q_CHECK_PTR(thumbs_);
    Page * current = thumbs_->currentPage();
    if(!current)
        return;

    showPageText(current);
}

void MainWindow::writeSettings() {
    QSettings settings(ORGANIZATION, APPLICATION);
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("recent-files", recent_files_);
    settings.endGroup();
}
