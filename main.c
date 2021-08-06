#include "shell/utils.h"
#include "shell/builtins.h"
#include "shell/signals.h"
#include "shell/parsing.h"

int main(scpAttribute_unused int argc, char** argv, char** envp) {
	scpshellSignals_init();

	pid_t pid;
	int status;

	char* path = strdup(getenv("PATH"));
	if (!path) SCP_EXCEPTION(scpException_Exception, "no path variable in env");
	if (*path == '\0') SCP_EXCEPTION(scpException_Exception, "empty path variable in env");

	char** pathv = (char**)calloc(2, sizeof(char*));
	*pathv = path;
	size_t pathc = 1;
	for (size_t i = 0; path[i]; ++i) {
		if (path[i] == ':') {
			pathv = (char**)realloc(pathv, (++pathc + 1) * sizeof(char*));
			pathv[pathc - 1] = path + i + 1;
			pathv[pathc] = NULL;
			path[i] = '\0';
		}
	}

	struct scpHashMap* builtins = scpshellBuiltins_build_hashmap();

	char buffer[SCPSHELL_INPUT_BUFFER_SIZE];

	while (!feof(stdin)) {
		fputs("$ ", stdout);
		fgets(buffer, SCPSHELL_INPUT_BUFFER_SIZE, stdin);

		if (scpshellSignals_sigint_recieved) {
			scpshellSignals_sigint_recieved = 0;
			fputc('\n', stdout);
			continue;
		}

		if (scpshellUtils_flush_buffer(stdin, buffer)) {
			fprintf(stderr, "%s: input was too long to fit in the buffer\n", *argv);
			continue;
		}

		if (*buffer == '\0')
			continue;

		size_t new_argc = scpshellParsing_get_argc(buffer);
		char** new_argv = (char**)calloc((new_argc + 1), sizeof(char*));

		scpshellParsing_make_argv(new_argc, new_argv, buffer);
		char* cmd = scpshellUtils_to_lower(*new_argv);
		scpshellBuiltins_pair* pair = (scpshellBuiltins_pair*)scpHashMap.search(builtins, cmd);

		if (pair) {
			status = pair->value(new_argc, new_argv);
			if (status)
				printf("proccess exited with code %i\n", status);
		} else if ((pid = fork()) == 0) {
			if (execve(*new_argv, new_argv, __environ) == -1) {
				switch (errno) {
					case EINVAL:
					case EACCES:
					case ELOOP:
					case ETXTBSY:
					case ENOEXEC:
					case ENOTDIR:
					case ENOENT:
						break;
					case E2BIG:
						fprintf(stderr, "%s: argument list too long\n", *argv);
						return EXIT_FAILURE;
					case ENAMETOOLONG:
						fprintf(stderr, "%s: file name too long\n", *argv);
						return EXIT_FAILURE;
					case ENOMEM:
						fprintf(stderr, "%s: out of memory\n", *argv);
						return EXIT_FAILURE;
					default:
						fprintf(stderr, "%s: unknown error: %s\n", *argv, *new_argv);
						return EXIT_FAILURE;
				}
			}
			char* old_cmd = strdup(*new_argv);
			size_t old_cmd_len = strlen(old_cmd);
			for (size_t i = 0; i < pathc; ++i) {
				size_t path_len = (size_t)strlen(pathv[i]);
				cmd = realloc(cmd, (path_len + old_cmd_len + 2) * sizeof(char));
				for (size_t j = 0; j < path_len; ++j)
					cmd[j] = pathv[i][j];
				cmd[path_len] = '/';
				for (size_t j = 0; j < old_cmd_len; ++j)
					cmd[path_len + 1 + j] = old_cmd[j];
				cmd[path_len + 1 + old_cmd_len] = '\0';
				execve(cmd, new_argv, envp);
			}
			fprintf(stderr, "%s: command not found: %s\n", *argv, old_cmd);
			free(old_cmd);
			exit(EXIT_FAILURE);
		} else if (pid == -1)
			SCP_EXCEPTION(scpException_Exception, "failed to fork");
		else {
			if (waitpid(pid, &status, 0) == -1) {
				switch (errno) {
					case EINTR:
						break;
					case ECHILD:
						SCP_EXCEPTION(scpException_Exception, "failed to waitpid, no child proccess");
						break;
					case EINVAL:
						SCP_EXCEPTION(scpException_Exception, "failed to waitpid, no child proccess");
						break;
					default:
						SCP_EXCEPTION(scpException_Exception, "failed to waitpid, unknown error");
						break;
				}
			}
			if (WEXITSTATUS(status))
				printf("proccess exited with code %i\n", WEXITSTATUS(status));
			if (WIFSIGNALED(status))
				WTERMSIG(status);
		}

		if (scpshellSignals_sigint_recieved == 1) {
			scpshellSignals_sigint_recieved = 0;
			fputs("proccess was killed\n", stdout);
		}

		free(cmd);
		free(new_argv);
	}

	scpHashMap.delete(builtins);
	free(path);
	return EXIT_SUCCESS;
}
