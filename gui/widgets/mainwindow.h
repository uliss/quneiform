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
class QTimer;

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
      * @see recognizeOthers()
      */
    void recognizeAll();

    /**
     * Recognizes all pages that were not recognized
     * @see recognizeAll()
     */
    void recognizeOthers();
protected:
    void closeEvent(QCloseEvent * event);
private slots:
    void about();
    void autosavePacket();
    void disableViewActions();
    void disableZoomInAction();
    void disableZoomOutAction();
    void enableViewActions();
    void enableZoomActions();
    void exportPacket();
    void handlePacketChanged();
    void handlePacketSaved();
    void handleReportBug();
    void handleShowFullScreen();
    void handleShowMinimized();
    void handleViewSplitChange();
    void imageDuplication(const QString& path);
    void openImagesDialog();
    void openPacketDialog();
    void openPacket(const QString& path);
    void openRecentImage(const QString& path);
    void recognitionSettings();
    void recognizePage(Page * page);
    void recognizePages(const QList<Page*>& pages);
    void rotate(int angle);
    void rotateLeft();
    void rotateRight();
    void savePacket();
    void savePacket(const QString& path);
    void savePacketAs();
    void savePage(Page * page);
    void segmentPage(Page * page);
    void selectLanguage(const Language& lang);
    void showLog();
    void showPageFault(Page * page);
    void showPageImage(Page * page);
    void showPageText(Page * page);
    void showPreferences();
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
    QString openImageDefaultDir() const;
    QString openPacketDefaultDir() const;
    bool openMultiPage(const QString& path);
    void readSettings();
    void setupAutosaveTimer();
    void setupDefaultLanguage();
    void setupPacket();
    void setupIcons();
    void setupImageView();
    void setupLanguageMenu();
    void setupLanguageSelect();
    void setupLanguageUi();
    void setupLogAction();
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
    QStringList supportedImagesFilter() const;
    void updateAutosaveTimer();
    void updateLayoutVisibility();
    void updatePreferences();
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
    QTimer * autosave_timer_;
};

#endif // MAINWINDOW_H
