/* $Id: error.h,v 1.1 2002/01/14 07:59:59 mkirchner Exp $ */

/*
* from: Stevens - Unix Network Programming
*/

#ifndef __ERROR_H__
#define __ERROR_H__
#include <stdlib.h>		/* for abort() */
#include <stdarg.h>		/* ANSI C header file */
#include <syslog.h>		/* for syslog() */
#include <stdio.h>		/* for fprintf and friends */
#include <string.h>		/* for strlen(), strcat() */
#include <errno.h>

#define MAXLINE 4096

int daemon_proc;		/* set nonzero by daemon_init() */

void err_ret(const char *, ...);
void err_sys(const char *, ...);
void err_dump(const char *, ...);
void err_msg(const char *, ...);
void err_quit(const char *, ...);
#endif
