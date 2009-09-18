/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//#include "lnslang.h"

	#ifndef __PUANSO_H
	#	include "puanso.h"
	#endif

	static double delta1; 	// д [x2-x1]
	static double delta2; 	// д [x2э-x1э]
	static double delta3;		// д [x2*x2*x2-x1*x1*x1]
		// for next values sum is for y = [0..curLine]

	static double MYc;     	// д delta1(y)*y
	static double MXc_2;   	// д delta2(y)
		// tensor of inertia on absolute coords;
	static double Mxx;			//	д [delta1(y)*yэ]
	static double Mxy_2;		// д [delta2(y)*y]
	static double Myy_6;		// д [delta3(y)]
		// tensor of inertia on center mass coords;
	double relMxx;		//	д [delta1(y)*yэ]
	double relMxy;		// д [delta2(y)*y]
	double relMyy;		// д [delta3(y)]

	double xx1, xx2, yy;
	static double sum;

void TPuanso::flashPuanso( void ){
	delta1 = delta2 = delta3 = mass = MYc = MXc_2 = Mxx = Mxy_2 = Myy_6 = 0;
   left = 0x7FF0;
	right = 0;
//		down = 0x7FF0;
//		up = 0;
}

void TPuanso::increase( int x1, int x2, int y ){

   left = MIN(x1, left);
   right = MAX(x2, right);
//   down = MIN(y, down );
//   up  = MAX(y, up);
	xx1 = (double)x1;
	xx2 = (double)x2;
	yy  = (double)y;

	sum = xx1 + xx2;
	assert(sum>=0);
	delta1 = xx2 - xx1 + (double)1.0;
	assert(delta1>0.0);
	delta2 = delta1*sum;
	delta3 = delta1*(	(double)2.0*(sum*sum - xx2 * xx1 - xx2 - 2.0*xx1 + (double)1.0) +
							(double)3.0*(sum-(double)1.0) +
							(double)1.0
						 );
/********
	delta3 = delta1*(	2*(sum*sum - (x2+2)*(x1+1) + 4 ) +
							3*(sum-1) +
							1.0
						 );
************/
	mass	+= delta1;
	MXc_2 += delta2;
	MYc	+= delta1 * yy;
	Mxx   += delta1 * yy * yy;
	Mxy_2 += delta2 * yy;
	Myy_6	+= delta3;
}

void TPuanso::atOwnCoords( void ){
	if (mass <=0)
      return;

	Xc = MXc_2 / (2.0 * mass);
	Yc = MYc / mass;

	relMxx = Mxx- mass*Yc*Yc;
	relMxy =  ( Mxy_2 / (double)2.0 ) - mass*Xc*Yc;
	relMyy = ( Myy_6 / (double)6.0 ) - mass*Xc*Xc;

	double relMyy_xx = relMyy-relMxx;
	if ( fabs(relMyy_xx) < ZERO_DIVIDER ){
		Phi = STRANGE_PHI;
		mainMxx = 1;
		mainMyy = 1;
	}
	else {
		Phi =  (relMxy)/(relMyy_xx);
		/* tg2Phi =  (2 * relMxy)/(Myy-Mxx); */
		/* from this moment Phi assumed near 0 */
		mainMxx = relMxx + (relMyy-relMxx)*Phi*Phi - relMxy*Phi*(double)2.0;
		mainMyy = relMyy - (relMyy-relMxx)*Phi*Phi + relMxy*Phi*(double)2.0;
	};

   if (1 + right-left > 0)
      aveW = mass / (1 + right-left);
   else
      aveW = 1.;
}

Bool TPuanso::isALine( void )
{
   int len = 1 + right-left;
   if (len <=0)
      return FALSE;

   if (aveW > 16.)
      return FALSE;  // too wide

   if (Phi > (double)MAX_ABS_INCLINE )
      return FALSE;
   if (Phi < -(double)MAX_ABS_INCLINE )
      return FALSE;
   if (len  > 64)
   {
      if ( mainMyy < mainMxx * (double)MAX_MyyMxx_RATIO )
      {
         return FALSE;
      }
   }
   else
   {
      if (len  < aveW * 4.)
         return FALSE;
   }

   if (aveW < 0.1)
      return FALSE;

/* 04 oct 98
   double aveWpow3 = aveW * aveW * aveW;
   double Cubus = mainMxx / aveWpow3;
   if (Cubus > 1500) // VP: before 16 sep 98 == 1000
   {
      return FALSE;
   }
*/
   /////////////////////////
   return TRUE;
}
