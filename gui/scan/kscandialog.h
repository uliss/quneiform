/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#ifndef KSCANDIALOG_H
#define KSCANDIALOG_H

#include <QDialog>

namespace KSaneIface {
class KSaneWidget;
}

class QHBoxLayout;

class KScanDialog : public QDialog
{
    Q_OBJECT
public:
    KScanDialog(QWidget * parent = 0);
    ~KScanDialog();
    QString imagePath() const;
    void run();
signals:
    void pageSaved(const QString& path);

private Q_SLOTS:
    void imageReady(QByteArray& data, int width, int height, int bytes_per_line, int format);
private:
    QString autosaveDir() const;
    QString autosaveImageName(const QString& dir) const;
    QString makeFullAutosaveName(const QString& dir) const;
    void initLayout();
    void initUi();
    bool autoSaveImage(const QString& path);
    bool chooseScanner(QString * device);
    bool openScanner(const QString& device);
private:
    KSaneIface::KSaneWidget * sane_widget_;
    QHBoxLayout * layout_;
    QImage * image_;
    QString saved_;
};

#endif // KSCANDIALOG_H
