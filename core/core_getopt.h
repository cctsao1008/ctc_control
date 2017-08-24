#if 0 //GETOPT_1 https://github.com/skeeto/getopt

/* A minimal POSIX getopt() implementation in ANSI C
 *
 * This is free and unencumbered software released into the public domain.
 *
 * This implementation supports the convention of resetting the option
 * parser by assigning optind to 0. This resets the internal state
 * appropriately.
 *
 * Ref: http://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
 */
#ifndef GETOPT_H
#define GETOPT_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int optind = 1;
static int opterr = 1;
static int optopt;
static char *optarg;

static int
getopt(int argc, char * const argv[], const char *optstring)
{
    static int optpos = 1;
    const char *arg;
    (void)argc;

    /* Reset? */
    if (optind == 0) {
        optind = 1;
        optpos = 1;
    }

    arg = argv[optind];
    if (arg && strcmp(arg, "--") == 0) {
        optind++;
        return -1;
    } else if (!arg || arg[0] != '-' || !isalnum(arg[1])) {
        return -1;
    } else {
		char *opt = (char *)strchr(optstring, arg[optpos]);
        optopt = arg[optpos];
        if (!opt) {
            if (opterr && *optstring != ':')
                fprintf(stderr, "%s: illegal option: %c\n", argv[0], optopt);
            return '?';
        } else if (opt[1] == ':') {
            if (arg[optpos + 1]) {
                optarg = (char *)arg + optpos + 1;
                optind++;
                optpos = 1;
                return optopt;
            } else if (argv[optind + 1]) {
                optarg = (char *)argv[optind + 1];
                optind += 2;
                optpos = 1;
                return optopt;
            } else {
                if (opterr && *optstring != ':')
                    fprintf(stderr, 
                            "%s: option requires an argument: %c\n", 
                            argv[0], optopt);
                return *optstring == ':' ? ':' : '?';
            }
        } else {
            if (!arg[++optpos]) {
                optind++;
                optpos = 1;
            }
            return optopt;
        }
    }
}

#endif

#endif // END_GETOPT_1

#if 0 //GETOPT_2 https://github.com/skandhurkat/Getopt-for-Visual-Studio

#ifndef __GETOPT_H__
/**
 * DISCLAIMER
 * This file is part of the mingw-w64 runtime package.
 *
 * The mingw-w64 runtime package and its code is distributed in the hope that it 
 * will be useful but WITHOUT ANY WARRANTY.  ALL WARRANTIES, EXPRESSED OR 
 * IMPLIED ARE HEREBY DISCLAIMED.  This includes but is not limited to 
 * warranties of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
 /*
 * Copyright (c) 2002 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Sponsored in part by the Defense Advanced Research Projects
 * Agency (DARPA) and Air Force Research Laboratory, Air Force
 * Materiel Command, USAF, under agreement number F39502-99-1-0512.
 */
/*-
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Dieter Baron and Thomas Klausner.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma warning(disable:4996);

#define __GETOPT_H__

/* All the headers include this file. */
#include <crtdefs.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	REPLACE_GETOPT		/* use this getopt as the system getopt(3) */

#ifdef REPLACE_GETOPT
static int	opterr = 1;		/* if error message should be printed */
static int	optind = 1;		/* index into parent argv vector */
static int	optopt = '?';		/* character checked for validity */
#undef	optreset		/* see getopt.h */
#define	optreset		__mingw_optreset
static int	optreset;		/* reset getopt */
char    *optarg;		/* argument associated with option */
#endif

//extern int optind;		/* index of first non-option in argv      */
//extern int optopt;		/* single option character, as parsed     */
//extern int opterr;		/* flag to enable built-in diagnostics... */
//				/* (user may set to zero, to suppress)    */
//
//extern char *optarg;		/* pointer to argument of current option  */

#define PRINT_ERROR	((opterr) && (*options != ':'))

#define FLAG_PERMUTE	0x01	/* permute non-options to the end of argv */
#define FLAG_ALLARGS	0x02	/* treat non-options as args to option "-1" */
#define FLAG_LONGONLY	0x04	/* operate as getopt_long_only */

/* return values */
#define	BADCH		(int)'?'
#define	BADARG		((*options == ':') ? (int)':' : (int)'?')
#define	INORDER 	(int)1

#ifndef __CYGWIN__
#define __progname __argv[0]
#else
extern char __declspec(dllimport) *__progname;
#endif

#ifdef __CYGWIN__
static char EMSG[] = "";
#else
#define	EMSG		""
#endif

static int getopt_internal(int, char * const *, const char *,
			   const struct option *, int *, int);
static int parse_long_options(char * const *, const char *,
			      const struct option *, int *, int);
static int gcd(int, int);
static void permute_args(int, int, int, char * const *);

static char *place = EMSG; /* option letter processing */

/* XXX: set optreset to 1 rather than these two */
static int nonopt_start = -1; /* first non option argument (for permute) */
static int nonopt_end = -1;   /* first option after non options (for permute) */

