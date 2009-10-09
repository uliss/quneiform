/*
 * pumaimpl.h
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#ifndef PUMAIMPL_H_
#define PUMAIMPL_H_

#include <string>
#include <stdexcept>

#include "cfcompat.h"
#include "rect.h"
#include "memorybuffer.h"

namespace CIF {

struct PumaException: std::runtime_error {
	PumaException(const std::string& msg) :
		std::runtime_error(msg) {
	}
};

class PumaImpl {
public:
	PumaImpl();
	~PumaImpl();

	void close();
	void open(char * dib);
	void recognize();
	void save(const std::string& outputFilename, int format) const;
public:
	static unsigned char * mainBuffer();
	static unsigned char * workBuffer();
	static const size_t MainBufferSize = 500000;
	static const size_t WorkBufferSize = 180000;
private:
	void clearAll();
	void extractComponents();
	void extractStrings();
	void layout();
	void loadLayouFromFile(const std::string& fname);
	void modulesInit();
	void modulesDone();
	void pass1();
	void pass2();
	void spellCorrection();
	void preOpenInitialize();
	void postOpenInitialize();
	void recognizePass1();
	void recognizeSetup(int lang);
	void saveLayoutToFile(const std::string& fname);
	void savePass1(const std::string& fname);
	void setTemplate(const Rect& rect);
private:
	static FixedBuffer<unsigned char, MainBufferSize> main_buffer_;
	static FixedBuffer<unsigned char, WorkBufferSize> work_buffer_;
};

}

#endif /* PUMAIMPL_H_ */
