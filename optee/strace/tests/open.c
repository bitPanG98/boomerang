/*
 * Copyright (c) 2016 Dmitry V. Levin <ldv@altlinux.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "tests.h"
#include <sys/syscall.h>

#ifdef __NR_open

# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

int
main(void)
{
	static const char sample[] = "open.sample";
	int fd = syscall(__NR_open, sample, O_RDONLY|O_CREAT, 0400);

	if (fd < 0) {
		printf("open(\"%s\", O_RDONLY|O_CREAT, 0400)"
		       " = %d %s (%m)\n", sample, fd, errno2name());
	} else {
		printf("open(\"%s\", O_RDONLY|O_CREAT, 0400) = %d\n",
		       sample, fd);
		close(fd);
		if (unlink(sample))
			perror_msg_and_fail("unlink");

		fd = syscall(__NR_open, sample, O_RDONLY);
		printf("open(\"%s\", O_RDONLY) = %d %s (%m)\n",
		       sample, fd, errno2name());

		fd = syscall(__NR_open, sample, O_WRONLY|O_NONBLOCK|0x80000000);
		printf("open(\"%s\", O_WRONLY|O_NONBLOCK|0x80000000)"
		       " = %d %s (%m)\n", sample, fd, errno2name());
	}

	puts("+++ exited with 0 +++");
	return 0;
}

#else

SKIP_MAIN_UNDEFINED("__NR_open")

#endif
