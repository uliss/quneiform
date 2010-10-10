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

#include <QtCore/QDebug>
#include "document.h"
#include "page.h"


Document::Document(QObject * parent) :
    QObject(parent), language_(-1) {
}

Document::~Document() {

}

void Document::append(Page * page) {
    for (PageList::const_iterator it = pages_.begin(); it != pages_.end(); ++it) {
        if ((*it)->imagePath() == page->imagePath())
            emit imageDuplicated(page->imagePath());
    }

    pages_.append(page);
    page->setParent(this);
    emit
    pageAdded(page);

    qDebug() << "[Document::append()]";
}

int Document::countSelected() const {
    int res = 0;
    foreach(Page * page, pages_){
        if (page->isSelected())
            res++;
    }
    return res;
}

int Document::language() const {
	return language_;
}

Page * Document::page(int index) {
    if (0 <= index && index < pages_.size())
        return pages_.at(index);
    else
        return 0;
}

int Document::pageCount() const {
    return pages_.count();
}

void Document::recognize(Page * page) {
    Q_CHECK_PTR(page);
    page->recognize();
    emit pageRecognized(page);
}

void Document::recognizeAll() {
    foreach(Page * page, pages_) {
    	recognize(page);
    }

    emit allPagesRecognized();
}

void Document::recognizeSelected() {
    foreach(Page * page, pages_) {
    	if (!page->isSelected())
    		continue;
    	recognize(page);
    }

    emit allPagesRecognized();
}

void Document::remove(Page * page) {
    if (pages_.indexOf(page) < 0)
        return;

    emit pageRemoved(page);
    pages_.removeAll(page);
    delete page;
    qDebug() << "[Document::remove()]";
}

void Document::removeSelected() {
	foreach(Page * page, pages_) {
		if (page->isSelected())
			remove(page);
    }
}

void Document::setLanguage(int lang) {
	// TODO check
        language_ = lang;
}
