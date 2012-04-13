#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "global.h"
#include "http.h"
#include "url.h"

Req *req = NULL;	/* 请求 */
Resp *resp = NULL;	/* 回应 */

/* 初始化指定范围的请求头 */
void init_req_begin_end(Req *req, const Url *url, unsigned int beg, unsigned int end)
{
	char enum_mthd_str[COM_LEN], enum_httpv_str[COM_LEN];
	char byte_beg_str[COM_LEN], byte_end_str[COM_LEN];

	assert(req != NULL && url != NULL);

	strcpy(req->res, url->res);
	strcpy(req->host, url->host);
	req->port = url->port;
	req->ver = DEFAULT_HTTPV;
	req->mthd = DEFAULT_MTHD;
	req->begin = beg;
	req->end = end;

	enum_mthd_to_str(enum_mthd_str, req->mthd);
	enum_httpv_to_str(enum_httpv_str, req->ver);
	/* 将数字转换成字符串 */
	m_utoa(req->begin, byte_beg_str);
	m_utoa(req->end, byte_end_str);

	snprintf(req->str, sizeof(req->str), "%s %s %s\r\nHost: %s\r\nRange: bytes = %s - %s\r\n\r\n", 
			enum_mthd_str, req->res, enum_httpv_str, 
			req->host, 
			byte_beg_str, byte_end_str);
}

/* 服务器返回str[]响应信息, 构造响应头 */
void init_resp(Resp *resp, const char *str)
{
    enum httpv ver;
    
    assert(resp != NULL && str != NULL);

    strcpy(resp->str, str);
    m_tolower(resp->str);

    str_httpv_to_enum(resp->str, &ver);
    resp->ver = ver;
    resp_get_webserver(resp->str, resp->web_ser);
    resp->status = resp_get_status(resp->str);
    resp->total_bytes = resp_get_total_bytes(resp->str);
    resp->begin = resp_get_begin_bytes(resp->str);
    resp->end = resp_get_end_bytes(resp->str);
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

/* 将枚举版本转换成相应的字符串 */
void str_httpv_to_enum(const char *str, enum httpv *ver)
{
    char *buf = NULL;
    
	assert(str != NULL && ver != NULL);

    buf = calloc(1, strlen(str) + 1);
    if (buf == NULL)
    {
        merr_msg("calloc error!");
        return;
    }

    strcpy(buf, str);
    m_tolower(buf);
    if (strcmp(buf, "http/1.1") == 0)
    {
        *ver = HTTP1_1;
    }
    else if (strcmp(buf, "http/1.0") == 0)
    {
        *ver = HTTP1_0;
    }
    else
    {
        *ver = HTTP0_9;
    }

    free(buf);
    buf = NULL;
}

/* 获取HTTP版信息 */
void resp_get_httpv(const char *resp, char *ver)
{
    assert(resp != NULL && ver != NULL);
    
    sscanf(resp, "%s", ver);
    m_tolower(ver);
}

/* 获取响应头的状态码 */
int resp_get_status(const char *resp)
{
    int status;
    
    assert(resp != NULL);

    sscanf(resp, "%*s%d", &status);

    return status;
}

/* 获取Web Server 版本信息 */
void resp_get_webserver(const char *resp, char *websrv)
{
    char *ptr = NULL, *buf = NULL;
    
    assert(resp != NULL && websrv != NULL);

    buf = (char *) calloc (1, strlen(resp) + 1);
    if (buf == NULL)
    {
        merr_sys("calloc error!");
        return;
    }

    strcpy(buf, resp);
    m_tolower(buf);

    ptr = strstr(buf, "server");
    if (ptr != NULL)
    {
        sscanf(ptr, "%*s%s", websrv);
    }
    else
    {
        merr_msg("illegal response from web server!");
        return;
    }

    free(buf);
    buf = NULL;
}

/* 获取响应头的成功请求的字节总数 */
int resp_get_total_bytes(const char *resp)
{
    int total_bytes;
    char *buf = NULL, *ptr = NULL;

    assert(resp != NULL);
    
    buf = (char *) calloc (1, strlen(resp) + 1);
    strcpy(buf, resp);
    m_tolower(buf);

    ptr = strstr(buf, "length");

    if (ptr != NULL)
    {
        sscanf(ptr, "%*s%d", &total_bytes);
    }
    else
    {
        merr_msg("illegal response from web server!");
        return -1;
    }

    free(buf);
    buf = NULL;

    return total_bytes;
}

/* 获取响应头的开始字节 */
int resp_get_begin_bytes(const char *resp)
{
    int beg;
    char *buf = NULL, *ptr = NULL;

    assert(resp != NULL);

    buf = (char *) calloc (1, strlen(resp) + 1);
    if (buf == NULL)
    {
        merr_sys("calloc error!");
        return -1;
    }

    strcpy(buf, resp);
    m_tolower(buf);

    ptr = strstr(buf, "range");
    if (ptr != NULL)
    {
        /* Content-Range: bytes 0-1023/2385 */
        sscanf(ptr, "%*s%*s%d", &beg);
    }
    else
    {
        merr_msg("illegal response header!");
        return -1;
    }

    free(buf);
    buf = NULL;
    
    return beg;
}

/* 获取响应头的结束字节 */
int resp_get_end_bytes(const char *resp)
{
    int beg, end;
    char *buf = NULL, *ptr = NULL;

    assert(resp != NULL);

    buf = (char *) calloc (1, strlen(resp) + 1);
    if (buf == NULL)
    {
        merr_msg("calloc error!");
        return -1;
    }

    strcpy(buf, resp);
    m_tolower(buf);

    ptr = strstr(buf, "range");

    if (ptr != NULL)
    {
        /* Content-Range: bytes 0-1023/2385 */
        sscanf(ptr, "%*s%*s%d%*c%d", &beg, &end);
    }
    else
    {
        merr_msg("illegal response header!");
    }

    free(buf);
    buf = NULL;

    return end;
}
