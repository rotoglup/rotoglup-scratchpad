/* **************************************************************************
 * @version $Id: rwops.h,v 0.0 19 Mar 2006 18:24:00 legolas558 Exp $
 *
 * File:		rwops.h
 * Content:		header file for RWops object
 * Notes:		-
 *
 * Copyright(c) 2006 by legolas558
 *
 * https://sourceforge.net/projects/rwops
 *
 * This software is released under a LGPL license. See LGPL.TXT
 * You must accept the license before using this software.
 *
 * **************************************************************************/

#ifndef	__RWOPS_H
#define	__RWOPS_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <windows.h>

#ifndef	vsscanf
#ifdef	WIN32
#include "vsscanf.win.h"
#endif
#endif

#include "cppcompat.h"


typedef struct S_RW_mem {
			char *base;
	 		char *here;
			char *stop;
			int	ahead;
			char *stop_dy;
};
#define RW_mem	struct S_RW_mem

#ifdef URL_SUPPORT
typedef int (_cdecl perf_cb(void *, CURLcode));
#endif

/* This is the read/write operation structure -- very basic */

typedef struct RWops {
	/* Seek to 'offset' relative to whence, one of stdio's whence values:
		SEEK_SET, SEEK_CUR, SEEK_END
	   Returns the final offset in the data source.
	 */
	int (*seek)(struct RWops *context, int offset, int whence);

	/* Read up to 'maxnum' objects each of size 'size' from the data
	   source to the area pointed at by 'ptr'.
	   Returns the number of objects read, or -1 if the read failed.
	 */
	int (*read)(struct RWops *context, void *ptr, int size, int maxnum);

	/* Write exactly 'num' objects each of size 'objsize' from the area
	   pointed at by 'ptr' to data source.
	   Returns 'num', or -1 if the write failed.
	 */
	int (*write)(struct RWops *context, const void *ptr, int size, int num);

	/* Close and free an allocated SDL_FSops structure */
	int (*close)(struct RWops *context);

	/* Flush output */
	int (*flush)(struct RWops *);

//	int (*vscanf)(struct RWops *context, const char *format, ...);

	char *(*gets)(struct RWops *context, char *line, int max_length);


    int    type;
#ifdef SEM_SUPPORT
	sem_t	lock;
#endif
    union {
        struct {
             FILE *fp;
            int    autoclose,
                    autoflush;             /* set to 1 for console outputs */
        } stdio;
        RW_mem mem;
#ifdef	URL_SUPPORT
        struct {
            struct _RW_mem    mem_get,     /* GET memory stream */
                              mem_post;    /* POST memory stream */
            int                shared;     /* curl object usage policy */
            const char    *url;            /* url is kept if curl handler is shared */
            CURL    *curl;                 /* associated CURL, maybe shared */
			sem_t	*curl_sem;
			perf_cb	*wait_cb;
			void	*wait_cb_data;
        } url;
#endif
    } hidden;

} RWops;

#define _RW_TIMEOUT		5000

#define _RW_FILE	1
#define	_RW_MEM		2
#define	_RW_URL		3
/* 4kb granularity for http request/response memory streams */
#define	__RW_BLOCK	(1024 * 4)

/* Functions to create RWops structures from various data sources */

RWops	*RWAllocMem(int size, int dynamic );

RWops	*RWFromFile(const char *filename, const char *mode);
#define	RWopen(filename) RWFromFile(filename, "r")
#define	RWopenb(filename) RWFromFile(filename, "rb")

RWops	*RWFromFP(FILE *fp, int autoclose, int autoflush);

RWops	*RWFromMem(void *mem, int size, int autofree);

RWops	*RWFromConstMem(const void *mem, int size, int dynamic );

void	RWappend(RWops *dest, RWops *append);

#ifdef URL_SUPPORT
RWops	*RWFromUrl(const char *url, CURL *open_curl, int post, sem_t *curl_sem, perf_cb *wait_cb, void *wait_cb_data);
#define	DEFAULT_NOWWW_DELAY	4000
#endif

int		RWAssertMem(RWops	*rw, int dynamic );
// if called on a dynamic mem, makes it static

long	RWsize(struct RWops *ctx);

int		RWprintf(RWops	*rw, char *format, ...);
int		RWvprintf(RWops	*rw, const char *format, va_list args);

RWops	*RWAlloc(void);
void	RWFree(RWops *area);

/* Macros to easily read and write from a RWops structure */
#define RWseek(ctx, offset, whence)	(ctx)->seek(ctx, offset, whence)
#define	RWrewind(ctx)	RWseek(ctx, 0, SEEK_SET)
#define RWread(ctx, ptr, size, n)	(ctx)->read(ctx, ptr, size, n)
#define RWwrite(ctx, ptr, size, n)	(ctx)->write(ctx, ptr, size, n)
#define RWclose(ctx)		(ctx)->close(ctx)
#define RWflush(ctx)		(ctx)->flush(ctx)

#define	RWhere(ctx)	(ctx->hidden.mem.here)
#define	RWbase(ctx)	(ctx->hidden.mem.base)
#define	RWstop(ctx)	(ctx->hidden.mem.stop)

#define RWgets(ctx,l,i) (ctx)->gets(ctx,l,i)

int	RW_print(RWops *rw,char *string);
int	RW_printf(RWops	*rw, char *format, ...);

int	RWsscanf(RWops	*rw, char *format, ...);

long	RWtell(RWops *rw);

bool RWskipline(RWops *rw);

#endif
