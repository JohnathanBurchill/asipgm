/*

    ASIPGM: main.c

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

#include "main.h"
#include "asipgm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cdf.h>

int main(int argc, char **argv)
{
    int status = 0;
    ProgramState state = {0};

    int nOptions = 0;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp("--help", argv[i]) == 0)
        {
            usage(&state, argv[0]);
            return EXIT_SUCCESS;
        }
        else if (strcmp("--help-options", argv[i]) == 0)
        {
            state.showOptions = true;
            usage(&state, argv[0]);
            return EXIT_SUCCESS;
        }
        else if (strncmp("--", argv[i], 2) == 0)
        {
            fprintf(stderr, "Unknown option %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    if (argc - nOptions != 2)
    {
        usage(&state, argv[0]);
        return EXIT_FAILURE;
    }

    state.filename = argv[1];

    ThemisImages images = {0};

    status = readThemisPgm(state.filename, &images);
    if (status != ASIPGM_OK)
    {
        fprintf(stderr, "Error reading images\n");
        goto cleanup;
    }

    char imageUtcString[TT2000_3_STRING_LEN+1] = {0};

    for (int i = 0; i < images.nImages; i++)
    {
        encodeTT2000(images.imageTimesTT2000[i], imageUtcString, 3);
        fprintf(stderr, "Image %5d: UTC=%s\n", i, imageUtcString);
        for (int c = 0; c < THEMIS_ASI_COLUMNS; c++)
        {
            for (int r = 0; r < THEMIS_ASI_ROWS; r++)
                fprintf(stdout, "%6d", images.images[i][255-c][255-r]);

            fprintf(stdout, "\n");
        }
    }

cleanup:
    free(images.imageTimesTT2000);
    free(images.images);

    return status;
}

void usage(ProgramState *state, char *name)
{
    fprintf(stderr, "Usage: %s <asipgmfilename> [--help] [--help-options]\n", name);

    return;
}