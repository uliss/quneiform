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

#ifndef THUMBNAILWIDGET_H_
#define THUMBNAILWIDGET _H_

#include <QString>
#include <QFrame>
#include <QPixmap>

class QLabel;
class QMouseEvent;
class QCheckBox;
class QVBoxLayout;
class QMenu;

class Page;
class PageIndicator;
class ThumbnailList;

// ThumbnailWidget represents a single thumbnail in the ThumbnailList
class ThumbnailWidget: public QFrame
{
    Q_OBJECT
public:
    ThumbnailWidget(Page * page, ThumbnailList * parent);

    /**
      * Highlights thumbnail
      * @see setChecked()
      */
    void highlight(bool value);

    /**
      * Checks if thumb is checked
      */
    bool isChecked() const;

    /**
      * Returns pointer to thumb page
      */
    Page * page() const;

    /**
      * Checks thumbnail checkbox
      */
    void setChecked(bool value);

    /**
      * Sets thumbnail name
      */
    void setName(const QString& name);

    QSize sizeHint() const;

    /**
      * Toggles thumb selection
      */
    void toggleSelection();
signals:
    void clicked();
    void contextMenuCreated(QMenu*);
    void invalidImage(const QString& path);
    void recognize(Page*);
    void removed(Page*);
    void save(Page*);
    void showPageFault(Page*);
    void toggled(bool);
protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void mousePressEvent(QMouseEvent * event);
private:
    QPixmap makeThumb() const;
    void setupActions();
    void setupCheckBox();
    void setupFrame();
    void setupIndicator();
    void setupLabel();
    void setupLayout();
    void setupPixmap();
    void setupToolTip();
private slots:
    void pageFaultForward();
    void recognizeThumb();
    void removePage();
    void handlePageRotate();
    void savePage();
    void selectPage(bool value);
    void showProperties();
    void showFormatSettings();
    void showRecognizeSettings();
    void updatePageIndicators();
private:
    Page * page_;
    QVBoxLayout * layout_;
    QLabel * thumb_;
    QCheckBox * checked_;
    PageIndicator * indicator_;
    QAction * act_recognize_;
    QAction * act_save_as_;
    QAction * act_properties_;
    QAction * act_delete_;
};

#endif /* THUMBNAILWIDGET_H_ */
