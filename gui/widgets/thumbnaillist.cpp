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

#include <QScrollBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QToolButton>
#include <QDebug>
#include <QUrl>

#include "packet.h"
#include "page.h"
#include "thumbnaillist.h"
#include "thumblayout.h"
#include "thumbscene.h"
#include "thumbnailwidget.h"
#include "widgetbar.h"

static const int LIST_WIDTH = 170;

ThumbnailList::ThumbnailList(QWidget * parent) :
    QGraphicsView(parent),
    packet_(NULL),
    layout_(NULL),
    current_page_(NULL),
    select_all_(NULL),
    act_recognize_(NULL),
    act_save_as_(NULL),
    act_properties_(NULL),
    act_delete_(NULL),
    scene_(NULL),
    context_thumb_(NULL),
    drag_in_progress_(false)
{
    setAcceptDrops(true);
    setupLayout();
    setScrollBars();
    setupActions();
    setupScene();
}

void ThumbnailList::append(ThumbnailWidget * thumb) {
    Q_CHECK_PTR(thumb);

    layout_->append(thumb);
    scene_->addItem(thumb);

    connect(thumb, SIGNAL(clicked(int)), SLOT(thumbClick(int)));
    connect(thumb, SIGNAL(dragged(ThumbnailWidget*, QPointF)), SLOT(handleThumbDrag(ThumbnailWidget*, QPointF)));
    connect(thumb, SIGNAL(dropped(ThumbnailWidget*, QPointF)), SLOT(handleThumbDrop(ThumbnailWidget*, QPointF)));
    connect(thumb, SIGNAL(showPageFault(Page*)), SIGNAL(showPageFault(Page*)));
    connect(thumb, SIGNAL(createContextMenu(ThumbnailWidget*,QPoint)), SLOT(showThumbContextMenu(ThumbnailWidget*,QPoint)));

    updateThumbNames();
}

int ThumbnailList::count() const {
    Q_CHECK_PTR(layout_);

    return layout_->count();
}

Page * ThumbnailList::currentPage() {
    return current_page_;
}

void ThumbnailList::pageAdd(Page * page) {
    Q_CHECK_PTR(page);

    ThumbnailWidget * thumb = new ThumbnailWidget(page);
    append(thumb);
}

void ThumbnailList::pageRemove(Page * page) {
    ThumbnailWidget * th = thumb(page);
    if(th)
        thumbRemove(th);
}


void ThumbnailList::removeSelectedPages(Page * page) {
    QList<ThumbnailWidget*> selected = layout_->selected();

    if(selected.isEmpty()) {
        packet_->remove(page);
        return;
    }

    foreach(ThumbnailWidget * t, selected) {
        packet_->remove(t->page());
    }
}

void ThumbnailList::thumbRemove(ThumbnailWidget * thumb) {
    Q_CHECK_PTR(thumb);
    Page * page = thumb->page();

    if(current_page_ == page)
    	current_page_ = NULL;

    layout_->remove(thumb);
    scene_->removeItem(thumb);

    updateThumbNames();
    updateLayout();

    emit thumbRemovalFinished(page);
}

void ThumbnailList::selectAll() {
    layout_->selectAll();
}

void ThumbnailList::setDocument(Packet * doc) {
    Q_CHECK_PTR(doc);

    packet_ = doc;
    connect(packet_, SIGNAL(pageAdded(Page*)), SLOT(pageAdd(Page*)));
    connect(packet_, SIGNAL(pageRemoved(Page*)), SLOT(pageRemove(Page*)));
    connect(packet_, SIGNAL(reorder()), SLOT(reorder()));
}

void ThumbnailList::setupActions() {
    setupActionSelectAll();
    setupActionRecognize();
    setupActionSaveAs();
    setupActionProperties();
    setupActionDelete();
}

void ThumbnailList::setupLayout() {
    setFixedWidth(LIST_WIDTH);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout_ = new ThumbLayout;
}

void ThumbnailList::setScrollBars() {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //verticalScrollBar()->setEnabled(false);
    setAttribute(Qt::WA_StaticContents);
}

