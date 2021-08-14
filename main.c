#include "shell/utils.h"
#include "shell/builtins.h"
#include "shell/signals.h"
#include "shell/parser.h"
#include "shell/env.h"

int main(int argc, char** argv, char** envp) {
	scpshellSignals_init();
	struct scpshellEnv* env = scpshellEnv_new(argc, argv, envp);

	pid_t pid;
	int status;

	char buffer[SCPSHELL_INPUT_BUFFER_SIZE];

	while (!feof(stdin)) {
		fputs("$ ", stdout);
		fgets(buffer, SCPSHELL_INPUT_BUFFER_SIZE, stdin);

		if (scpshellSignals_sigint_recieved()) {
			fputc('\n', stdout);
			continue;
		}

		if (scpIO_flush_buffer(stdin, buffer)) {
			fprintf(stderr, "%s: input was too long to fit in the buffer\n", *argv);
			continue;
		}

		if (*buffer == '\0')
			continue;

		env->sub_argc = scpshellParsing_get_argc(buffer);
		env->sub_argv = (char**)calloc((size_t)env->sub_argc + 1, sizeof(char*));

		scpshellParsing_make_argv(env->sub_argc, env->sub_argv, buffer);
		char* cmd = scpString_new_lower(*env->sub_argv);
		scpshellBuiltins_pair* pair = scpshellBuiltins_search(env, cmd);

		if (pair) {
			status = pair->value(env);
			if (status)
				printf("proccess exited with code %i\n", status);
		} else if ((pid = fork()) == 0) {
			if (execve(*env->sub_argv, env->sub_argv, env->sub_envp) == -1) {
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
						fprintf(stderr, "%s: unknown error: %s\n", *argv, *env->sub_argv);
						return EXIT_FAILURE;
				}
			}
			char* old_cmd = scpString_strdup(*env->sub_argv);
			size_t old_cmd_len = strlen(old_cmd);
			for (int i = 0; i < env->pathc; ++i) {
				size_t path_len = (size_t)strlen(env->pathv[i]);
				cmd = realloc(cmd, (path_len + old_cmd_len + 2) * sizeof(char));
				for (size_t j = 0; j < path_len; ++j)
					cmd[j] = env->pathv[i][j];
				cmd[path_len] = '/';
				for (size_t j = 0; j < old_cmd_len; ++j)
					cmd[path_len + 1 + j] = old_cmd[j];
				cmd[path_len + 1 + old_cmd_len] = '\0';
				execve(cmd, env->sub_argv, env->sub_envp);
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

		if (scpshellSignals_sigint_recieved())
			fputs("proccess was killed\n", stdout);

		free(cmd);
		free(env->sub_argv);
	}

	scpshellEnv_delete(env);
	return EXIT_SUCCESS;
}
