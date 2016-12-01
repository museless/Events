/*---------------------------------------------
 *     modification time: 2016.12.01 22:30
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: eventsaver.c
 *     creation time: 2016.12.01 22:30
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Seven part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Eventsaver control
 *       Part Five:  Eventsaver operate
 *       Part Six:   Eventsaver helper
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "eventsaver.h"


/*---------------------------------------------
 *             Part One: Define
-*---------------------------------------------*/

#define CHECK_SAVER_VAILD(saver, ret) \
    if (!saver) { \
        errno = EINVAL; \
        return  ret; \
    }

#define CLEAR_NODES(saver) \
    for (int idx = 0; idx < NUM_NODES; idx++) \
        saver->nodes[idx].head = NULL;

#define LOCK_SAVER(saver) \
    mato_lock((saver)->lock);

#define UNLOCK_SAVER(saver) \
    mato_unlock((saver)->lock);

#define HASH() \
    (fd % NUM_NODES)


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/

static Event   *_traverse(Eventsaver *saver, int32_t fd, bool is_del);


/*---------------------------------------------
 *         Part Four: Eventsaver control 
 *
 *             1. eventsaver_create
 *             2. eventsaver_destroy
 *
-*---------------------------------------------*/

/*-----eventsaver_create-----*/
bool eventsaver_create(Eventsaver *saver)
{
    CHECK_SAVER_VAILD(saver, false);

    if (!mmdp_create(&saver->mem, DEF_CHUNKSIZE))
        return  false;

    mato_init(saver->lock, 1);
    CLEAR_NODES(saver);

    return  true;
}


/*-----eventsaver_destroy-----*/
bool eventsaver_destroy(Eventsaver *saver)
{
    CHECK_SAVER_VAILD(saver, false);
    LOCK_SAVER(saver);

    mmdp_free_pool(&saver->mem);
    CLEAR_NODES(saver);

    UNLOCK_SAVER(saver);

    return  true;
}


/*---------------------------------------------
 *         Part Five: Eventsaver operate 
 *
 *          1. eventsaver_add
 *          2. eventsaver_delete
 *          3. eventsaver_search
 *
-*---------------------------------------------*/

/*-----eventsaver_add-----*/
Event *eventsaver_add(Eventsaver *saver, int32_t fd, int32_t ev, Evdata *data)
{
    CHECK_SAVER_VAILD(saver, false);
    LOCK_SAVER(saver);

    int32_t     offset = HASH();
    Event      *node = mmdp_malloc(&saver->mem, sizeof(Event));

    if (!node) {
        UNLOCK_SAVER(saver);
        return  false;
    }

    node->fd = fd;
    node->ev = ev;
    node->data = *data;

    node->next = saver->nodes[offset].head;
    saver->nodes[offset].head = node;
    UNLOCK_SAVER(saver);

    return  node;
}


/*-----eventsaver_delete-----*/
bool eventsaver_delete(Eventsaver *saver, int32_t fd)
{
    CHECK_SAVER_VAILD(saver, false);
    LOCK_SAVER(saver);
    _traverse(saver, fd, true);
    UNLOCK_SAVER(saver);

    return  true;
}


/*-----eventsaver_search-----*/
Event *eventsaver_search(Eventsaver *saver, int32_t fd)
{
    CHECK_SAVER_VAILD(saver, NULL);
    LOCK_SAVER(saver);

    Event  *result = _traverse(saver, fd, false);

    UNLOCK_SAVER(saver);

    return  result;
}


/*---------------------------------------------
 *         Part Six: Eventsaver helper
 *
 *          1. _traverse
 *
-*---------------------------------------------*/

/*-----_traverse-----*/
Event *_traverse(Eventsaver *saver, int32_t fd, bool is_del)
{
    int32_t     offset = HASH();
    Event      *node = saver->nodes[offset].head;
    Event      *last = NULL;

    while (node) {
        if (node->fd == fd) {
            if (is_del) {
                if (last) {
                    last->next = node->next;

                } else {
                    saver->nodes[offset].head = NULL;
                }

                mmdp_free(&saver->mem, node);
                node = NULL;
            }

            return  node;
        }

        last = node;
        node = node->next;
    }

    return  NULL;
}

