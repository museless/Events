/*---------------------------------------------
 *     modification time: 2016.11.13 23:10
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: fdhash.h 
 *     creation time: 2016.11.13 23:10
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Typedef
 *       Part Three: Struct
 *
 *       Part Four:  Function
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#pragma once

#include "events.h"
#include <signal.h>
#include <sys/signalfd.h>


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct signalfd_siginfo SigInfo;

typedef void    (*sigfdproc)(SigInfo *, int32_t);


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

int32_t signalfd_create(Events *event, int32_t signum, ev_handle handle)
        __attribute__((nonnull(1, 3)));

bool    signalfd_oper(Events *events, int32_t sigfd, Evdata *data)
        __attribute__((nonnull(1, 3)));

