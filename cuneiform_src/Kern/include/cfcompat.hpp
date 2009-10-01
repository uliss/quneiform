/*
 * cfcompat.hpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CFCOMPAT_HPP_
#define CFCOMPAT_HPP_

#include <string>
#include "globus.h"

namespace CIF {

FUN_EXPO__ std::string InstallPath();

FUN_EXPO__ std::string MakePath(const std::string& dir,
		const std::string& basename, const std::string& ext = "");
}

#endif /* CFCOMPAT_HPP_ */
