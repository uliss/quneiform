/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>

class Page;
class QTextDocument;

class TextEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEditor(QWidget * parent = 0);
    ~TextEditor();
    void showPage(Page * page);
signals:
    void charSelected(const QRect& bbox);
protected:
    void contextMenuEvent(QContextMenuEvent * event);
private slots:
    void clearText();
    void showCurrentChar();
private:
    void connectPageSignal(Page * page);
private:
    Page * page_;
    QTextDocument * doc_;
};

#endif // TEXTEDITOR_H
