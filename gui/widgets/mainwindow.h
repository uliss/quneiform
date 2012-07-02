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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Language;
class Packet;
class LanguageMenu;
class LanguageSelect;
class OpenProgressDialog;
class Page;
class PageRecognitionQueue;
class RecentMenu;
class ImageWidget;
class ThumbnailList;
class TextEditor;
class QProgressDialog;
class QHBoxLayout;
class QSplitter;

class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    /**
      * Opens given files
      */
    void open(const QStringList& paths);

    /**
      * Recognizes all opened images
      */
    void recognizeAll();
protected:
    void closeEvent(QCloseEvent * event);
private slots:
    void about();
    void disableViewActions();
    void disableZoomInAction();
    void disableZoomOutAction();
    void enableViewActions();
    void enableZoomActions();
    void handleReportBug();
    void handleShowFullScreen();
    void handleShowMinimized();
    void handleViewSplitChange();
    void imageDuplication(const QString& path);
    void openImages();
    void openImages(const QStringList& paths);
    void openPacket();
    void openPacket(const QString& path);
    void openRecentImage(const QString& path);
    void packetChange();
    void packetSave();
    void recognitionSettings();
    void recognizePage(Page * page);
    void recognizePages(const QList<Page*>& pages);
    void rotate(int angle);
    void rotateLeft();
    void rotateRight();
    void savePacket();
    void savePacket(const QString& path);
    void savePage(Page * page);
    void selectLanguage(const Language& lang);
    void showLog();
    void showPageFault(Page * page);
    void showPageImage(Page * page);
    void showPageText(Page * page);
    void showSettings();
    void showScanDialog();
    void showViewContentOnly();
    void showViewThumbnails();
    void updateCurrentLanguage(Page * p);
    void updateCurrentPage();
    void updatePageDocument(Page*);

    void debugShowCImage();
private:
#ifndef NDEBUG
    void addDebugMenu();
#endif

    void addRecentMenu(QMenu * menu);
    bool confirmRotation(Page * p);
    bool confirmRotationRecognized();
    bool confirmRotationSelected();
    void changePacketLanguage(const Language& lang);
    void connectActions();
    void connectThumbs();
    void enableViewActions(bool value);
    bool openImage(const QString& path, bool allowDuplication = false);
    bool openMultiPage(const QString& path);
    void readSettings();
    void setupDefaultLanguage();
    void setupPacket();
    void setupImageView();
    void setupLanguageMenu();
    void setupLanguageSelect();
    void setupLanguageUi();
    void setupRecent();
    void setupRecentImages();
    void setupRecentPackets();
    void setupRecognitionQueue();
    void setupShortcuts();
    void setupTextView();
    void setupThumbs();
    void setupViewSplit();
    void setupUi();
    void setupUiLayout();
    void writeSettings();
protected:
    void showEvent(QShowEvent * event);
private:
    Ui::MainWindow * ui_;
    Packet * packet_;
    LanguageSelect * lang_select_;
    LanguageMenu * lang_menu_;
    OpenProgressDialog * progress_;
    ImageWidget * image_widget_;
    TextEditor * text_view_;
    ThumbnailList * thumbs_;
    QHBoxLayout * main_layout_;
    PageRecognitionQueue * recognition_queue_;
    RecentMenu * recent_packets_;
    RecentMenu * recent_images_;
    QSplitter * view_splitter_;
};

#endif // MAINWINDOW_H
