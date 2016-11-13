/*---------------------------------------------
 *     modification time: 2016.11.13 12:50
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
 *         Part Three: Local function 
-*---------------------------------------------*/



/*---------------------------------------------
 *    Part Four: Eventpool control api
 *
 *      1. eventpool_create
 *
-*---------------------------------------------*/

/*-----eventpool_create-----*/
bool eventpool_create(Eventpool *pool)
{
    if (!pool) {
        errno = EINVAL;
        return  false;
    }

    return  true;
}

