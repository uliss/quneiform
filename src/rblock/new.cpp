# include <setjmp.h>
# include <string.h>

# include <stdlib.h>
#include <iostream>

# include "new_c.h"
#include "minmax.h"

using namespace CIF;

extern Handle hNotUseAntonCross;
static int IsInPoly(Point a, POLY_ * pPoly);

Handle hCcom;

uint32_t NumberOfLettersInArea(Rect rect, int Number) {
    uint32_t Result = 0;
    ROOT *pRoot;

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++) {
        if ((pRoot->xColumn + pRoot->nWidth - 1 <= rect.right()) && (pRoot->yRow + pRoot->nHeight
                - 1 <= rect.bottom()) && (pRoot->xColumn >= rect.left()) && (pRoot->yRow
                >= rect.top()) && (pRoot->nBlock == Number))
            Result++;

    }

    return Result;
}

Bool32 DeleteVertex(POLY_ * poly, int position) {
    int i;

    poly->com.count--;
    for (i = position; i <= poly->com.count; i++) {
        poly->com.Vertex[i] = poly->com.Vertex[i + 1];
    }

    return TRUE;
}

Bool32 InsertVertex(POLY_ * poly, int position, Point point) {
    int i;

    poly->com.count++;
    for (i = poly->com.count - 1; i >= position; i--) {
        poly->com.Vertex[i] = poly->com.Vertex[i - 1];
    }
    poly->com.Vertex[position] = point;

    return TRUE;
}

Bool32 InsertBottom(POLY_ * rectangle, POLY_ * poly) {
    int i;
    Point point;

    for (i = 0; i < poly->com.count - 1; i++) {
        if ((poly->com.Vertex[i].x() < rectangle->com.Vertex[3].x()) && (poly->com.Vertex[i].y()
                < rectangle->com.Vertex[3].y()) && (poly->com.Vertex[i + 1].x()
                > rectangle->com.Vertex[2].x())) {
            point.rx() = rectangle->com.Vertex[3].x();
            point.ry() = poly->com.Vertex[i].y();
            InsertVertex(poly, i + 1, point);
            InsertVertex(poly, i + 2, rectangle->com.Vertex[3]);
            InsertVertex(poly, i + 3, rectangle->com.Vertex[2]);
            point.rx() = rectangle->com.Vertex[2].x();
            InsertVertex(poly, i + 4, point);
            return TRUE;
        }
    }

    return TRUE;
}

Bool32 InsertTop(POLY_ * rectangle, POLY_ * poly) {
    int i;
    Point point;

    for (i = 0; i < poly->com.count - 1; i++) {
        if ((poly->com.Vertex[i].x() > rectangle->com.Vertex[1].x()) && (poly->com.Vertex[i].y()
                > rectangle->com.Vertex[1].y()) && (poly->com.Vertex[i + 1].x()
                < rectangle->com.Vertex[0].x())) {
            point.rx() = rectangle->com.Vertex[1].x();
            point.ry() = poly->com.Vertex[i].y();
            InsertVertex(poly, i + 1, point);
            InsertVertex(poly, i + 2, rectangle->com.Vertex[1]);
            InsertVertex(poly, i + 3, rectangle->com.Vertex[0]);
            point.rx() = rectangle->com.Vertex[0].x();
            InsertVertex(poly, i + 4, point);
            return TRUE;
        }
    }

    return TRUE;
}

Bool32 InsertLeft(POLY_ * rectangle, POLY_ * poly) {
    int i;
    Point point;

    for (i = 0; i < poly->com.count - 1; i++) {
        if ((poly->com.Vertex[i].x() > rectangle->com.Vertex[0].x()) && (poly->com.Vertex[i].y()
                < rectangle->com.Vertex[0].y()) && (poly->com.Vertex[i + 1].y()
                > rectangle->com.Vertex[3].y())) {
            point.rx() = poly->com.Vertex[i].x();
            point.ry() = rectangle->com.Vertex[0].y();
            InsertVertex(poly, i + 1, point);
            InsertVertex(poly, i + 2, rectangle->com.Vertex[0]);
            InsertVertex(poly, i + 3, rectangle->com.Vertex[3]);
            point.ry() = rectangle->com.Vertex[3].y();
            InsertVertex(poly, i + 4, point);
            return TRUE;
        }
    }

    return TRUE;
}

