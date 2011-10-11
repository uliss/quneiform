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
#include <QMenu>
#include <QDebug>

#include "packet.h"
#include "page.h"
#include "thumbnaillist.h"
#include "thumblayout.h"
#include "thumbscene.h"
#include "thumbnailwidget.h"
#include "quneiform_debug.h"

static const int LIST_WIDTH = 170;
static const QString ICON_THUMB_DELETE(":/img/oxygen/22x22/list_remove.png");
static const QString ICON_THUMB_SAVE_AS(":/img/oxygen/22x22/document_save_as.png");
static const QString ICON_THUMB_RECOGNIZE(":/img/oxygen/22x22/document_preview.png");
static const QString ICON_THUMB_PROPERTIES(":/img/oxygen/22x22/document_properties.png");

ThumbnailList::ThumbnailList(QWidget * parent) :
    QGraphicsView(parent),
    packet_(NULL),
    current_page_(NULL),
    scene_(NULL),
    layout_(NULL),
    context_thumb_(NULL),
    act_select_all_(NULL),
    act_recognize_(NULL),
    act_save_as_(NULL),
    act_properties_(NULL),
    act_delete_(NULL)
{
    setAcceptDrops(true);
    setupLayout();
    setupScrollBars();
    setupActions();
    setupScene();
}

void ThumbnailList::thumbAppend(ThumbnailWidget * thumb) {
    Q_CHECK_PTR(thumb);

    layout_->append(thumb);
    scene_->addItem(thumb);

    connect(thumb, SIGNAL(clicked(int)), SLOT(handleThumbClick(int)));
    connect(thumb, SIGNAL(dragged(ThumbnailWidget*, QPointF)), SLOT(handleThumbDrag(ThumbnailWidget*, QPointF)));
    connect(thumb, SIGNAL(dropped(ThumbnailWidget*, QPointF)), SLOT(handleThumbDrop(ThumbnailWidget*, QPointF)));
    connect(thumb, SIGNAL(showPageFault(Page*)), SIGNAL(showPageFault(Page*)));
    connect(thumb, SIGNAL(createContextMenu(ThumbnailWidget*,QPoint)), SLOT(handleThumbContextMenu(ThumbnailWidget*,QPoint)));
}

Page * ThumbnailList::currentPage() {
    return current_page_;
}

void ThumbnailList::pageAdd(Page * page) {
    if(!page) {
        CF_ERROR("NULL page pointer given");
        return;
    }

    thumbAppend(new ThumbnailWidget(page));
}

void ThumbnailList::pageRemove(Page * page) {
    ThumbnailWidget * th = layout_->findByPage(page);
    if(th)
        thumbRemove(th);
}

void ThumbnailList::thumbRemove(ThumbnailWidget * thumb) {
    Q_CHECK_PTR(thumb);
    Page * page = thumb->page();

    if(current_page_ == page)
    	current_page_ = NULL;

    layout_->remove(thumb);
    scene_->removeItem(thumb);

    updateLayout();
}

void ThumbnailList::selectAll() {
    layout_->selectAll();
}

