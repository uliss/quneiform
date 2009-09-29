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
	static unsigned char * mainBuffer();
	static unsigned char * workBuffer();
	static const size_t MainBufferSize = 500000;
	static const size_t WorkBufferSize = 180000;
private:
	void binarizeImage();
	void clearAll();
	void layout();
	void modulesDone();
	bool preOpenInitialize();
	bool postOpenInitialize(const char*);
private:
	static unsigned char * main_buffer_;
	static unsigned char * work_buffer_;
private:
	static void freeMainBuffer();
	static void freeWorkBuffer();
	static void initMainBuffer();
	static void initWorkBuffer();
};

}

#endif /* PUMAIMPL_H_ */