ThumbnailWidget * ThumbnailList::thumb(Page * page) {
    Q_CHECK_PTR(layout_);

    return layout_->findByPage(page);
}

void ThumbnailList::thumbClick(int modifiers) {
    ThumbnailWidget * th = qobject_cast<ThumbnailWidget*> (QObject::sender());
    if (!th)
        return;

    if(modifiers == Qt::ControlModifier) {
        layout_->multiSelect(th);
    }
    else if(modifiers == Qt::ShiftModifier) {
        layout_->selectRange(th);
    }
    else {
        layout_->select(th, true);
        current_page_ = th->page();
        emit thumbSelected(current_page_);
    }
}

void ThumbnailList::updateThumbNames() {
    for (int i = 0; i < layout_->count(); i++)
        layout_->at(i)->setName(QString("%1").arg(i + 1));
}

void ThumbnailList::setupScene()
{
    scene_ = new ThumbScene(this);
    scene_->setBackgroundBrush(QPalette().background());
    connect(scene_, SIGNAL(dropImages(QStringList)), SIGNAL(openDraggedImages(QStringList)));
    setScene(scene_);
}

void ThumbnailList::setLanguage(const Language &lang)
{
    if(current_page_)
        current_page_->setLanguage(lang);

    foreach(ThumbnailWidget * t, layout_->selected()) {
        t->page()->setLanguage(lang);
    }
}

QList<ThumbnailWidget*> ThumbnailList::selected()
{
    Q_CHECK_PTR(layout_);
    return layout_->selected();
}

void ThumbnailList::handleThumbDrag(ThumbnailWidget * sender, const QPointF& scenePos)
{
    setCursor(Qt::OpenHandCursor);

    ThumbnailWidget * target_thumb = targetDropThumb(scenePos);

    if(!target_thumb)
        return;

    // self drop
    if(target_thumb == sender)
        return;

    drag_in_progress_ = true;

    highlightAll(false);
    target_thumb->highlight(true);
}

void ThumbnailList::handleThumbDrop(ThumbnailWidget * sender, const QPointF& scenePos)
{
    Q_CHECK_PTR(sender);

    setCursor(Qt::ArrowCursor);
    highlightAll(false);

    drag_in_progress_ = false;

    ThumbnailWidget * target_thumb = targetDropThumb(scenePos);

    if(!target_thumb)
        return;

    // self drop
    if(target_thumb == sender)
        return;

    packet_->moveBefore(sender->page(), target_thumb->page());
}

void ThumbnailList::updateLayout()
{
    Q_CHECK_PTR(scene_);
    Q_CHECK_PTR(layout_);

    scene_->setSceneRect(QRectF(QPointF(), layout_->size()));
}

void ThumbnailList::reorder()
{
    Q_CHECK_PTR(layout_);

    layout_->sortByPages(packet_->pages());
    updateThumbNames();
}

bool ThumbnailList::isValidThumbDropPosition(const QPointF& scenePos)
{
    Q_CHECK_PTR(layout_);
    Q_CHECK_PTR(scene_);

    if(layout_->count() < 2)
        return false;

    if(!scene_->sceneRect().contains(scenePos))
        return false;

    return true;
}

ThumbnailWidget * ThumbnailList::targetDropThumb(const QPointF& scenePos)
{
    Q_CHECK_PTR(scene_);

    if(!isValidThumbDropPosition(scenePos))
        return NULL;

    QGraphicsItem * target = scene_->itemAt(scenePos);
    if(!target) {
        qDebug() << "[Warning] no target found at" << scenePos;
        return NULL;
    }

    ThumbnailWidget * target_thumb = dynamic_cast<ThumbnailWidget*>(target->topLevelItem());

    if(!target_thumb) {
        qDebug() << "[Warning] not a thumb found at" << scenePos;
        return NULL;
    }

    return target_thumb;
}

void ThumbnailList::highlightAll(bool value)
{
    Q_CHECK_PTR(layout_);
    layout_->highlightAll(value);
}