void ThumbnailList::setPacket(Packet * packet) {
    Q_CHECK_PTR(packet);

    packet_ = packet;
    connect(packet_, SIGNAL(pageAdded(Page*)), SLOT(pageAdd(Page*)));
    connect(packet_, SIGNAL(pageRemoved(Page*)), SLOT(pageRemove(Page*)));
    connect(packet_, SIGNAL(reorder()), SLOT(handleThumbReorder()));
    connect(packet_, SIGNAL(opened()), SLOT(updateLayout()));
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

void ThumbnailList::setupScrollBars() {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setAttribute(Qt::WA_StaticContents);
}

void ThumbnailList::handleThumbClick(ThumbnailWidget * thumb, int modifiers) {
    Q_CHECK_PTR(layout_);

    if (!thumb)
        return;

    if(modifiers == Qt::ControlModifier) {
        layout_->multiSelect(thumb);
    }
    else if(modifiers == Qt::ShiftModifier) {
        layout_->selectRange(thumb);
    }
    else {
        layout_->select(thumb);
        current_page_ = thumb->page();
        emit showPage(current_page_);
    }
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

void ThumbnailList::handleThumbDrag(ThumbnailWidget * sender, const QPointF& scenePos)
{
    setDragCursor(true);

    ThumbnailWidget * thumb = findThumbByPos(scenePos);

    if(!thumb || (thumb == sender)) {
        setDragCursor(false);
        return;
    }

    layout_->highlightAll(false);
    thumb->highlight(true);
}

void ThumbnailList::handleThumbDrop(ThumbnailWidget * sender, const QPointF& scenePos)
{
    Q_CHECK_PTR(sender);

    setDragCursor(false);
    layout_->highlightAll(false);

    ThumbnailWidget * thumb = findThumbByPos(scenePos);

    if(!thumb || (thumb == sender))
        return;

    packet_->moveBefore(sender->page(), thumb->page());
}

void ThumbnailList::updateLayout()
{
    Q_CHECK_PTR(scene_);
    Q_CHECK_PTR(layout_);

    scene_->setSceneRect(QRectF(QPointF(), layout_->size()));
}

void ThumbnailList::handleThumbReorder()
{
    Q_CHECK_PTR(layout_);
    Q_CHECK_PTR(packet_);

    layout_->sortByPages(packet_->pages());
}

bool ThumbnailList::isValidDropPos(const QPointF& scenePos)
{
    Q_CHECK_PTR(layout_);
    Q_CHECK_PTR(scene_);

    if(layout_->count() < 2)
        return false;

    if(!scene_->sceneRect().contains(scenePos))
        return false;

    return true;
}

ThumbnailWidget * ThumbnailList::findThumbByPos(const QPointF& scenePos)
{
    Q_CHECK_PTR(scene_);

    if(!isValidDropPos(scenePos))
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

void ThumbnailList::handleThumbContextMenu(ThumbnailWidget * sender, const QPoint& pos)
{
    context_thumb_ = sender;

    QMenu * menu = new QMenu(NULL);
    menu->addAction(act_select_all_);
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
    act_delete_ = new QAction(QIcon(ICON_THUMB_DELETE), tr("Delete"), this);
    act_delete_->setShortcut(Qt::CTRL + Qt::Key_Backspace);
    act_delete_->setShortcutContext(Qt::WidgetShortcut);
    connect(act_delete_, SIGNAL(triggered()), SLOT(contextThumbRemove()));

    addAction(act_delete_);
}

void ThumbnailList::setupActionProperties()
{
    act_properties_ = new QAction(QIcon(ICON_THUMB_PROPERTIES), tr("Properties"), this);
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
    act_select_all_ = new QAction(tr("Select all"), this);
    act_select_all_->setShortcut(QKeySequence::SelectAll);
    connect(act_select_all_, SIGNAL(triggered()), SLOT(selectAll()));
    addAction(act_select_all_);
}

void ThumbnailList::setupActionSaveAs()
{
    act_save_as_ = new QAction(QIcon(ICON_THUMB_SAVE_AS), tr("Save as"), this);
    act_save_as_->setShortcut(QKeySequence::SaveAs);
    act_save_as_->setShortcutContext(Qt::WidgetShortcut);
    connect(act_save_as_, SIGNAL(triggered()), SLOT(contextThumbSave()));
    addAction(act_save_as_);
}

void ThumbnailList::setupActionRecognize()
{
    act_recognize_ = new QAction(QIcon(ICON_THUMB_RECOGNIZE), tr("Recognize"), this);
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
            emit recognizePage(context_thumb_->page());

        return;
    }

    // if have selected thumbs - recognize all
    QList<Page*> pages;
    foreach(ThumbnailWidget * t, selected) {
        pages.append(t->page());
    }

    emit recognizePages(pages);
}

void ThumbnailList::contextThumbSave()
{
    if(context_thumb_)
        emit savePage(context_thumb_->page());
}

void ThumbnailList::setDragCursor(bool value)
{
    setCursor(value ? Qt::OpenHandCursor : Qt::ArrowCursor);
}
