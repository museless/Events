/*---------------------------------------------
 *     modification time: 2016-11-07 12:57:22
 *     mender: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *     file: singlelist.c
 *     creation time: 2016-11-07 12:57:22
 *     author: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *                  Include 
 *---------------------------------------------*/

#include "singlelist.h"


/*---------------------------------------------
 *               Local function
 *---------------------------------------------*/

static void *_list_operate(SingleList *list, void *data, bool is_del);


/*---------------------------------------------
 *                  Function
 *---------------------------------------------*/

/*-----list_init-----*/
bool list_init(SingleList *list, _ele_cmper cmper)
{
    if (!list) {
        errno = ENODATA;
        return  false;
    }

    list->head = list->tail = NULL;
    list->cnt = 0;
    list->cmper = cmper;

    mato_init(list->lock, 1);

    return  true;
}


/*-----list_insert-----*/
bool list_insert(SingleList *list, void *data)
{
    if (!list || !data) {
        errno = ENODATA;
        return  false;
    }

    mato_lock(list->lock);

    ListData   *node = (ListData *)data;

    if (!list->head)
        list->head = node; 

    if (list->tail)
        list->tail->next = node;

    node->next = NULL;
    list->tail = node;
    list->cnt += 1;

    mato_unlock(list->lock);

    return  true;
}


/*-----list_delete-----*/
void *list_delete(SingleList *list, void *data)
{
    return  _list_operate(list, data, true);
}


/*-----list_search-----*/
void *list_search(SingleList *list, void *data)
{
    return  _list_operate(list, data, false);
}


/*-----list_empty-----*/
bool list_empty(SingleList *list)
{
    if (!list || !list->head) {
        errno = ENODATA;
        return  false;
    }

    mato_lock(list->lock);
    list->head = list->tail = NULL;
    list->cnt = 0;
    mato_unlock(list->lock);

    return  true;
}


/*-----_list_operate-----*/
void *_list_operate(SingleList *list, void *data, bool is_del)
{
    if (!list || !list->head) {
        errno = ENODATA;
        return  NULL;
    }

    ListData  *node, *save = NULL;

    mato_lock(list->lock);

    for (node = list->head; node; save = node, node = node->next) {
        if (list->cmper(node, data)) {
            if (is_del) {
                if (node == list->head)
                    list->head = node->next;

                if (node == list->tail)
                    list->tail = save;

                if (save)
                    save->next = node->next;

                list->cnt -= 1;
            }

            mato_unlock(list->lock);

            return  node;
        }
    }

    mato_unlock(list->lock);

    return  NULL;
}


