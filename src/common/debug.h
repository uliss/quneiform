/*
 * debug.h
 *
 *  Created on: 19.10.2009
 *      Author: uliss
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>

#include "singleton.h"

namespace CIF {

template<bool>
class DebugImpl
{
public:
    DebugImpl() {
        os_ = &std::cerr;
    }

    ~DebugImpl() {
        os_->flush();
    }

    template<class T>
    std::ostream& operator<<(const T& val) {
        (*os_) << val;
        return *os_;
    }

    void setOutput(std::ostream& os) {
        os_->flush();
        os_ = &os;
    }
private:
    std::ostream * os_;
};

template<>
template<class T>
std::ostream& DebugImpl<false>::operator<<(const T&) {
    return *os_;
}

typedef DebugImpl<true> DebugImplSelected;

inline DebugImplSelected& Debug() {
    return Singleton<DebugImplSelected>::instance();
}

}

#endif /* DEBUG_H_ */