/* Error messages */
static const char recargchar[] = "option requires an argument -- %c";
static const char recargstring[] = "option requires an argument -- %s";
static const char ambig[] = "ambiguous option -- %.*s";
static const char noarg[] = "option doesn't take an argument -- %.*s";
static const char illoptchar[] = "unknown option -- %c";
static const char illoptstring[] = "unknown option -- %s";

static void
_vwarnx(const char *fmt,va_list ap)
{
  (void)fprintf(stderr,"%s: ",__progname);
  if (fmt != NULL)
    (void)vfprintf(stderr,fmt,ap);
  (void)fprintf(stderr,"\n");
}

static void
warnx(const char *fmt,...)
{
  va_list ap;
  va_start(ap,fmt);
  _vwarnx(fmt,ap);
  va_end(ap);
}

/*
 * Compute the greatest common divisor of a and b.
 */
static int
gcd(int a, int b)
{
	int c;

	c = a % b;
	while (c != 0) {
		a = b;
		b = c;
		c = a % b;
	}

	return (b);
}

/*
 * Exchange the block from nonopt_start to nonopt_end with the block
 * from nonopt_end to opt_end (keeping the same order of arguments
 * in each block).
 */
static void
permute_args(int panonopt_start, int panonopt_end, int opt_end,
	char * const *nargv)
{
	int cstart, cyclelen, i, j, ncycle, nnonopts, nopts, pos;
	char *swap;

	/*
	 * compute lengths of blocks and number and size of cycles
	 */
	nnonopts = panonopt_end - panonopt_start;
	nopts = opt_end - panonopt_end;
	ncycle = gcd(nnonopts, nopts);
	cyclelen = (opt_end - panonopt_start) / ncycle;

	for (i = 0; i < ncycle; i++) {
		cstart = panonopt_end+i;
		pos = cstart;
		for (j = 0; j < cyclelen; j++) {
			if (pos >= panonopt_end)
				pos -= nnonopts;
			else
				pos += nopts;
			swap = nargv[pos];
			/* LINTED const cast */
			((char **) nargv)[pos] = nargv[cstart];
			/* LINTED const cast */
			((char **)nargv)[cstart] = swap;
		}
	}
}

#ifdef REPLACE_GETOPT
/*
 * getopt --
 *	Parse argc/argv argument vector.
 *
 * [eventually this will replace the BSD getopt]
 */
int
getopt(int nargc, char * const *nargv, const char *options)
{

	/*
	 * We don't pass FLAG_PERMUTE to getopt_internal() since
	 * the BSD getopt(3) (unlike GNU) has never done this.
	 *
	 * Furthermore, since many privileged programs call getopt()
	 * before dropping privileges it makes sense to keep things
	 * as simple (and bug-free) as possible.
	 */
	return (getopt_internal(nargc, nargv, options, NULL, NULL, 0));
}
#endif /* REPLACE_GETOPT */

//extern int getopt(int nargc, char * const *nargv, const char *options);

#ifdef _BSD_SOURCE
/*
 * BSD adds the non-standard `optreset' feature, for reinitialisation
 * of `getopt' parsing.  We support this feature, for applications which
 * proclaim their BSD heritage, before including this header; however,
 * to maintain portability, developers are advised to avoid it.
 */
# define optreset  __mingw_optreset
extern int optreset;
#endif
#ifdef __cplusplus
}
#endif
/*
 * POSIX requires the `getopt' API to be specified in `unistd.h';
 * thus, `unistd.h' includes this header.  However, we do not want
 * to expose the `getopt_long' or `getopt_long_only' APIs, when
 * included in this manner.  Thus, close the standard __GETOPT_H__
 * declarations block, and open an additional __GETOPT_LONG_H__
 * specific block, only when *not* __UNISTD_H_SOURCED__, in which
 * to declare the extended API.
 */
#endif /* !defined(__GETOPT_H__) */

#if !defined(__UNISTD_H_SOURCED__) && !defined(__GETOPT_LONG_H__)
#define __GETOPT_LONG_H__

