/*
 * pumaimpl.cpp
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#include "pumaimpl.h"
#include "puma.h"

namespace CIF {

PumaImpl::PumaImpl() {
	if (!PUMA_Init())
		throw PumaException("Puma init failed");

//	PUMA_SetImportData(PUMA_Word32_Language, &langcode);
//	PUMA_SetImportData(PUMA_Bool32_DotMatrix, &dotmatrix);
//	PUMA_SetImportData(PUMA_Bool32_Fax100, &fax);
//	PUMA_SetImportData(PUMA_Bool32_OneColumn, &onecolumn);

}

PumaImpl::~PumaImpl() {
	PUMA_Done();
}

void PumaImpl::close() {
	PUMA_XClose();
}

void PumaImpl::open(char * dib) {
	PUMA_XOpen(dib, "none.txt");
}

void PumaImpl::recognize() {
	PUMA_XFinalRecognition();
}

}