Bool32 InsertRight(POLY_ * rectangle, POLY_ * poly) {
    int i;
    Point point;

    for (i = 0; i < poly->com.count - 1; i++) {
        if ((poly->com.Vertex[i].x() < rectangle->com.Vertex[2].x()) && (poly->com.Vertex[i].y()
                > rectangle->com.Vertex[2].y()) && (poly->com.Vertex[i + 1].y()
                < rectangle->com.Vertex[1].y())) {
            point.rx() = poly->com.Vertex[i].x();
            point.ry() = rectangle->com.Vertex[2].y();
            InsertVertex(poly, i + 1, point);
            InsertVertex(poly, i + 2, rectangle->com.Vertex[2]);
            InsertVertex(poly, i + 3, rectangle->com.Vertex[1]);
            point.ry() = rectangle->com.Vertex[1].y();
            InsertVertex(poly, i + 4, point);
            return TRUE;
        }
    }

    if ((poly->com.Vertex[poly->com.count - 1].x() < rectangle->com.Vertex[2].x())
            && (poly->com.Vertex[poly->com.count - 1].y() > rectangle->com.Vertex[2].y())
            && (poly->com.Vertex[0].y() < rectangle->com.Vertex[1].y())) {
        point.rx() = poly->com.Vertex[poly->com.count - 1].x();
        point.ry() = rectangle->com.Vertex[2].y();
        InsertVertex(poly, i + 1, point);
        InsertVertex(poly, i + 2, rectangle->com.Vertex[2]);
        InsertVertex(poly, i + 3, rectangle->com.Vertex[1]);
        point.ry() = rectangle->com.Vertex[1].y();
        InsertVertex(poly, i + 4, point);
    }

    return TRUE;
}

Bool32 InsertRectangleInPoly(POLY_ * rectangle, POLY_ * poly) {
    if (IsInPoly(rectangle->com.Vertex[0], poly)) {
        if (IsInPoly(rectangle->com.Vertex[1], poly))
            InsertTop(rectangle, poly);
        else
            InsertLeft(rectangle, poly);
    }
    else {
        if (IsInPoly(rectangle->com.Vertex[1], poly))
            InsertRight(rectangle, poly);
        else
            InsertBottom(rectangle, poly);
    }

    return TRUE;
}

Bool32 CrossedBy0(POLY_ * poly, POLY_ * rectangle) {
    int i;
    Point point;

    for (i = 0; i < poly->com.count - 2; i++) {
        if ((poly->com.Vertex[i].y() < rectangle->com.Vertex[0].y())
                && (poly->com.Vertex[i + 1].x() > rectangle->com.Vertex[0].x())
                && (poly->com.Vertex[i + 1].y() > rectangle->com.Vertex[0].y())
                && (poly->com.Vertex[i + 2].x() < rectangle->com.Vertex[0].x())) {
            point.ry() = rectangle->com.Vertex[0].y();
            point.rx() = poly->com.Vertex[i].x();
            InsertVertex(poly, i + 1, point);
            poly->com.Vertex[i + 2] = rectangle->com.Vertex[0];
            point.ry() = poly->com.Vertex[i + 3].y();
            point.rx() = rectangle->com.Vertex[0].x();
            InsertVertex(poly, i + 3, point);
            return TRUE;
        }
    }

    return TRUE;
}

