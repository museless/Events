/*---------------------------------------------
 *     modification time: 2016.11.22 01:10
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: eventsave.c
 *     creation time: 2016.11.13 12:50
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Six part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Eventsaver control api
 *       Part Five:  Eventsaver operate
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "eventsaver.h"


/*---------------------------------------------
 *              Part One: Define
-*---------------------------------------------*/

#define IS_INVAILD_SAVER(saver, ret) \
    if (!(saver)) { \
        errno = EINVAL; \
        return  (ret); \
    }

#define IS_INVAILD_TYPE(type, ret) \
    if (type < EVMIN || type > EVMAX) { \
        errno = EINVAL; \
        return  (ret); \
    }


/*---------------------------------------------
 *         Part Three: Local function 
-*---------------------------------------------*/

static Fdhash  *_hash_select(Eventsaver *saver, uint8_t type);


/*---------------------------------------------
 *    Part Four: Eventsaver control api
 *
 *          1. eventsaver_create
 *          2. eventsaver_destroy
 *
-*---------------------------------------------*/

/*-----eventsaver_create-----*/
bool eventsaver_create(Eventsaver *saver)
{
    IS_INVAILD_SAVER(saver, false);

    if (!fdhash_init(&saver->readhash, sizeof(Event)))
        return  false;

    if (!fdhash_init(&saver->writehash, sizeof(Event)))
        return  false;

    if (!fdhash_init(&saver->errorhash, sizeof(Event)))
        return  false;

    return  true;
}


/*-----eventsaver_destroy-----*/
bool eventsaver_destroy(Eventsaver *saver)
{
    IS_INVAILD_SAVER(saver, false);

    if (!fdhash_destroy(&saver->readhash))
        return  false;

    if (!fdhash_destroy(&saver->writehash))
        return  false;

    if (!fdhash_destroy(&saver->errorhash))
        return  false;

    return  true;
}


/*---------------------------------------------
 *      Part Five: Eventsaver operate
 *
 *          1. eventsaver_add
 *          2. eventsaver_delete
 *          3. eventsaver_search
 *
-*---------------------------------------------*/

/*-----eventsaver_add-----*/
bool eventsaver_add(Eventsaver *saver,
        uint8_t type, int32_t fd, Evdata *data) 
{
    if (!saver || !data) {
        errno = EINVAL;
        return  false;
    }

    IS_INVAILD_TYPE(type, false);

    Fdhash     *hash = _hash_select(saver, type);
    Datanode   *node = fdhash_insert(hash, fd);

    ((Event *)node)->data = *data;

    return  true;
}


/*-----eventsaver_delete-----*/
bool eventsaver_delete(Eventsaver *saver, uint8_t type, int32_t fd)
{
    IS_INVAILD_SAVER(saver, false);
    IS_INVAILD_TYPE(type, false);

    return  fdhash_delete(_hash_select(saver, type), fd);
}


/*-----eventsaver_search-----*/
Event *eventsaver_search(Eventsaver *saver, uint8_t type, int32_t fd)
{
    IS_INVAILD_SAVER(saver, false);
    IS_INVAILD_TYPE(type, false);

    return  (Event *)fdhash_search(_hash_select(saver, type), fd);
}


/*---------------------------------------------
 *             Part Six: Helper 
 *
 *             1. _hash_select 
 *
-*---------------------------------------------*/

/*-----_hash_select-----*/
Fdhash *_hash_select(Eventsaver *saver, uint8_t type)
{
    switch (type) {
        case EVREAD:
            return  &saver->readhash;

        case EVWRITE:
            return  &saver->writehash;

        case EVERROR:
            return  &saver->errorhash;
    }

    return  NULL;
}

