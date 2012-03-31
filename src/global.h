#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define m_error(msg) \
	fprintf(stderr, "%s:%s() %d: %s\r\n", __FILE__, __func__, __LINE__, msg);
#define GLOBAL extern
/* 哈希表的最大长度 */
#define HALEN	256
/* 以太网MTU(bytes) */
#define MAX_ETH_MTU	1500
#define bzero(base,n) memset(base, 0, n)
#define u_int unsigned int
#define M_HIDE 0
#define M_SHOW 1

void m_tolower(char *str);

/* max length to storate error message */
#define MAXLINE 4096

/* logical error */
void merr_msg(const char *fmt, ...);

/* system error */
void merr_sys(const char *fmt, ...);

/* fatal system call error */
void merr_dump(const char *fmt, ...);

/* show error messages and quit */
void merr_quit(const char *fmt, ...);

/* deal with error arglist */
void merr_deal(const int flg, const char *fmt, va_list ap);

#endif // _GLOBAL_H_
