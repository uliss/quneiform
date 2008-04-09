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

#include "gistoGramma.h"

Int16 *stat_gistoGramma(Int16 *X, Int16 n)
{
	if(X == NULL) return NULL;

	Int16 xMax = X[stat_index_GlobMax(X, n, 1)];
	Int16 xMin = X[stat_index_GlobMin(X, n)];
	Int16   ng = xMax - xMin + 1;

	Int16 *G = (Int16 *) calloc(ng, sizeof(Int16));
	if(G == NULL)
		return NULL;

	for(Int16 i = 0; i < n; i++)
	{
		G[X[i] - xMin]++;
	}
	return G;
}

Int16 stat_index_LeftLocMax(Int16 *Y, Int16 k0)
{
	if(Y == NULL) return -1;

	Int16 ret = -1, max = 0, iMax = 0;
	
	for(Int16 i = 0; i <= k0; i++)
	{
		if(Y[i] > max) 
		{
			max  = Y[i];
			iMax = i;
		}
	}

	ret = max != 0 ? iMax : -1;

	return ret;
}

Int16 stat_index_RightLocMax(Int16 *Y, Int16 k, Int16 k0)
{
	if(Y == NULL) return -1;

	Int16 ret = -1, max = 0, iMax = k0;

	if(k >= k0)
	{
		for(Int16 i = k0; i < k; i++)
		{
			if(Y[i] >= max)  
			{
				max  = Y[i];
				iMax = i;
			}
		}

		ret = max != 0 ?  iMax : -1;
	}

	return ret;
}

Int16 stat_index_GlobMax(Int16 *Y, Int16 k, Int16 inequality)
{
	if(Y == NULL) return -1;

	if(k > 1)
	{
		Int16 max = Y[0], iMax = 0;

		for(Int16 i=1; i<k; i++)
		{
			switch(inequality)
			{
				case 1: if(Y[i] > max)
						{
							max  = Y[i];
							iMax = i;
						}
						break;
				case 2: if(Y[i] >= max)
						{
							max  = Y[i];
							iMax = i;
						}
						break;
				default: ;
			}
		}

		return iMax;
	}
	else
	{
		return 0;
	}
}

Int16 stat_alt_indexMax(Int16 *Y, Int16 k, Int16 diff, Int16 b3)
{
	if(Y == NULL) return -1;

	if(k > 1)
	{
		Int16 max = Y[0], iMax = 0;

		for(Int16 i=1; i<k; i++)
		{
			if(Y[i] > max)
			{
				max  = Y[i];
				iMax = i;
			}
			else
			{
				if(Y[i] == max && b3 - i >= diff)
				{
					max  = Y[i];
					iMax = i;
				}
			}
		}

		return iMax;
	}
	else
	{
		return 0;
	}
}

Int16 stat_index_GlobMin(Int16 *Y, Int16 k)
{
	if(Y == NULL) return -1;
	
	if(k > 1)
	{
		Int16 min = Y[0], iMin = 0;

		for(Int16 i=1; i<k; i++)
		{
			if(Y[i] < min)
			{
				min  = Y[i];
				iMin = i;
			}
		}

		return iMin;
	}
	else
	{
		return 0;
	}
}

