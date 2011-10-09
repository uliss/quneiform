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

#include <QDebug>
#include <QMenu>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QApplication>
#include <QDrag>

#include "thumbnailwidget.h"
#include "thumbnaillist.h"
#include "imagecache.h"
#include "page.h"
#include "pageindicator.h"
#include "dialogs/recognitionsettingsdialog.h"
#include "dialogs/formatsettingsdialog.h"
#include "dialogs/pagepropertiesdialog.h"

static const int THUMB_IMAGE_HEIGHT = 100;
static const int THUMB_IMAGE_WIDTH = 100;
static const int THUMB_IMAGE_MARGIN = 4;
static const int THUMB_WIDTH = 150;
static const int THUMB_HEIGHT = 150;
static const int SELECT_FRAME_WIDTH = 2;

class ThumbnailWidget::Pixmap : public QGraphicsPixmapItem {
public:
    Pixmap(const QPixmap& p, QGraphicsItem * parent) :
        QGraphicsPixmapItem(p, parent),
        frame_(NULL)
    {
        setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
        frame_ = new QGraphicsRectItem(this);
        highlight(false);
        updateFrame();
    }

    void highlight(bool value) {
        if (value) {
            QPalette palette;
            QPen p(palette.highlight(), SELECT_FRAME_WIDTH);
            frame_->setPen(p);
        }
        else {
            frame_->setPen(QPen(QBrush(), 0));
        }
    }

    void setPixmap(const QPixmap& pixmap)  {
        QGraphicsPixmapItem::setPixmap(pixmap);
        updateFrame();
    }
private:
    void updateFrame() {
        frame_->setRect(boundingRect().adjusted(-1,-1,1,1));
    }
private:
    QGraphicsRectItem * frame_;
};

class ThumbnailWidget::Label : public QGraphicsSimpleTextItem {
public:
    Label(QGraphicsItem * parent) : QGraphicsSimpleTextItem(parent), highlighted_(false) {}

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
    {
        if(highlighted_) {
            QBrush brush(QPalette().highlight());
            painter->setBrush(brush);
            QPen old_pen = painter->pen();
            QPen p(QBrush(), 0);
            painter->setPen(p);
            painter->drawRect(boundingRect().adjusted(-1, -1, 1, 1));
            painter->setPen(old_pen);
        }

        QGraphicsSimpleTextItem::paint(painter, option, widget);
    }

    bool isHighlighted() const {
        return highlighted_;
    }

    void highlight(bool value) {
        if(highlighted_ == value)
            return;

        highlighted_ = value;
        update();
    }

private:
    bool highlighted_;
};

ThumbnailWidget::ThumbnailWidget(Page * page) :
    page_(page),
    pixmap_(NULL),
    label_(NULL),
    indicator_(NULL),
    drag_progress_(false),
    hightlighted_(false)
{
//    setFlags(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemClipsToShape);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setAcceptDrops(true);

    setupPixmap();
    setupLabel();
    setupToolTip();
    setupIndicator();
    setupActions();

    connect(page, SIGNAL(rotated(int)), SLOT(handlePageRotate()));
    connect(page, SIGNAL(changed()), SLOT(updatePageIndicators()));

    updatePageIndicators();
}

void ThumbnailWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent * event) {
    QMenu * menu = new QMenu(NULL);
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
    menu->exec(event->screenPos());
    delete menu;
    event->accept();
}

void ThumbnailWidget::savePage() {
    emit save(page_);
}

void ThumbnailWidget::showFormatSettings() {
    Q_CHECK_PTR(page_);

    FormatSettingsDialog dialog(page_->formatSettings());
    if(QDialog::Accepted == dialog.exec())
        page_->setFormatSettings(dialog.settings());
}

void ThumbnailWidget::showProperties() {
    Q_CHECK_PTR(page_);
    PagePropertiesDialog d(page_);
    d.exec();
}

void ThumbnailWidget::showRecognizeSettings() {
    Q_CHECK_PTR(page_);

    RecognitionSettingsDialog dlg(page_);
    dlg.exec();
}

void ThumbnailWidget::selectThumb(bool value) {
    Q_CHECK_PTR(pixmap_);
    pixmap_->highlight(value);
    label_->highlight(value);
}

void ThumbnailWidget::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    Q_CHECK_PTR(event);

    switch (event->button()) {
    case Qt::LeftButton:
        drag_start_pos_ = event->pos();
        emit clicked(event->modifiers());
        event->accept();
        break;
    case Qt::RightButton:
        break;
    default:
        qDebug() << Q_FUNC_INFO << "Unhandled mouse event";
    }
}