#ifdef __cplusplus
extern "C" {
#endif

struct option		/* specification for a long form option...	*/
{
  const char *name;		/* option name, without leading hyphens */
  int         has_arg;		/* does it take an argument?		*/
  int        *flag;		/* where to save its status, or NULL	*/
  int         val;		/* its associated status value		*/
};

enum    		/* permitted values for its `has_arg' field...	*/
{
  no_argument = 0,      	/* option never takes an argument	*/
  required_argument,		/* option always requires an argument	*/
  optional_argument		/* option may take an argument		*/
};

/*
 * parse_long_options --
 *	Parse long options in argc/argv argument vector.
 * Returns -1 if short_too is set and the option does not match long_options.
 */
static int
parse_long_options(char * const *nargv, const char *options,
	const struct option *long_options, int *idx, int short_too)
{
	char *current_argv, *has_equal;
	size_t current_argv_len;
	int i, ambiguous, match;

#define IDENTICAL_INTERPRETATION(_x, _y)                                \
	(long_options[(_x)].has_arg == long_options[(_y)].has_arg &&    \
	 long_options[(_x)].flag == long_options[(_y)].flag &&          \
	 long_options[(_x)].val == long_options[(_y)].val)

	current_argv = place;
	match = -1;
	ambiguous = 0;

	optind++;

	if ((has_equal = strchr(current_argv, '=')) != NULL) {
		/* argument found (--option=arg) */
		current_argv_len = has_equal - current_argv;
		has_equal++;
	} else
		current_argv_len = strlen(current_argv);

	for (i = 0; long_options[i].name; i++) {
		/* find matching long option */
		if (strncmp(current_argv, long_options[i].name,
		    current_argv_len))
			continue;

		if (strlen(long_options[i].name) == current_argv_len) {
			/* exact match */
			match = i;
			ambiguous = 0;
			break;
		}
		/*
		 * If this is a known short option, don't allow
		 * a partial match of a single character.
		 */
		if (short_too && current_argv_len == 1)
			continue;

		if (match == -1)	/* partial match */
			match = i;
		else if (!IDENTICAL_INTERPRETATION(i, match))
			ambiguous = 1;
	}
	if (ambiguous) {
		/* ambiguous abbreviation */
		if (PRINT_ERROR)
			warnx(ambig, (int)current_argv_len,
			     current_argv);
		optopt = 0;
		return (BADCH);
	}
	if (match != -1) {		/* option found */
		if (long_options[match].has_arg == no_argument
		    && has_equal) {
			if (PRINT_ERROR)
				warnx(noarg, (int)current_argv_len,
				     current_argv);
			/*
			 * XXX: GNU sets optopt to val regardless of flag
			 */
			if (long_options[match].flag == NULL)
				optopt = long_options[match].val;
			else
				optopt = 0;
			return (BADARG);
		}
		if (long_options[match].has_arg == required_argument ||
		    long_options[match].has_arg == optional_argument) {
			if (has_equal)
				optarg = has_equal;
			else if (long_options[match].has_arg ==
			    required_argument) {
				/*
				 * optional argument doesn't use next nargv
				 */
				optarg = nargv[optind++];
			}
		}
		if ((long_options[match].has_arg == required_argument)
		    && (optarg == NULL)) {
			/*
			 * Missing argument; leading ':' indicates no error
			 * should be generated.
			 */
			if (PRINT_ERROR)
				warnx(recargstring,
				    current_argv);
			/*
			 * XXX: GNU sets optopt to val regardless of flag
			 */
			if (long_options[match].flag == NULL)
				optopt = long_options[match].val;
			else
				optopt = 0;
			--optind;
			return (BADARG);
		}
	} else {			/* unknown option */
		if (short_too) {
			--optind;
			return (-1);
		}
		if (PRINT_ERROR)
			warnx(illoptstring, current_argv);
		optopt = 0;
		return (BADCH);
	}
	if (idx)
		*idx = match;
	if (long_options[match].flag) {
		*long_options[match].flag = long_options[match].val;
		return (0);
	} else
		return (long_options[match].val);
#undef IDENTICAL_INTERPRETATION
}

/*
 * getopt_internal --
 *	Parse argc/argv argument vector.  Called by user level routines.
 */
static int
getopt_internal(int nargc, char * const *nargv, const char *options,
	const struct option *long_options, int *idx, int flags)
{
	char *oli;				/* option letter list index */
	int optchar, short_too;
	static int posixly_correct = -1;

	if (options == NULL)
		return (-1);

	/*
	 * XXX Some GNU programs (like cvs) set optind to 0 instead of
	 * XXX using optreset.  Work around this braindamage.
	 */
	if (optind == 0)
		optind = optreset = 1;

	/*
	 * Disable GNU extensions if POSIXLY_CORRECT is set or options
	 * string begins with a '+'.
	 *
	 * CV, 2009-12-14: Check POSIXLY_CORRECT anew if optind == 0 or
	 *                 optreset != 0 for GNU compatibility.
	 */
	if (posixly_correct == -1 || optreset != 0)
		posixly_correct = (getenv("POSIXLY_CORRECT") != NULL);
	if (*options == '-')
		flags |= FLAG_ALLARGS;
	else if (posixly_correct || *options == '+')
		flags &= ~FLAG_PERMUTE;
	if (*options == '+' || *options == '-')
		options++;

	optarg = NULL;
	if (optreset)
		nonopt_start = nonopt_end = -1;
start:
	if (optreset || !*place) {		/* update scanning pointer */
		optreset = 0;
		if (optind >= nargc) {          /* end of argument vector */
			place = EMSG;
			if (nonopt_end != -1) {
				/* do permutation, if we have to */
				permute_args(nonopt_start, nonopt_end,
				    optind, nargv);
				optind -= nonopt_end - nonopt_start;
			}
			else if (nonopt_start != -1) {
				/*
				 * If we skipped non-options, set optind
				 * to the first of them.
				 */
				optind = nonopt_start;
			}
			nonopt_start = nonopt_end = -1;
			return (-1);
		}
		if (*(place = nargv[optind]) != '-' ||
		    (place[1] == '\0' && strchr(options, '-') == NULL)) {
			place = EMSG;		/* found non-option */
			if (flags & FLAG_ALLARGS) {
				/*
				 * GNU extension:
				 * return non-option as argument to option 1
				 */
				optarg = nargv[optind++];
				return (INORDER);
			}
			if (!(flags & FLAG_PERMUTE)) {
				/*
				 * If no permutation wanted, stop parsing
				 * at first non-option.
				 */
				return (-1);
			}
			/* do permutation */
			if (nonopt_start == -1)
				nonopt_start = optind;
			else if (nonopt_end != -1) {
				permute_args(nonopt_start, nonopt_end,
				    optind, nargv);
				nonopt_start = optind -
				    (nonopt_end - nonopt_start);
				nonopt_end = -1;
			}
			optind++;
			/* process next argument */
			goto start;
		}
		if (nonopt_start != -1 && nonopt_end == -1)
			nonopt_end = optind;

		/*
		 * If we have "-" do nothing, if "--" we are done.
		 */
		if (place[1] != '\0' && *++place == '-' && place[1] == '\0') {
			optind++;
			place = EMSG;
			/*
			 * We found an option (--), so if we skipped
			 * non-options, we have to permute.
			 */
			if (nonopt_end != -1) {
				permute_args(nonopt_start, nonopt_end,
				    optind, nargv);
				optind -= nonopt_end - nonopt_start;
			}
			nonopt_start = nonopt_end = -1;
			return (-1);
		}
	}

	/*
	 * Check long options if:
	 *  1) we were passed some
	 *  2) the arg is not just "-"
	 *  3) either the arg starts with -- we are getopt_long_only()
	 */
	if (long_options != NULL && place != nargv[optind] &&
	    (*place == '-' || (flags & FLAG_LONGONLY))) {
		short_too = 0;
		if (*place == '-')
			place++;		/* --foo long option */
		else if (*place != ':' && strchr(options, *place) != NULL)
			short_too = 1;		/* could be short option too */

		optchar = parse_long_options(nargv, options, long_options,
		    idx, short_too);
		if (optchar != -1) {
			place = EMSG;
			return (optchar);
		}
	}

	if ((optchar = (int)*place++) == (int)':' ||
	    (optchar == (int)'-' && *place != '\0') ||
	    (oli = (char*)strchr(options, optchar)) == NULL) {
		/*
		 * If the user specified "-" and  '-' isn't listed in
		 * options, return -1 (non-option) as per POSIX.
		 * Otherwise, it is an unknown option character (or ':').
		 */
		if (optchar == (int)'-' && *place == '\0')
			return (-1);
		if (!*place)
			++optind;
		if (PRINT_ERROR)
			warnx(illoptchar, optchar);
		optopt = optchar;
		return (BADCH);
	}
	if (long_options != NULL && optchar == 'W' && oli[1] == ';') {
		/* -W long-option */
		if (*place)			/* no space */
			/* NOTHING */;
		else if (++optind >= nargc) {	/* no arg */
			place = EMSG;
			if (PRINT_ERROR)
				warnx(recargchar, optchar);
			optopt = optchar;
			return (BADARG);
		} else				/* white space */
			place = nargv[optind];
		optchar = parse_long_options(nargv, options, long_options,
		    idx, 0);
		place = EMSG;
		return (optchar);
	}
	if (*++oli != ':') {			/* doesn't take argument */
		if (!*place)
			++optind;
	} else {				/* takes (optional) argument */
		optarg = NULL;
		if (*place)			/* no white space */
			optarg = place;
		else if (oli[1] != ':') {	/* arg not optional */
			if (++optind >= nargc) {	/* no arg */
				place = EMSG;
				if (PRINT_ERROR)
					warnx(recargchar, optchar);
				optopt = optchar;
				return (BADARG);
			} else
				optarg = nargv[optind];
		}
		place = EMSG;
		++optind;
	}
	/* dump back option letter */
	return (optchar);
}

/*
 * getopt_long --
 *	Parse argc/argv argument vector.
 */
int
getopt_long(int nargc, char * const *nargv, const char *options,
    const struct option *long_options, int *idx)
{

	return (getopt_internal(nargc, nargv, options, long_options, idx,
	    FLAG_PERMUTE));
}

/*
 * getopt_long_only --
 *	Parse argc/argv argument vector.
 */
int
getopt_long_only(int nargc, char * const *nargv, const char *options,
    const struct option *long_options, int *idx)
{

	return (getopt_internal(nargc, nargv, options, long_options, idx,
	    FLAG_PERMUTE|FLAG_LONGONLY));
}

//extern int getopt_long(int nargc, char * const *nargv, const char *options,
//    const struct option *long_options, int *idx);
//extern int getopt_long_only(int nargc, char * const *nargv, const char *options,
//    const struct option *long_options, int *idx);
/*
 * Previous MinGW implementation had...
 */
#ifndef HAVE_DECL_GETOPT
/*
 * ...for the long form API only; keep this for compatibility.
 */
# define HAVE_DECL_GETOPT	1
#endif

#ifdef __cplusplus
}
#endif

