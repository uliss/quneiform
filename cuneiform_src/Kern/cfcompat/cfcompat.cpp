/*
 * cfcompat.cpp
 *
 *  Created on: 20.09.2009
 *      Author: uliss
 */

#include "cfcompat.hpp"
#include "config.h"

namespace CIF {

std::string InstallPath()
{
	return INSTALL_DATADIR;
}

std::string MakePath(const std::string& dir, const std::string& basename,
		const std::string& ext) {
	const char DIRSEP = '/';
	std::string r(dir);
	if (!r.empty() && DIRSEP == r[r.length() - 1])
		r[r.length() - 1] = DIRSEP;

	r += basename;
	if (!ext.empty()) {
		if (ext[0] != '.')
			r += ".";
		r += ext;
	}
	return r;
}

}