Bool32 CrossedBy1(POLY_ * poly, POLY_ * rectangle) {
    int i;
    Point point;

    for (i = 0; i < poly->com.count - 2; i++) {
        if ((poly->com.Vertex[i].x() > rectangle->com.Vertex[1].x())
                && (poly->com.Vertex[i + 1].x() < rectangle->com.Vertex[1].x())
                && (poly->com.Vertex[i + 1].y() > rectangle->com.Vertex[1].y())
                && (poly->com.Vertex[i + 2].y() < rectangle->com.Vertex[1].y())) {
            point.rx() = rectangle->com.Vertex[1].x();
            point.ry() = poly->com.Vertex[i].y();
            InsertVertex(poly, i + 1, point);
            poly->com.Vertex[i + 2] = rectangle->com.Vertex[1];
            point.rx() = poly->com.Vertex[i + 3].x();
            point.ry() = rectangle->com.Vertex[1].y();
            InsertVertex(poly, i + 3, point);
            return TRUE;
        }
    }
    if ((poly->com.Vertex[poly->com.count - 2].x() > rectangle->com.Vertex[1].x())
            && (poly->com.Vertex[poly->com.count - 1].x() < rectangle->com.Vertex[1].x())
            && (poly->com.Vertex[poly->com.count - 1].y() > rectangle->com.Vertex[1].y())
            && (poly->com.Vertex[0].y() < rectangle->com.Vertex[1].y())) {
        point.rx() = rectangle->com.Vertex[1].x();
        point.ry() = poly->com.Vertex[poly->com.count - 2].y();
        InsertVertex(poly, poly->com.count - 1, point);

        InsertVertex(poly, poly->com.count - 1, rectangle->com.Vertex[1]);
        point.rx() = poly->com.Vertex[0].x();
        point.ry() = rectangle->com.Vertex[1].y();
        poly->com.Vertex[poly->com.count - 1] = point;

        return TRUE;
    }

    return TRUE;
}

Bool32 CrossedBy2(POLY_ * poly, POLY_ * rectangle) {
    int i;
    Point point;

    for (i = 0; i < poly->com.count - 2; i++) {
        if ((poly->com.Vertex[i].y() > rectangle->com.Vertex[2].y())
                && (poly->com.Vertex[i + 1].x() < rectangle->com.Vertex[2].x())
                && (poly->com.Vertex[i + 1].y() < rectangle->com.Vertex[2].y())
                && (poly->com.Vertex[i + 2].x() > rectangle->com.Vertex[2].x())) {
            point.ry() = rectangle->com.Vertex[2].y();
            point.rx() = poly->com.Vertex[i].x();
            InsertVertex(poly, i + 1, point);
            poly->com.Vertex[i + 2] = rectangle->com.Vertex[2];
            point.ry() = poly->com.Vertex[i + 3].y();
            point.rx() = rectangle->com.Vertex[2].x();
            InsertVertex(poly, i + 3, point);
            return TRUE;
        }
    }
    if ((poly->com.Vertex[poly->com.count - 1].y() > rectangle->com.Vertex[2].y())
            && (poly->com.Vertex[0].x() < rectangle->com.Vertex[2].x()) && (poly->com.Vertex[0].y()
            < rectangle->com.Vertex[2].y()) && (poly->com.Vertex[1].x()
            > rectangle->com.Vertex[2].x())) {

        point.ry() = poly->com.Vertex[0].y();
        point.rx() = rectangle->com.Vertex[2].x();
        InsertVertex(poly, 1, point);
        point.ry() = rectangle->com.Vertex[2].y();
        point.rx() = poly->com.Vertex[0].x();

        InsertVertex(poly, 1, rectangle->com.Vertex[2]);
        poly->com.Vertex[0] = point;

        return TRUE;
    }

    return TRUE;
}

Bool32 CrossedBy3(POLY_ * poly, POLY_ * rectangle) {
    int i;
    Point point;

    for (i = 0; i < poly->com.count - 2; i++) {
        if ((poly->com.Vertex[i].x() < rectangle->com.Vertex[3].x())
                && (poly->com.Vertex[i + 1].x() > rectangle->com.Vertex[3].x())
                && (poly->com.Vertex[i + 1].y() < rectangle->com.Vertex[3].y())
                && (poly->com.Vertex[i + 2].y() > rectangle->com.Vertex[3].y())) {
            point.rx() = rectangle->com.Vertex[3].x();
            point.ry() = poly->com.Vertex[i].y();
            InsertVertex(poly, i + 1, point);
            poly->com.Vertex[i + 2] = rectangle->com.Vertex[3];
            point.rx() = poly->com.Vertex[i + 3].x();
            point.ry() = rectangle->com.Vertex[3].y();
            InsertVertex(poly, i + 3, point);
            return TRUE;
        }
    }

    return TRUE;
}

