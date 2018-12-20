#ifdef HAVE_CONFIG_H
#undef HAVE_CONFIG_H
#endif

#define HAVE_LIBGEN_H 1
#define HAVE_LIMITS_H 1
#define HAVE_POLL_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_UIO_H 1
#define HAVE_UNISTD_H 1
#define HAVE_UTIME_H 1

#define HAVE_DIRNAME 1
#define HAVE_GETCWD 1
#define HAVE_GETENV 1
#define HAVE_GETOPT 1
#define HAVE_KILLPG 1
#define HAVE_MMAP 1
#define HAVE_REALPATH 1
#define HAVE_SETENV 1
#define HAVE_SETPGID 1
#define HAVE_SETRLIMIT 1
#define HAVE_SETSID 1
#define HAVE_SIGACTION 1
#define HAVE_SNPRINTF 1
#define HAVE_STRERROR 1
#define HAVE_STRFTIME 1
#define HAVE_UNSETENV 1
#define HAVE_VSNPRINTF 1
#define HAVE_WAITPID

char *stresep(char **stringp, const char *delim, int esc);

#include <filemon.h>
#include <dev/filemon/filemon.h>
#ifndef FILEMON_SET_FD
#define meta_job_child(x) meta_job_child()
#endif

#include <string.h>
#ifdef strsep
#define HAVE_STRSEP
#endif