#endif /* !defined(__UNISTD_H_SOURCED__) && !defined(__GETOPT_LONG_H__) */

#endif // END_GETOPT_2

#if 0 //GETOPT_3 https://github.com/skeeto/optparse

/* Optparse --- portable, reentrant, embeddable, getopt-like option parser
 *
 * This is free and unencumbered software released into the public domain.
 *
 * To get the implementation, define OPTPARSE_IMPLEMENTATION.
 * Optionally define OPTPARSE_API to control the API's visibility
 * and/or linkage (static, __attribute__, __declspec).
 *
 * The POSIX getopt() option parser has three fatal flaws. These flaws
 * are solved by Optparse.
 *
 * 1) Parser state is stored entirely in global variables, some of
 * which are static and inaccessible. This means only one thread can
 * use getopt(). It also means it's not possible to recursively parse
 * nested sub-arguments while in the middle of argument parsing.
 * Optparse fixes this by storing all state on a local struct.
 *
 * 2) The POSIX standard provides no way to properly reset the parser.
 * This means for portable code that getopt() is only good for one
 * run, over one argv with one option string. It also means subcommand
 * options cannot be processed with getopt(). Most implementations
 * provide a method to reset the parser, but it's not portable.
 * Optparse provides an optparse_arg() function for stepping over
 * subcommands and continuing parsing of options with another option
 * string. The Optparse struct itself can be passed around to
 * subcommand handlers for additional subcommand option parsing. A
 * full reset can be achieved by with an additional optparse_init().
 *
 * 3) Error messages are printed to stderr. This can be disabled with
 * opterr, but the messages themselves are still inaccessible.
 * Optparse solves this by writing an error message in its errmsg
 * field. The downside to Optparse is that this error message will
 * always be in English rather than the current locale.
 *
 * Optparse should be familiar with anyone accustomed to getopt(), and
 * it could be a nearly drop-in replacement. The option string is the
 * same and the fields have the same names as the getopt() global
 * variables (optarg, optind, optopt).
 *
 * Optparse also supports GNU-style long options with optparse_long().
 * The interface is slightly different and simpler than getopt_long().
 *
 * By default, argv is permuted as it is parsed, moving non-option
 * arguments to the end. This can be disabled by setting the `permute`
 * field to 0 after initialization.
 */
