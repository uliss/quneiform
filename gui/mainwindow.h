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

class Packet;
class LanguageMenu;
class LanguageSelect;
class Page;
class PageRecognitionQueue;
class RecentMenu;
class ImageWidget;
class ThumbnailList;
class TextEditor;
class QProgressDialog;
class QHBoxLayout;

class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);
public slots:
    void about();
    void imageDuplication(const QString& path);
    void open(const QStringList& paths);
    void openImages();
    void openImages(const QStringList& paths);
    void openPacket();
    void openPacket(const QString& path);
    void packetChange();
    void packetSave();
    void recognizeAll();
    void recognizePage(Page * page);
    void rotate(int factor);
    void rotateLeft();
    void rotateRight();
    void savePacket();
    void savePacket(const QString& path);
    void savePage(Page * page);
    void showPageImage(Page * page);
    void showPageText(Page * page);
private slots:
    void disableViewActions();
    void disableZoomInAction();
    void disableZoomOutAction();
    void enableViewActions();
    void enableZoomActions();
    void openRecentImage(const QString& path);
    void selectLanguage(int lang);
    void showPageFault(Page * page);
    void showSettings();
    void updateCurrentPage();
private:
    void addRecentMenu(QMenu * menu);
    void changePacketLanguage(int lang);
    void connectActions();
    void connectThumbs();
    void enableViewActions(bool value);
    bool openImage(const QString& path, bool allowDuplication = false);
    void readSettings();
    void setupPacket();
    void setupImageView();
    void setupLanguageMenu();
    void setupLanguageSelect();
    void setupLanguageUi();
    void setupOpenProgress();
    void setupRecent();
    void setupRecentImages();
    void setupRecentPackets();
    void setupRecognitionQueue();
    void setupShortcuts();
    void setupTextView();
    void setupThumbs();
    void setupUi();
    void setupUiLayout();
    void writeSettings();
private:
    Ui::MainWindow * ui_;
    Packet * packet_;
    LanguageSelect * lang_select_;
    LanguageMenu * lang_menu_;
    QProgressDialog * progress_;
    ImageWidget * image_widget_;
    TextEditor * text_view_;
    ThumbnailList * thumbs_;
    QHBoxLayout * main_layout_;
    PageRecognitionQueue * recognition_queue_;
    RecentMenu * recent_packets_;
    RecentMenu * recent_images_;
};

#endif // MAINWINDOW_H
