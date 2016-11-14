/*---------------------------------------------
 *     modification time: 2016.11.13 23:10
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: fdhash.c
 *     creation time: 2016.11.13 23:10
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
 *       Part Four:  Fd hash control
 *       Part Five:  Fd hash operate
 *       Part Six:   Fd hash helper
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "fdhash.h"


/*---------------------------------------------
 *             Part One: Define
-*---------------------------------------------*/

#define CHECK_HASH_VAILD(hash, ret) \
    if (!hash) { \
        errno = EINVAL; \
        return  ret; \
    }

#define CLEAR_NODES(hash) \
    for (int idx = 0; idx < NUM_NODES; idx++) \
        hash->nodes[idx].head = NULL;

#define LOCK_HASH(hash) \
    mato_lock((hash)->lock);

#define UNLOCK_HASH(hash) \
    mato_unlock((hash)->lock);


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/

static int32_t   _hash(int32_t fd);
static Datanode *_traverse(Fdhash *hash, int32_t fd, bool is_del);


/*---------------------------------------------
 *         Part Four: Fd hash control 
 *
 *             1. fdhash_init 
 *             2. fdhash_destroy
 *
-*---------------------------------------------*/

/*-----fdhash_init-----*/
bool fdhash_init(Fdhash *hash, int32_t size)
{
    CHECK_HASH_VAILD(hash, false);

    if (!mmdp_create(&hash->mem, DEF_CHUNKSIZE))
        return  false;

    mato_init(hash->lock, 1);
    CLEAR_NODES(hash);

    hash->nsize = size;

    return  true;
}


/*-----fdhash_destroy-----*/
bool fdhash_destroy(Fdhash *hash)
{
    CHECK_HASH_VAILD(hash, false);

    mato_lock(hash->lock);

    mmdp_free_pool(&hash->mem);
    CLEAR_NODES(hash);

    mato_unlock(hash->lock);

    return  true;
}


/*---------------------------------------------
 *         Part Five: Fd hash operate 
 *
 *          1. fdhash_insert
 *          2. fdhash_delete
 *          3. fdhash_search
 *
-*---------------------------------------------*/

/*-----fdhash_insert-----*/
Datanode *fdhash_insert(Fdhash *hash, int32_t fd)
{
    CHECK_HASH_VAILD(hash, false);
    LOCK_HASH(hash);

    int32_t     offset = _hash(fd);
    Datanode   *node = mmdp_malloc(&hash->mem, hash->nsize);

    if (!node) {
        UNLOCK_HASH(hash);
        return  false;
    }

    node->ref = fd;
    node->next = hash->nodes[offset].head;
    hash->nodes[offset].head = node;
    UNLOCK_HASH(hash);

    return  node;
}


/*-----fdhash_delete-----*/
bool fdhash_delete(Fdhash *hash, int32_t fd)
{
    CHECK_HASH_VAILD(hash, false);
    LOCK_HASH(hash);
    _traverse(hash, fd, true);
    UNLOCK_HASH(hash);

    return  true;
}


/*-----fdhash_search-----*/
Datanode *fdhash_search(Fdhash *hash, int32_t fd)
{
    CHECK_HASH_VAILD(hash, NULL);
    LOCK_HASH(hash);

    Datanode   *result = _traverse(hash, fd, false);

    UNLOCK_HASH(hash);

    return  result;
}


/*---------------------------------------------
 *         Part Six: Fd hash helper
 *
 *          1. _hash
 *          2. _traverse
 *
-*---------------------------------------------*/

/*-----_hash-----*/
int32_t _hash(int32_t fd)
{
    return  (fd % NUM_NODES);
}


/*-----_traverse-----*/
Datanode *_traverse(Fdhash *hash, int32_t fd, bool is_del)
{
    int32_t     offset = _hash(fd);
    Datanode   *node = hash->nodes[offset].head;
    Datanode   *last = NULL;

    while (node) {
        if (node->ref == fd) {
            if (is_del) {
                if (last) {
                    last->next = node->next;

                } else {
                    hash->nodes[offset].head = last->next;
                }

                mmdp_free(&hash->mem, node);
                node = NULL;
            }

            return  node;
        }

        last = node;
        node = node->next;
    }

    return  NULL;
}

