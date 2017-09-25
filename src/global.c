#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "global.h"

void m_tolower(char *str)
{
    int i, len; 

    assert(str != NULL);

    len = strlen(str);
    for (i = 0; i < len; ++i)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] ^= ' ';
    }
}

void m_toupper(char *str)
{
    int i;

    assert(str != NULL);

    i = strlen(str);
    while (--i >= 0)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] ^= ' ';
    }
}

/* logical error */
void merr_msg(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    merr_deal(M_HIDE, fmt, ap);
    va_end(ap);
    return;
}

/* system error */
void merr_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    merr_deal(M_SHOW, fmt, ap);
    va_end(ap);
    exit(-1);
}

/* fatal system call error */
void merr_dump(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    merr_deal(M_SHOW, fmt, ap);
    va_end(ap);
    abort();
    exit(-1);
}

/* show error messages and quit */
void merr_quit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    merr_deal(M_SHOW, fmt, ap);
    va_end(ap);
    exit(1);
}

/* deal with error arglist */
void merr_deal(const int flg, const char *fmt, va_list ap)
{
    char buf[MAXLINE];
    int m_errno = 0, len = 0;

    snprintf(buf, sizeof(buf), "TIPS on %s, line %d:\r\n\t", __FILE__, __LINE__);

    len = strlen(buf);
    vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);

    m_errno = errno;    /* get the latest types of error */
    len = strlen(buf);
    if (flg == M_SHOW)    /* show lastest error message */
    {
        snprintf(buf + len, sizeof(buf) - len, ": %s", strerror(m_errno));
    }

    len = strlen(buf);
    strncat(buf, "\r\n", sizeof(buf) - len);

    fflush(stdout);    /* in case stderr and stdout are the same */
    fputs(buf, stderr);
    fflush(stderr);
}

/* show tips, frequently show messages */
void mydebug(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    merr_deal(M_HIDE, fmt, ap);
    va_end(ap);
}

/* 字符串转换成整型数 */
unsigned int m_atou(const char *str)
{
    unsigned int num, i, len;

    assert(str != NULL);
    num = 0;
    len = strlen(str);
    for (i = 0; i < len; ++i)
    {
        num = num * 10 + (str[i] - '0');
    }

    return num;
}

/* 整数转换成字符串 */
void m_utoa(int num, char *str)
{
    int i, j;
    char ch;

    j = 0;
    if (num < 0)
    {
        str[j++] = '-';
        i = -num;
    }
    else
    {
        i = num;
    }

    if (num == 0)
    {
        strcpy(str, "0");
        return;
    }
    while (i)
    {
        str[j++] = i % 10 + '0';
        i /= 10;
    }
    str[j] = '\0';

    if (num < 0)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }

    while (i < j)
    {
        ch = str[i];
        str[i] = str[j - 1];
        str[j - 1] = ch;

        ++i;
        --j;
    }
}
