MKPATH += mk:../mk:bmake-min/mk
CFLAGS += '-D_PATH_DEFSYSPATH="$(MKPATH):"_path_defsyspath' -Imk -DMAKE_NATIVE -DUSE_META '-D__RCSID(s)=extern char **environ' '-D__COPYRIGHT(s)=extern char **environ' -DHAVE_LIBGEN_H -DHAVE_LIMITS_H -DHAVE_POLL_H -DHAVE_STDINT_H -DHAVE_STDLIB_H -DHAVE_STRING_H -DHAVE_SYS_SELECT_H -DHAVE_SYS_SOCKET_H -DHAVE_SYS_UIO_H -DHAVE_UNISTD_H -DHAVE_UTIME_H -DHAVE_DIRNAME -DHAVE_GETCWD -DHAVE_GETENV -DHAVE_GETOPT -DHAVE_KILLPG -DHAVE_MMAP -DHAVE_REALPATH -DHAVE_SETENV -DHAVE_SETPGID -DHAVE_SETRLIMIT -DHAVE_SETSID -DHAVE_SIGACTION -DHAVE_SNPRINTF -DHAVE_STRERROR -DHAVE_STRFTIME -DHAVE_UNSETENV -DHAVE_VSNPRINTF -DHAVE_WAITPID
LDFLAGS += -s

SRCS ?= arch.c buf.c compat.c cond.c dir.c dirname.c for.c getopt.c hash.c job.c lstAppend.c lstAtEnd.c lstAtFront.c lstClose.c lstConcat.c lstDatum.c lstDeQueue.c lstDestroy.c lstDupl.c lstEnQueue.c lstFind.c lstFindFrom.c lstFirst.c lstForEach.c lstForEachFrom.c lstInit.c lstInsert.c lstIsAtEnd.c lstIsEmpty.c lstLast.c lstMember.c lstNext.c lstOpen.c lstPrev.c lstRemove.c lstReplace.c lstSucc.c main.c make_malloc.c make.c meta.c metachar.c parse.c realpath.c setenv.c sigcompat.c str.c stresep.c strlcpy.c strlist.c suff.c targ.c trace.c util.c var.c
OBJS ?= $(SRCS:.c=.o)

build: bmake
clean:
	rm -f *.o bmake

bmake: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

# XXX this is stupid
meta.o: meta.c
	$(CC) $(CFLAGS) '-Dmeta_init(x)=meta_init(x); char *stresep(char **stringp, const char *delim, int esc); void meta_init(x)' -c meta.c
