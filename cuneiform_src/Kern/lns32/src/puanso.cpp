/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé 
        îòêàç îò ãàðàíòèé. 
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â 
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.  
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò 
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ 
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ. 

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ, 
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ 
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ 
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ 
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ 
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ, 
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ 
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ, 
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ 
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

#ifndef PPS_MAC
#pragma hdrstop
#endif

	#ifndef __PUANSO_H
	#	include "puanso.h"
	#endif

	static double delta1; 	// ä [x2-x1]
	static double delta2; 	// ä [x2ý-x1ý]
	static double delta3;		// ä [x2*x2*x2-x1*x1*x1]
		// for next values sum is for y = [0..curLine]

	static double MYc;     	// ä delta1(y)*y
	static double MXc_2;   	// ä delta2(y)
		// tensor of inertia on absolute coords;
	static double Mxx;			//	ä [delta1(y)*yý]
	static double Mxy_2;		// ä [delta2(y)*y]
	static double Myy_6;		// ä [delta3(y)]
		// tensor of inertia on center mass coords;
	double relMxx;		//	ä [delta1(y)*yý]
	double relMxy;		// ä [delta2(y)*y]
	double relMyy;		// ä [delta3(y)]

	double xx1, xx2, yy;
	static double sum;

void TPuanso::flashPuanso( void ){
	delta1 = delta2 = delta3 = mass = MYc = MXc_2 = Mxx = Mxy_2 = Myy_6 = 0;
   left = 0x7FF0;
	right = 0;
//		down = 0x7FF0;
//		up = 0;
};

void TPuanso::increase( int x1, int x2, int y ){

   left = min(x1, left);
   right = max(x2, right);
//   down = min(y, down );
//   up  = max(y, up);
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
};

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
};

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
};
