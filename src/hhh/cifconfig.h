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

class ConfigImpl {
public:
	ConfigImpl() :
		verbose_(false) {
	}

	bool verbose() const {
		return verbose_;
	}

	void setVerbose(bool value) {
		verbose_ = value;
	}
private:
	bool verbose_;
};

typedef Singleton<ConfigImpl> Config;

}

#endif /* CIFCONFIG_H_ */
