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
#include "blockelement.h"
#include <cassert>
#include <iostream>

using namespace std;

namespace CIF
{

BlockIterator::BlockIterator(ElementList * list) : m_list(list)
{
    assert(m_list != 0);
    m_it = m_list->begin();
}

Element * BlockIterator::current()
{
    return m_it->get();
}

void BlockIterator::first()
{
    assert(m_list != 0);
    m_it = m_list->begin();
}

bool BlockIterator::isDone() const
{
    assert(m_list);
    return m_it == m_list->end();
}

void BlockIterator::next()
{
    ++m_it;
}

BlockRIterator::BlockRIterator(ElementList * list) : m_list(list)
{
    assert(m_list != 0);
    m_it = m_list->rbegin();
}

Element * BlockRIterator::current()
{
    return m_it->get();
}

void BlockRIterator::first()
{
    assert(m_list != 0);
    m_it = m_list->rbegin();
}

bool BlockRIterator::isDone() const
{
    assert(m_list);
    return m_it == m_list->rend();
}

void BlockRIterator::next()
{
    ++m_it;
}

Element * BlockElement::back()
{
    return m_children.back().get();
}

void BlockElement::clear()
{
    m_children.clear();
}

BlockIterator * BlockElement::createIterator() const
{
    return new BlockIterator(&m_children);
}


BlockRIterator * BlockElement::createRIterator() const
{
    return new BlockRIterator(&m_children);
}

void BlockElement::exportTo(const Exporter& exp) const
{
    for (ElementList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
        (*it)->exportTo(exp);
}

bool BlockElement::empty() const
{
    return m_children.empty();
}

Element* BlockElement::front()
{
    return m_children.front().get();
}

void BlockElement::insert(const ElementPtr & ptr, size_t pos)
{
    if (!isValid(ptr)) {
        cerr << "BlockElement::insert Warning! trying to insert wrong Element type ("
        << typeid(*ptr.get()).name() << " to " << typeid(*this).name() << ")" << endl;
        return;
    }
    m_children.insert(m_children.begin() + pos, ptr);
}

void BlockElement::pop()
{
    assert(m_children.empty());
    m_children.pop_back();
}

void BlockElement::push(const ElementPtr & ptr)
{
    if (!isValid(ptr)) {
        cerr << "BlockElement::insert Warning! trying to insert wrong Element type ("
        << typeid(*ptr.get()).name() << " to " << typeid(*this).name() << ")" << endl;
        return;
    }
    m_children.push_back(ptr);
}

void BlockElement::remove(size_t pos)
{
    assert(pos < m_children.size());
    m_children.erase(m_children.begin() + pos);
}

size_t BlockElement::size() const
{
    return m_children.size();
}

bool BlockElement::isValid(const ElementPtr & el) const
{
    return false;
}

}


