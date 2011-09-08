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

#ifndef PERCENTCOUNTER_H
#define PERCENTCOUNTER_H

#include <boost/function.hpp>

#include "globus.h"

namespace cf {

typedef boost::function<void (unsigned char)> PercentCallback;

class CLA_EXPO PercentCounter
{
public:
    PercentCounter(PercentCounter * parent = NULL);
    virtual ~PercentCounter() {}

    /**
      * Adds value to counter
      * @see get()
      */
    void add(unsigned char perc);

    /**
      * Returns counter contribution to parent counter
      * @see setContribution()
      */
    unsigned char contribution() { return contrib_; }

    /**
      * Returns counter value
      * @see add()
      */
    unsigned char get() const { return perc_; }

    /**
      * Returns pointer to parent counter
      * @see setParent()
      */
    PercentCounter * parent() { return parent_; }

    /**
      * Resets counter value
      */
    void reset();

    /**
      * Resets counter callback
      */
    void resetCallback() { cb_ = PercentCallback(); }

    /**
      * Sets counter callback - called on every counter change
      */
    void setCallback(PercentCallback cb) { cb_ = cb; }

    /**
      * Sets counter callback - called on every counter change
      */
    template<class T>
    void setCallback(T * obj, void (T::*cb)(unsigned char));

    /**
      * Sets counter contribution to parent counter
      * @see contribution()
      */
    void setContribution(unsigned char contrib);

    /**
      * Sets pointer to parent counter
      * @see parent()
      */
    void setParent(PercentCounter * parent);
private:
    PercentCallback cb_;
    PercentCounter * parent_;
    unsigned char perc_;
    unsigned char contrib_;
};

template<class T>
inline void PercentCounter::setCallback(T * obj, void (T::*callback)(unsigned char)) {
    cb_ = std::bind1st(std::mem_fun(callback), obj);
}

}

#endif // PERCENTCOUNTER_H
