/*
 * cifconfig.h
 *
 *  Created on: 14.10.2009
 *      Author: uliss
 */

#ifndef CIFCONFIG_H_
#define CIFCONFIG_H_

#include "singleton.h"

namespace CIF {

class ConfigImpl
{
public:
    ConfigImpl() :
        debug_(true), debug_level_(DEBUG_HIGH) {
    }

    enum debug_level_t
    {
        DEBUG_NONE = 0,
        DEBUG_LOW = 2,
        DEBUG_MEDIUM = 4,
        DEBUG_HIGH = 8
    };

    bool debugLow() const {
        return debug() && debugLevel() >= DEBUG_LOW;
    }

    bool debugMedium() const {
        return debug() && debugLevel() >= DEBUG_MEDIUM;
    }

    bool debugHigh() const {
        return debug() && debugLevel() >= DEBUG_HIGH;
    }

    bool debug() const {
        return debug_;
    }

    int debugLevel() const {
        return debug_level_;
    }

    void setDebug(bool value) {
        debug_ = value;
        if (!debug_)
            debug_level_ = 0;
    }

    void setDebugLevel(int level) {
        debug_level_ = level;
    }
private:
    bool debug_;
    int debug_level_;
};

typedef Singleton<ConfigImpl> Config;

}

#endif /* CIFCONFIG_H_ */
