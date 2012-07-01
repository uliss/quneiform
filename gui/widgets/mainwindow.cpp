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
#include <QSplitter>
#include <QHBoxLayout>
#include <QDebug>
#include <QMenuBar>
#include <QLabel>
#include <QImageReader>
#include <QUrl>
#include <QDesktopServices>

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
#include "dialogs/aboutdialog.h"
#include "dialogs/exportdialog.h"
#include "dialogs/packetopenprogressdialog.h"
#include "dialogs/recognitionprogressdialog.h"
#include "dialogs/recognitionsettingsdialog.h"
#include "dialogs/settings.h"
#include "dialogs/logviewerdialog.h"
#include "dialogs/openprogressdialog.h"
#include "internal/cimageview.h"
#include "recentmenu.h"
#include "exportsettings.h"
#include "imageutils.h"
#include "scan/scannerdialog.h"

static const int VERSION_MAJOR = 0;
static const int VERSION_MINOR = 0;
static const int VERSION_PATCH = 1;
static const int MAX_RECENT_FILES = 5;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui_(new Ui::MainWindow),
        packet_(new Packet(this)),
        progress_(new OpenProgressDialog(this)),
        image_widget_(NULL),
        view_splitter_(NULL)
{
    setupUi();
    setupPacket();
    setupShortcuts();
    connectActions();
    connectThumbs();
    setupRecognitionQueue();
    setupRecent();
    setupViewSplit();
    readSettings();
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
    else if(p->hasReadAreas())
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
    if(!thumbs_)
        return;

    thumbs_->setLanguage(lang);
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
    connect(ui_->actionRecognitionSettings, SIGNAL(triggered()), SLOT(recognitionSettings()));
    connect(ui_->actionScan, SIGNAL(triggered()), SLOT(showScanDialog()));
    connect(ui_->actionViewThumbnails, SIGNAL(triggered()), SLOT(showViewThumbnails()));
    connect(ui_->actionViewContentOnly, SIGNAL(triggered()), SLOT(showViewContentOnly()));
    connect(ui_->actionReportBug, SIGNAL(triggered()), SLOT(handleReportBug()));
    connect(ui_->actionSplitHorizontal, SIGNAL(triggered()), SLOT(handleViewSplitChange()));
    connect(ui_->actionSplitVertical, SIGNAL(triggered()), SLOT(handleViewSplitChange()));
    connect(ui_->actionFullScreen, SIGNAL(triggered()), SLOT(handleShowFullScreen()));

    QActionGroup * view_group = new QActionGroup(this);
    view_group->addAction(ui_->actionViewThumbnails);
    view_group->addAction(ui_->actionViewContentOnly);
    ui_->actionViewThumbnails->setChecked(true);
}

