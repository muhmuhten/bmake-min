/*	$NetBSD: job.h,v 1.28 2006/06/29 22:01:17 rillig Exp $	*/

/*
 * Copyright (c) 1988, 1989, 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Adam de Boor.
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
 *
 *	from: @(#)job.h	8.1 (Berkeley) 6/6/93
 */

/*
 * Copyright (c) 1988, 1989 by Adam de Boor
 * Copyright (c) 1989 by Berkeley Softworks
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Adam de Boor.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
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
 *
 *	from: @(#)job.h	8.1 (Berkeley) 6/6/93
 */

/*-
 * job.h --
 *	Definitions pertaining to the running of jobs in parallel mode.
 */
#ifndef _JOB_H_
#define _JOB_H_

#define TMPPAT	"/tmp/makeXXXXXX"

#ifdef USE_SELECT
/*
 * Emulate poll() in terms of select().  This is not a complete
 * emulation but it is sufficient for make's purposes.
 */

#define poll emul_poll
#define pollfd emul_pollfd

struct emul_pollfd {
    int fd;
    short events;
    short revents;
};

#define	POLLIN		0x0001
#define	POLLOUT		0x0004

int
emul_poll(struct pollfd *fd, int nfd, int timeout);
#endif

/*
 * The POLL_MSEC constant determines the maximum number of milliseconds spent
 * in poll before coming out to see if a child has finished. 
 */
#define POLL_MSEC	5000


/*-
 * Job Table definitions.
 *
 * Each job has several things associated with it:
 *	1) The process id of the child shell
 *	2) The graph node describing the target being made by this job
 *	3) A LstNode for the first command to be saved after the job
 *	   completes. This is NILLNODE if there was no "..." in the job's
 *	   commands.
 *	4) An FILE* for writing out the commands. This is only
 *	   used before the job is actually started.
 *	5) A union of things used for handling the shell's output. Different
 *	   parts of the union are used based on the value of the usePipes
 *	   flag. If it is true, the output is being caught via a pipe and
 *	   the descriptors of our pipe, an array in which output is line
 *	   buffered and the current position in that buffer are all
 *	   maintained for each job. If, on the other hand, usePipes is false,
 *	   the output is routed to a temporary file and all that is kept
 *	   is the name of the file and the descriptor open to the file.
 *	6) A word of flags which determine how the module handles errors,
 *	   echoing, etc. for the job
 *
 * The job "table" is kept as a linked Lst in 'jobs', with the number of
 * active jobs maintained in the 'nJobs' variable. At no time will this
 * exceed the value of 'maxJobs', initialized by the Job_Init function.
 *
 * When a job is finished, the Make_Update function is called on each of the
 * parents of the node which was just remade. This takes care of the upward
 * traversal of the dependency graph.
 */
struct pollfd;

#define JOB_BUFSIZE	1024
typedef struct Job {
    int       	pid;	    /* The child's process ID */
    GNode    	*node;      /* The target the child is making */
    LstNode 	tailCmds;   /* The node of the first command to be
			     * saved when the job has been run */
    FILE 	*cmdFILE;   /* When creating the shell script, this is
			     * where the commands go */
    short      	flags;	    /* Flags to control treatment of job */
#define	JOB_IGNERR	0x001	/* Ignore non-zero exits */
#define	JOB_SILENT	0x002	/* no output */
#define JOB_SPECIAL	0x004	/* Target is a special one. i.e. run it locally
				 * if we can't export it and maxLocal is 0 */
#define JOB_IGNDOTS	0x008  	/* Ignore "..." lines when processing
				 * commands */
#define JOB_RESUME	0x100	/* Job needs to be resumed b/c it stopped,
				 * for some reason */
#define JOB_CONTINUING	0x200	/* We are in the process of resuming this job.
				 * Used to avoid infinite recursion between
				 * JobFinish and JobRestart */
#define JOB_TRACED	0x400	/* we've sent 'set -x' */

    union {
	struct {
	    int	  	op_inPipe;	/* Input side of pipe associated
					 * with job's output channel */
	    struct pollfd *op_inPollfd;	/* pollfd associated with inPipe */
	    int   	op_outPipe;	/* Output side of pipe associated with
					 * job's output channel */
	    char  	op_outBuf[JOB_BUFSIZE + 1];
	    	  	    	    	/* Buffer for storing the output of the
					 * job, line by line */
	    int   	op_curPos;	/* Current position in op_outBuf */
	}   	    o_pipe;	    /* data used when catching the output via
				     * a pipe */
	struct {
	    char  	of_outFile[sizeof(TMPPAT)+2];
	    	  	    	    	/* Name of file to which shell output
					 * was rerouted */
	    int	    	of_outFd;	/* Stream open to the output
					 * file. Used to funnel all
					 * from a single job to one file
					 * while still allowing
					 * multiple shell invocations */
	}   	    o_file;	    /* Data used when catching the output in
				     * a temporary file */
    }       	output;	    /* Data for tracking a shell's output */
} Job;

