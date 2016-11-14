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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#include "mempool.h"


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/

#define NUM_NODES   0x25


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct fdhash   Fdhash;
typedef struct hashnode Hashnode; 
typedef struct datanode Datanode;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct datanode {
    Datanode   *next;
    int32_t     ref;
};

struct hashnode {
    Datanode   *head;
};

struct fdhash {
    Mempool     mem;
    MATOS       lock;
    Hashnode    nodes[NUM_NODES];
    int32_t     nsize;
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool        fdhash_init(Fdhash *hash, int32_t size)
            __attribute__((nonnull(1)));

bool        fdhash_destroy(Fdhash *hash)
            __attribute__((nonnull(1)));

Datanode   *fdhash_insert(Fdhash *hash, int32_t fd)
            __attribute__((nonnull(1)));

bool        fdhash_delete(Fdhash *hash, int32_t fd)
            __attribute__((nonnull(1)));

Datanode   *fdhash_search(Fdhash *hash, int32_t fd)
            __attribute__((nonnull(1)));

