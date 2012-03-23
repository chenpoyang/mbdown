#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>

#define m_error(msg) fprintf(stderr, "%s:%s() %d: %s\r\n", __FILE__, __func__, __LINE__, msg);
#define GLOBAL extern

/* the max length of hash_table */
#define HALEN	256


#endif // _GLOBAL_H_
