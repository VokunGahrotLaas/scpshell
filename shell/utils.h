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

struct scpshellEnvType;

struct scpshellEnv {
	// main args
	int argc;
	char** argv;
	char** envp;
	// sub args
	int sub_argc;
	char** sub_argv;
	char** sub_envp;
	// path
	char* path;
	char** pathv;
	int pathc;
	// builtins
	struct scpHashMap* builtins;
};

typedef int (*scpshellBuiltins_func)(struct scpshellEnv* env);

typedef struct scpshellBuiltins_pair {
	const char* key;
	const scpshellBuiltins_func value;
} scpshellBuiltins_pair;

#endif // SCPSHELL_UTILS_h
