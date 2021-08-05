#ifndef SCPSHELL_UTILS_h
#define SCPSHELL_UTILS_h

// C std libs
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// Unix std libs
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <pwd.h>

// scplib
#include "scp/utils.h"
#include "scp/exceptions.h"
#include "scp/containers/hashmap.h"

#ifndef SCPSHELL_INPUT_BUFFER_SIZE
#define SCPSHELL_INPUT_BUFFER_SIZE 4096
#endif

char* scpshellUtils_to_lower(const char* str);
bool scpshellUtils_flush_buffer(FILE* stream, char* buffer);

char* scpshellUtils_to_lower(const char* str) {
	char* cpy = strdup(str);
	for (char* p = cpy; *p; ++p)
		if ('A' <= *p && *p <= 'Z')
			*p += 'a' - 'A';
	return cpy;
}

bool scpshellUtils_flush_buffer(FILE* stream, char* buffer) {
	char* endl = strchr(buffer, '\n');
	if (!endl)
		while (!feof(stream) && fgetc(stream) != '\n');
	else
		*endl = '\0';
	return !endl;
}

#endif // SCPSHELL_UTILS_h