void ThumbnailList::showThumbContextMenu(ThumbnailWidget * sender, const QPoint& pos)
{
    context_thumb_ = sender;

    QMenu * menu = new QMenu(NULL);
    menu->addAction(select_all_);
    menu->addAction(act_delete_);
    menu->addSeparator();
    menu->addAction(act_recognize_);
    menu->addAction(act_properties_);
    menu->addSeparator();
    menu->addAction(tr("Recognition settings"), this, SLOT(contextThumbRecognizeSettings()));
    menu->addAction(tr("Format settings"), this, SLOT(contextThumbFormatSettings()));
    menu->addSeparator();
    menu->addAction(act_save_as_);
    menu->exec(pos);
    delete menu;

    context_thumb_ = NULL;
}

void ThumbnailList::contextThumbRemove()
{
    Q_CHECK_PTR(layout_);

    QList<ThumbnailWidget*> selected = layout_->selected();

    // if no thumbs selected and have context thumb
    // remove this one only
    if(selected.isEmpty()) {
        if(context_thumb_)
            packet_->remove(context_thumb_->page());

        return;
    }

    // remove all selected thumbs
    foreach(ThumbnailWidget * t, selected) {
        packet_->remove(t->page());
    }
}

void ThumbnailList::setupActionDelete()
{
    act_delete_ = new QAction(QIcon(":/img/oxygen/22x22/list_remove.png"), tr("Delete"), this);
    act_delete_->setShortcut(Qt::CTRL + Qt::Key_Backspace);
    act_delete_->setShortcutContext(Qt::WidgetShortcut);
    connect(act_delete_, SIGNAL(triggered()), SLOT(contextThumbRemove()));

    addAction(act_delete_);
}

void ThumbnailList::setupActionProperties()
{
    act_properties_ = new QAction(QIcon(":/img/oxygen/22x22/document_properties.png"),
                                  tr("Properties"),
                                  this);
    connect(act_properties_, SIGNAL(triggered()), SLOT(contextThumbProperties()));
    addAction(act_properties_);
}

void ThumbnailList::contextThumbProperties()
{
    if(context_thumb_)
        context_thumb_->showProperties();
}

void ThumbnailList::setupActionSelectAll()
{
    select_all_ = new QAction(tr("Select all"), this);
    select_all_->setShortcut(QKeySequence::SelectAll);
    connect(select_all_, SIGNAL(triggered()), SLOT(selectAll()));
    addAction(select_all_);
}

void ThumbnailList::setupActionSaveAs()
{
    act_save_as_ = new QAction(QIcon(":/img/oxygen/22x22/document_save_as.png"),
                               tr("Save as"),
                               this);
    act_save_as_->setShortcut(QKeySequence::SaveAs);
    act_save_as_->setShortcutContext(Qt::WidgetShortcut);
    connect(act_save_as_, SIGNAL(triggered()), SLOT(contextThumbSave()));
    addAction(act_save_as_);
}

void ThumbnailList::setupActionRecognize()
{
    act_recognize_ = new QAction(QIcon(":/img/oxygen/22x22/document_preview.png"),
                                 tr("Recognize"),
                                 this);
    connect(act_recognize_, SIGNAL(triggered()), SLOT(contextThumbRecognize()));
    act_recognize_->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R));
    addAction(act_recognize_);
}

void ThumbnailList::contextThumbFormatSettings()
{
    if(context_thumb_)
        context_thumb_->showFormatSettings();
}

void ThumbnailList::contextThumbRecognizeSettings()
{
    if(context_thumb_)
        context_thumb_->showRecognizeSettings();
}

void ThumbnailList::contextThumbRecognize()
{
    Q_CHECK_PTR(layout_);

    QList<ThumbnailWidget*> selected = layout_->selected();

    // if no selection and have context thumb
    // recognize context thumb page only
    if(selected.isEmpty()) {
        if(context_thumb_)
            emit thumbRecognize(context_thumb_->page());

        return;
    }

    // if have selected thumbs - recognize all
    QList<Page*> pages;
    foreach(ThumbnailWidget * t, selected) {
        pages.append(t->page());
    }

    emit thumbRecognizeList(pages);
}

void ThumbnailList::contextThumbSave()
{
    if(context_thumb_)
        emit save(context_thumb_->page());
}