int GetPOLYHeight(POLY_ * poly) {
    int i;
    int min_top;
    int max_bottom;

    if (!(poly))
        return 0;
    if (!(poly->com.count))
        return 0;

    max_bottom = min_top = poly->com.Vertex[0].y();
    for (i = 1; i < poly->com.count; i++) {
        if (poly->com.Vertex[i].y() < min_top)
            min_top = poly->com.Vertex[i].y();
        else {
            if (poly->com.Vertex[i].y() > max_bottom)
                max_bottom = poly->com.Vertex[i].y();
        }
    }
    return max_bottom - min_top;
}

Bool32 PolysIsCrossed(POLY_ * poly1, POLY_ * poly2, Bool32 pic) {

    if (!pic) {
        if (GetPOLYHeight(poly1) < GetPOLYHeight(poly2))
            return TRUE;
    }

    if (IsInPoly(poly2->com.Vertex[0], poly1)) {
        CrossedBy0(poly1, poly2);
        return TRUE;
    }
    if (IsInPoly(poly2->com.Vertex[1], poly1)) {
        CrossedBy1(poly1, poly2);
        return TRUE;
    }
    if (IsInPoly(poly2->com.Vertex[2], poly1)) {
        CrossedBy2(poly1, poly2);
        return TRUE;
    }
    if (IsInPoly(poly2->com.Vertex[3], poly1)) {
        CrossedBy3(poly1, poly2);
        return TRUE;
    }

    return TRUE;
}

Bool32 PageRoatateBlocks(Handle hPage) {
    Handle hBlock;
    POLY_ block;
    int i, j;
    div_t d1;
    int defect;

    j = 0;
    hBlock = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE);
    while (hBlock != NULL) {
        j++;
        CPAGE_GetBlockData(hPage, hBlock, TYPE_IMAGE, &block, sizeof(block));

        if (block.com.count == 4) {
            if (nIncline >= 0) {
                defect = ((block.com.Vertex[1].x() - block.com.Vertex[0].x()) * nIncline)
                        / INCLINE_FACTOR;
                block.com.Vertex[1].ry() += defect;
                block.com.Vertex[3].ry() -= defect;
                defect = ((block.com.Vertex[3].y() - block.com.Vertex[0].y()) * nIncline)
                        / INCLINE_FACTOR;
                block.com.Vertex[0].rx() += defect;
                block.com.Vertex[2].rx() -= defect;
            }
            else {
                defect = ((block.com.Vertex[1].x() - block.com.Vertex[0].x()) * nIncline)
                        / INCLINE_FACTOR;
                block.com.Vertex[0].ry() -= defect;
                block.com.Vertex[2].ry() += defect;
                defect = ((block.com.Vertex[3].y() - block.com.Vertex[0].y()) * nIncline)
                        / INCLINE_FACTOR;
                block.com.Vertex[1].rx() += defect;
                block.com.Vertex[3].rx() -= defect;
            }
        }

        for (i = 0; i < block.com.count; i++) {
            IDEAL_XY (block.com.Vertex[i].rx (), block.com.Vertex[i].ry ());
        }

        for (i = 0; i < block.com.count - 1; i++) {
            d1 = div(i, 2);
            if (d1.rem == 0) {
                if (block.com.Vertex[i].y() != block.com.Vertex[i + 1].y())
                    block.com.Vertex[i + 1].ry() = block.com.Vertex[i].y();
            }
            else {
                if (block.com.Vertex[i].x() != block.com.Vertex[i + 1].x())
                    block.com.Vertex[i].rx() = block.com.Vertex[i + 1].x();

            }
        }
        if (block.com.Vertex[0].x() != block.com.Vertex[block.com.count - 1].x())
            block.com.Vertex[block.com.count - 1].rx() = block.com.Vertex[0].x();

        fprintf(stderr, "  <4 О 1 %4d %4d %4d %4d %d \n", block.com.Vertex[0].x(),
                block.com.Vertex[0].y(), block.com.Vertex[1].x(), block.com.Vertex[1].y(),
                block.com.Vertex[2].y() - block.com.Vertex[1].y());
        LDPUMA_FPuts(resFile_pict, tmp_str);

        CPAGE_SetBlockData(hPage, hBlock, TYPE_IMAGE, &block, sizeof(block));
        hBlock = CPAGE_GetBlockNext(hPage, hBlock, TYPE_IMAGE);
    }

    fprintf(stderr, "  <4 Р %d %d %d \n", j, 0, 0);
    LDPUMA_FPuts(resFile_pict, tmp_str);

    return TRUE;
}

