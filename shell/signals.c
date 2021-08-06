#include "shell/signals.h"

static void scpshellSignals_handler(int signo);

static volatile sig_atomic_t _scpshellSignals_sigint_recieved = 0;

void scpshellSignals_init(void) {
	struct sigaction sa;
	sa.sa_handler = scpshellSignals_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		SCP_EXCEPTION(scpException_Exception, "signal handler failed");
}

void scpshellSignals_handler(int signo) {
	switch (signo) {
		case SIGINT:
			_scpshellSignals_sigint_recieved = 1;
			break;
		default:
			SCP_EXCEPTION(scpException_Exception, "unhandled signal");
	}
}

bool scpshellSignals_sigint_recieved(void) {
	bool r = _scpshellSignals_sigint_recieved == 1;
	_scpshellSignals_sigint_recieved = 0;
	return r;
}
