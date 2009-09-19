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

// RShellLinesCom.h: interface for the ShellLine's functions.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//
//	Description: ShellLine's functions and classes definition
//
//	Implemented: by B.M. Shahverdiev
//
//
#include "globus.h"

#ifdef __RSHELLLINES_EXPORTS__
  #define RSHELLLINES_FUNC  FUN_EXPO
#else
  #define RSHELLLINES_FUNC  FUN_IMPO
#endif


#define VerticalLine     0
#define HorizantalLine   1

#define HalfOtstup       4
#define Otstup           4
#define MaxNumber        4

#define MIN_H_NO_CLEAN  10

#include "cline.h"

#include "cpage.h"

#include "compat_defs.h"
//#include "decl.h"

//#define SecondPath       2
//#define ThirdPath        3

/*
struct CLineImage
{
	int Wide;
	int Height;
	int bytewide;
	uchar* lp;
	int fl_delete;
	uchar* pmasp;
	int size_mas;
	CLineImage();
	~CLineImage();
	int SetDibPtr(uchar* lpDibData,int wide,int heght,int bwide);
	uchar* GetPmasp(Rect32* pRc);
};
*/
struct DOT_STRUCT
 {
  int32_t left;
  int32_t right;
  int32_t top;
  int32_t bottom;
  int32_t i_left;
  int32_t i_top;
  int32_t i_right;
  Bool  flag;
 };
/*
class  CSL_POLY:public CObject
{
public:
  CSL_POLY(){count=4;}
 ~CSL_POLY(){}

 int16_t   count;
 Point32 Vertex[MaxNumber];
};
*/
//extern "C" {

typedef struct group_lines_mass
{
    CLINE_handle* hLinesMass;
    int           numder_of_lines;
    int           number_of_cp;
    int32_t*        m_arCutPointsL;
    int32_t*        m_arCutPointsR;
    int32_t*        m_arFrequency;
    int32_t         m_top;
    int32_t         m_bottom;
    int32_t         GroupOffset;
    int32_t         GroupVerOffset;
} GLM;

/////////////////////////////////////////////////////////////////////////////
// Interval's class
/*class RSHELLLINES_CLASS CInterval:public CObject
{
public:
  CInterval(){}
 ~CInterval(){}

  int32_t  Pos;
  int32_t  Lent;
};
*/
/////////////////////////////////////////////////////////////////////////////
// Rect32's class
/*class RSHELLLINES_CLASS CPRect32:public CObject
{
public:
  CPRect32(){m_flag=TRUE;};
 ~CPRect32(){};

  Rect32  rect;
  Bool    m_flag;
};
*/

/////////////////////////////////////////////////////////////////////////////
// Event's class
/*class RSHELLLINES_CLASS CEvent:public CObject
{
public:
  CEvent();
 ~CEvent();

  Bool            Hori;                    //гориз.(1) или вертик.(0)
  Bool            Increase;                //наращивать или уменьшать уровень
  int32_t           Lev_0;                   //уровень первого интервала
  int             Width;
  int32_t           EventLength;

  CObArray        m_arIntervals;

  int             GetCountIntervals(void);
  CInterval*      GetInterval(int i);
  CInterval*      GetNewInterval(void);
*/
  void            SetEventWidth(CLINE_handle hEvent, DEvent* pEvent, uint32_t* LineHist, uint32_t RectBegin, uint32_t LineLength); //
/*};
*/
/////////////////////////////////////////////////////////////////////////////
// Stripe's class
/*class RSHELLLINES_API CStripe:public CObject
{
public:
  CStripe();
 ~CStripe();

  int32_t           Level;                   //уровень полосы

  CObArray        m_arIntervals;

  int             GetCountIntervals(void);
  CInterval*      GetInterval(int i);
  CInterval*      GetNewInterval(void);
};
*/
/////////////////////////////////////////////////////////////////////////////
// CutPoint's class

