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


#ifndef PAGERECOGNIZER_H
#define PAGERECOGNIZER_H

#include <QThread>
#include <QString>
#include <QImage>

class Page;

class PageRecognizer : public QThread
{
    Q_OBJECT
public:
    explicit PageRecognizer(Page * p, QObject * parent = NULL);
    void run();
    void setLanguage(int language);
    void setPage(Page * p);
signals:
    void failed(const QString& msg);
    void finished(int percent);
private:
    void doRecognize();
    void formatResult();
    bool isFormatConvertionNeeded(int format) const;
    QImage loadImage() const;
    void openImage();
    void recognize();
    void saveOcrText();
    void setRecognizeOptions();
private:
    Page * page_;
    int language_;
};

#endif // PAGERECOGNIZER_H
