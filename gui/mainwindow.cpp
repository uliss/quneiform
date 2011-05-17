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
#include <QSplitter>
#include <QHBoxLayout>
#include <QDebug>
#include <QMenuBar>
#include <QLabel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "packet.h"
#include "languagemenu.h"
#include "languageselect.h"
#include "page.h"
#include "imagewidget.h"
#include "thumbnailwidget.h"
#include "thumbnaillist.h"
#include "texteditor.h"
#include "pagerecognitionqueue.h"
#include "recognitionprogressdialog.h"
#include "recentmenu.h"
#include "settings.h"
#include "aboutdialog.h"

static const int VERSION_MAJOR = 0;
static const int VERSION_MINOR = 0;
static const int VERSION_PATCH = 1;
static const int MAX_RECENT_FILES = 5;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui_(new Ui::MainWindow),
        packet_(new Packet(this)),
        progress_(NULL),
        image_widget_(NULL) {
    setupUi();
    setupPacket();
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
    AboutDialog about;
    about.exec();
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

bool MainWindow::confirmRotation(Page * p) {
    Q_CHECK_PTR(p);

    if(p->isRecognized())
        return confirmRotationRecognized();
    else if(!p->pageArea().isNull())
        return confirmRotationSelected();
    else
        return true;
}

bool MainWindow::confirmRotationRecognized() {
    return QMessageBox::warning(this,
                                tr("Image rotation"),
                                tr("Image is already recognized. Rotation will remove page layout.\n"
                                   "Are you shure?"),
                                QMessageBox::Cancel | QMessageBox::Yes,
                                QMessageBox::Cancel) == QMessageBox::Yes;
}

bool MainWindow::confirmRotationSelected() {
    return QMessageBox::warning(this,
                                tr("Image rotation"),
                                tr("Image has recognition area. Rotation will remove it.\n"
                                   "Are you shure?"),
                                QMessageBox::Cancel | QMessageBox::Yes,
                                QMessageBox::Cancel) == QMessageBox::Yes;
}

void MainWindow::changePacketLanguage(const Language& lang) {
    qDebug() << Q_FUNC_INFO << lang.name();

    if(thumbs_->currentPage())
        thumbs_->currentPage()->setLanguage(lang);

    foreach(Page * p, packet_->selectedPages()) {
        p->setLanguage(lang);
    }
}

void MainWindow::closeEvent(QCloseEvent * event) {
    if(packet_)  {
        if(!packet_->isEmpty() && packet_->isChanged()) {
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
    connect(thumbs_, SIGNAL(thumbSelected(Page*)), SLOT(updateCurrentLanguage(Page*)));
    connect(thumbs_, SIGNAL(thumbSelected(Page*)), SLOT(updatePageDocument(Page*)));
    connect(thumbs_, SIGNAL(thumbRecognize(Page*)), SLOT(recognizePage(Page*)));
    connect(thumbs_, SIGNAL(save(Page*)), SLOT(savePage(Page*)));
    connect(thumbs_, SIGNAL(openDraggedImages(QStringList)), SLOT(openImages(QStringList)));
    connect(thumbs_, SIGNAL(showPageFault(Page*)), SLOT(showPageFault(Page*)));
}

void MainWindow::disableViewActions() {
    enableViewActions(false);
}

void MainWindow::disableZoomInAction() {
    ui_->actionZoom_In->setEnabled(false);
    ui_->actionZoom_Out->setEnabled(true);
}

void MainWindow::disableZoomOutAction() {
    ui_->actionZoom_In->setEnabled(true);
    ui_->actionZoom_Out->setEnabled(false);
}

void MainWindow::packetChange() {
    setWindowModified(true);
}

void MainWindow::packetSave() {
    Q_ASSERT(packet_);

    setWindowModified(false);
    setWindowFilePath(packet_->fileName());
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

void MainWindow::enableZoomActions() {
    if(!ui_->actionZoom_In->isEnabled())
        ui_->actionZoom_In->setEnabled(true);
    if(!ui_->actionZoom_Out->isEnabled())
        ui_->actionZoom_Out->setEnabled(true);
}

void MainWindow::enableViewActions() {
    enableViewActions(true);
}

void MainWindow::imageDuplication(const QString& path) {
    if(QMessageBox::Yes == QMessageBox::question(this,
                                               tr("Warning"),
                                               tr("Image \"%1\" already opened. Do you want to add it anyway?").arg(path),
                                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
        openImage(path, true);
}

bool MainWindow::openImage(const QString& path, bool allowDuplication) {
    Q_CHECK_PTR(packet_);
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
    p->setLanguage(lang_select_->currentLanguage());

    if(p->isNull()) {
        QMessageBox warning(QMessageBox::Critical, tr("Quneiform OCR"),
                            tr("Error while opening \"%1\"").arg(path), QMessageBox::Ok, progress_);
        warning.setInformativeText(tr("Unable to open image"));
        warning.exec();
        return false;
    }

    packet_->append(p, allowDuplication);
    recent_images_->add(path);

    return true;
}

void MainWindow::open(const QStringList& paths) {
    if(paths.isEmpty())
        return;

    QFileInfo info;
    foreach(QString path, paths) {
        info.setFile(path);
        if(info.suffix() == "qfp")
            openPacket(path);
        else
            openImage(path);
    }
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
    progress_->setMinimumDuration(1000);

    for(int i = 0, total = files.count(); i < total; i++) {
        progress_->setValue(i);
        QApplication::processEvents();
        if(progress_->wasCanceled())
            break;

        progress_->setLabelText(tr("Opening image \"%1\"").arg(files.at(i)));
        openImage(files.at(i));
    }

    progress_->reset();
    progress_->hide();
}

void MainWindow::openPacket() {
    QString packet = QFileDialog::getOpenFileName(this, tr("Open Quneiform packet"), "",
                                                      tr("Quneiform packets (*.qfp)"));
    openPacket(packet);
}

void MainWindow::openPacket(const QString& path) {
    Q_CHECK_PTR(packet_);

    if(path.isEmpty())
        return;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(!packet_->open(path)) {
        QMessageBox::warning(this,
                             tr("Quneiform OCR"),
                             tr("Can't read packet \"%1\"").arg(path));
    }
    QApplication::restoreOverrideCursor();

    if(!packet_->isEmpty())
        selectLanguage(packet_->firstPage()->language());

    recent_packets_->add(path);
    setWindowModified(false);
}

void MainWindow::openRecentImage(const QString& path) {
    Q_CHECK_PTR(packet_);
    if(packet_->hasPage(path))
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
    Q_CHECK_PTR(packet_);

    if(!packet_->countSelected()) {
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

    for(int i = 0; i < packet_->pageCount(); i++) {
        Page * p = packet_->pageAt(i);
        if(p->isSelected())
            recognition_queue_->add(p);
    }

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

void MainWindow::rotate(int angle) {
    Page * p = thumbs_->currentPage();
    if (!p) {
        qDebug() << "No page selected";
        return;
    }

    if(confirmRotation(p)) {
        p->clearLayout();
        p->unsetFlag(Page::RECOGNIZED);
        p->rotate(angle);
    }
}

void MainWindow::rotateLeft() {
    rotate(-90);
}

void MainWindow::rotateRight() {
    rotate(90);
}

void MainWindow::savePacket() {
    Q_CHECK_PTR(packet_);

    QString fname;

    if(packet_->isNew() || packet_->fileName().isEmpty()) {
        QFileInfo fi(packet_->fileName());
        fname = QFileDialog::getSaveFileName(this,
                                             tr("Save Quneiform packet to"),
                                             fi.baseName(),
                                             tr("Quneiform packets (*.qfp)"));
    }
    else {
        fname = packet_->fileName();
    }

    savePacket(fname);
}

void MainWindow::savePacket(const QString& path) {
    Q_CHECK_PTR(packet_);

    if(path.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "packet name is empty";
        return;
    }

    if(!packet_->save(path)) {
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

    try {
        page->exportTo(filename);
    }
    catch(Page::Exception& e) {
        QMessageBox::warning(this,
                             tr("Quneiform OCR"),
                             e.message());
    }
}

void MainWindow::selectLanguage(const Language& lang) {
    Q_CHECK_PTR(lang_select_);
    Q_CHECK_PTR(lang_menu_);

    if(sender() != lang_select_)
        lang_select_->select(lang);

    if(sender() != lang_menu_)
        lang_menu_->select(lang);

    changePacketLanguage(lang);
}

void MainWindow::setupPacket() {
    Q_CHECK_PTR(packet_);

    setWindowFilePath(packet_->fileName());
    connect(packet_, SIGNAL(changed()), SLOT(packetChange()));
    connect(packet_, SIGNAL(saved()), SLOT(packetSave()));
    connect(packet_, SIGNAL(imageDuplicated(QString)), SLOT(imageDuplication(QString)));
}

void MainWindow::setupImageView() {
    image_widget_ = new ImageWidget(this);
    connect(image_widget_, SIGNAL(pageDeleted()), SLOT(disableViewActions()));
    connect(image_widget_, SIGNAL(scaleIsTooBig()), SLOT(disableZoomInAction()));
    connect(image_widget_, SIGNAL(scaleIsTooSmall()), SLOT(disableZoomOutAction()));
    connect(image_widget_, SIGNAL(scaled()), SLOT(enableZoomActions()));
}

void MainWindow::setupLanguageMenu() {
    lang_menu_ = new LanguageMenu;
    ui_->menuRecognition->addMenu(lang_menu_);
    connect(lang_menu_, SIGNAL(languageSelected(Language)), SLOT(selectLanguage(Language)));
}

void MainWindow::setupLanguageSelect() {
    lang_select_ = new LanguageSelect(this);
    ui_->mainToolBar->addWidget(lang_select_);
    connect(lang_select_, SIGNAL(languageSelected(Language)), SLOT(selectLanguage(Language)));
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
    RecognitionProgressDialog * r_dlg = new RecognitionProgressDialog(this);
    connect(recognition_queue_, SIGNAL(started()), SLOT(disableViewActions()));
    connect(recognition_queue_, SIGNAL(finished(int)), SLOT(updateCurrentPage()));
    connect(recognition_queue_, SIGNAL(finished(int)), SLOT(enableViewActions()));
    r_dlg->connectToQueue(recognition_queue_);
}

void MainWindow::setupShortcuts() {
    // there's no default shortcut for quit action in windows and Qt < 4.6
#if QT_VERSION < 0x040600 || defined(Q_WS_WIN)
    ui_->actionExit->setShortcut(QKeySequence("Ctrl+Q"));
#else
    ui_->actionExit->setShortcut(QKeySequence::Quit);
#endif

    ui_->actionOpen->setShortcut(QKeySequence::Open);
    ui_->actionZoom_In->setShortcut(QKeySequence::ZoomIn);
    ui_->actionZoom_Out->setShortcut(QKeySequence::ZoomOut);
}

void MainWindow::setupTextView() {
    text_view_ = new TextEditor(this);
    connect(text_view_, SIGNAL(charSelected(QRect)), image_widget_, SLOT(showChar(QRect)));
}

void MainWindow::setupThumbs() {
    thumbs_ = new ThumbnailList(this);
    thumbs_->setDocument(packet_);
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
    text_view_->showPage(page);
}

void MainWindow::showPageFault(Page * page) {
    Q_CHECK_PTR(page);
    Q_CHECK_PTR(recognition_queue_);

    QString msg = recognition_queue_->pageError(page->imagePath());
    if(msg.isEmpty())
        return;

    QMessageBox::critical(this, tr("Recognition error"), msg);
}

void MainWindow::showSettings() {
    Q_CHECK_PTR(image_widget_);

    Settings s;
    int state = s.exec();

    if(state == QDialog::Accepted)
        image_widget_->updateSettings();
}

void MainWindow::updateCurrentLanguage(Page * p) {
    Q_CHECK_PTR(p);
    Q_CHECK_PTR(lang_select_);
    Q_CHECK_PTR(lang_menu_);

    Language lang = p->language();

    // if page has a valid language
    if(lang.isValid()) {
        lang_select_->select(lang);
        lang_menu_->select(lang);
    }
    else { // set current language to page
        Language current_l = lang_select_->currentLanguage();
        if(current_l.isValid())
            p->setLanguage(current_l);
    }
}

void MainWindow::updateCurrentPage() {
    Q_CHECK_PTR(thumbs_);
    Page * current = thumbs_->currentPage();
    if(!current)
        return;

    showPageText(current);
}

void MainWindow::updatePageDocument(Page * page) {
    if((!page) || (!page->isRecognized()))
        return;

    if(!page->document() || (!page->document()->isEmpty()))
        return;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    page->updateTextDocument();
    QApplication::restoreOverrideCursor();
}

void MainWindow::writeSettings() {
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}
