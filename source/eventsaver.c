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
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Eventsaver control api
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


