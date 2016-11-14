/*---------------------------------------------
 *     modification time: 2016.11.14 21:20
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: eventpool.c
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
 *       Part Four:  Events control api
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "eventpool.h"


/*---------------------------------------------
 *              Part One: Define
-*---------------------------------------------*/

#define IS_INVAILD_POOL(pool, ret) \
    if (!(pool)) { \
        errno = EINVAL; \
        return  (ret); \
    }


/*---------------------------------------------
 *         Part Three: Local function 
-*---------------------------------------------*/

static bool _handle(void *object, int32_t fd, uint8_t type);


/*---------------------------------------------
 *    Part Four: Eventpool control api
 *
 *          1. eventpool_create
 *          2. eventpool_destroy
 *
-*---------------------------------------------*/

/*-----eventpool_create-----*/
bool eventpool_create(Eventpool *pool)
{
    IS_INVAILD_POOL(pool, false);

    if (!fdhash_init(&pool->readhash, sizeof(Event)))
        return  false;

    if (!fdhash_init(&pool->writehash, sizeof(Event)))
        return  false;

    if (!fdhash_init(&pool->errorhash, sizeof(Event)))
        return  false;

    pool->epfd = events_create(&pool->events, MAX_PROC, (void *)pool, _handle);

    if (pool->epfd == -1)
        return  false;

    return  true;
}


/*-----eventpool_destroy-----*/
bool eventpool_destroy(Eventpool *pool)
{
    IS_INVAILD_POOL(pool, false);

    if (!fdhash_destroy(&pool->readhash))
        return  false;

    if (!fdhash_destroy(&pool->writehash))
        return  false;

    if (!fdhash_destroy(&pool->errorhash))
        return  false;

    if (!events_destroy(&pool->events))
        return  false;

    return  true;
}


/*---------------------------------------------
 *         Part Six: Eventpool private
 *
 *              1. _handle
 *
-*---------------------------------------------*/

/*-----_handle-----*/
bool _handle(void *object, int32_t fd, uint8_t type)
{
    switch (type) {
      case EVREAD:
      case EVWRITE:
      case EVERROR:
          break;
    }

    return  true;
}

