#pragma once

/* include */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "satomic.h"


/* typedef */
typedef struct listdata     ListData;
typedef struct singlelist   SingleList;

typedef bool (*_ele_cmper)(const void *, const void *);


/* struct */
struct listdata {
    ListData   *next;
};

struct singlelist {
    ListData   *head;
    ListData   *tail;

    int32_t     cnt;
    _ele_cmper  cmper;

    MATOS       lock;
};


/* api */
bool    list_init(SingleList *list, _ele_cmper cmper);
bool    list_insert(SingleList *list, void *data);

void   *list_delete(SingleList *list, void *data);
void   *list_search(SingleList *list, void *data);

bool    list_empty(SingleList *list);

