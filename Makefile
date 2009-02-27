MKPATH ?= mk:../mk:bmake-min/mk
CFLAGS += '-D_PATH_DEFSYSPATH="$(MKPATH):"_path_defsyspath' -Imissing/links -Imissing '-D__RCSID(s)=int abs()' '-D__COPYRIGHT=__RCSID' -DMAKE_NATIVE -DUSE_META -DHAVE_CONFIG_H
CFLAGS += -DHAVE_AR_H
LDFLAGS += -s

SRCS ?= arch.c buf.c compat.c cond.c dir.c for.c getenv.c getopt.c hash.c job.c lstAppend.c lstAtEnd.c lstAtFront.c lstClose.c lstConcat.c lstDatum.c lstDeQueue.c lstDestroy.c lstDupl.c lstEnQueue.c lstFind.c lstFindFrom.c lstFirst.c lstForEach.c lstForEachFrom.c lstInit.c lstInsert.c lstIsAtEnd.c lstIsEmpty.c lstLast.c lstMember.c lstNext.c lstOpen.c lstPrev.c lstRemove.c lstReplace.c lstSucc.c main.c make.c make_malloc.c parse.c setenv.c sigcompat.c str.c strlist.c suff.c targ.c trace.c util.c var.c
OBJS ?= $(SRCS:.c=.o)

build: bmake
clean:
	rm -f *.o bmake

bmake: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
