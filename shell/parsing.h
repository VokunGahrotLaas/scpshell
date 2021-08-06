#ifndef SCPSHELL_PARSING_H
#define SCPSHELL_PARSING_H

#include "shell/utils.h"

size_t scpshellParsing_get_argc(const char* str);
void scpshellParsing_make_argv(size_t argc, char** argv, char* buffer);

size_t scpshellParsing_get_argc(const char* buffer) {
	size_t count = 1;
	bool last_was_space = true;
	for (; *buffer; ++buffer) {
		if (*buffer == ' ') {
			if (!last_was_space)
				++count;
			last_was_space = true;
		} else
			last_was_space = false;
	}
	if (last_was_space && count > 1)
		--count;
	return count;
}

void scpshellParsing_make_argv(size_t argc, char** argv, char* buffer) {
	for (; *buffer == ' '; ++buffer);
	for (size_t i = 0; i < argc; ++i) {
		argv[i] = buffer;
		for (; *buffer && *buffer != ' '; ++buffer);
		if (!*buffer) break;
		*buffer++ = '\0';
		for (; *buffer == ' '; ++buffer);
	}
	argv[argc] = NULL;
}

#endif // SCPSHELL_PARSING_H