//#define DirectedUpwards       0  //defines for Direction
//#define DirecteDownwards      1
//#define DirectedForBothSides  2
/*
class RSHELLLINES_CLASS CCutPoint:public CObject
{
public:
  CCutPoint();
 ~CCutPoint();

  int    Direction;
  int32_t  Level;                            //уровень полосы

  CObArray        m_arIntervals;

  int             GetCountIntervals(void);
  CInterval*      GetInterval(int i);
  CInterval*      GetNewInterval(void);
};
*/
/////////////////////////////////////////////////////////////////////////////
// Component's class

#define  InitialPointHasCrossPoint  1  //defines for Component
#define  MiddleHasCrossPoint        2
#define  EndPointHasCrossPoint      4
/*
class RSHELLLINES_API CComponent:public CObject
{
public:
  CComponent();
 ~CComponent();

  int    CrossPointFlag;
  int32_t  BeginPoint;
  int32_t  EndPoint;
};
*/
/////////////////////////////////////////////////////////////////////////////
// СDotLines's class
/*class RSHELLLINES_API CDotLine:public CObject
{
public:
  CDotLine();
 ~CDotLine();

  CPRect32        m_LeftDot;       // левая точка
  CPRect32        m_RightDot;      // правая точка
  Bool            m_LeftDotState;  //FALSE-линия не имеет лев. продольжение
  Bool            m_RightDotState; //FALSE-линия не имеет прав.продольжение

  CObArray        m_arDots;        // все точки без сортировки
  CWordArray      m_dist;          // все расстояния между точками

  Bool            m_Flag;          //Если == FALSE то плохая точечная линия

  int             GetCountOfDots(void);
  CPRect32*       GetDot(int i);
  void            AddDot(CPRect32*);
  void            SetLeftDot(CPRect32* in_dot);
  void            SetRightDot(CPRect32* in_dot);
};
*/
// For Flags
//#define LI_IsTrue       0x00000040 			// правильно выделенная линия
//#define LI_IsFalse      0x00000080			// не линия
//#define LI_Pointed      0x00001000          // точечная

// For Status
#define ST_Unknown      0x00000000 			//
#define ST_Basil_Short  0x00000001			//
#define ST_Good         0x00000002          //
#define ST_Bad          0x00000004          //

/////////////////////////////////////////////////////////////////////////////
// Line's class
//class CPageLines;
/*
class RSHELLLINES_API CLine:public CObject
{
public:
  CLine();
 ~CLine();
*/
RSHELLLINES_FUNC(void)            InitLine(DLine* linedata);  //
/*  uint32_t          Flags;                   // common use info
  uint32_t          Tail;                    // остаток линии за таблицой
  Point32         BeginPoint;
  Point32         EndPoint;
  Rect32          rect;                    // прямоугольник линии
  Bool            ProcessingType;          // тип обработки линии(гор. или верт.)
  int             Width;
  int32_t           LineEventsLength;        // сумма черных точек линии
  double          Degree;
  int             RelationshipIndex;       // индекс коробки компонент
  Bool            FlagCalculatedWidth;
  Bool            FlagExtensible;
  Bool            FlagDot;                 // устанавливается когда линия точечная
  Bool            FlagBad;
  Bool            FlagLongStrip;
  int             PathNumber;              // номер прохода

  NR_SimpLine     Line;                    // простейшее описание линии
  NR_SimpLine     LineMid;                 // Средние точки линии
  int32_t           Status;                  // история линии в программе
  NORM_DATATYPES  Type;                    // *Unknown, *Simple, *Defis, *Pointed
  LINE_DIR        Dir;
  char            Qual;                    // quality (or estimation) at percents
  char            Dens;                    // density at percents
  void            *Specline;               // (NR_DefiLine), (NR_PoinLine)
  CSL_POLY        poly;

  CObArray        m_arStripes;             // все полосы и интервалы
  CObArray        m_arStripesVerticalLine; // все полосы и интервалы для вертикальной линии
//  CObArray        m_arCorrectedStripes;    // полосы и интервалы после коррекции
  CObArray        m_arEvents;              // фрагменты
  CObArray        m_arCutPoints;           // точки пересечения с ортогональными линиями
  CObArray        m_arComponents;          // компоненты линии с признаками точек пересечения
//take'em away!  CWordArray      m_arSeparationPoints;    // точки деления линии на подлинии(компоненты)
  CWordArray      m_arFriendsIndex;        // индексы дружественных линий

*/
  Bool            SetLineNormDataTypes(NORM_DATATYPES, DLine* line); //