#ifndef OPTPARSE_H
#define OPTPARSE_H

#ifndef OPTPARSE_API
#  define OPTPARSE_API
#endif

struct optparse {
    char **argv;
    int permute;
    int optind;
    int optopt;
    char *optarg;
    char errmsg[64];
    int subopt;
};

enum optparse_argtype {
    OPTPARSE_NONE,
    OPTPARSE_REQUIRED,
    OPTPARSE_OPTIONAL
};

struct optparse_long {
    const char *longname;
    int shortname;
    enum optparse_argtype argtype;
};

/**
 * Initializes the parser state.
 */
OPTPARSE_API
void optparse_init(struct optparse *options, char **argv);

/**
 * Read the next option in the argv array.
 * @param optstring a getopt()-formatted option string.
 * @return the next option character, -1 for done, or '?' for error
 *
 * Just like getopt(), a character followed by no colons means no
 * argument. One colon means the option has a required argument. Two
 * colons means the option takes an optional argument.
 */
OPTPARSE_API
int optparse(struct optparse *options, const char *optstring);

/**
 * Handles GNU-style long options in addition to getopt() options.
 * This works a lot like GNU's getopt_long(). The last option in
 * longopts must be all zeros, marking the end of the array. The
 * longindex argument may be NULL.
 */
OPTPARSE_API
int optparse_long(struct optparse *options,
                  const struct optparse_long *longopts,
                  int *longindex);

/**
 * Used for stepping over non-option arguments.
 * @return the next non-option argument, or NULL for no more arguments
 *
 * Argument parsing can continue with optparse() after using this
 * function. That would be used to parse the options for the
 * subcommand returned by optparse_arg(). This function allows you to
 * ignore the value of optind.
 */
OPTPARSE_API
char *optparse_arg(struct optparse *options);

/* Implementation */
#ifdef OPTPARSE_IMPLEMENTATION

#define OPTPARSE_MSG_INVALID "invalid option"
#define OPTPARSE_MSG_MISSING "option requires an argument"
#define OPTPARSE_MSG_TOOMANY "option takes no arguments"

static int
optparse_error(struct optparse *options, const char *msg, const char *data)
{
    unsigned p = 0;
    const char *sep = " -- '";
    while (*msg)
        options->errmsg[p++] = *msg++;
    while (*sep)
        options->errmsg[p++] = *sep++;
    while (p < sizeof(options->errmsg) - 2 && *data)
        options->errmsg[p++] = *data++;
    options->errmsg[p++] = '\'';
    options->errmsg[p++] = '\0';
    return '?';
}

OPTPARSE_API
void
optparse_init(struct optparse *options, char **argv)
{
    options->argv = argv;
    options->permute = 1;
    options->optind = 1;
    options->subopt = 0;
    options->optarg = 0;
    options->errmsg[0] = '\0';
}

static int
optparse_is_dashdash(const char *arg)
{
    return arg != 0 && arg[0] == '-' && arg[1] == '-' && arg[2] == '\0';
}

static int
optparse_is_shortopt(const char *arg)
{
    return arg != 0 && arg[0] == '-' && arg[1] != '-' && arg[1] != '\0';
}

