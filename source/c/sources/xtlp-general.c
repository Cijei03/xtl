#include "xtlp-general.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef __STDC_WANT_LIB_EXT2__
	#define __STDC_WANT_LIB_EXT2__ 1
#endif
#include <string.h>

char* const xtlpMakeContextString(char** C, const uint32_t count)
{
	char WorkBuffer[512] = {};

	for (size_t i = 0; i < count; i++)
	{
		strcat(WorkBuffer, "/");
		strcat(WorkBuffer, C[i]);
	};
	return strdup(WorkBuffer);
}

size_t conlen(char** C)
{
    size_t len = 0;
    for (; C[len] != NULL; len++) {}
    return len;
}