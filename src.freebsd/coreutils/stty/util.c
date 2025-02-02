/*-
 * Copyright (c) 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
#if 0
static char sccsid[] = "@(#)util.c	8.3 (Berkeley) 4/2/94";
#endif
#endif /* not lint */
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <unistd.h>
#include <termios.h>

#include "stty.h"
#include "extern.h"

/*
 * Gross, but since we're changing the control descriptor from 1 to 0, most
 * users will be probably be doing "stty > /dev/sometty" by accident.  If 1
 * and 2 are both ttys, but not the same, assume that 1 was incorrectly
 * redirected.
 */
void
checkredirect(void)
{
	struct stat sb1, sb2;

	if (isatty(STDOUT_FILENO) && isatty(STDERR_FILENO) &&
	    !fstat(STDOUT_FILENO, &sb1) && !fstat(STDERR_FILENO, &sb2) &&
	    (sb1.st_rdev != sb2.st_rdev))
warnx("stdout appears redirected, but stdin is the control descriptor");
}

static const int baudlist[] = {
	0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600,
	19200, 38400, 57600, 115200, 230400, 460800, 500000, 576000, 921600,
	1000000, 1152000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000,
};

static const speed_t baudvals[] = {
	B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400,
	B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000,
	B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000,
	B3000000, B3500000, B4000000,
};

int get_baud(speed_t s) {
	if (s & CBAUDEX)
		s = (s & ~CBAUDEX) + 15;
	return baudlist[s];
}

speed_t get_speed(unsigned long b) {
	for (size_t i = 0; i < (sizeof(baudlist) / sizeof(int)); ++i) {
		if ((unsigned long)baudlist[i] != b)
			continue;
		return baudvals[i];
	}
	errx(1, "unknown speed for baud %lu", b);
}