static int
optparse_is_longopt(const char *arg)
{
    return arg != 0 && arg[0] == '-' && arg[1] == '-' && arg[2] != '\0';
}

static void
optparse_permute(struct optparse *options, int index)
{
    char *nonoption = options->argv[index];
    int i;
    for (i = index; i < options->optind - 1; i++)
        options->argv[i] = options->argv[i + 1];
    options->argv[options->optind - 1] = nonoption;
}

static int
optparse_argtype(const char *optstring, char c)
{
    int count = OPTPARSE_NONE;
    if (c == ':')
        return -1;
    for (; *optstring && c != *optstring; optstring++);
    if (!*optstring)
        return -1;
    if (optstring[1] == ':')
        count += optstring[2] == ':' ? 2 : 1;
    return count;
}

OPTPARSE_API
int
optparse(struct optparse *options, const char *optstring)
{
    int type;
    char *next;
    char *option = options->argv[options->optind];
    options->errmsg[0] = '\0';
    options->optopt = 0;
    options->optarg = 0;
    if (option == 0) {
        return -1;
    } else if (optparse_is_dashdash(option)) {
        options->optind++; /* consume "--" */
        return -1;
    } else if (!optparse_is_shortopt(option)) {
        if (options->permute) {
            int index = options->optind++;
            int r = optparse(options, optstring);
            optparse_permute(options, index);
            options->optind--;
            return r;
        } else {
            return -1;
        }
    }
    option += options->subopt + 1;
    options->optopt = option[0];
    type = optparse_argtype(optstring, option[0]);
    next = options->argv[options->optind + 1];
    switch (type) {
    case -1: {
        char str[2] = {0, 0};
        str[0] = option[0];
        options->optind++;
        return optparse_error(options, OPTPARSE_MSG_INVALID, str);
    }
    case OPTPARSE_NONE:
        if (option[1]) {
            options->subopt++;
        } else {
            options->subopt = 0;
            options->optind++;
        }
        return option[0];
    case OPTPARSE_REQUIRED:
        options->subopt = 0;
        options->optind++;
        if (option[1]) {
            options->optarg = option + 1;
        } else if (next != 0) {
            options->optarg = next;
            options->optind++;
        } else {
            char str[2] = {0, 0};
            str[0] = option[0];
            options->optarg = 0;
            return optparse_error(options, OPTPARSE_MSG_MISSING, str);
        }
        return option[0];
    case OPTPARSE_OPTIONAL:
        options->subopt = 0;
        options->optind++;
        if (option[1])
            options->optarg = option + 1;
        else
            options->optarg = 0;
        return option[0];
    }
    return 0;
}

OPTPARSE_API
char *
optparse_arg(struct optparse *options)
{
    char *option = options->argv[options->optind];
    options->subopt = 0;
    if (option != 0)
        options->optind++;
    return option;
}

static int
optparse_longopts_end(const struct optparse_long *longopts, int i)
{
    return !longopts[i].longname && !longopts[i].shortname;
}

static void
optparse_from_long(const struct optparse_long *longopts, char *optstring)
{
    char *p = optstring;
    int i;
    for (i = 0; !optparse_longopts_end(longopts, i); i++) {
        if (longopts[i].shortname) {
            int a;
            *p++ = longopts[i].shortname;
            for (a = 0; a < (int)longopts[i].argtype; a++)
                *p++ = ':';
        }
    }
    *p = '\0';
}

/* Unlike strcmp(), handles options containing "=". */
static int
optparse_longopts_match(const char *longname, const char *option)
{
    const char *a = option, *n = longname;
    if (longname == 0)
        return 0;
    for (; *a && *n && *a != '='; a++, n++)
        if (*a != *n)
            return 0;
    return *n == '\0' && (*a == '\0' || *a == '=');
}

/* Return the part after "=", or NULL. */
static char *
optparse_longopts_arg(char *option)
{
    for (; *option && *option != '='; option++);
    if (*option == '=')
        return option + 1;
    else
        return 0;
}

static int
optparse_long_fallback(struct optparse *options,
                       const struct optparse_long *longopts,
                       int *longindex)
{
    int result;
    char optstring[96 * 3 + 1]; /* 96 ASCII printable characters */
    optparse_from_long(longopts, optstring);
    result = optparse(options, optstring);
    if (longindex != 0) {
        *longindex = -1;
        if (result != -1) {
            int i;
            for (i = 0; !optparse_longopts_end(longopts, i); i++)
                if (longopts[i].shortname == options->optopt)
                    *longindex = i;
        }
    }
    return result;
}

