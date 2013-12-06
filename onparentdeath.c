/*
 * Copyright (C) 2012 Andrew Ayer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization.
 */

/*
 * onparentdeath - execute the given command when onparentdeath's parent process dies
 */
#include <sys/prctl.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv)
{
	sigset_t		orig_signal_mask;
	sigset_t		signals;
	int			sig;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s PROGRAM [ARGS...]\n", argv[0]);
		return 2;
	}

	/* Block SIGUSR1 */
	sigemptyset(&signals);
	sigaddset(&signals, SIGUSR1);
	sigprocmask(SIG_BLOCK, &signals, &orig_signal_mask);

	/* Arrange to receive a SIGUSR1 when our parent dies */
	if (prctl(PR_SET_PDEATHSIG, SIGUSR1) == -1) {
		perror("prctl");
		return 1;
	}

	/* Ignore SIGHUP - but if our parent dies from SIGHUP then we'll get SIGUSR1 and execute the command */
	signal(SIGHUP, SIG_IGN);

	if (getppid() != 1) {
		/* Wait for our parent to die */
		/* (Only wait if our parent hasn't already died - otherwise, if our parent had died before we
		 * called prctl above, we would wait forever.) */
		sigwait(&signals, &sig);
	}

	/* Restore the signal mask before execing the command */
	sigprocmask(SIG_SETMASK, &orig_signal_mask, NULL);

	/* Exec the command */
	execvp(argv[1], argv + 1);
	perror(argv[1]);
	return 127;
}