void PageLayoutBlocks(Handle hCCOM) {
    Handle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());

    file_name = LDPUMA_GetFileName(NULL);
    hCcom = hCCOM;

    if (ReadRoots(hCCOM, TRUE)) {
        LayoutPart1();

        std::cerr << "  <4 Н После поворота \n";

        PageRoatateBlocks(hPage);

        std::cerr << "  <4 К После поворота \n";
        std::cerr << "  <3 К Картинки \n";
        std::cerr << "  <2 К Страница \n";

        free(CellsPage);
        CellsPage = NULL;
    }

}

Bool32 DeletePoly(POLY_ * all_polys, int max, int i) {
    int j;

    for (j = i; j < max - 1; j++) {
        all_polys[j] = all_polys[j + 1];
    }

    return TRUE;
}

Bool32 BlockAnalyse() {
    BLOCK *p;

    uint32_t nAverageHeight;

    ROOT *pRoot;

    RecRaster rec;
    RecVersions vs;
    int16_t Language;

    uint32_t i = 0;
    int nRealLetters = 0;

    nAverageHeight = 0;

    if (nRoots == 0)
        return TRUE;

    for (i = 0; i < nRoots; i++) {
        nAverageHeight += pRoots[i].nHeight;
    }
    nAverageHeight /= nRoots;

    AGAIN_PQ:

    for (p = pBlocksList; p != NULL; p = p->pNext) {

        if (p->Rect.bottom() - p->Rect.top() < nAverageHeight * 0.7) {

            BlocksRemoveDescriptor(p);
            goto AGAIN_PQ;
        }

        if ((uint32_t) (p->Rect.right() - p->Rect.left()) < nAverageHeight) {
            BlocksRemoveDescriptor(p);
            goto AGAIN_PQ;
        }

        if ((p->nLetters < 2) && ((p->nAverageHeight < nAverageHeight / 0.7) || (p->nAverageHeight
                > nAverageHeight * 0.7))) {
            nRealLetters = 0;
            if (p->nRoots < 20) {
                pRoot = p->pRoots;
                while (pRoot != NULL) {
                    if (!CCOM_GetRaster(static_cast<CCOM_comp *> (pRoot->pComp), &rec))
                        continue;
                    if (!CCOM_GetLanguage(hCcom, &Language))
                        continue;
                    if (!RSTR_RecogOneLetter(&rec, Language, &vs))
                        continue;
                    if ((vs.lnAltCnt > 0) && (vs.Alt[0].Prob > 240) && (vs.Alt[0].Code != 'i')
                            && (vs.Alt[0].Code != 'I') && (vs.Alt[0].Code != 'l')) {
                        nRealLetters++;
                    }
                    pRoot = pRoot->u1.pNext;
                }
            }
            if ((nRealLetters > 1) || (nRealLetters == 1 && p->nRoots < 4)) {
                continue;
            }

            BlocksRemoveDescriptor(p);
            goto AGAIN_PQ;
        }
    }

    return TRUE;
}

