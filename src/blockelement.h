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
#ifndef CIFBLOCKELEMENT_H
#define CIFBLOCKELEMENT_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "element.h"

namespace CIF
{

typedef std::vector<ElementPtr> ElementList;
typedef Iterator<Element*> EIterator;

/**
 * @class BlockIterator defines iterator for BlockElement
 */
class BlockIterator : public EIterator
{
        typedef ElementList::iterator iterator;
        ElementList * m_list;
        iterator m_it;
    public:
        BlockIterator(ElementList * list);

        /**
         * Returns current element, pointed by iterator
         * @return pointer to element
         */
        Element * current();

        /**
         * Sets interator to first position, of pointed container
         */
        void first();

        /**
         * Checks if there's no more elements in container
         * @return true, if there's no more element in container
         */
        bool isDone() const;

        /**
         * Moves iterator to next element
         */
        void next();
};

/**
 * @class BlockRIterator defines reverse iterator for BlockElement
 */
class BlockRIterator : public EIterator
{
        typedef ElementList::reverse_iterator iterator;
        ElementList * m_list;
        iterator m_it;
    public:
        BlockRIterator(ElementList * list);

        /**
         * Returns current element, pointed by iterator
         * @return pointer to element
         */
        Element * current();

        /**
         * Sets interator to first position, of pointed container
         */
        void first();

        /**
         * Checks if there's no more elements in container
         * @return true, if there's no more element in container
         */
        bool isDone() const;

        /**
         * Moves iterator to next element
         */
        void next();
};

typedef boost::scoped_ptr<BlockIterator> BlockIteratorPtr;
typedef boost::scoped_ptr<BlockRIterator> BlockRIteratorPtr;
typedef BlockIteratorPtr BPtr;

/**
 * @author Serj Poltavskiy <serge.uliss at gmail.com>
 */
class BlockElement : public Element
{
    public:
        BlockElement() {}
        ~BlockElement() {}
        virtual Element * back();
        void clear();
        BlockIterator * createIterator() const;
        BlockRIterator * createRIterator() const;
        bool empty() const;
        virtual void exportTo(const Exporter& exp) const;
        virtual Element * front();
        virtual void insert(const ElementPtr& ptr, size_t pos);
        virtual void pop();
        virtual void push(const ElementPtr& ptr);
        virtual void remove(size_t pos);
        size_t size() const;
    private:
        mutable ElementList m_children;
        virtual bool isValid(const ElementPtr& el) const;
};

template<class T>
class FilteredIterator : public Iterator<Element*>
{
        EIterator * bit_;
        typedef ElementList::iterator iterator;
    public:
        FilteredIterator(EIterator * bit) : bit_(bit) {
            first();
        }

        /**
         * Returns current element
         * @return
         */
        T * current() {
            return static_cast<T*>(bit_->current());
        }

        /**
         * Sets iterator to first element of type T
         */
        void first() {
            bit_->first();
            while (!bit_->isDone()) {
                if (dynamic_cast<T*>(bit_->current()) != 0)
                    break;
                bit_->next();
            }
        }

        /**
          * Checks if there's no more elements of type T in container
          * @return true, if there's no more element in container
          */
        bool isDone() const {
            return bit_->isDone();
        }

        /**
         * Moves iterator to next element of type T in container
         */
        void next() {
            while (!bit_->isDone()) {
                bit_->next();
                if (bit_->isDone() || (dynamic_cast<T*>(bit_->current()) != 0))
                    break;
            }
        }

};

}

#endif
