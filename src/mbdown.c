#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "synchro.h"
#include "url.h"
#include "retr.h"

GLOBAL RootUrl *root_url;                /* root url manager */
GLOBAL unsigned int count_url_id;        /* for generating unique url id */

int main(int argc, char *argv[])
{
    Url *url = NULL;
    int len;
    /**
     * @brief    初始化 url 哈希表 url 管理器, 和相应的锁
     */
    len = HALEN;
    root_url = NULL;
    init_rooturl_id(&root_url, &count_url_id);
    url = new_url_node(&root_url, argv[1]);
    init_mutex();

    if (root_url == NULL)
    {
        mydebug("root_url == %s", "NULL");
    }

    if (argc >= 2 && argv[1] != NULL)
    {
        download(url);
    }
    else
    {
        merr_msg("usage: filename url");
    }

    release_root_url(&root_url);
    destroy_mutex();

    return 0;
}