/*  NORM_DATATYPES  GetLineNormDataTypes(void);

  int             GetCountStripes(void);
  CStripe*        GetStripe(int i);
  CStripe*        GetNewStripe();

  int             GetCountStripesVerticalLine(void);
  CStripe*        GetStripeVerticalLine(int i);
*/  void            GetNewStripeVerticalLine(DEvent* eventdata); //

/*  int             GetCountEvents(void);
  CEvent*         GetNewEvent();
  CEvent*         GetEvent(int i);

//  int             GetCountCorrectedStripes(void);
//  CStripe*        GetNewCorrectedStripe();
//  CStripe*        GetCorrectedStripe(int i);

  int             GetCountCutPoints(void);
  CCutPoint*      GetNewCutPoint();
  CCutPoint*      GetCutPoint(int i);

  int             GetCountComponents(void);
  CComponent*     GetNewComponent();
  CComponent*     GetComponent(int i);
*/
  void            SetLineProcessingType(DLine* linedata); //
  void            SetWidth(CLINE_handle line, DLine* pCLine); //
  void            SetLength(CLINE_handle line, DLine* pCLine); //
  void            SetComponentsCrossPointFlag(CLINE_handle line, DLine* pCLine); //

RSHELLLINES_FUNC(Bool32)          CheckSeparationPoints(CLINE_handle hLine, CLINE_handle hComp = NULL); //

  void            SetSeparationPoints(CLINE_handle hContainer, CLINE_handle hLine, DLine* pCLine, Bool32 PLines); //
  void            SetCutPoints(CLINE_handle line, DLine* pCLine); //
  void            SetNewLine(CLINE_handle hContainer, CPDLine pCLine, int32_t IntervalBeg, int32_t IntervalEnd, CLINE_handle hLineOld, Bool32 PossibleSymbolAboveLine); //
  void            SetDegree(DLine* line); //
  Bool            CheckPoint(CLINE_handle line, int CurPoint); //
//  void            CheckShortVerticalLines(CPageLines* PLines);
  void            ConvertStructs(CLINE_handle line, DLine* pCLine, int32_t nIncline); //
  void            FindTailComponent(CLINE_handle line, DLine* pCLine); //
  void            GetCorrectLine_End_X(CLINE_handle line, DLine* pCLine); //
  void            GetComponentBeg_Y_End_Y(int32_t Beg, int32_t End, int32_t* Beg_X, int32_t* End_X, int32_t* Beg_Y, int32_t* End_Y, CLINE_handle hLine, Bool32 ProcessingType); //
  Bool32          IfExistCutLine(CLINE_handle hContainer, DLine* pCLine, int left,int right); //
