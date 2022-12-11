/*

    ASIPGM: include/asipgm.h

    Copyright (C) 2022  Johnathan K Burchill

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _THEMIS_H
#define _THEMIS_H

#include <stdint.h>
#include <stdbool.h>

#define THEMIS_ASI_COLUMNS 256
#define THEMIS_ASI_ROWS 256

enum AsiPgmErrors
{
    ASIPGM_OK = 0,
    ASIPGM_ARGUMENTS,
    ASIPGM_MEM,
    ASIPGM_PGM_FILE
};

typedef uint16_t ThemisImage[256][256];

typedef struct ThemisImages
{
    long nImages;
    long long *imageTimesTT2000;
    ThemisImage *images;

} ThemisImages;

int readThemisPgm(char * filename, ThemisImages *images);


#endif // _THEMIS_H
