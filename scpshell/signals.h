#ifndef SCPSHELL_SIGNALS_H
#define SCPSHELL_SIGNALS_H

#include "scpshell/utils.h"

static volatile sig_atomic_t scpshellSignals_sigint_recieved = 0;

void scpshellSignals_init(void);
static void scpshellSignals_handler(int signo);

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
			scpshellSignals_sigint_recieved = 1;
			break;
		default:
			SCP_EXCEPTION(scpException_Exception, "unhandled signal");
	}
}

#endif // SCPSHELL_SIGNALS_H
