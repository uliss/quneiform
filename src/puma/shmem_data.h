/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef SHMEM_DATA_H
#define SHMEM_DATA_H

namespace cf {

enum WorkerExitValues {
    WORKER_UNKNOWN_ERROR = 1,
    WORKER_SEGMENT_NOT_FOUND = 2,
    WORKER_SAVE_ERROR = 3,
    WORKER_RECOGNITION_ERROR = 4,
    WORKER_SHMEM_ERROR = 5,
    WORKER_TERMINATE_ERROR = 6,
    WORKER_ABORT_ERROR = 7,
    WORKER_SEGFAULT_ERROR = 8,
    WORKER_TIMEOUT_ERROR = 9
};

}

#endif // SHMEM_DATA_H
