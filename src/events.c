/*---------------------------------------------
 *     modification time: 2016.11.03 15:05
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: events.c
 *     creation time: 2016.11.03 15:05
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Eight part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Eventpool open close api
 *       Part Five:  Eventpool control api
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "events.h"


/*---------------------------------------------
 *             Part One: Define
-*---------------------------------------------*/

#define PER_PEV_SIZE    sizeof(Epollev)

#define IS_INVAILD_POOL(pool) \
    do { \
        if (!pool || !pool->evs) { \
            errno = EINVAL; \
            return  false; \
        } \
    } while (0)


/*---------------------------------------------
 *          Part Three: Local function
-*---------------------------------------------*/

static void    *_block_alloc(Mempool *pool, uint size);

static void    *_chunk_alloc(Mempool *pool, uint size);
static void    *_chunk_divide(Chunk *chunk, uint size, uint border);
static void    *_chunk_new(Mempool *pool, uint size);
static bool     _block_free(Mempool *pool, void *addr);

static Block   *_block_search(Block *block, void *addr);
static Chunk   *_chunk_search(Mempool *pool, void *addr);

static void     _chunk_record(Mempool *pool, void *chunk);


/*---------------------------------------------
 *    Part Four: Eventpool open close api
 *
 *          1. events_create
 *          2. events_destroy
 *
-*---------------------------------------------*/

/*-----events_create-----*/
bool events_create(Eventpool *pool, int32_t init_size)
{
    if (!pool || init_size < 1) {
        errno = EINVAL;
        return  false;
    }

    if (!(pool->evs = calloc(init_size, PER_PEV_SIZE)))
        return  false;

    if ((pool->ep_fd = epoll_create1(EPOLL_CLOEXEC)) == -1)
        return  false;

    pool->ep_currsize = pool->ep_fsize = init_size; 
    pool->ev_cnt = 0;

    return  true;
}


/*-----events_destroy-----*/
bool events_destroy(Eventpool *pool)
{
    IS_INVAILD_POOL(pool);

    if (close(pool->ep_fd) == -1)
        return  false;

    free(pool->evs);
    
    pool->evs = NULL;
    pool->ev_cnt = pool->ep_currsize = pool->ep_fsize = 0;
    pool->ep_fd = 0;

    return  true;
}


/*---------------------------------------------
 *     Part Five: Eventpool control api
 *
 *          1. events_run 
 *
-*---------------------------------------------*/

/*-----events_runforever-----*/
void events_run(Eventpool *pool, int32_t times, int32_t timeout)
{
    IS_INVAILD_POOL(pool);

    int32_t nevents, cnt = 0;

    while (times == -1 || cnt < times) {
        nevents = epoll_wait(pool->ep_fd, pool->evs, pool->ev_cnt, timeout);
        cnt++;
    }
}

