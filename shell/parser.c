#include "shell/parser.h"

int scpshellParsing_get_argc(const char* buffer) {
	int count = 1;
	scpBool last_was_space = scpTrue;
	for (; *buffer; ++buffer) {
		if (*buffer == ' ') {
			if (!last_was_space)
				++count;
			last_was_space = scpTrue;
		} else
			last_was_space = scpFalse;
	}
	if (last_was_space && count > 1)
		--count;
	return count;
}

void scpshellParsing_make_argv(int argc, char** argv, char* buffer) {
	for (; *buffer == ' '; ++buffer);
	for (int i = 0; i < argc; ++i) {
		argv[i] = buffer;
		for (; *buffer && *buffer != ' '; ++buffer);
		if (!*buffer) break;
		*buffer++ = '\0';
		for (; *buffer == ' '; ++buffer);
	}
	argv[argc] = NULL;
}
