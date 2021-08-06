#include "shell/utils.h"

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