/*  int             SetVStripeMasMaxSize(int size);
};
*/
/////////////////////////////////////////////////////////////////////////////
// CGroupOfFriendLines class
/*class RSHELLLINES_CLASS CGroupOfFriendLines:public CObject
{
public:
  CGroupOfFriendLines();
 ~CGroupOfFriendLines();

  CWordArray      m_arFriendsIndex;        // индексы дружественных линий (m_arLines)
  CWordArray      m_arCutPointsL;           // точки пересечения (left for interval)
  CWordArray      m_arCutPointsR;           // точки пересечения (right for interval)
  CWordArray      m_arFrequency;           // и частота

  double          Degree;                  // +- 0.05
  Bool            ProcessingType;          // тип обработки линий(гор. или верт.)
  int             RelationshipIndex;       // индекс коробки компонент
  uint16_t            m_top;
  uint16_t            m_bottom;

  int             GetCountOfFriendLines(void);
};
*/
/////////////////////////////////////////////////////////////////////////////
// CGroupOfExtensibleLines class
/*class RSHELLLINES_CLASS CGroupOfExtensibleLines:public CObject
{
public:
  CGroupOfExtensibleLines();
 ~CGroupOfExtensibleLines();

  double          Degree;                  // +- 0.05
  Bool            ProcessingType;          // тип обработки линий(гор. или верт.)
  CWordArray      m_arFriendsIndex;        // индексы дружественных линий (m_arLines)

  int             GetCountOfExtensibleLines(void);
};
*/
/////////////////////////////////////////////////////////////////////////////
// PageLine's class
/*class RSHELLLINES_CLASS CPageLines:public CObject
{
public:
  CPageLines();
 ~CPageLines();

  Bool                     FlagDot;  //Если == TRUE то проверяется точечные линии

  CObArray                 m_arLines;
  CObArray                 m_arNewDotLines;  // точечные линии собранные из коробок компонент
  CObArray                 m_arGroupOfFriendLines;
  CObArray                 m_arGroupOfExtensibleLines;
  CObArray                 m_arComps;
  CObArray                 m_arDotComps;
  CLineImage*              m_pImage;

  int                      GetCountLines(void);
  CLine*                   GetNewLine(void);
  CLine*                   GetLine(int i);

  int                      GetCountNewDotLines(void);
  CDotLine*                GetNewDotLine(void);
  CDotLine*                GetDotLine(int i);
  void                     DrowRectForChains(void);

  int                      GetCountGroupOfFriendLines(void);
  CGroupOfFriendLines*     GetNewGroupOfFriendLines(void);
  CGroupOfFriendLines*     GetGroupOfFriendLines(int i);

  int                      GetCountGroupOfExtensibleLines(void);
  CGroupOfExtensibleLines* GetNewGroupOfExtensibleLines(void);
  CGroupOfExtensibleLines* GetGroupOfExtensibleLines(int i);

  int                      GetCountComps(void);
  CSL_POLY*                GetNewComp(void);
  CSL_POLY*                GetComp(int i);

  int                      GetCountDotComps(void);
  CPRect32*                GetNewDotComp(void);
  CPRect32*                GetDotComp(int i);
  void                     DrowRectForDotComp(void);
*/
  Bool                     IsItNewLine(CLINE_handle hContainer, DLine* pLineCP, int32_t GroupVerOffset, Bool32 IfDrawResult); //
  Bool                     CheckLinesForExtensible(CLINE_handle hLine, GLM* hGroupEx); //
  Bool                     CheckLinesImpositionForExtensible(CLINE_handle hLine, GLM* hGroupEx); //
  Bool                     CheckLinesDegreeForExtensible(CLINE_handle hLine, GLM* hGroupEx); //
//  Bool                     CheckGroupsForExtensible(void);

RSHELLLINES_FUNC(void)       FindFriendLines(CLINE_handle hContainer, GLM* friendlinesmass); //
RSHELLLINES_FUNC(void)       DrawFriendLines(CLINE_handle hContainer, GLM* friendlinesmass); //

RSHELLLINES_FUNC(void)       FindGroupOfExtensibleLines(CLINE_handle hContainer, GLM* friendlinesmass, Bool32 IfDrawResult); //
  void                     DrawGroupOfExtensibleLines(CLINE_handle hContainer, GLM* hGroup, int CountLines); //

RSHELLLINES_FUNC(void)       FindLosedVerticalLines(CLINE_handle hContainer, GLM* friendlinesmass, CLINE_handle hFictContainer, int CountLines, Bool32 IfDrawResult); //
RSHELLLINES_FUNC(void)       DrawLosedVerticalLines(GLM* friendlinesmass, int CountLines); //

