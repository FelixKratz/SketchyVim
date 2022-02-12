/* vi:set ts=8 sts=4 sw=4 noet:
 *
 * VIM - Vi IMproved	by Bram Moolenaar
 *
 * Do ":help uganda"  in Vim to read copying and usage conditions.
 * Do ":help credits" in Vim to see a list of people who contributed.
 */

/*
 * proto.h: include the (automatically generated) function prototypes
 */

/*
 * Don't include these while generating prototypes.  Prevents problems when
 * files are missing.
 */
#if !defined(PROTO) && !defined(NOPROTO)

/*
 * Machine-dependent routines.
 */
/* avoid errors in function prototypes */
#if !defined(FEAT_X11)
#define Display int
#define Widget int
#endif
#define GdkEvent int
#define GdkEventKey int
#ifndef FEAT_X11
#define XImage int
#endif

#if defined(UNIX) || defined(VMS)
#include "os_unix.pro"
#endif
#ifdef MSWIN
#include "os_mswin.pro"
#include "os_win32.pro"
#include "winclip.pro"
#if (defined(__GNUC__) && !defined(__MINGW32__))
extern int _stricoll(char *a, char *b);
#endif
#endif
#ifdef VMS
#include "os_vms.pro"
#endif
#ifdef __BEOS__
#include "os_beos.pro"
#endif

#include "autocmd.pro"
#include "blob.pro"
#include "buffer.pro"
#include "change.pro"
#include "charset.pro"
#include "debugger.pro"
#include "dict.pro"
#include "diff.pro"
#include "digraph.pro"
#include "edit.pro"
#include "eval.pro"
#include "evalfunc.pro"
#include "ex_cmds.pro"
#include "ex_cmds2.pro"
#include "ex_docmd.pro"
#include "ex_eval.pro"
#include "ex_getln.pro"
#include "fileio.pro"
#include "findfile.pro"
#include "fold.pro"
#include "getchar.pro"
#include "hashtab.pro"
#include "indent.pro"
#include "json.pro"
#include "list.pro"
#include "main.pro"
#include "mark.pro"
#include "memfile.pro"
#include "memline.pro"
#ifdef FEAT_ARABIC
#include "arabic.pro"
#endif

/* These prototypes cannot be produced automatically. */
int smsg(const char *, ...)
#ifdef USE_PRINTF_FORMAT_ATTRIBUTE
    __attribute__((format(printf, 1, 0)))
#endif
    ;

int smsg_attr(int, const char *, ...)
#ifdef USE_PRINTF_FORMAT_ATTRIBUTE
    __attribute__((format(printf, 2, 3)))
#endif
    ;

int smsg_attr_keep(int, const char *, ...)
#ifdef USE_PRINTF_FORMAT_ATTRIBUTE
    __attribute__((format(printf, 2, 3)))
#endif
    ;

/* These prototypes cannot be produced automatically. */
int semsg(const char *, ...)
#ifdef USE_PRINTF_FORMAT_ATTRIBUTE
    __attribute__((format(printf, 1, 0)))
#endif
    ;

/* These prototypes cannot be produced automatically. */
void siemsg(const char *, ...)
#ifdef USE_PRINTF_FORMAT_ATTRIBUTE
    __attribute__((format(printf, 1, 0)))
#endif
    ;

int vim_snprintf_add(char *, size_t, const char *, ...)
#ifdef USE_PRINTF_FORMAT_ATTRIBUTE
    __attribute__((format(printf, 3, 4)))
#endif
    ;

int vim_snprintf(char *, size_t, const char *, ...)
#ifdef USE_PRINTF_FORMAT_ATTRIBUTE
    __attribute__((format(printf, 3, 4)))
#endif
    ;

int vim_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap);
int vim_vsnprintf_typval(char *str, size_t str_m, const char *fmt, va_list ap, typval_T *tvs);

#include "message.pro"
#include "message2.pro"
#include "misc1.pro"
#include "misc2.pro"
#ifndef HAVE_STRPBRK /* not generated automatically from misc2.c */
char_u *vim_strpbrk(char_u *s, char_u *charset);
#endif
#ifndef HAVE_QSORT
/* Use our own qsort(), don't define the prototype when not used. */
void qsort(void *base, size_t elm_count, size_t elm_size, int (*cmp)(const void *, const void *));
#endif
#include "mbyte.pro"
#include "move.pro"
#include "normal.pro"
#include "ops.pro"
#include "option.pro"
#ifdef FEAT_QUICKFIX
#include "quickfix.pro"
#endif
#include "regexp.pro"
#include "screen.pro"
#if defined(FEAT_PERSISTENT_UNDO)
#include "sha256.pro"
#endif
#include "search.pro"
#ifdef FEAT_SIGNS
#include "sign.pro"
#endif
#include "state_insert_literal.pro"
#include "state_machine.pro"
#include "syntax.pro"
#include "tag.pro"
#include "term.pro"
#ifdef FEAT_TERMINAL
#include "terminal.pro"
#endif
#if defined(HAVE_TGETENT) && (defined(VMS))
#include "termlib.pro"
#endif
#include "ui.pro"
#include "undo.pro"
#include "usercmd.pro"
#include "userfunc.pro"
#include "version.pro"
#include "window.pro"

#ifdef FEAT_LUA
#include "if_lua.pro"
#endif

#ifdef FEAT_MZSCHEME
#include "if_mzsch.pro"
#endif

#ifdef FEAT_PYTHON
#include "if_python.pro"
#endif

#ifdef FEAT_PYTHON3
#include "if_python3.pro"
#endif

/* Ugly solution for "BalloonEval" not being defined while it's used in some
 * .pro files. */
#define BalloonEval int

#ifdef FEAT_JOB_CHANNEL
#include "channel.pro"

/* Not generated automatically, to add extra attribute. */
void ch_log(channel_T *ch, const char *fmt, ...)
#ifdef USE_PRINTF_FORMAT_ATTRIBUTE
    __attribute__((format(printf, 2, 3)))
#endif
    ;

#endif

#if defined(FEAT_JOB_CHANNEL)
#if defined(UNIX) || defined(MACOS_X) || defined(VMS)
#include "pty.pro"
#endif
#endif

#ifdef FEAT_OLE
#include "if_ole.pro"
#endif

#ifdef MACOS_CONVERT
#include "os_mac_conv.pro"
#endif
#if defined(MACOS_X_DARWIN) && defined(FEAT_CLIPBOARD)
/* functions in os_macosx.m */
void clip_mch_lose_selection(VimClipboard *cbd);
int clip_mch_own_selection(VimClipboard *cbd);
void clip_mch_request_selection(VimClipboard *cbd);
void clip_mch_set_selection(VimClipboard *cbd);
#endif
#endif /* !PROTO && !NOPROTO */
