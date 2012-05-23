#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "retr.h"
#include "progress.h"
#include "url.h"
#include "global.h"
#include "http.h"

static Progress bar;
static Req req;
static Resp resp;


void * retr(const Url *url, const int beg, const int end)
{
    assert(beg >= 0 && end >= 0 && url != NULL);

    init_req_begin_end(&req, url, beg, end);

    puts("req:");
    puts(req.str);

    return NULL;
}

void download(const Url *dest_url)
{
    struct sockaddr_in serv;
    int sock_fd, rec_bytes, is_first, totals;
    char hostname[64];
    char buf[MAXBUF], res[MAXBUF], tmp_buf[MAXBUF];
    char *ptr = NULL, *tmp_ptr = NULL;
    FILE *file = NULL, *file_tmp;
    char filename[64];
    char resource[1024];
    const char *url = dest_url->url;

    get_filename(url, filename);
    get_resource(url, resource);

       if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        m_error("socket error!");
        return;
    }

    /* 初始化服务器socket地址 */
    bzero(&serv, sizeof(serv));
    get_host_name(url, hostname);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(80);

    if (inet_pton(AF_INET, hostname, &serv.sin_addr) < 0)
    {
        m_error("inet_pton error!");
        return;
    }

    if (connect(sock_fd, (struct sockaddr *)&serv, sizeof(serv)) < 0)
    {
        m_error("connect error!");
        return;
    }

    snprintf(buf, sizeof(buf), 
            "GET %s HTTP/1.1\r\nUser-Agent: mbdown/1.0\r\nHost: %s\r\n\r\n", 
            resource, hostname);
    puts("send:");
    puts(buf);
    send(sock_fd, buf, sizeof(buf), 0);

    is_first = 1;
    file = fopen(filename, "wb");
    file_tmp = file;
    puts(bar.buf);
    while ((rec_bytes = recv(sock_fd, buf, sizeof(buf), 0)) > 0)
    {
        if (is_first)
        {
            is_first = 0;
            ptr = strstr(buf, "\r\n\r\n");
            if (ptr == NULL)
            {
                merr_sys("bad receive!");
                return;
            }
            strncpy(tmp_buf, buf, ptr - buf + 4);
            tmp_buf[ptr - buf + 4] = '\0';
            init_resp(&resp, tmp_buf);
            puts("resp:");
            puts(resp.str);

            /**
             * @brief    获取头部
             */
            ptr += 4;
            strncpy(res, buf, ptr - buf);
            res[ptr - buf] = '\0';
            puts("recv header:");
            puts(res);

            m_tolower(res);
            tmp_ptr = strstr(res, "length");
            if (NULL == tmp_ptr)
            {
                merr_sys("bad header!");
                return;
            }
            sscanf(tmp_ptr, "%*s%d", &totals);
            retr(dest_url, 0, totals);
            init_progress(&bar, totals);
            rec_bytes -= (ptr - buf);
            is_first = 0;
        }
        else
        {
            ptr = buf;
        }

        if (file == NULL)
        {
            merr_msg("file open error!");
            return;
        }
        if (totals <= 0 )
        {
            return;
        }

        if (totals > 0)    /* 还有数据没收完 */
        {
            if (totals <= rec_bytes)
            {
                append_bytes(file, ptr, totals);
                update_progress(&bar, totals);
                display_image(&bar);
                totals = 0;
            }
            else
            {
                append_bytes(file, ptr, rec_bytes);
                update_progress(&bar, rec_bytes);
                display_image(&bar);
                totals -= rec_bytes;
            }
        }
        else
        {
            puts("\n");
        }
    }
    fflush(file);
    fclose(file_tmp);
    shutdown(sock_fd, 0);
}

/* 加入新的数据 */
void append_bytes(FILE *file, const char *buf, const int size)
{
    fwrite(buf, 1, size, file);
}

void get_filename(const char *url, char *filename)
{
    const char *ptr = NULL, *tmp_ptr = NULL;
    char tmpfile[64], str[64];
    int id = 0;
    FILE *fp = NULL;

    assert(url != NULL && filename != NULL);

    ptr = strstr(url, "/");
    if (ptr == NULL)
    {
        merr_msg("TIPS ON %s, LINE %d\r\n:%s", __FILE__, __LINE__, "illegal url");
        return;
    }

    while ((tmp_ptr = strstr(ptr + 1, "/")) != NULL)
    {
        ptr = tmp_ptr;
    }

    strcpy(filename, ptr + 1);
    strcpy(tmpfile, filename);
    while ((fp = fopen(tmpfile, "r")) != NULL)
    {
        ++id;
        fclose(fp);
        m_utoa(id, str);
        strcpy(tmpfile, filename);
        strcat(tmpfile, ".");
        strcat(tmpfile, str);
    }
    strcpy(filename, tmpfile);

    printf("\tfilename:");
    puts(filename);
}
