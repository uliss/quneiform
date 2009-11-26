/*
 * singleton.h
 *
 *  Created on: 21.09.2009
 *      Author: uliss
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <memory>

namespace CIF
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
