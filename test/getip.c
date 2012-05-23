#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <netdb.h>
#include <arpa/inet.h>

void getip(const char *url, char *ret_ip);

int main(int argc, const char *argv[])
{
    char ip_buf[16];

    if (argc != 2)
    {
        printf("usage: %s domain\n", argv[1]);
        exit(-1);
    }

    getip(argv[1], ip_buf);
    puts(ip_buf);

    return 0;
}

void getip(const char *url, char *ret_ip)
{
    struct hostent *addr = NULL;
    char **ptr_alias = NULL;
    char ip_buf[16];

    assert(url != NULL && ret_ip != NULL);

    addr = gethostbyname(url);
    if (addr == NULL)
    {
        printf("illegal domain name!");
        exit(-1);
    }

    printf("global name:%s\n\t", addr->h_name);
    ptr_alias = addr->h_aliases;
    while (*ptr_alias != NULL)
    {
        printf("alias: %s\n", *ptr_alias);
        ++ptr_alias;
    }

    ptr_alias = addr->h_addr_list;
    while (*ptr_alias != NULL)
    {
        inet_ntop(addr->h_addrtype, *ptr_alias, ip_buf, sizeof(ip_buf));
        strcpy(ret_ip, ip_buf);
        return;
        ++ptr_alias;
    }

    strcpy(ret_ip, "");
}
