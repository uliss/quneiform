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
#include "pumadef.h"

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
	void save(const std::string& outputFilename, puma_format_t format) const;
	void saveToMemory(void * dest, size_t size, puma_format_t format) const;
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
	void rout(const std::string& fname, puma_format_t format) const;
	void routToMemory(void * dest, size_t size, puma_format_t format) const;
	void saveLayoutToFile(const std::string& fname);
	void saveToText(const std::string& fname) const;
	void specialProject();
private:
	MemBuffer<MainBufferSize> main_buffer_;
	MemBuffer<WorkBufferSize> work_buffer_;
	static std::string layout_filename_;
};

}

#endif /* PUMAIMPL_H_ */
