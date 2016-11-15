/*---------------------------------------------
 *     modification time: 2016.11.14 21:20
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

#define HAS_SELECT(saver, type) \
    (type == READ) ? &saver->readhash : \
    ((type == WRITE) ? &saver->writehash : &saver->errorhash)


/*---------------------------------------------
 *         Part Three: Local function 
-*---------------------------------------------*/


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
bool eventsaver_add(Eventsaver *saver, uint8_t type, 
        int32_t fd, ev_handle functor)
{
    if (!saver || !functor) {
        errno = EINVAL;
        return  false;
    }

    Fdhash     *hash = HAS_SELECT(saver, type);
    Datanode   *node = fdhash_insert(hash, fd);

    ((Event *)node)->handle = functor;

    return  true;
}


/*-----eventsaver_delete-----*/
bool eventsaver_delete(Eventsaver *saver, uint8_t type, int32_t fd)
{
    IS_INVAILD_SAVER(saver, false);

    return  fdhash_delete(HAS_SELECT(saver, type), fd);
}


/*-----eventsaver_search-----*/
bool eventsaver_search(Eventsaver *saver, uint8_t type, int32_t fd)
{
    IS_INVAILD_SAVER(saver, false);

    return  fdhash_search(HAS_SELECT(saver, type), fd) ? true : false;
}


