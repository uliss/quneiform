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

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPixmap>
#include <QtGui/QLayout>
#include <QtGui/QMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>
#include <QtGui/QMenu>

#include "thumbnailwidget.h"
#include "thumbnaillist.h"
#include "imagecache.h"
#include "page.h"
#include "pageindicator.h"
#include "recognitionsettingsdialog.h"
#include "formatsettingsdialog.h"

static const int THUMB_IMAGE_HEIGHT = 95;
static const int THUMB_IMAGE_WIDTH = 95;
static const int THUMB_IMAGE_MARGIN = 5;
static const int THUMB_WIDTH = 150;
static const int THUMB_HEIGHT = 150;

ThumbnailWidget::ThumbnailWidget(Page * page, ThumbnailList * parent) :
        QFrame(parent),
        page_(page),
        layout_(NULL),
        thumb_(NULL),
        checked_(NULL),
        act_recognize_(NULL),
        act_save_as_(NULL),
        act_delete_(NULL)
{
    setupFrame();
    setupLayout();
    setupPixmap();
    setupLabel();
    setupToolTip();
    setupIndicator();
    setupCheckBox();
    setupActions();

    connect(checked_, SIGNAL(toggled(bool)), SLOT(selectPage(bool)));
    connect(this, SIGNAL(contextMenuCreated(QMenu*)), parent, SLOT(setupContextMenu(QMenu*)));
    connect(this, SIGNAL(invalidImage(const QString&)), parent, SLOT(handleInvalidImage(const QString&)));
    connect(page, SIGNAL(rotated(int)), SLOT(rotate(int)));
    connect(page, SIGNAL(changed()), SLOT(updatePageIndicators()));

    setFocusPolicy(Qt::ClickFocus);
    updatePageIndicators();
}

void ThumbnailWidget::contextMenuEvent(QContextMenuEvent * event) {
    QMenu * menu = new QMenu(this);
    emit contextMenuCreated(menu);
    menu->addAction(act_delete_);
    menu->addSeparator();
    menu->addAction(act_recognize_);
    menu->addAction(act_properties_);
    menu->addSeparator();
    menu->addAction(tr("Recognition settings"), this, SLOT(showRecognizeSettings()));
    menu->addAction(tr("Format settings"), this, SLOT(showFormatSettings()));
    menu->addSeparator();
    menu->addAction(act_save_as_);
    menu->exec(event->globalPos());
    delete menu;
}

void ThumbnailWidget::savePage() {
    emit save(page_);
}

void ThumbnailWidget::selectPage(bool value) {
    qDebug() << "[ThumbnailWidget::selectPage]" << value;
    page_->setSelected(value);
}

void ThumbnailWidget::showFormatSettings() {
    Q_CHECK_PTR(page_);

    FormatSettingsDialog dialog(page_->formatSettings());
    if(QDialog::Accepted == dialog.exec())
        page_->setFormatSettings(dialog.settings());
}

void ThumbnailWidget::showProperties() {
    Q_CHECK_PTR(page_);
    QMessageBox::information(NULL, tr("Properties - %1").arg(page_->name()), pageProperties());
}

void ThumbnailWidget::showRecognizeSettings() {
    Q_CHECK_PTR(page_);

    RecognitionSettingsDialog dlg(page_->recognitionSettings());
    if(QDialog::Accepted == dlg.exec())
        page_->setRecognitionSettings(dlg.settings());
}

void ThumbnailWidget::highlight(bool value) {
    if (value)
        setBackgroundRole(QPalette::Highlight);
    else
        setBackgroundRole(QPalette::Background);
}

bool ThumbnailWidget::isChecked() const {
    Q_CHECK_PTR(checked_);
    return checked_->isChecked();
}

void ThumbnailWidget::mousePressEvent(QMouseEvent * event) {
    Q_CHECK_PTR(event);

    switch (event->button()) {
    case Qt::LeftButton:
        emit clicked();
        break;
    case Qt::RightButton:
        break;
    default:
        qDebug("Unhandled mouse event");
    }
}

Page * ThumbnailWidget::page() const {
    return page_;
}

void ThumbnailWidget::pageFaultForward() {
    Q_ASSERT(indicator_ == sender());

    if(page_)
        emit showPageFault(page_);
}

QString ThumbnailWidget::pageProperties() const {
    Q_CHECK_PTR(page_);

    QString res = tr("Filename: \"%1\"\n").arg(page_->imagePath());
    res += tr("Size: %1x%2\n").arg(page_->imageSize().width()).arg(page_->imageSize().height());
    res += tr("Rotation: %1\n").arg(page_->angle());

    if(page_->isRecognized())
        res += tr("Page is recognized\n");
    else
        res += tr("Page is not recognized\n");

    if(page_->isExported())
        res += tr("Page is saved\n");
    else
        res += tr("Page is not saved\n");

    return res;
}

void ThumbnailWidget::recognizeThumb() {
    emit recognize(page_);
}