Bool32 OutputFragments(Handle hPage) {
    extern BLOCK *pBlocksList;
    BLOCK *p = NULL;
    Bool32 rc = TRUE;
    int count;
    uint32_t BlockNumber;
    int32_t Min, PrevMin;
    Handle hBlock = NULL;
    Handle h;
    Handle h_next;

    int i, j, max, nPics;
    POLY_ *pPics;
    Bool32 Same;

    POLY_ block;
    POLY_ *all_polys;

    std::cerr << "  <2 Н Страница =" << file_name << " \n";
    std::cerr << "  <3 Н Фрагменты \n";
    std::cerr << "  <4 Н Результат работы старого фрагментатора \n";

    BlocksExtract();

    BlockAnalyse();
    all_polys = static_cast<POLY_ *> (malloc(COMPS_QUANTUM * sizeof(POLY_)));
    pPics = static_cast<POLY_ *> (malloc(PICS_QUANTUM * sizeof(POLY_)));

    BlocksBuildLeftAndRightLists();
    BlocksBuildTopAndBottomLists();

    count = 0;
    BlockNumber = 0;
    i = 0;
    for (p = pTopBlocksList; p != NULL; p = p->pDown) {
        if (p->Type == 111) {
            CPAGE_SetBlockUserNum(hPage, p->pHystogram, ++BlockNumber);
            continue;
        }

        if (i % COMPS_QUANTUM == 0) {
            all_polys = static_cast<POLY_ *> (realloc(all_polys, (size_t) ((i / COMPS_QUANTUM + 1)
                    * COMPS_QUANTUM * sizeof(POLY_))));
        }

        all_polys[i].com.type = TYPE_TEXT;

        all_polys[i].com.number = p->nNumber;

        all_polys[i].com.Color = 0;
        all_polys[i].com.count = 4;
        all_polys[i].com.Vertex[0].rx() = p->Rect.left();
        all_polys[i].com.Vertex[0].ry() = p->Rect.top();
        all_polys[i].com.Vertex[1].rx() = p->Rect.right();
        all_polys[i].com.Vertex[1].ry() = p->Rect.top();
        all_polys[i].com.Vertex[2].rx() = p->Rect.right();
        all_polys[i].com.Vertex[2].ry() = p->Rect.bottom();
        all_polys[i].com.Vertex[3].rx() = p->Rect.left();
        all_polys[i].com.Vertex[3].ry() = p->Rect.bottom();
        all_polys[i].alphabet = 0;

        fprintf(stderr, "  <4 О 1 %4d %4d %4d %4d %d \n", all_polys[i].com.Vertex[0].x(),
                all_polys[i].com.Vertex[0].y(), all_polys[i].com.Vertex[1].x(),
                all_polys[i].com.Vertex[1].y(), all_polys[i].com.Vertex[2].y()
                        - all_polys[i].com.Vertex[1].y());

        i++;

    }
    max = i;

    fprintf(stderr, "  <4 Р %d %d %d \n", i, 0, 0);
    std::cerr << "  <4 К Результат работы старого фрагментатора \n";

    i = 0;
    for (h = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE); h != NULL; h = CPAGE_GetBlockNext(hPage, h,
            TYPE_IMAGE)) {
        if (i % COMPS_QUANTUM == 0) {
            pPics = static_cast<POLY_ *> (realloc(pPics, (size_t) ((i / COMPS_QUANTUM + 1)
                    * COMPS_QUANTUM * sizeof(POLY_))));
        }
        CPAGE_GetBlockData(hPage, h, TYPE_IMAGE, &pPics[i++], sizeof(POLY_));
    }
    nPics = i;

    for (i = 0; i < max; i++) {
        for (j = 0; j < nPics; j++) {
            if (pPics[j].com.count != 4)
                continue;
            block = pPics[j];
            block.com.Vertex[0].rx() -= BOUND;
            block.com.Vertex[0].ry() -= BOUND;
            block.com.Vertex[1].rx() += BOUND;
            block.com.Vertex[1].ry() -= BOUND;
            block.com.Vertex[2].rx() += BOUND;
            block.com.Vertex[2].ry() += BOUND;
            block.com.Vertex[3].rx() -= BOUND;
            block.com.Vertex[3].ry() += BOUND;
            count = 0;

            if (IsInPoly(block.com.Vertex[0], &all_polys[i]))
                count++;
            if (IsInPoly(block.com.Vertex[1], &all_polys[i]))
                count++;
            if (IsInPoly(block.com.Vertex[2], &all_polys[i]))
                count++;
            if (IsInPoly(block.com.Vertex[3], &all_polys[i]))
                count++;

            switch (count) {
            case 1:
                PolysIsCrossed(&all_polys[i], &block, TRUE);
                break;
            case 2:
                InsertRectangleInPoly(&block, &all_polys[i]);
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                break;
            }
        }
    }

    AGAIN: for (i = 0; i < max; i++) {
        for (j = 0; j < max; j++) {
            if (all_polys[j].com.count != 4)
                continue;
            if (i == j)
                continue;
            block = all_polys[j];

            if (!(IsInPoly(block.com.Vertex[0], &all_polys[i])) && !(IsInPoly(block.com.Vertex[1],
                    &all_polys[i])) && !(IsInPoly(block.com.Vertex[2], &all_polys[i]))
                    && !(IsInPoly(block.com.Vertex[3], &all_polys[i]))) {
                continue;
            }

            if ((IsInPoly(block.com.Vertex[0], &all_polys[i])) && (IsInPoly(block.com.Vertex[1],
                    &all_polys[i])) && (IsInPoly(block.com.Vertex[2], &all_polys[i])) && (IsInPoly(
                    block.com.Vertex[3], &all_polys[i]))) {
                DeletePoly(all_polys, max, j);
                if (max > 0)
                    max--;

            }

            block.com.Vertex[0].rx() -= BOUND;
            block.com.Vertex[0].ry() -= BOUND;
            block.com.Vertex[1].rx() += BOUND;
            block.com.Vertex[1].ry() -= BOUND;
            block.com.Vertex[2].rx() += BOUND;
            block.com.Vertex[2].ry() += BOUND;
            block.com.Vertex[3].rx() -= BOUND;
            block.com.Vertex[3].ry() += BOUND;
            count = 0;

            if (IsInPoly(block.com.Vertex[0], &all_polys[i]))
                count++;
            if (IsInPoly(block.com.Vertex[1], &all_polys[i]))
                count++;
            if (IsInPoly(block.com.Vertex[2], &all_polys[i]))
                count++;
            if (IsInPoly(block.com.Vertex[3], &all_polys[i]))
                count++;

            switch (count) {
            case 1:
                if (!LDPUMA_Skip(hNotUseAntonCross))
                    PolysIsCrossed(&all_polys[i], &block, TRUE);
                else
                    PolysIsCrossed(&all_polys[i], &block, FALSE);
                break;
            case 2:
                if (all_polys[i].alphabet != block.com.number) {
                    InsertRectangleInPoly(&block, &all_polys[i]);
                    all_polys[i].alphabet = block.com.number;
                    break;
                }
                else {
                    goto AGAIN;
                }
            case 3:
                break;
            case 4:
                break;
            default:
                break;
            }
        }

    }

    std::cerr << "  <4 Н Финал \n";

    PrevMin = -2000;
    Min = -1000;

    while (Min != PrevMin) {
        PrevMin = Min;
        Min = 65535;

        for (i = 0; i < max; i++) {
            if ((all_polys[i].com.Vertex[0].y() < Min)
                    && (all_polys[i].com.Vertex[0].y() > PrevMin)) {
                Min = all_polys[i].com.Vertex[0].y();
            }
        }

        for (i = 0; i < max; i++) {
            if (all_polys[i].com.Vertex[0].ry() == Min) {
                all_polys[i].com.number = ++BlockNumber;
            }
        }
    }

    j = 0;
    for (i = 0; i < max; i++) {
        all_polys[i].alphabet = 0;
        BlockNumber = all_polys[i].com.number;
        all_polys[i].com.number = 0;
        all_polys[i].negative = TYPE_POSITIVE;
        all_polys[i].orient = TYPE_LEFTRIGHT;

        if (all_polys[i].com.count == 4) {
            j++;
            fprintf(stderr, "  <4 О 1 %4d %4d %4d %4d %d \n", all_polys[i].com.Vertex[0].x(),
                    all_polys[i].com.Vertex[0].y(), all_polys[i].com.Vertex[1].x(),
                    all_polys[i].com.Vertex[1].y(), all_polys[i].com.Vertex[2].y()
                            - all_polys[i].com.Vertex[1].y());
        }

        hBlock = CPAGE_CreateBlock(hPage, TYPE_TEXT, 0, 0, &all_polys[i], sizeof(POLY_));
        if (!hBlock) {
            SetReturnCode_rblock(CPAGE_GetReturnCode());
            rc = FALSE;
            break;
        }
        CPAGE_SetBlockUserNum(hPage, hBlock, BlockNumber);
    }

    fprintf(stderr, "  <4 Р %d %d %d \n", j, 0, 0);
    std::cerr << "  <4 К Финал \n";
    std::cerr << "  <3 К Фрагменты \n";
    std::cerr << "  <2 К Страница \n";

    PrevMin = -2000;
    Min = -1000;
    while (Min != PrevMin) {
        PrevMin = Min;
        Min = 65535;

        for (i = 0; i < nPics; i++) {
            if ((pPics[i].com.Vertex[0].y() < Min) && (pPics[i].com.Vertex[0].y() > PrevMin)) {
                Min = pPics[i].com.Vertex[0].y();
            }
        }

        for (i = 0; i < nPics; i++) {
            if (pPics[i].com.Vertex[0].y() == Min) {
                pPics[i].com.number = ++BlockNumber;
            }
        }
    }

    for (h = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE); h != NULL; h = CPAGE_GetBlockNext(hPage, h,
            TYPE_IMAGE)) {
        CPAGE_GetBlockData(hPage, h, TYPE_IMAGE, &block, sizeof(POLY_));
        for (i = 0; i < nPics; i++) {
            Same = TRUE;
            for (j = 0; j < pPics[i].com.count; j++) {
                if (pPics[i].com.Vertex[j].x() != block.com.Vertex[j].x()
                        || pPics[i].com.Vertex[j].y() != block.com.Vertex[j].y()) {
                    Same = FALSE;
                    break;
                }
            }
            if (Same) {
                BlockNumber = pPics[i].com.number;
                pPics[i].com.number = 0;
                CPAGE_SetBlockUserNum(hPage, h, BlockNumber);
            }
        }
    }

    h = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE);
    while (h) {
        h_next = CPAGE_GetBlockNext(hPage, h, TYPE_IMAGE);
        CPAGE_GetBlockData(hPage, h, TYPE_IMAGE, &block, sizeof(POLY_));
        if (block.negative == TYPE_NEGATIVE) {
            BlockNumber = CPAGE_GetBlockUserNum(hPage, h);
            CPAGE_DeleteBlock(hPage, h);
            block.alphabet = 0;
            block.com.number = 0;
            hBlock = CPAGE_CreateBlock(hPage, TYPE_TEXT, 0, 0, &block, sizeof(POLY_));
            CPAGE_SetBlockUserNum(hPage, hBlock, BlockNumber);
        }
        h = h_next;
    }

    LT_FreeAllData();

    free(all_polys);
    free(pPics);

    return rc;
}

