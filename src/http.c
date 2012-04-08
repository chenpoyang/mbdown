#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "global.h"
#include "http.h"
#include "url.h"

Req *req = NULL;	/* 请求 */
Resp *resp = NULL;	/* 回应 */

/* 初始化请求头 */
void init_req(Req *req, const Url *url, unsigned int beg, unsigned int end)
{
	char enum_mthd_str[COM_LEN], enum_httpv_str[COM_LEN];
	char byte_beg_str[COM_LEN], byte_end_str[COM_LEN];
	assert(req != NULL && url != NULL);

	strcpy(req->res, url->res);
	strcpy(req->host, url->host);
	req->port = url->port;
	req->ver = DEFAULT_HTTPV;
	req->mthd = DEFAULT_MTHD;
	req->byte_begin = beg;
	req->byte_end = end;

	enum_mthd_to_str(enum_mthd_str, req->mthd);
	enum_httpv_to_str(enum_httpv_str, req->ver);
	/* 将数字转换成字符串 */
	m_utoa(req->byte_begin, byte_beg_str);
	m_utoa(req->byte_end, byte_end_str);

	snprintf(req->str, sizeof(req->str), "%s %s %s\r\nHost: %s\r\nRange: bytes = %s - %s\r\n\r\n", 
			enum_mthd_str, req->res, enum_httpv_str, 
			req->host, 
			byte_beg_str, byte_end_str);
}

/* 服务器返回str[]响应信息, 构造响应头 */
void init_resp(Resp *resq, const char *str)
{
}

/* 将枚举方式转换成相应的字符串 */
void enum_mthd_to_str(char *str, enum method mthd)
{
	assert(str != NULL);

	switch (mthd)
	{
		case OPTIONS:
			strcpy(str, "OPTIONS");
			break;
		case HEAD:
			strcpy(str, "HEAD");
			break;
		case POST:
			strcpy(str, "POST");
			break;
		case PUT:
			strcpy(str, "PUT");
			break;
		case DELETE:
			strcpy(str, "DELETE");
			break;
		case TRACE:
			strcpy(str, "TRACE");
			break;
		case CONNECT:
			strcpy(str, "CONNECT");
			break;
		default:
			strcpy(str, "GET");
			break;
	}
}
/* 将枚举版本转换成相应的字符串 */
void enum_httpv_to_str(char *str, enum httpv ver)
{
	assert(str != NULL);

	switch (ver)
	{
		case HTTP0_9:
			strcpy(str, "HTTP/0.9");
			break;
		case HTTP1_0:
			strcpy(str, "HTTP/1.0");
			break;
		default:
			strcpy(str, "HTTP/1.1");
			break;
	}
}