void ThumbnailWidget::rotate(int angle) {
    Q_CHECK_PTR(page_);

    QTransform t;
    QPixmap new_pixmap = thumb_->pixmap()->transformed(t.rotate(angle));
    thumb_->setPixmap(new_pixmap);
}


void ThumbnailWidget::removePage() {
    emit removed(page_);
}

void ThumbnailWidget::setChecked(bool value) {
    Q_CHECK_PTR(checked_);

    checked_->setChecked(value);
}

void ThumbnailWidget::setName(const QString& name) {
    Q_CHECK_PTR(checked_);

    checked_->setText(name);
}

void ThumbnailWidget::setupActions() {
    act_recognize_ = new QAction(QIcon(":/img/oxygen/22x22/document_preview.png"),
                                 tr("Recognize"),
                                 this);
    connect(act_recognize_, SIGNAL(triggered()), this, SLOT(recognizeThumb()));
    act_recognize_->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R));
    act_recognize_->setShortcutContext(Qt::WidgetShortcut);
    addAction(act_recognize_);

    act_save_as_ = new QAction(QIcon(":/img/oxygen/22x22/document_save_as.png"),
                               tr("Save as"),
                               this);
    act_save_as_->setShortcut(QKeySequence::SaveAs);
    act_save_as_->setShortcutContext(Qt::WidgetShortcut);
    connect(act_save_as_, SIGNAL(triggered()), this, SLOT(savePage()));
    addAction(act_save_as_);

    act_properties_ = new QAction(QIcon(":/img/oxygen/22x22/document_properties.png"),
                                  tr("Properties"),
                                  this);
    connect(act_properties_, SIGNAL(triggered()), this, SLOT(showProperties()));
    addAction(act_properties_);

    act_delete_ = new QAction(QIcon(":/img/oxygen/22x22/list_remove.png"), tr("Delete"), this);
    act_delete_->setShortcut(Qt::CTRL + Qt::Key_Backspace);
    act_delete_->setShortcutContext(Qt::WidgetShortcut);
    connect(act_delete_, SIGNAL(triggered()), this, SLOT(removePage()));
    addAction(act_delete_);
}

void ThumbnailWidget::setupCheckBox() {
    Q_CHECK_PTR(layout_);
    Q_CHECK_PTR(page_);

    checked_ = new QCheckBox;
    checked_->setChecked(page_->isSelected());
    layout_->addWidget(checked_, 0, Qt::AlignHCenter);
}

void ThumbnailWidget::setupFrame() {
    setAutoFillBackground(true);
    //setFrameShape(QFrame::Box);
}

void ThumbnailWidget::setupIndicator() {
    indicator_ = new PageIndicator(this);
    indicator_->move(0, height() - indicator_->height());
    connect(indicator_, SIGNAL(showWarningDetails()), SLOT(pageFaultForward()));
//    layout_->addWidget(indicator_, 0, Qt::AlignHCenter);
}

void ThumbnailWidget::setupLabel() {
    Q_CHECK_PTR(page_);
    Q_CHECK_PTR(layout_);

    //page_name_ = new QLabel;
    //page_name_->setTextFormat(Qt::PlainText);
    //layout_->addWidget(page_name_, 0, Qt::AlignHCenter);
}

void ThumbnailWidget::setupLayout() {
    layout_ = new QVBoxLayout(this);
    layout_->setSpacing(0);
    layout_->setMargin(0);
    setFixedWidth(THUMB_WIDTH);
    setFixedHeight(THUMB_HEIGHT);
}

void ThumbnailWidget::setupPixmap() {
    Q_CHECK_PTR(page_);
    Q_CHECK_PTR(layout_);

    QPixmap image;
    if(!ImageCache::load(page_->imagePath(), &image)) {
        qDebug() << "[ThumbnailWidget::setupPixmap] can't load pixmap";
        return;
    }

    thumb_ = new QLabel;
    thumb_->setMargin(THUMB_IMAGE_MARGIN);
    QTransform thumb_rotate;
    thumb_rotate.rotate(page_->angle());
    image = image.transformed(thumb_rotate);
    if(image.height() > image.width())
        image = image.scaledToHeight(THUMB_IMAGE_HEIGHT);
    else
        image = image.scaledToWidth(THUMB_IMAGE_WIDTH);

    thumb_->setPixmap(image);
    // stretch image
    static const int STRETCH_KOEF = 4;
    layout_->addWidget(thumb_, STRETCH_KOEF, Qt::AlignHCenter);
}

void ThumbnailWidget::setupToolTip() {
    Q_CHECK_PTR(page_);

    setToolTip(page_->imagePath());
}

void ThumbnailWidget::toggleSelection() {
    Q_CHECK_PTR(checked_);

    checked_->toggle();
}

void ThumbnailWidget::updatePageIndicators() {
    Q_CHECK_PTR(indicator_);
    indicator_->setRecognized(page_->isRecognized());
    indicator_->setSaved(page_->hasFlag(Page::EXPORTED));
    indicator_->setWarning(page_->hasFlag(Page::RECOGNITION_FAILED));
}