void MainWindow::connectThumbs() {
    Q_CHECK_PTR(ui_);
    connect(thumbs_, SIGNAL(showPage(Page*)), SLOT(showPageImage(Page*)));
    connect(thumbs_, SIGNAL(showPage(Page*)), SLOT(showPageText(Page*)));
    connect(thumbs_, SIGNAL(showPage(Page*)), SLOT(updateCurrentLanguage(Page*)));
    connect(thumbs_, SIGNAL(showPage(Page*)), SLOT(updatePageDocument(Page*)));
    connect(thumbs_, SIGNAL(recognizePage(Page*)), SLOT(recognizePage(Page*)));
    connect(thumbs_, SIGNAL(recognizePages(QList<Page*>)), SLOT(recognizePages(QList<Page*>)));
    connect(thumbs_, SIGNAL(savePage(Page*)), SLOT(savePage(Page*)));
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

void MainWindow::handleReportBug()
{
    QDesktopServices::openUrl(QUrl("https://github.com/uliss/quneiform/issues", QUrl::TolerantMode));
}

void MainWindow::handleShowFullScreen()
{
    static QString prev_text;

    if(isFullScreen()) {
        showNormal();
        ui_->actionFullScreen->setText(prev_text);
    }
    else {
        prev_text = ui_->actionFullScreen->text();
        ui_->actionFullScreen->setText(tr("Go to normal mode"));
        showFullScreen();
    }
}

void MainWindow::handleViewSplitChange()
{
    if(sender() == (QObject*) ui_->actionSplitVertical)
        view_splitter_->setOrientation(Qt::Horizontal);
    else if(sender() == (QObject*) ui_->actionSplitHorizontal)
        view_splitter_->setOrientation(Qt::Vertical);
    else
        qWarning() << Q_FUNC_INFO << "unknown split type";
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

bool MainWindow::openImage(const QString& path, bool allowDuplication)
{
    Q_CHECK_PTR(packet_);

    QFileInfo info(path);
    if(!info.exists()) {
        QMessageBox warning(QMessageBox::Critical, tr("Quneiform OCR"),
                            tr("Error while opening \"%1\"").arg(path), QMessageBox::Ok, progress_);
        warning.setInformativeText(tr("File not exists"));
        warning.exec();
        return false;
    }

    if(utils::looksLikeMultiPageDocument(path))
        return openMultiPage(path);

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

bool MainWindow::openMultiPage(const QString& path)
{
    Q_ASSERT(packet_);
    Q_ASSERT(lang_select_);
    Q_ASSERT(recent_images_);
    Q_ASSERT(progress_);

    QByteArray format = utils::imagePluginFormat(path);
    if(format.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "unsupported format:" << path;
        return false;
    }

    QImageReader r(path, format);
    int total = r.imageCount();

    qDebug() << Q_FUNC_INFO << "image count:" << total;

    for(int i = 0 ; i < total; i++) {
        if(progress_->wasCanceled())
            break;

        progress_->load(path, i);

        Page * p = new Page(ImageURL(path, i));
        p->setLanguage(lang_select_->currentLanguage());

        if(p->isNull()) {
            qWarning() << Q_FUNC_INFO << "can't open page:" << i << "from" << path;
            return false;
        }

        packet_->append(p, true);
        QApplication::processEvents();

        progress_->loadDone();
    }

    recent_images_->add(path);

    return true;
}

void MainWindow::open(const QStringList& paths) {
    if(paths.isEmpty())
        return;

    progress_->start(paths);

    QFileInfo info;
    foreach(QString path, paths) {
        if(progress_->wasCanceled())
            break;

        progress_->load(path);

        info.setFile(path);
        if(info.suffix() == "qfp")
            openPacket(path);
        else
            openImage(path);

        progress_->loadDone();
        QCoreApplication::processEvents();
    }

    progress_->stop();
}

void MainWindow::openImages() {
    QStringList file_ext;

    file_ext << "*.png"
                    << "*.xmp"
                    << "*.jpg" << "*.jpeg"
                    << "*.tif" << "*.tiff"
                    << "*.bmp"
                    << "*.pnm" << "*.pbm" << "*.pgm" << "*.ppm";

    QList<QByteArray> supported_formats = QImageReader::supportedImageFormats();
    if(supported_formats.contains("gif"))
        file_ext << "*.gif";
    if(supported_formats.contains("pdf"))
        file_ext << "*.pdf";
    if(supported_formats.contains("djvu"))
        file_ext << "*.djvu";

    QStringList files = QFileDialog::getOpenFileNames(NULL,
                                                      tr("Open images"),
                                                      "",
                                                      tr("Images (%1)").arg(file_ext.join(" ")));
    openImages(files);
}

void MainWindow::openImages(const QStringList& files) {
    Q_CHECK_PTR(progress_);

    progress_->start(files);

    for(int i = 0, total = files.count(); i < total; i++) {
        QApplication::processEvents();
        if(progress_->wasCanceled())
            break;

        progress_->load(files.at(i));
        openImage(files.at(i));
        progress_->loadDone();
    }

    progress_->stop();
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

    PacketOpenProgressDialog progress(packet_, this);

    if(!packet_->open(path)) {
        QMessageBox::warning(this,
                             tr("Quneiform OCR"),
                             tr("Can't read packet \"%1\"").arg(path));
    }

    if(!packet_->isEmpty()) {
        Language lang = packet_->firstPage()->language();
        lang_select_->select(lang);
        lang_menu_->select(lang);
    }

    recent_packets_->add(path);
    setWindowModified(false);
}

void MainWindow::openRecentImage(const QString& path) {
    Q_CHECK_PTR(packet_);
    if(packet_->hasPage(path))
        return;

    openImage(path, false);
}

void MainWindow::readSettings()
{
    QSettings s;
    s.beginGroup("MainWindow");
    resize(s.value("size", QSize(800, 600)).toSize());
    move(s.value("pos", QPoint(200, 200)).toPoint());
    s.endGroup();

    // view
    s.beginGroup("view");
    // splitter
    QString split_setting = s.value("split", QString("Horizontal")).toString();
    if(split_setting == "Horizontal")
        ui_->actionSplitHorizontal->setChecked(true);
    else if(split_setting == "Vertical")
        ui_->actionSplitVertical->setChecked(true);
    else
        qWarning() << Q_FUNC_INFO << "unknown split type:" << split_setting;

    QByteArray split_state = s.value("split_state", QByteArray()).toByteArray();
    view_splitter_->restoreState(split_state);

    // thumbs
    if(s.value("hide_thumbnails", false).toBool())
        thumbs_->hide();

    s.endGroup();
}

void MainWindow::recognizeAll() {
    Q_CHECK_PTR(packet_);
    Q_CHECK_PTR(recognition_queue_);

    for(int i = 0; i < packet_->pageCount(); i++) {
        Page * p = packet_->pageAt(i);
        recognition_queue_->add(p);
    }

    recognition_queue_->start();
}

void MainWindow::recognizePage(Page * page) {
    Q_CHECK_PTR(recognition_queue_);

    if(!page)
        return;

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

void MainWindow::recognizePages(const QList<Page*>& pages) {
    Q_CHECK_PTR(recognition_queue_);

    foreach(Page * p, pages) {
        recognition_queue_->add(p);
    }

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

    recent_packets_->add(path);
}

void MainWindow::savePage(Page * page) {
    Q_CHECK_PTR(page);

    if(!page->isRecognized()) {
        QMessageBox::warning(this,
                             tr("Quneiform OCR"),
                             tr("Page is not recognized yet"));
        return;
    }

    QString filename;
    ExportDialog dialog(page->imagePath(), this);
    if(dialog.exec()) {
        filename = dialog.fileName();
    }

//    QFileInfo info(page->imagePath());
//    QString filename_suggest = info.baseName() + ".html";
//    QString filename = QFileDialog::getSaveFileName(this, tr("Saving page"),
//                                filename_suggest,
//                                tr("HTML documents (*.html *.htm)"));
    if(filename.isEmpty())
        return;

    try {
        page->exportTo(filename, dialog.settings());
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

void MainWindow::showLog()
{
    LogViewerDialog d;
    d.exec();
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
    connect(image_widget_, SIGNAL(gestureRotateAttempt(int)), SLOT(rotate(int)));
    connect(image_widget_, SIGNAL(recognize(Page*)), SLOT(recognizePage(Page*)));
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
    setupDefaultLanguage();
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
    ui_->actionSavePacket->setShortcut(QKeySequence::Save);

    ui_->actionFullScreen->setShortcut(QKeySequence("Ctrl+Alt+F"));
}

void MainWindow::setupTextView() {
    text_view_ = new TextEditor(this);
    connect(text_view_, SIGNAL(charSelected(QRect)), image_widget_, SLOT(showChar(QRect)));
}

void MainWindow::setupThumbs() {
    thumbs_ = new ThumbnailList(this);
    thumbs_->setPacket(packet_);
}

void MainWindow::setupViewSplit()
{
    QActionGroup * split_group = new QActionGroup(this);
    split_group->addAction(ui_->actionSplitHorizontal);
    split_group->addAction(ui_->actionSplitVertical);
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

#ifndef NDEBUG
    addDebugMenu();
#endif
}

void MainWindow::setupUiLayout() {
    main_layout_ = new QHBoxLayout;
    main_layout_->setSpacing(0);
    main_layout_->setMargin(1);
    main_layout_->setContentsMargins(0, 0, 0, 0);
    main_layout_->addWidget(thumbs_);

    view_splitter_ = new QSplitter(Qt::Vertical);
    view_splitter_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view_splitter_->addWidget(image_widget_);
    view_splitter_->addWidget(text_view_);
    main_layout_->addWidget(view_splitter_);

    ui_->centralWidget->setLayout(main_layout_);
}

void MainWindow::showPageImage(Page * page) {
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
    QMessageBox::critical(this, tr("Recognition error"), msg);
}

void MainWindow::showSettings() {
    Q_CHECK_PTR(image_widget_);

    Settings s;
    int state = s.exec();

    if(state == QDialog::Accepted)
        image_widget_->updateSettings();
}

void MainWindow::showScanDialog()
{
    ScannerDialog dlg;
    dlg.exec();
}

void MainWindow::showViewContentOnly()
{
    thumbs_->hide();
}

void MainWindow::showViewThumbnails()
{
    thumbs_->show();
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

void MainWindow::debugShowCImage()
{
#ifndef NDEBUG
    CImageView v;
    v.exec();
#endif
}

#ifndef NDEBUG
void MainWindow::addDebugMenu()
{
    QMenu * debug_menu = menuBar()->addMenu("Debug");
    debug_menu->addAction("Show CImage", this, SLOT(debugShowCImage()));
    debug_menu->addAction("Log", this, SLOT(showLog()));
}
#endif

void MainWindow::writeSettings()
{
    QSettings s;
    s.beginGroup("MainWindow");

#ifdef Q_WS_MAC
    // qt incorretly calculates window height
    // with tool bar
    static const int MAC_MAGICK_HEIGHT_FIX = 76;
    QSize window_sz = size();
    window_sz.rheight() += MAC_MAGICK_HEIGHT_FIX;
    s.setValue("size", window_sz);
#else
    settings.setValue("size", size());
#endif

    s.setValue("pos", pos());
    s.endGroup();

    s.beginGroup("language");
    s.setValue("lastLanguage", lang_select_->currentLanguage().code());
    s.endGroup();

    s.beginGroup("view");
    s.setValue("split", view_splitter_->orientation() == Qt::Vertical
               ? QString("Horizontal")
                 : QString("Vertical"));

    s.setValue("split_state", view_splitter_->saveState());
    s.setValue("hide_thumbnails", thumbs_->isHidden());
    s.endGroup();
}

void MainWindow::recognitionSettings()
{
    RecognitionSettingsDialog dialog;
    dialog.setup(packet_->pages());
    dialog.exec();
}

void MainWindow::setupDefaultLanguage()
{
    QSettings settings;
    settings.beginGroup("language");
    int lcode = settings.value("lastLanguage", -1).toInt();

    Language default_lang;

    if(lcode == -1)
        default_lang = Language::fromIsoCode2(QLocale::languageToString(QLocale().language()));
    else
        default_lang = Language(lcode);

    if(!default_lang.isValid())
        default_lang = Language::english();

    lang_select_->select(default_lang);
    lang_menu_->select(default_lang);
}
