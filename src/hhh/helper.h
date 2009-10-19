/*
 * helper.h
 *
 *  Created on: 25.09.2009
 *      Author: uliss
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <sstream>

#ifndef NDEBUG
#define DBG(msg) std::cerr << msg << std::endl;
#else
#define DBG(msg) ;
#endif

namespace CIF {

inline std::string replaceFileExt(const std::string& filename,
		const std::string& new_ext) {
	return filename.substr(0, filename.rfind('.')) + new_ext;
}

inline std::string removeFileExt(const std::string& filename) {
    return filename.substr(0, filename.rfind('.'));
}

template<class T>
std::string toString(const T& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}
}

#endif /* HELPER_H_ */