//  void                     GetBigComps(Handle hCComp);
RSHELLLINES_FUNC(void)       DrawBigComps(CLINE_handle hContainer); //

RSHELLLINES_FUNC(void)       FindDotLines(Handle hCCOM,Handle hCPAGE, CLINE_handle hContainer); //
  Bool                     GetDotComps(Handle hCCOM,Handle hCPAGE, HANDLE &hbuf,char* &pDotBuffer, int32_t *CountDotComps); //
  void                     FindChains(Handle hCCOM, char* &pDotBuffer, int32_t CountDotComps, CLINE_handle hContainer); //
  void                     CheckChains(Handle hCCOM, char* &pDotBuffer, int32_t BegIndex,int32_t CountDots, CLINE_handle hContainer); //
  void                     AddNewDotLines(Handle hCCOM, char* &pDotBuffer, int32_t CountDots, CLINE_handle hContainer); //
  void                     UnionBreakup(CLINE_handle hContainer); //

RSHELLLINES_FUNC(void)       SetLinesAndCompsRelationship(CLINE_handle hContainer, CLINE_handle hFictContainer); //

RSHELLLINES_FUNC(void)       DrowAllLines(CLINE_handle hContainer, Handle hDrowAllLines); //
/*  int                      SetImage(Handle hCPage);
};
*/
RSHELLLINES_FUNC(Bool32) FindLineFrag(CLINE_handle processedline, Bool32 OnlyPosyAndStat, Bool32 Is2ndPath, CLINE_handle hContainer, Bool32 IfNeedFragment = TRUE, Bool32 IfStraightFrag = FALSE); //
RSHELLLINES_FUNC(Bool32) FindLineAttr(CLINE_handle line, DLine* pCLine, Bool32 AbleSeeOldAttr); //
RSHELLLINES_FUNC(Bool32) GetNextPartOfLine(CLINE_handle hContainer, CLINE_handle hLine/*, CPDLine oldlinedata*/); //

void                       AddLine2Group(GLM* hGroup, CLINE_handle hLine);
RSHELLLINES_FUNC(void)      DeleteGroup(GLM* hGroup, int CountLines);
RSHELLLINES_FUNC(void)      DrawFragsForAllLines(CLINE_handle hContainer, Handle hDebugDrawAllLineFragments);
RSHELLLINES_FUNC(int32_t)     CountBlackRaster(CPDLine pLine, CPDLine pLineExt, Bool32 IsHor, int32_t delta, Handle hDrawRaster);
RSHELLLINES_FUNC(Bool32)    SetExtLines(CLINE_handle hExtContainer, CLINE_handle hContainer, CLINE_handle* hLinesMass, int32_t CountLines);
RSHELLLINES_FUNC(Bool32)    RSL_CorrectDoubleLines(CLINE_handle hLine1, CLINE_handle hLine2);
RSHELLLINES_FUNC(Bool32)    RSL_SplitLine(CLINE_handle hLine, CLINE_handle hContainer);

//possible return values for RSL_VerifyShortLine
#define RSL_IS_LINE 0
#define RSL_BIG_ANGLE_DIFF 1
#define RSL_NO_COMP_FOUND 2
#define RSL_BAD_RASTER_FORM 3
#define RSL_BAD_LINE_PARAMS 4

#define MAX_CROSS_POINTS 4

RSHELLLINES_FUNC(int32_t)    RSL_VerifyShortLine(CPDLine pLine, Handle hCCOM, PAGEINFO* page_info, uchar lang, uchar debug_flags, int32_t *cross_point = NULL);
RSHELLLINES_FUNC(Bool)  SL_GetRaster(Rect32* rect, uchar** ppData, PAGEINFO* page_info/*, CIMAGEBITMAPINFOHEADER* image_info*/); //


//}
