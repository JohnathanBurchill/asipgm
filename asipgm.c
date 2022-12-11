/*

    ASIPGM: asipgm.c

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

#include "asipgm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cdf.h>

#define MAX_LINE_LENGTH 1025

int readThemisPgm(char * filename, ThemisImages *images)
{
    if (filename == NULL)
        return ASIPGM_ARGUMENTS;

    int status = ASIPGM_OK;

    if (access(filename, F_OK) != 0)
        return ASIPGM_PGM_FILE;

    FILE *f = fopen(filename, "r");
    if (f == NULL)
        return ASIPGM_PGM_FILE;

    unsigned char line[MAX_LINE_LENGTH];
    char *res = NULL;

    if (fgets(line, MAX_LINE_LENGTH, f) == NULL)
    {
        status = ASIPGM_PGM_FILE;
        goto cleanup;
    }
    if (strncmp("P5", line, 2) != 0)
    {
        status = ASIPGM_PGM_FILE;
        goto cleanup;
    }


    int nRead = 0;

    double year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    long long requestStartTT2000 = 0;
    long long readoutStartTT2000 = 0;
    // char requestUtcString[TT2000_3_STRING_LEN+1] = {0};
    // char readoutUtcString[TT2000_3_STRING_LEN+1] = {0};

    int columns = 0;
    int rows = 0;
    int maxValue = 0;
    void *mem = NULL;
    size_t bytesRead = 0;
    uint16_t tmp = 0;

    // Each themis PGM file contains typically 20 images.
    mem = realloc(images->images, sizeof *images->images * (images->nImages + 20));
    if (mem == NULL)
    {
        status = ASIPGM_MEM;
        goto cleanup;
    }
    images->images = mem;
    mem = realloc(images->imageTimesTT2000, sizeof *images->imageTimesTT2000 * (images->nImages + 20));
    if (mem == NULL)
    {
        status = ASIPGM_MEM;
        goto cleanup;
    }
    images->imageTimesTT2000 = mem;

    while (fgets(line, MAX_LINE_LENGTH, f) != NULL)
    {
        if (strncmp(line, "#\"Image request start\"", 22) != 0)
            continue;

        nRead = sscanf(line + 23, "%4lf-%2lf-%2lf %2lf:%2lf:%lf", &year, &month, &day, &hour, &minute, &second);
        requestStartTT2000 = computeTT2000(year, month, day, hour, minute, second, TT2000END);
        res = fgets(line, MAX_LINE_LENGTH, f);
        if (res == NULL || strncmp(line, "#\"Image readout start\"", 22) != 0)
        {
            status = ASIPGM_PGM_FILE;
            goto cleanup;
        }
        nRead = sscanf(line + 23, "%4lf-%2lf-%2lf %2lf:%2lf:%lf", &year, &month, &day, &hour, &minute, &second);
        readoutStartTT2000 = computeTT2000(year, month, day, hour, minute, second, TT2000END);

        images->nImages++;
        images->imageTimesTT2000[images->nImages-1] = requestStartTT2000 + (readoutStartTT2000 - requestStartTT2000) / 2;

        // encodeTT2000(readoutStartTT2000, readoutUtcString, 3);
        
        // fprintf(stderr, "Request start as read: %4.0lf-%02.0lf-%02.0lfT%02.0lf:%02.0lf:%012.9lf, TT2000 to UTC: %s\n", year, month, day, hour, minute, second, readoutUtcString);

        res = fgets(line, MAX_LINE_LENGTH, f);
        res = fgets(line, MAX_LINE_LENGTH, f);
        nRead = sscanf(line, "%d %d", &columns, &rows);
        res = fgets(line, 100, f);
        nRead = sscanf(line, "%d", &maxValue);
        if (res == NULL)
        {
            status = ASIPGM_PGM_FILE;
            goto cleanup;
        }
        if (columns != THEMIS_ASI_COLUMNS || rows != THEMIS_ASI_ROWS)
        {
            status = ASIPGM_PGM_FILE;
            goto cleanup;
        }
        fseek(f, -1, SEEK_CUR);

        bytesRead = fread(images->images[images->nImages-1], 1, sizeof(ThemisImage), f);

    }

cleanup:
    if (f != NULL)
        fclose(f);

    return status;

}