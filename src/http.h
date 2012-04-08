#ifndef _HTTP_H_
#define _HTTP_H_
#include "url.h"

#define MAX_LEN 256
#define R_LEN	128
#define H_LEN	64
#define COM_LEN	64
#define DEFAULT_MTHD GET
#define DEFAULT_HTTPV HTTP1_1

/*
HEAD /index.html HTTP/1.1
Host: 127.0.0.1
Range:bytes=0-1023

HTTP/1.1 206 Partial Content
Server: nginx/1.0.10
Date: Sat, 31 Mar 2012 13:34:19 GMT
Content-Type: text/html
Content-Length: 1024
Last-Modified: Mon, 26 Mar 2012 14:06:02 GMT
Connection: keep-alive
Vary: Accept-Encoding
Content-Range: bytes 0-1023/2385
*/

/* 请求方式 */
enum method {
	OPTIONS, 
	HEAD, 
	GET, 
	POST, 
	PUT, 
	DELETE, 
	TRACE, 
	CONNECT
};

/* HTTP协议的版本 */
enum httpv {
	HTTP0_9,
	HTTP1_0,
	HTTP1_1
};

/* HTTP请求 */
typedef struct req {
	char str[MAX_LEN];		/* 封装的请求头 */
	char res[R_LEN];		/* 资源位置 */
	char host[H_LEN];		/* 主机 */
    unsigned int port;		/* default: 80 */
	enum httpv ver;			/* HTTP协议版本 */
	enum method mthd;		/* 请求方式 */
	/* [begin, end] */
	unsigned byte_begin;	/* 起始字节 */
	unsigned byte_end;		/* 结束字节 */
}Req;

/* HTTP回应 */
typedef struct resp {
	char str[MAX_LEN];		/* 封装的回应报文 */
	enum httpv ver;			/* eg: HTTP/1.1 --> HTTP1_1 */
	int status;				/* 状态, eg: 206--> Partial Content */
	char web_ser[COM_LEN];	/* 相应的Web服务器 */
	unsigned int len;		/* 字节长度 */
	unsigned resp_begin;	/* 回应的起始字节 */
	unsigned resp_end;		/* 回应的结束字节 */
	unsigned resp_total;	/* 总字节数 */
}Resp;

/* 初始化请求头 */
void init_req(Req *req, const Url *url, unsigned int beg, unsigned int end);
/* 服务器返回str[]响应信息, 构造响应头 */
void init_resp(Resp *resq, const char *str);
/* 将枚举方式转换成相应的字符串 */
void enum_mthd_to_str(char *str, enum method mthd);
/* 将枚举版本转换成相应的字符串 */
void enum_httpv_to_str(char *str, enum httpv ver);

#endif // _REQUEST_H_