void ThumbnailWidget::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if(hightlighted_) {
        QBrush brush(QPalette().highlight());
        painter->setBrush(brush);
        QPen old_pen = painter->pen();
        QPen p(QBrush(), 0);
        painter->setPen(p);
        painter->drawRect(boundingRect());
        painter->setPen(old_pen);
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

void ThumbnailWidget::recognizeThumb() {
    emit recognize(page_);
}

void ThumbnailWidget::handlePageRotate() {
    Q_CHECK_PTR(pixmap_);
    Q_CHECK_PTR(page_);

    if(!page_->thumb())
        return;

    pixmap_->setPixmap(*page_->thumb());
    updatePixmapPos();
}

void ThumbnailWidget::removePage() {
    emit removed(page_);
}

void ThumbnailWidget::setName(const QString& name) {
    Q_CHECK_PTR(label_);
    label_->setText(name);
    qreal x = (THUMB_WIDTH - label_->boundingRect().width()) / 2;
    qreal y = label_->pos().y();
    label_->setPos(x, y);
}

void ThumbnailWidget::setupActions() {
    act_recognize_ = new QAction(QIcon(":/img/oxygen/22x22/document_preview.png"),
                                 tr("Recognize"),
                                 this);
    connect(act_recognize_, SIGNAL(triggered()), this, SLOT(recognizeThumb()));
    act_recognize_->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R));
    act_recognize_->setShortcutContext(Qt::WidgetShortcut);
//    addAction(act_recognize_);

    act_save_as_ = new QAction(QIcon(":/img/oxygen/22x22/document_save_as.png"),
                               tr("Save as"),
                               this);
    act_save_as_->setShortcut(QKeySequence::SaveAs);
    act_save_as_->setShortcutContext(Qt::WidgetShortcut);
    connect(act_save_as_, SIGNAL(triggered()), this, SLOT(savePage()));
//    addAction(act_save_as_);

    act_properties_ = new QAction(QIcon(":/img/oxygen/22x22/document_properties.png"),
                                  tr("Properties"),
                                  this);
    connect(act_properties_, SIGNAL(triggered()), this, SLOT(showProperties()));
//    addAction(act_properties_);

    act_delete_ = new QAction(QIcon(":/img/oxygen/22x22/list_remove.png"), tr("Delete"), this);
    act_delete_->setShortcut(Qt::CTRL + Qt::Key_Backspace);
    act_delete_->setShortcutContext(Qt::WidgetShortcut);
    connect(act_delete_, SIGNAL(triggered()), this, SLOT(removePage()));
//    addAction(act_delete_);
}

void ThumbnailWidget::setupIndicator() {
    static const int INDICATOR_MARGIN = 5;
    indicator_ = new PageIndicator(this);
    indicator_->setZValue(20);
    indicator_->setPos(INDICATOR_MARGIN,
                       boundingRect().height() - indicator_->boundingRect().height() - INDICATOR_MARGIN);
    connect(indicator_, SIGNAL(showWarningDetails()), SLOT(pageFaultForward()));
}

void ThumbnailWidget::setupLabel() {
    label_ = new Label(this);
    label_->setPos(THUMB_WIDTH / 2, THUMB_HEIGHT - 30);
}

void ThumbnailWidget::setupPixmap() {
    QPixmap thumb(THUMB_IMAGE_HEIGHT, THUMB_IMAGE_WIDTH);

    if(!page_ || !page_->thumb()) {
        thumb.fill();
    }
    else {
        thumb = *page_->thumb();
    }

    pixmap_ = new Pixmap(thumb, this);
    updatePixmapPos();
}

void ThumbnailWidget::setupToolTip() {
    Q_CHECK_PTR(page_);

    setToolTip(page_->imagePath());
}

void ThumbnailWidget::toggleSelection() { 
    selectThumb(!isThumbSelected());
    update();
}

void ThumbnailWidget::updatePageIndicators() {
    Q_CHECK_PTR(indicator_);
    indicator_->setRecognized(page_->isRecognized());
    indicator_->setSaved(page_->hasFlag(Page::EXPORTED));
    indicator_->setWarning(page_->hasFlag(Page::RECOGNITION_FAILED));
}

QRectF ThumbnailWidget::boundingRect() const
{
    return QRectF(0, 0, THUMB_WIDTH, THUMB_HEIGHT);
}

bool ThumbnailWidget::isThumbSelected() const
{
    Q_CHECK_PTR(label_);
    return label_->isHighlighted();
}

QString ThumbnailWidget::name() const
{
    Q_CHECK_PTR(label_);
    return label_->text();
}

void ThumbnailWidget::updatePixmapPos()
{
    Q_CHECK_PTR(pixmap_);
    pixmap_->setPos((THUMB_WIDTH - pixmap_->boundingRect().width())/2, THUMB_IMAGE_MARGIN);
}

void ThumbnailWidget::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if(!(event->buttons() & Qt::LeftButton))
        return;

    if((event->scenePos() - drag_start_pos_).manhattanLength() < QApplication::startDragDistance())
        return;

    drag_progress_ = true;
    emit dragged(this, event->scenePos());
}

void ThumbnailWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if(!drag_progress_) {
        QGraphicsObject::mouseReleaseEvent(event);
        return;
    }

    drag_progress_ = false;
    emit dropped(this, event->scenePos());
}

void ThumbnailWidget::highlight(bool value)
{
    hightlighted_ = value;
    update();
}