OPTPARSE_API
int
optparse_long(struct optparse *options,
              const struct optparse_long *longopts,
              int *longindex)
{
    int i;
    char *option = options->argv[options->optind];
    if (option == 0) {
        return -1;
    } else if (optparse_is_dashdash(option)) {
        options->optind++; /* consume "--" */
        return -1;
    } else if (optparse_is_shortopt(option)) {
        return optparse_long_fallback(options, longopts, longindex);
    } else if (!optparse_is_longopt(option)) {
        if (options->permute) {
            int index = options->optind++;
            int r = optparse_long(options, longopts, longindex);
            optparse_permute(options, index);
            options->optind--;
            return r;
        } else {
            return -1;
        }
    }

    /* Parse as long option. */
    options->errmsg[0] = '\0';
    options->optopt = 0;
    options->optarg = 0;
    option += 2; /* skip "--" */
    options->optind++;
    for (i = 0; !optparse_longopts_end(longopts, i); i++) {
        const char *name = longopts[i].longname;
        if (optparse_longopts_match(name, option)) {
            char *arg;
            if (longindex)
                *longindex = i;
            options->optopt = longopts[i].shortname;
            arg = optparse_longopts_arg(option);
            if (longopts[i].argtype == OPTPARSE_NONE && arg != 0) {
                return optparse_error(options, OPTPARSE_MSG_TOOMANY, name);
            } if (arg != 0) {
                options->optarg = arg;
            } else if (longopts[i].argtype == OPTPARSE_REQUIRED) {
                options->optarg = options->argv[options->optind];
                if (options->optarg == 0)
                    return optparse_error(options, OPTPARSE_MSG_MISSING, name);
                else
                    options->optind++;
            }
            return options->optopt;
        }
    }
    return optparse_error(options, OPTPARSE_MSG_INVALID, option);
}

#endif /* OPTPARSE_IMPLEMENTATION */
#endif /* OPTPARSE_H */

#endif // END_GETOPT_3

#if 0 //GETOPT_4 https://github.com/wc-duck/getopt
/* a getopt.
   version 0.1, march, 2012
   Copyright (C) 2012- Fredrik Kihlander
   https://github.com/wc-duck/getopt
   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.
   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:
   1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
   Fredrik Kihlander
*/

#include <getopt.h>

#include <stdio.h>  /* for vsnprintf */
#include <stdarg.h> /* for va_list */
#include <string.h>
#if !defined(_MSC_VER)
#   include <strings.h> /* for strncasecmp */
#else
#   include <ctype.h> /* tolower */
#endif

static int str_case_cmp_len(const char* s1, const char* s2, unsigned int len)
{
#if defined (_MSC_VER)
	for(unsigned int i = 0; i < len; i++)
	{
		int c1 = tolower(s1[i]);
		int c2 = tolower(s2[i]);
		if(c1 < c2) return -1;
		if(c1 > c2) return  1;
		if(c1 == '\0' && c1 == c2) return 0;
	}
	return 0;
#else /* defined (_MSC_VER) */
	return strncasecmp(s1, s2, len);
#endif /* defined (_MSC_VER) */
}

static int str_format(char* buf, size_t buf_size, const char* fmt, ...)
{
	va_list args;
	va_start( args, fmt );
	int ret = vsnprintf( buf, buf_size, fmt, args );
#if defined(_MSC_VER)
	buf[buf_size - 1] = '\0';
#endif
	va_end( args );
	return ret;
}

int getopt_create_context( getopt_context_t* ctx, int argc, const char** argv, const getopt_option_t* opts )
{
	ctx->argc            = argc - 1; /* stripping away file-name! */
	ctx->argv            = argv + 1; /* stripping away file-name! */
	ctx->opts            = opts;
	ctx->current_index   = 0;
	ctx->current_opt_arg = 0x0;

	/* count opts */
	ctx->num_opts = 0;
	const getopt_option_t* opt = opts;
	while( !(opt->name == 0x0 && opt->name_short == 0) )
	{
		if( opt->value == '!' || 
			opt->value == '?' || 
			opt->value == '+' || 
			opt->value ==  0  || 
			opt->value == -1)
			return -1;

		ctx->num_opts++; opt++;
	}

	ctx->num_opts = (int)(opt - opts);

	return 0;
}

