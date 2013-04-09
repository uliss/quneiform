/*
   Copyright (C) 2007 Kåre Särs <kare.sars@kolumbus.fi>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef LABELED_COMBO_H
#define LABELED_COMBO_H

#include <QFrame>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>

/**
 * A label and a combobox.
 */
class LabeledCombo : public QFrame
{
    Q_OBJECT

        public:
   /**
    * create a label and combobox combination.
    * \param parent parent widget
    * \param label is the lext for the label
    * \param list a stringlist with values the list should contain.
    */
    LabeledCombo(QWidget *parent, const QString& label, const QStringList& list);

   /**
    * \return the active text
    */
    QString currentText() const;

   /**
    * \return the text at position i
    */
    QString text(int i) const;

   /**
    * \return the number of list entries.
    */
    int count() const;
    void wigetSizeHints(int *lab_w, int *cmb_w);
    void setColumnWidths(int lab_w, int cmb_w);

public slots:
    /**
     * Add string entries to the combobox
     */
    void addItems(const QStringList &list) {
        combo->addItems(list);
    }
    /**
     * Remove all string entries
     */
    void clear(void) {combo->clear();}
   /**
    * If the given string can be found in the comobox, activate that entry.
    * If not, the entry is not changed.
    */
    void setCurrentText( const QString &);

   /**
    * set the current item of the combobox.
    */
    void setCurrentIndex(int);

   /**
    * add an icon for a string in the combobox
    * @param icon the icon to set.
    * @param str the string for which the pixmap should be set.
    */
    bool setIcon(const QIcon& icon, const QString& str);


private slots:
    void prActivated(const QString &);
    void prActivated(int);

signals:
    void activated(int);
    void activated(const QString&);

private:
    QLabel *label;
    QComboBox *combo;
    QGridLayout *layout;
};

#endif
