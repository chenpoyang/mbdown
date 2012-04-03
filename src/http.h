#ifndef _HTTP_H_
#define _HTTP_H_

#define MAX_LEN 256
#define R_LEN	128
#define H_LEN	64
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
struct req {
	char str[MAX_LEN];	/* 封装的请求头 */
	char res[R_LEN];	/* 资源位置 */
	char host[H_LEN];	/* 主机 */
    unsigned int port;
	enum method mthd;	/* 请求方式 */
}Req;

/* HTTP回应 */
struct resp {
	char str[MAX_LEN];
}Resp;

#endif // _REQUEST_H_
