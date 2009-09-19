/***************************************************************************
 *   Copyright (C) 2008 by Сергей Полтавский   *
 *   poltava@gentoo   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CIFITERATOR_H
#define CIFITERATOR_H

namespace CIF
{

/**
 @author Serj Poltavskiy <serge.uliss at gmail.com>
*/
template<class T>
class Iterator
{
    public:
        Iterator() {}
        virtual ~Iterator() {}
        virtual T current() = 0;
        virtual void first() = 0;
        virtual bool isDone() const = 0;
        virtual void next() = 0;
    private:
        Iterator(const Iterator&);
        Iterator& operator=(const Iterator&);
};

template<class T>
class NullIterator : public Iterator<T>
{
    public:
        T current() {
            return 0;
        }

        void first() {}

        bool isDone() const {
            return true;
        }

        void next() {}
};

}

#endif