int getopt_next( getopt_context_t* ctx )
{
	/* are all options processed? */
	if(ctx->current_index == ctx->argc )
		return -1;

	/* reset opt-arg */
	ctx->current_opt_arg = 0x0;

	const char* curr_token = ctx->argv[ ctx->current_index ];
	
	/* this token has been processed! */
	ctx->current_index++;

	/* check if item is no option */
	if( curr_token[0] && curr_token[0] != '-' )
	{
		ctx->current_opt_arg = curr_token;
		return '+'; /* return '+' as identifier for no option! */
	}
	
	const getopt_option_t* found_opt = 0x0;
	const char* found_arg = 0x0;

	/* short opt */
	if( curr_token[1] != '\0' && curr_token[1] != '-' && curr_token[2] == '\0' )
	{
		int i = 0;
		for( ; i < ctx->num_opts; i++ )
		{
			const getopt_option_t* opt = ctx->opts + i;

			if( opt->name_short == curr_token[1] )
			{
				found_opt = opt;

				/* if there is an value when: - current_index < argc and value in argv[current_index] do not start with '-' */
				if( ( ( ctx->current_index != ctx->argc) && ( ctx->argv[ctx->current_index][0] != '-' ) ) && 
					  ( opt->type == GETOPT_OPTION_TYPE_OPTIONAL || opt->type == GETOPT_OPTION_TYPE_REQUIRED ) )
				{
					found_arg = ctx->argv[ctx->current_index];
					ctx->current_index++; /* next token has been processed aswell! */
				}
				break;
			}
		}
	}
	/* long opt */
	else if(curr_token[1] == '-' && curr_token[2] != '\0')
	{
		const char* check_option = curr_token + 2;

		int i = 0;
		for( ; i < ctx->num_opts; i++ )
		{
			const getopt_option_t* opt = ctx->opts + i;

			unsigned int name_len = (unsigned int)strlen( opt->name );

			if( str_case_cmp_len( opt->name, check_option, name_len ) == 0 )
			{
				check_option += name_len;

				/* find arg if there is any */
				switch( *check_option )
				{
					case '\0':
					{
						if( ctx->current_index < ctx->argc ) /* are there more tokens that can contain the '='? */
						{
							const char* next_token = ctx->argv[ ctx->current_index ];
							if( next_token[0] == '=' )
							{
								ctx->current_index++; /* next token has been processed aswell! */

								if( next_token[1] != '\0' ) /* does this token contain the arg-value? */
									found_arg = next_token + 1;
								else if( ctx->current_index < ctx->argc )
									found_arg = ctx->argv[ ctx->current_index++ ]; /* next token has been processed aswell! */
							}
							else if( next_token[0] != '-' )
							{
								ctx->current_index++; /* next token has been processed aswell! */
								found_arg = next_token;
							}
						}
					}
					break;
					case '=':
						if( check_option[1] != '\0' )
							found_arg = check_option + 1;
						else if( ctx->current_index < ctx->argc )
							found_arg = ctx->argv[ ctx->current_index++ ]; /* next token has been processed aswell! */
					break;
					default:
						/* not found, matched for example --test but it was --testing*/
						continue;
				}

				found_opt = opt;
				break;
			}
		}
	}
	/* malformed opt "-", "-xyz" or "--" */
	else
	{
		ctx->current_opt_arg = curr_token;
		return '!';
	}

	/* found no matching option! */
	if(found_opt == 0x0)
	{
		ctx->current_opt_arg = curr_token;
		return '?';
	}

	if(found_arg != 0x0)
	{
		switch(found_opt->type)
		{
			case GETOPT_OPTION_TYPE_FLAG_SET:
			case GETOPT_OPTION_TYPE_FLAG_AND:
			case GETOPT_OPTION_TYPE_FLAG_OR:
			case GETOPT_OPTION_TYPE_NO_ARG:
				/* these types should have no argument, usage error! */
				ctx->current_opt_arg = found_opt->name;
				return '!';

			case GETOPT_OPTION_TYPE_OPTIONAL:
			case GETOPT_OPTION_TYPE_REQUIRED:
				ctx->current_opt_arg = found_arg;
				return found_opt->value;
		}
	}
	/* no argument found */
	else
	{
		switch(found_opt->type)
		{
			case GETOPT_OPTION_TYPE_FLAG_SET: *found_opt->flag  = found_opt->value; return 0;
			case GETOPT_OPTION_TYPE_FLAG_AND: *found_opt->flag &= found_opt->value; return 0;
			case GETOPT_OPTION_TYPE_FLAG_OR:  *found_opt->flag |= found_opt->value; return 0; /* zero is "a flag was set!" */
			
			case GETOPT_OPTION_TYPE_NO_ARG:
			case GETOPT_OPTION_TYPE_OPTIONAL:
				return found_opt->value;

			/* the option requires an argument! (--option=arg, -o arg) */
			case GETOPT_OPTION_TYPE_REQUIRED:
					ctx->current_opt_arg = found_opt->name;
					return '!';
		}
	}

 	return -1;
}

const char* getopt_create_help_string( getopt_context_t* ctx, char* buffer, size_t buffer_size )
{
	size_t buffer_pos = 0;
	int    opt_index  = 0;
	for( ; opt_index < ctx->num_opts; ++opt_index )
	{
		const getopt_option_t* opt = ctx->opts + opt_index;

		size_t outpos;
		char long_name[64];
		int chars_written = str_format( long_name, 64, "--%s", opt->name );
		if( chars_written < 0 )
			return buffer;

		outpos = (size_t)chars_written;

		switch( opt->type )
		{
			case GETOPT_OPTION_TYPE_REQUIRED:
				str_format(long_name + outpos, 64 - outpos, "=<%s>", opt->value_desc);
				break;
			case GETOPT_OPTION_TYPE_OPTIONAL:
				str_format(long_name + outpos, 64 - outpos, "(=%s)", opt->value_desc);
				break;
			default:
				break;
		}

		if(opt->name_short == 0x0)
			chars_written = str_format( buffer + buffer_pos, buffer_size - buffer_pos, "   %-32s - %s\n", long_name, opt->desc );
		else
			chars_written = str_format( buffer + buffer_pos, buffer_size - buffer_pos, "-%c %-32s - %s\n", opt->name_short, long_name, opt->desc);

		if( chars_written < 0 )
			return buffer;
		
		buffer_pos += (size_t)chars_written;
	}

	return buffer;
}
#endif // END_GETOPT_4

