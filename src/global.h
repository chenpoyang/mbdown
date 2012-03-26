#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>

#define m_error(msg) fprintf(stderr, "%s:%s() %d: %s\r\n", __FILE__, __func__, __LINE__, msg);
#define GLOBAL extern

/* 哈希表的最大长度 */
#define HALEN	256

#define u_int unsigned int

void m_tolower(char *str);

#endif // _GLOBAL_H_
