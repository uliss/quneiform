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

#include "membuffer.h"

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

	void analyze();
	void close();
	void open(char * dib);
	void recognize();
	void save(const std::string& outputFilename, int format) const;
public:
	static const size_t MainBufferSize = 500000;
	static const size_t WorkBufferSize = 180000;
private:
	void binarizeImage();
	void clearAll();
	void extractComponents();
	void extractStrings();
	void format();
	void layout();
	void makeStrings();
	void modulesDone();
	void modulesInit();
	void normalize();
	void pageMarkup();
	void preOpenInitialize();
	void postOpenInitialize();
	void readLayoutFromFile(const std::string& fname);
	void recognizeSetup();
	void recognizeStrings();
	void recognizeStringsPass1();
	void recognizeStringsPass2();
	void saveLayoutToFile(const std::string& fname);
	void saveToText(const std::string& filename) const;
	void specialProject();
private:
	MemBuffer<MainBufferSize> main_buffer_;
	MemBuffer<WorkBufferSize> work_buffer_;
	static std::string layout_filename_;
};

}

#endif /* PUMAIMPL_H_ */
