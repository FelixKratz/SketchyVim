/*
 * osdef.h is automagically created from osdef?.h.in by osdef.sh -- DO NOT EDIT
 */
/* autoconf cannot fiddle out declarations. Use our homebrewn tools. (jw) */
/*
 * Declarations that may cause conflicts belong here so that osdef.sh
 * can clean out the forest. Everything else belongs in os_unix.h
 *
 * How this works:
 * - This file contains all unix prototypes that Vim might need.
 * - The shell script osdef.sh is executed at compile time to remove all the
 *   prototypes that are in an include file. This results in osdef.h.
 * - osdef.h is included in vim.h.
 *
 * sed cannot always handle so many commands, this is file 1 of 2
 */

#ifndef fopen	/* could be redefined to fopen64() */
#endif
#ifdef HAVE_FSEEKO
#endif
#ifdef HAVE_FSEEKO
#endif
#ifndef ferror	/* let me say it again: "macros should never have prototypes" */
#endif
#if defined(sun) || defined(_SEQUENT_)
/* used inside of stdio macros getc(), puts(), putchar()... */
extern int	_flsbuf(int, FILE *);
extern int	_filbuf(FILE *);
#endif

#if !defined(HAVE_SELECT)
struct pollfd;			/* for poll() */
extern int	poll(struct pollfd *, long, int);
#endif

#ifdef HAVE_MEMSET
#endif
#ifdef HAVE_STRPBRK
#endif
#ifdef USEBCOPY
#else
# ifdef USEMEMCPY
# else
#  ifdef USEMEMMOVE
#  endif
# endif
#endif
#ifndef __BIONIC__  // Android's libc #defines bzero to memset.
// used inside of FD_ZERO macro
#endif
#ifdef HAVE_SETSID
#endif
#ifdef HAVE_SETPGID
#endif
#ifdef HAVE_STRTOL
#endif
#ifdef HAVE_STRFTIME
#endif
#ifdef HAVE_STRCASECMP
#endif
#ifdef HAVE_STRNCASECMP
#endif
#ifndef strdup
#endif

#ifndef USE_SYSTEM
# ifndef __TANDEM
# endif
#endif


#ifdef HAVE_SIGSET
#endif

#if defined(HAVE_SETJMP_H)
# ifdef HAVE_SIGSETJMP
# else
# endif
#endif


#ifndef __TANDEM
#endif
#if defined(HAVE_GETCWD) && !defined(sun) && !defined(__TANDEM)
#else
#endif
#ifndef __alpha	/* suggested by Campbell */
#endif
/*
 * osdef2.h.in - See osdef1.h.in for a description.
 */

#ifndef __TANDEM
#else
#endif

#ifndef __TANDEM
#endif

#ifndef __TANDEM
#endif
#ifndef __TANDEM
#endif
#ifndef stat	/* could be redefined to stat64() */
#endif
#ifndef lstat	/* could be redefined to lstat64() */
#endif
#ifndef __TANDEM
#endif



#ifdef HAVE_TERMIOS_H
#endif

#ifdef HAVE_SYS_STATFS_H
#endif

#ifdef HAVE_GETTIMEOFDAY
#endif

#ifdef HAVE_GETPWNAM
#endif

#ifdef USE_TMPNAM
#else
#endif

#ifdef ISC
extern int	_Xmblen(char const *, size_t);
#else
		/* This is different from the header but matches mblen() */
extern int	_Xmblen(char *, size_t);
#endif