#define outPipe	  	output.o_pipe.op_outPipe
#define inPipe	  	output.o_pipe.op_inPipe
#define inPollfd	output.o_pipe.op_inPollfd
#define outBuf		output.o_pipe.op_outBuf
#define curPos		output.o_pipe.op_curPos
#define outFile		output.o_file.of_outFile
#define outFd	  	output.o_file.of_outFd


/*-
 * Shell Specifications:
 * Each shell type has associated with it the following information:
 *	1) The string which must match the last character of the shell name
 *	   for the shell to be considered of this type. The longest match
 *	   wins.
 *	2) A command to issue to turn off echoing of command lines
 *	3) A command to issue to turn echoing back on again
 *	4) What the shell prints, and its length, when given the echo-off
 *	   command. This line will not be printed when received from the shell
 *	5) A boolean to tell if the shell has the ability to control
 *	   error checking for individual commands.
 *	6) The string to turn this checking on.
 *	7) The string to turn it off.
 *	8) The command-flag to give to cause the shell to start echoing
 *	   commands right away.
 *	9) The command-flag to cause the shell to Lib_Exit when an error is
 *	   detected in one of the commands.
 *
 * Some special stuff goes on if a shell doesn't have error control. In such
 * a case, errCheck becomes a printf template for echoing the command,
 * should echoing be on and ignErr becomes another printf template for
 * executing the command while ignoring the return status. Finally errOut
 * is a printf template for running the command and causing the shell to 
 * exit on error. If any of these strings are empty when hasErrCtl is FALSE, 
 * the command will be executed anyway as is and if it causes an error, so be 
 * it. Any templates setup to echo the command will escape any '$ ` \ "'i
 * characters in the command string to avoid common problems with 
 * echo "%s\n" as a template.
 */
typedef struct Shell {
    const char	 *name;		/* the name of the shell. For Bourne and C
				 * shells, this is used only to find the
				 * shell description when used as the single
				 * source of a .SHELL target. For user-defined
				 * shells, this is the full path of the shell.
				 */
    Boolean 	  hasEchoCtl;	/* True if both echoOff and echoOn defined */
    const char   *echoOff;	/* command to turn off echo */
    const char   *echoOn;	/* command to turn it back on again */
    const char   *noPrint;	/* command to skip when printing output from
				 * shell. This is usually the command which
				 * was executed to turn off echoing */
    int           noPLen;	/* length of noPrint command */
    Boolean	  hasErrCtl;	/* set if can control error checking for
				 * individual commands */
    const char	 *errCheck;	/* string to turn error checking on */
    const char	 *ignErr;	/* string to turn off error checking */
    const char	 *errOut;	/* string to use for testing exit code */
    const char	 *newline;	/* string literal that results in a newline
				 * character when it appears outside of any
				 * 'quote' or "quote" characters */
    char   commentChar;		/* character used by shell for comment lines */

    /*
     * command-line flags
     */
    const char   *echo;		/* echo commands */
    const char   *exit;		/* exit on error */
}               Shell;

extern const char *shellPath;
extern const char *shellName;

extern int	job_pipe[2];	/* token pipe for jobs. */
extern int	jobTokensRunning; /* tokens currently "out" */
extern int	maxJobs;	/* Max jobs we can run */
extern int	maxJobTokens;	/* Number of token for the job pipe */

void Shell_Init(void);
const char *Shell_GetNewline(void);
void Job_Touch(GNode *, Boolean);
Boolean Job_CheckCommands(GNode *, void (*abortProc )(const char *, ...));
void Job_CatchChildren(Boolean);
void Job_CatchOutput(void);
void Job_Make(GNode *);
void Job_Init(void);
Boolean Job_Full(void);
Boolean Job_Empty(void);
ReturnStatus Job_ParseShell(char *);
int Job_Finish(void);
void Job_End(void);
void Job_Wait(void);
void Job_AbortAll(void);
void JobFlagForMigration(int);
void Job_TokenReturn(void);
Boolean Job_TokenWithdraw(void);
void Job_ServerStart(void);

#endif /* _JOB_H_ */
