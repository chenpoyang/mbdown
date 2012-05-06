#ifndef _RETR_H_
#define _RETR_H_

#include "url.h"

void download(const Url *url);
void * retr(const Url *url, const int beg, const int end);
void append_bytes(FILE *file, const char *buf, const int size);
void get_filename(const char *url, char *filename);


#endif /* _RETR_H_ */