void CalculatePageIncline(Handle hCCOM, int32_t * lpNominator, int32_t * lpDenominator) {

    *lpDenominator = INCLINE_FACTOR;
    *lpNominator = nIncline;
}

int IsInPoly(Point a, POLY_ * p) {
    int i, y, n, ind;
    int Count = 0;

    n = p->com.count;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;

        if (p->com.Vertex[i].y() == p->com.Vertex[j].y())
            continue;
        if (p->com.Vertex[i].y() > a.y() && p->com.Vertex[j].y() > a.y())
            continue;
        if (p->com.Vertex[i].y() < a.y() && p->com.Vertex[j].y() < a.y())
            continue;
        y = p->com.Vertex[i].y();
        ind = i;
        if (p->com.Vertex[j].y() > y) {
            y = p->com.Vertex[j].y();
            ind = j;
        }
        if ((y == a.y()) && (p->com.Vertex[ind].x() >= a.x()))
            Count++;
        else if (MIN (p->com.Vertex[i].y (), p->com.Vertex[j].y ()) == a.y())
            continue;
        else {
            double t = ((double) (a.y() - p->com.Vertex[i].y()) / ((double) (p->com.Vertex[j].y()
                    - (double) p->com.Vertex[i].y())));

            if (t > 0 && t < 1 && (double) p->com.Vertex[i].x() + t
                    * ((double) p->com.Vertex[j].x() - (double) p->com.Vertex[i].x())
                    >= (double) a.x())
                Count++;
        }
    }
    return Count & 1;
}