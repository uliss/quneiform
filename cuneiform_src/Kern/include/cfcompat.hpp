/*
 * cfcompat.hpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#ifndef CFCOMPAT_HPP_
#define CFCOMPAT_HPP_

#include <string>
#include <cstdlib>
#include "globus.h"

namespace CIF {

template<class T>
T * malloc(size_t n = 1) {
	return static_cast<T*>(::malloc(n * sizeof(T)));
}

template <class T>
T * calloc(size_t n = 1) {
	return static_cast<T*>(::calloc(n, sizeof(T)));
}

FUN_EXPO__ std::string InstallPath();

FUN_EXPO__ std::string MakePath(const std::string& dir,
		const std::string& basename, const std::string& ext = "");
}

#endif /* CFCOMPAT_HPP_ */
