/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <memory>

namespace cf
{

template<class T>
class CreateUsingNew
{
    public:
        static T * create() {
            return new T;
        }
};

template<class T>
class CreateUsingStatic
{
    public:
        static T * create() {
            static T obj;
            return &obj;
        }
};

template<class T>
class CreateUsingNewSmartPtr
{
    public:
        static T * create() {
            static std::auto_ptr<T> ptr_;
            T * tmp = new T;
            ptr_.reset(tmp);
            return ptr_.get();
        }
};

template < class T, template<class > class CreationPolicy = CreateUsingNew >
class Singleton
{
    public:
        static T& instance();
    private:
        Singleton();
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);
        static T * instance_;
};

template<class T, template<class > class CreationPolicy>
T * Singleton<T, CreationPolicy>::instance_ = 0;

template<class T, template<class > class CreationPolicy>
T& Singleton<T, CreationPolicy>::instance()
{
    if (!instance_) {
        instance_ = CreationPolicy<T>::create();
    }

    return *instance_;
}

}

#endif /* SINGLETON_H_ */
