/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include <QGraphicsSceneEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QKeyEvent>

#include "selectionlist.h"
#include "selection.h"

#include "resources/cursor_selection_area.xpm"
#include "resources/cursor_selection_area_add.xpm"

static const QColor SHADOW(0, 0, 0, 30);
static const int MIN_SELECTION_WIDTH = 8;
static const int MIN_SELECTION_HEIGHT = 8;

SelectionList::SelectionList(const QRectF& rect, QGraphicsItem * parent) :
    QGraphicsRectItem(rect, parent),
    rubber_band_(NULL),
    selection_type_(SELECT_NONE),
    mode_(MODE_NONE),
    turned_(false)
{
    setActive(true);
    setPen(QPen(Qt::NoPen));
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void SelectionList::setSelectionType(SelectionList::selection_t type)
{
    selection_type_ = type;
}

void SelectionList::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QGraphicsRectItem::paint(painter, option, widget);

    if(selections_.empty())
        return;

    QPainterPath path;
    path.addRect(scene()->sceneRect());

    foreach(Selection * s, selections_) {
        path.addRect(s->rect());
    }

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(SHADOW);
    painter->drawPath(path);
    painter->restore();
}

QRect SelectionList::selectionBoundingRect() const
{
    QRectF res;

    foreach(Selection * s, selections_) {
        if(res.isNull())
            res = s->normalRect();

        res.unite(s->normalRect());
    }

    return QRect((int) res.left(), (int) res.top(), (int) res.width(), (int) res.height());
}

QList<QRect> SelectionList::selectionRects() const
{
    QList<QRect> res;

    foreach(Selection * s, selections_) {
        QRectF rf = s->normalRect();
        res.append(QRect((int) rf.left(), (int) rf.top(), (int) rf.width(), (int) rf.height()));
    }

    return res;
}

void SelectionList::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    // resize only with left button
    if(event->button() & Qt::LeftButton)
        return;

    if(selection_type_ != SELECT_NONE) {
        resizeSelection(event);
        event->accept();
    }
    else {
        QGraphicsRectItem::mouseMoveEvent(event);
    }
}

void SelectionList::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    // selection only with left button
    if(event->button() != Qt::LeftButton)
        return;

    if(selection_type_ != SELECT_NONE) {
        startSelection(event);
        event->accept();
    }
    else {
        QGraphicsRectItem::mousePressEvent(event);
    }
}

void SelectionList::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if(event->button() != Qt::LeftButton)
        return;

    if(selection_type_ != SELECT_NONE) {
        finishSelection(event);
        event->accept();
    }
    else {
        QGraphicsRectItem::mouseReleaseEvent(event);
    }
}

void SelectionList::addSelection(const QRectF& r)
{
    foreach(Selection * s, selections_) {
        if(s->rect().intersects(r)) {
            qDebug() << Q_FUNC_INFO << "intersected selection";
        }
    }

    selections_.append(makeSelection(r));

    emit changed();
}

void SelectionList::clearSelections()
{
    foreach(Selection * s, selections_) {
        scene()->removeItem(s);
    }

    selections_.clear();

    emit changed();
}

bool SelectionList::isTurned() const
{
    return turned_;
}

void SelectionList::setTurned(bool value)
{
    turned_ = value;
}

void SelectionList::setSelectionMode(SelectionList::selection_mode_t mode)
{
    mode_ = mode;

    switch(mode) {
    case MODE_ADD:
        setCursor(QCursor(QPixmap(cursor_selection_area_add_xpm)));
        break;
    case MODE_REPLACE:
        setCursor(QCursor(QPixmap(cursor_selection_area_xpm)));
        break;
    default:
        unsetCursor();
        break;
    }
}

void SelectionList::setSelection(const QRectF& r)
{
    clearSelections();
    selections_.append(makeSelection(r));

    emit changed();
}

void SelectionList::handleSelectionDelete(Selection * s)
{
    if(!selections_.contains(s))
        return;

    delete s;
    selections_.removeAll(s);
    updateSelections();
}

Selection * SelectionList::makeSelection(const QRectF& r)
{
    Selection * s = new Selection(this, r);
    s->setAcceptHoverEvents(true);
    s->setZValue(1);

    switch(selection_type_) {
    case SELECT_AREA:
        s->setSelectionType(Selection::AREA);
        break;
    case SELECT_TEXT:
        s->setSelectionType(Selection::TEXT);
        break;
    case SELECT_IMAGE:
        s->setSelectionType(Selection::IMAGE);
        break;
    case SELECT_TABLE:
        s->setSelectionType(Selection::TABLE);
        break;
    default:
        s->setSelectionType(Selection::AREA);
        break;
    }

    return s;
}

Selection * SelectionList::selectionAt(int i)
{
    if(i < selections_.size())
        return selections_.at(i);
    return NULL;
}

void SelectionList::updateSelections()
{
    emit changed();
}

void SelectionList::finishSelection(QGraphicsSceneMouseEvent * event)
{
    removeRubberBand();

    QRectF rect(selection_start_, event->pos());
    rect = rect.normalized();
    rect = scene()->sceneRect().intersected(rect);

    qDebug() << Q_FUNC_INFO << "selection created: " << rect;

    if(rect.width() < MIN_SELECTION_WIDTH)
        rect.adjust(-MIN_SELECTION_WIDTH, 0, MIN_SELECTION_WIDTH, 0);
    if(rect.height() < MIN_SELECTION_HEIGHT)
        rect.adjust(0, -MIN_SELECTION_HEIGHT, 0, MIN_SELECTION_HEIGHT);
    rect = scene()->sceneRect().intersected(rect);

    switch(mode_) {
    case MODE_REPLACE:
        setSelection(rect);
        break;
    case MODE_ADD:
        addSelection(rect);
        break;
    default:
        break;
    }

    selection_type_ = SELECT_NONE;
    mode_ = MODE_NONE;
    unsetCursor();
}

void SelectionList::startSelection(QGraphicsSceneMouseEvent * event)
{
    selection_start_ = event->pos();

    // remove previous
    removeRubberBand();
    addRubberBand();
}

void SelectionList::addRubberBand()
{
    rubber_band_ = new QGraphicsRectItem(QRectF(), this);
    rubber_band_->setPen(QPen(Qt::DotLine));
}

void SelectionList::removeRubberBand()
{
    if(rubber_band_) {
        scene()->removeItem(rubber_band_);
        delete rubber_band_;
        rubber_band_ = NULL;
    }
}

void SelectionList::resizeSelection(QGraphicsSceneMouseEvent * event)
{
    Q_CHECK_PTR(rubber_band_);

    rubber_band_->setRect(QRectF(selection_start_, event->pos()));
}

void SelectionList::keyPressEvent(QKeyEvent * event)
{
    QGraphicsRectItem::keyPressEvent(event);

    if(selection_type_ == SELECT_NONE)
        return setSelectionMode(MODE_NONE);

    if(event->key() == Qt::Key_Control)
        setSelectionMode(MODE_ADD);
}

void SelectionList::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsRectItem::keyReleaseEvent(event);

    if(selection_type_ == SELECT_NONE)
        return setSelectionMode(MODE_NONE);

    if(event->key() == Qt::Key_Control)
        setSelectionMode(MODE_REPLACE);
}
