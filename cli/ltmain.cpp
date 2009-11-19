/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char szQuickHelpText[] = "usage: layout [-ght] [<roots_file>]\n";

static char szHelpText[] = "Layout Version 1.0  - Stand-alone program for page layout debugging\n"
    "Written in 1991 by Yuri Panchul\n"
    "\n"
    "Usage: layout [-ght] [<roots_file>]\n";

char szRootsFilename[100] = DEFAULT_ROOTS_FILENAME;

void PageLayout(void);

int main(int nArgc, char **pArgv) {
    int i;
    int nFilesDefined = 0;
    char *p;

    for (i = 1; i < nArgc; i++) {
        if (*pArgv[i] == '-') {
            for (p = pArgv[i] + 1; *p; p++)
                switch (*p) {
# ifdef LT_DEBUG
                case 'g':
                LT_DebugGraphicsLevel++;
                break;
# endif
                case 'h':
                    puts(szHelpText);
                    exit(0);
# ifdef LT_DEBUG
                    case 't':
                    bDebugTimeFlag = TRUE;
                    break;
# endif
                default:
                    fprintf(stderr, "Bad option: \"%s\"\n%s\n", pArgv[i], szQuickHelpText);
                    exit(-1);
                    break;
                }
        }
        else {
            switch (nFilesDefined) {
            case 0:
                strcpy(szRootsFilename, pArgv[i]);
                nFilesDefined++;
                break;

            default:
                fprintf(stderr, "Unnecessary filename: \"%s\"\n%s\n", pArgv[i], szQuickHelpText);
                exit(-1);
            }
        }
    }

    RootsLoadFile(szRootsFilename);
    PageLayout();
    exit(0);
}
