///////////////////////////////////////////////////////////////////////////////
//
//		编译选项：系统平台，编译器，语言
//
//系统选择
#define LINUX
//#define WINDOW

//编译器选择
//#define VC	//Visual C++
//#define LCC	//Lccwin32
#define GCC		//gcc 

//语言选择
#define CN		//chinese
//#define EN	//english
///////////////////////////////////////////////////////////////////////////////

//字符串定义
    #define PRO_VERSION			"Version: 2.1"
    #define STR_VERSION 		"freedown/2.1"
    #define STR_SYSTEM_LINUX 	"(X11;linux 686)"
    #define STR_SYSTEM_WINDOWS	"(windows NT 6.1)"
    #define STR_MOD_LINUX		"w"
    #define STR_MOD_WINDOWS		"wb"

//linux大文件读写的编译选项
#ifdef LINUX
    #define _LARGEFILE_SOURCE
    #define _LARGEFILE64_SOURCE
    #define _FILE_OFFSET_BITS 64
#endif

//头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>


//linux
#ifdef LINUX
    #include <unistd.h>	//unix标准库
    #include <fcntl.h>	//低级文件控制
    #include <netdb.h>   //网络数据基本库
    #include <sys/select.h>	//select函数支持
    #include <sys/time.h>	//时间库
    #include <sys/socket.h>   //网络函数库
    #include <sys/types.h>	//数据类型定义
    #include <sys/stat.h>	//文件与目录
    #include <netinet/in.h>	//网络地址结构定义
    #include <arpa/inet.h>	//inet_ 函数库
        #define closesocket close
        #define fseeko fseeko64
        #define ftello ftello64
        #define GetLastError() errno
        #define STR_SYSTEM	STR_SYSTEM_LINUX
        #define STR_MOD		STR_MOD_WINDOWS
#endif
//window
#ifdef WINDOW
    #ifdef VC
        #define fseeko _fseeki64
        #define ftello _ftelli64
    #endif
    #ifdef LCC
        #define fseeko fseeki64
        #define ftello ftelli64
    #endif
        #define STR_SYSTEM	STR_SYSTEM_WINDOWS
        #define STR_MOD		STR_MOD_WINDOWS
    #include <WinSock2.h>
    #pragma comment ( lib , "Ws2_32.lib")
#endif

//english
#ifdef EN
    #define LINKNAME 	"HTTPLINK"
    #define RETURNCODE 	"HTTPCODE"
    #define FILENAME 		"OUTFILE"
    #define DATALEN		"DATALEN"
    #define MAKEFILE 		"creating file"
    #define FINISH 		"Download Success"
    #define ISERROR		"Download failed"
    #define REMAIN		"Remaining data"
    #define OPEN_REQUEST 	"Thread open: Request file" 
    #define OPEN_LOADING	"Thread Downloading now..."
    #define WAIT_FAIL 	"Thread waiting: Request failed"
    #define CLOSE_OVERLOAD	"Thread close: End of data"
    #define OUT_OVER 		"Thread out: All data end"
    #define OUT_MOREFAIL 	"Thread out: Too many failed"
#endif
//chinese
#ifdef CN
    #define LINKNAME 	"请求地址"
    #define RETURNCODE 	"响应代码"
    #define FILENAME 		"输出文件"
    #define DATALEN		"数据长度"
    #define MAKEFILE 		"正在创建文件"
    #define FINISH 		"完成下载"
    #define ISERROR		"下载错误"
    #define REMAIN		"剩余数据"
    #define OPEN_REQUEST 	"号线打开: 请求数据 "
    #define OPEN_LOADING	"号线: 下载数据中 ... "
    #define WAIT_FAIL 	"号线等待: 请求失败"
    #define CLOSE_OVERLOAD	"号线关闭: 数据下载完毕 "
    #define OUT_OVER 		"号线退出: 没有可请求的数据 "
    #define OUT_MOREFAIL 	"号线退出: 失败太多, 放弃请求 "
#endif

//文件流定义
typedef FILE *STREAM;	//文件流
typedef FILE *Stream;	//文件流
typedef char *String;	//字符流

//数据段在所下载文件中的偏移
typedef struct data
{
    long long beg , end; //开始偏移和结束偏移
}DATA;

// 偏移栈
typedef struct stack
{
    struct data *datas;	//栈指针
    struct data *ptr;	//栈顶
    int size;		//栈大小
}STACK;

//数据结构 请求列队
typedef struct sockfd
{
    #define DEF_DELAY 10000
    unsigned sock;	//套接字
    unsigned fails;	//请求失败次数
    unsigned delay;	//失败后,到下一次发起请求的延时(　以列队的循环次数为单位.默认值为10000,每过10000次循环,再发起一次请求)
    struct data d;	//请求的数据段在文件中的偏移
}SFD;

///////////////////////////////////////////////////////////////////////////////
//
//		下载器的入口函数
//参数: 发起连接数 , 资源链接 , 保存文件名( 可选,不选设为NULL, 程序会把链接的最后一个斜杠后面的字符串作文件名)
//返回:0下载成功,其它下载失败
//
///////////////////////////////////////////////////////////////////////////////
int freedown( int nConnections , char* link , char* filename );

///////////////////////////////////////////////////////////////////////////////
//
//  functions for stack
//
///////////////////////////////////////////////////////////////////////////////
//分配空间
int allocStack( struct stack *s , int size );
//添加空间
int reallocStack( struct stack *s );
//释放空间
void freeStack( struct stack *s );
//返回空间大小
int sizeStack( struct stack *s );
//进栈
int pushStack( struct stack *s , struct data *d );
//出栈
int popStack( struct stack *s , struct data *d );

///////////////////////////////////////////////////////////////////////////////
//
//  functions for qsort
//
///////////////////////////////////////////////////////////////////////////////
int cmp( void* a , void* b );

///////////////////////////////////////////////////////////////////////////////
//
//  functions for freedown
//
///////////////////////////////////////////////////////////////////////////////
//填充偏移栈
int fillStack( struct stack *s , long long nDatalen , int nConnections , int nDeflen );
//从链接中取文件名
char *getFilenameptr( char* link );
//取主机名
char *getHostptr( char* link );
//取资源名
char* getSrcptr( char* link );
//取HTTP请求头
char* getRequest( char* hostname , char* srcname , struct data* d );
//取HTTP响应头
char* getRespond( char* hostname , char* srcname);
//取网络地址
struct sockaddr_in * getDestaddr( char* hostname , short port );
//测试并取新链接
char* checkLink( char* link );
//取资源大小
long long getDatalen( char* link );
//打开文件流
STREAM openfile( char* filename ,const char* mod );
//打开一个连接,并过虑掉响应头,再保存数据到文件
int openConnect( STREAM file  , struct sockfd *fd  , char* link);
//体现下载器核心的逻辑的函数
int downfile( STREAM file , char* link , long long dataLen , int nConnections );

///////////////////////////////////////////////////////////////////////////////
//
//  the main function
//
///////////////////////////////////////////////////////////////////////////////
int main( int argc , char **argv)
{
    int ret = 0;
#ifdef WINDOW
    WSADATA wsaData;
    WSAStartup( MAKEWORD( 1,1) , &wsaData ) ;
#endif
    if( argc == 4 )
    {
        ret = freedown( atoi( argv[1] ) , argv[2] , argv[3] );
    }
    else if( argc == 3)
    {
        ret = freedown( atoi( argv[1] ) , argv[2] , NULL );
    }
    else
    {
        printf( "Usage: %s <connections> <link> [filename] \n"
            PRO_VERSION"\n" , argv[0]);
    }
#ifdef WINDOW
    WSACleanup();
#endif
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//
//  functions for stack
//
///////////////////////////////////////////////////////////////////////////////
int allocStack( struct stack *s , int size )
{
    if( s->datas != NULL )
    {
        freeStack( s );
        s->datas = NULL ;
    }
    if( size <= 0 )
    {
        return -1;
    }
    s->size = size;
    s->datas = (struct data *)calloc( s->size , sizeof( struct data ) );
    if( s->datas == NULL )
    {
        perror( "calloc");
        return -1;
    }
    s->ptr = s->datas;
    return 0;
}
void freeStack( struct stack *s )
{
    if( s->datas != NULL )
    {
        free( s->datas );
    }
    s->ptr = s->datas = NULL ;
    s->size = 0;
}
int reallocStack( struct stack *s )
    {
    int pos;
    if( s->datas == NULL || s->ptr == NULL || s->size == 0 )
    {
        return -1;
    }
    s->size *= 2;
    pos = s->ptr - s->datas;
    s->datas = (struct data*)realloc( s->datas , s->size * sizeof( struct data ) );
    if( s->datas == NULL )
    {
        perror( "realloc");
        return -1;
    }
    s->ptr = s->datas + pos ;
    return 0;
}
int sizeStack( struct stack *s )
{
    return s->size;
}
int pushStack( struct stack *s , struct data *d )
{
    if( s->datas == NULL  || s->ptr == NULL )
    {
        return -1;
    }
    if( s->ptr - s->datas + 1 == s->size )
    {
        if( reallocStack( s ) == -1 )
        {
            return -1;
        }
    }
    s->ptr->beg = d->beg;
    s->ptr->end = d->end;
    s->ptr += 1;
    return 0;
}
int  popStack( struct stack *s , struct data*d )
{
    if( s->datas == NULL || s->ptr == NULL )
    {
        return -1;
    }
    if( s->ptr == s->datas )
    {
        return 1;
    }
    s->ptr -=1;
    d->beg = s->ptr->beg;
    d->end = s->ptr->end;
    return 0;
}
///////////////////////////////////////////////////////////////////////////////
//
//  functions for qsort
//
///////////////////////////////////////////////////////////////////////////////
int cmp( void* a , void* b ) 
{
    int nA = *(int*)a;
    int nB = *(int*)b;
    return nB-nA;
}
///////////////////////////////////////////////////////////////////////////////
//
//  functions for freedown
//
///////////////////////////////////////////////////////////////////////////////
int fillStack( struct stack *s , long long nDatalen , int nConnections , int nDeflen )
{
    int index;	//其实变量名取 i 就行了.
    long long len = nDatalen / nConnections ; //对文件按连接数分割
    long long nDatalenCpy = nDatalen;
    struct data d;
    
    len = len > nDeflen ? nDeflen : len ;	//如果长度大于默认,则取默认值
    for( index = 0 ; nDatalenCpy > 0 ; index ++ )
    {
        d.end = --nDatalenCpy;
        if( nDatalenCpy > len ) 
        {
            d.beg =( nDatalenCpy-=len );
        }
        else
        {
            d.beg = nDatalenCpy = 0 ;
        }
        if( pushStack( s , &d ) != 0 )	//进栈
        {
            return -1;
        }		
    }
    return 0;
}

char *getFilenameptr( char* link )
{
    int ret = 0;
    char * ptr1 = NULL;
    char * ptr2 = NULL;
    static char strBuffer[128] ;	
    memset( strBuffer , 0 , sizeof( strBuffer ) );
    
    ptr1 = strstr( link , "http://" );
    if( ptr1 == link )
    {
        ptr1 +=7;
    }
    else
    {
        ptr1 = link;
    }
    
    while( (ptr1 = strstr( ptr1 , "/" )) )
    {
        ptr2 = ++ptr1;
    }
    
    if( ptr2 == NULL )
    {
        return NULL;
    }
    
    ret = sscanf( ptr2 , "%s" , strBuffer );
    if( ret != 1 )
    {
        return NULL ;
    }
    
    return strBuffer;
}


char *getHostptr( char* link )
{
    int ret = 0;
    static char strBuffer[256];
    char* ptr;
    memset( strBuffer , 0 , sizeof( strBuffer ) );
    
    ptr = strstr( link , "http://" );
    if( ptr == link )
    {
        ptr+=7;
    }
    else
    {
        ptr = link;
    }
    
    ret = sscanf( ptr, "%[^/]" , strBuffer ) ;
    if( ret != 1 )
    {
        return NULL;
    }
    return strBuffer ;
}
char* getSrcptr( char* link )
{
    int ret = 0;
    static char strBuffer[512] ;
    char *ptr;
    memset( strBuffer , 0 , sizeof( strBuffer ) );
    
    ptr = strstr( link , "http://" );
    if( ptr == link )
    {
        ptr +=7;
    }
    else
    {
        ptr = link;
    }
    
    ret = sscanf( ptr , "%*[^/]%s" , strBuffer );
    if( ret == 0 )
    {
        strcat( strBuffer , "/" );
    }
    
    return strBuffer ;
}
char* getRequest( char* hostname , char* srcname , struct data* d )
{
	int ret = 0;
	static char strBuffer[1440];

	if( d == NULL )
	{
		ret = sprintf( strBuffer , "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: "
					STR_VERSION
					" "
					STR_SYSTEM
					"\r\n\r\n" , 
					srcname , hostname );
	}
	else
	{
		ret = sprintf( strBuffer , "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: "
					STR_VERSION
					" "
					STR_SYSTEM
					"\r\nRange: bytes=%lld-%lld\r\n\r\n" , 
					srcname , hostname , d->beg , d->end );
	}
	if( ret <= 0 )
	{
		return NULL;
	}
	return strBuffer ;
}
struct sockaddr_in * getDestaddr( char* hostname , short port)
{
	static struct sockaddr_in addr;
	struct hostent *host;

	if( hostname == NULL )	//参数防错
	{
		return NULL;
	}

	host = gethostbyname( hostname ) ;
	if( host == NULL )
	{
		return NULL;
	}
	
	memset( &addr , 0 , sizeof( struct sockaddr ) );
	addr.sin_family = AF_INET ;
	addr.sin_port = htons( port );
	addr.sin_addr.s_addr = *(unsigned long*)host->h_addr;

	return &addr;
}
char* getRespond( char* hostname , char* srcname)
{
	int ret = 0;
	int len = 0;
	char* ptr;
	char* ptrRequest;
	unsigned sock;
	struct sockaddr_in *addr;
	static char strBuffer[1440];
	memset( strBuffer , 0 , sizeof( strBuffer ) );

	ptrRequest = getRequest( hostname , srcname , NULL );
	if( ptrRequest == NULL )
	{
		return NULL;
	}

	sock = socket( AF_INET , SOCK_STREAM , 0 );
	if( sock == -1 )
	{
		return NULL;
	}

	addr = getDestaddr( hostname , 80 );
	if( addr == NULL )
	{
		goto out;
	}
	ret = connect( sock , (struct sockaddr*)addr , sizeof( struct sockaddr ) );
	if( ret == -1 )
	{
		goto out;
	}

	len = strlen( ptrRequest );
	ret = send( sock , ptrRequest , len , 0 );
	if( ret != len )
	{
		goto out;
	}
	
	ret = recv( sock , strBuffer , sizeof( strBuffer ) , 0 );
	if( ret <= 0 )
	{
		goto out;
	}
	
	ptr = strstr( strBuffer , "\r\n\r\n" );
	if( ptr == NULL )
	{
		goto out;
	}
	ptr += 4;
	*ptr = 0;

out:
	closesocket( sock );
	if( *strBuffer == 0 ) return NULL;
	return strBuffer;
}
char* checkLink( char* link )
{
	char* ptrTmp1;
	char* ptrTmp2;
	char* ptrHost = NULL;
	char* ptrSrc = NULL;
	char* ptrRespond = NULL;
	char strCode[64];
	static char strLink[512];

//检查参数有效性
	if( link == NULL )
	{
		return NULL;
	}

//保存链接地址
	memset( strLink , 0 , sizeof( strLink ) );
	strcpy( strLink , link );
	printf( LINKNAME": %s\n" , strLink );			//输出地址名
	do{
	//取主机名
		ptrHost = getHostptr( strLink);
		if( ptrHost== NULL ) 
		{
			return NULL;
		}
	//取资源链接
		ptrSrc = getSrcptr( strLink);
		if( ptrSrc == NULL )	
		{
			return NULL;
		}
	//取响应代码
		ptrRespond = getRespond( ptrHost , ptrSrc );
		if( ptrRespond == NULL )
		{
			return NULL;
		}
	//输出响应代码
		ptrTmp1 = strstr( ptrRespond , "\r\n" );
		if( ptrTmp1 == NULL )
		{
			return NULL;
		}
		strncpy( strCode , ptrRespond , ptrTmp1 - ptrRespond );
		strCode[ ptrTmp1 - ptrRespond ] = 0;
		printf( RETURNCODE": %s\n\n" , strCode ); 		//输出响应代码
	//匹配响应代码,是否为200 ok
		ptrTmp1 = strstr( ptrRespond , "HTTP/1.1 20" );
		if( ptrTmp1 == ptrRespond )
		{
			break;
		}
		ptrTmp1 = strstr( ptrRespond , "HTTP/1.0 20" );
		if( ptrTmp1 == ptrRespond )
		{
			break;
		}
	//如果不是,取location字段值
		ptrTmp1 = strstr( ptrRespond , "Location: " );
		if( ptrTmp1 == NULL ) 	return NULL;
	//重新取地址
		ptrTmp1 += 10 ;
		ptrTmp2 = strstr( ptrTmp1 , "\r\n" );
		memset( strLink , 0 , sizeof( strLink ) );
		strncpy( strLink , ptrTmp1 , ptrTmp2 - ptrTmp1 );
		printf( LINKNAME": %s\n" , strLink );			//输出地址名
	}while( *strLink );

	return strLink ;
}
long long getDatalen( char* link )
{
	int ret = 0;
	int len = 0;
	char* ptr = NULL;
	char* ptrHost = NULL;
	char* ptrSrc = NULL;
	char* ptrRequest = NULL;
	char strBuffer[ 1440 ] = {0};
	long long dataLen = 0;
	unsigned sock = 0;
	struct sockaddr_in * addr = NULL;

	if( link == NULL )
	{
		return 0;
	}
	
	ptrHost = getHostptr( link );
	ptrSrc = getSrcptr( link );
	if( ptrHost == NULL || ptrSrc == NULL )
	{
		goto out;
	}
	
	ptrRequest = getRequest( ptrHost , ptrSrc , NULL  ) ;
	if( ptrRequest == NULL )
	{
		goto out;
	}
	
	sock = socket( AF_INET , SOCK_STREAM , 0 );
	if( sock < 1 )
	{
		goto out;
	}
	
	addr = getDestaddr( ptrHost , 80 );
	if( addr == NULL )
	{
		goto out;
	}
	ret = connect( sock , (struct sockaddr*)addr , sizeof( struct sockaddr ) );
	if( ret != 0 )
	{
		goto out;
	}

	len = strlen( ptrRequest );
	ret = send( sock , ptrRequest , len , 0 );
	if( ret != len )
	{
		goto out;
	}
	
	ret = recv( sock , strBuffer , sizeof( strBuffer ) , 0 );
	if( ret <= 0 )
	{
		goto out;
	}

	ptr = strstr( strBuffer , "Content-Length: " );
	if( ptr == NULL )
	{
		goto out;
	}
	
	ret = sscanf( ptr , "Content-Length: %lld" , &dataLen );
	if( ret != 1 )
	{
		goto out;
	}

out:
	if( sock > 0 ) closesocket( sock ) ;
	return dataLen;	
}
STREAM openfile( char* filename ,const char* mod )
{
	int index = 0;
	char* ptr1 = NULL;
	char* ptr2 = NULL;
	char nameCpy[256];
	static STREAM file;
	if( filename == NULL || mod == NULL )
	{
		return NULL;
	}

	memset(  nameCpy , 0 , sizeof( nameCpy) );
	if( strcpy( nameCpy , filename ) == NULL )
	{
		return NULL;
	}

	ptr1 = filename;
	while( (ptr1 = strstr( ptr1 , "." )) )
	{
		ptr2 = ++ptr1;
	}

	if( ptr2 == NULL )
	{
		ptr1 = nameCpy + strlen( nameCpy )-1;
		ptr2 = filename + strlen( filename )-1;
	}
	else
	{
		ptr1 = nameCpy + (ptr2 - filename)-1;
		ptr2 -=1;
	}
	
	index = 0;
	while( (file = fopen( nameCpy , "rb" )) )
	{
		fclose( file ) ;
		index++;
		*ptr1 = 0;
		if( sprintf( nameCpy , "%s(%d)%s" , nameCpy , index , ptr2 ) == -1 )
		{
			return NULL;
		}
	}

	file = fopen( nameCpy , mod );
	return file;
}
int openConnect( STREAM file  , struct sockfd *fd  , char* link)
{
	int ret = 0;
	int len = 0;
	char* ptr;
	char* ptrHost;
	char* ptrSrc;
	char* ptrRequest;	
	char strBuffer[1440] = {0};
	struct sockaddr_in *addr;

	if( fd == NULL || file == NULL || link == NULL )
	{
		return -1;
	}

	ptrHost = getHostptr( link );
	ptrSrc = getSrcptr( link );
	if( ptrHost == NULL || ptrSrc == NULL  )
	{
		return -1;
	}

	ptrRequest = getRequest( ptrHost , ptrSrc  , &fd->d ) ;
	if( ptrRequest == NULL )
	{
		return -1;
	}

	addr = getDestaddr( ptrHost , 80 ) ;
	if( addr == NULL )
	{
		return -1;
	}

	fd->sock = socket( AF_INET , SOCK_STREAM , 0 );
	if( fd->sock < 1 )
	{
		return -1;
	}

	ret = connect( fd->sock , (struct sockaddr*)addr , sizeof(struct sockaddr ) );
	if( ret != 0 )
	{
		goto out;
	}
	
	len = strlen( ptrRequest ) ;
	ret = send( fd->sock , ptrRequest , len , 0 );
	if( ret != len )
	{
		goto out;
	}

	len = ret;
	ret = recv( fd->sock , strBuffer , sizeof( strBuffer ) , 0 );
	if( ret <= 0 )
	{
		goto out;
	}

	ptr = strstr( strBuffer , "HTTP/1.1 20" );
	if( ptr != strBuffer )
	{
		ptr = strstr( strBuffer , "HTTP/1.0 20" );
		if( ptr != strBuffer )
		{
			goto out;	
		}
	}

	ptr = strstr( strBuffer , "\r\n\r\n" );
	if( ptr == NULL )
	{
		goto out;
	}
	ptr+=4;

	len = ret - (ptr - strBuffer);
	fseeko( file , fd->d.beg , SEEK_SET ) ;
	ret = fwrite( ptr , 1 , len , file ) ;
	if( ret != len )
	{ 	
		goto out;	
	}
	fd->d.beg = ftello( file ) ;

	return ret;
out:
	closesocket( fd->sock );
	return -1;
}
int downfile( STREAM file , char* link , long long dataLen , int nConnections )
{
	int ret;
	int i;
	int len;
	unsigned maxfds = 0;
	fd_set readFDS;
	struct timeval timeout;

	struct stack s = {0};
	struct sockfd *fds =NULL;

	char strBuffer[4096];
	long long remain ;

	fds = (struct sockfd*)calloc( nConnections , sizeof( struct sockfd ) );
	if( fds == NULL )
	{
		return -1;
	}
	if( allocStack( &s , 100 ) != 0 )
	{
		return -1;
	}
	if( fillStack( &s , dataLen , nConnections , 10*1000*1000 ) != 0 )
	{
		return -1;
	}

	remain = dataLen;
	while( remain > 0 )
	{
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;

		FD_ZERO( &readFDS );
		for( i = 0 ; i < nConnections ; i ++ )
		{
			if( (fds+i)->delay == 0xffffffff )
			{
				continue;
			}
			if( (fds+i)->sock == 0 && (fds+i)->delay > 0)
			{
				(fds+i)->delay --;
				continue;
			}
			if( (fds+i)->sock == 0 && (fds+i)->delay == 0 )
			{
				ret = popStack( &s , &(fds+i)->d ) ;
				if( ret == 1 )
				{
					(fds+i)->delay = 0xffffffff ;
					printf( "\r%d"OUT_OVER"                              \n", i  );
					continue;
				}
				else if( ret == -1 )
				{
					goto out;
				}

				printf( "\r%d"OPEN_REQUEST" %lld - %lld bytes               \n" , 
						i , (fds+i)->d.beg , (fds+i)->d.end );
				ret = openConnect( file  , fds+i, link ) ;
				if( ret == -1 ) 
				{
					if( pushStack ( &s , &(fds+i)->d ) == -1 )
					{
						goto out ;
					}
					(fds+i)->fails++;
					(fds+i)->sock = 0;
					(fds+i)->delay = DEF_DELAY;
					printf( "\r%d"WAIT_FAIL
							"                              \n" , i );
					if( ( fds+i)->fails > 10 )
					{ 
						(fds+i)->delay = 0xffffffff;
						printf( "\r%d"
								OUT_MOREFAIL
								"                      \n" , i  );
					}
					continue;
				}
				else if( ret >= 0 )
				{
					remain -= ret;		
					printf( "\r%d"OPEN_LOADING"                           \n" , i  );
				}

			}

			FD_SET( (fds+i)->sock , &readFDS) ;
			maxfds  = maxfds > (fds+i)->sock ? maxfds : (fds+i)->sock ;
		}//end for

		ret = select( maxfds+1 , &readFDS , NULL , NULL , &timeout );
		if( ret < 0 )
		{	 
			fprintf( stderr , "ERR:downfile->select->%d;" , GetLastError() );
			goto out;	
		}
		if( ret == 0 )
		{
			continue;
		}
		for( i = 0 ; i < nConnections ; i ++ )
		{
			if( ! FD_ISSET( (fds+i)->sock , &readFDS ) )
			{
				continue;
			}
			memset( strBuffer , 0 , sizeof(strBuffer ) ) ;
			ret = recv( (fds+i)->sock , strBuffer , sizeof( strBuffer ) , 0 );
			if( ret < 0 ) 
			{	
				goto out;	
			}
			if( ret == 0 ) 
			{
				if( (fds+i)->d.beg <= (fds+i)->d.end ) 
				{
					ret = pushStack( &s , &(fds+i)->d ) ;
					if( ret == -1 ) 
					{
						goto out;	
					}
				}

				printf( "\r%d"CLOSE_OVERLOAD"                              \n" , i  );
				closesocket( (fds+i)->sock ) ;
				(fds+i)->sock = 0 ;
			}
			len = ret;
			fseeko( file , (fds+i)->d.beg , SEEK_SET ) ;
			ret = fwrite( strBuffer , 1 , len , file ) ;
			if( ret != len ) 
			{	
				goto out;	
			}
			(fds+i)->d.beg = ftello( file );
			remain-=ret;
			printf( "\r"REMAIN": %lld  bytes                " ,  remain );
		}
	}//end while
out:
	for( i = 0 ; i < nConnections ; i ++)
	{
		if( (fds+i)->sock > 0 && (fds+i)->sock != 0xffffffff )  
		{
			closesocket( (fds+i)->sock ) ;
		}
	}
	freeStack( &s );
	if( fds )	free( fds );

	if( remain != 0 )
	{ 
		return GetLastError();
	}
	return 0;	
}
int freedown( int nConnections , char* link , char* filename )
{
	int ret = 0;
	STREAM file;
	char* ptrLink ;
	char* ptrFilename;
	long long dataLen = 0;
	if( nConnections < 1 || link == NULL )
	{
		fprintf( stderr, "ERR:freedown ->nConnections , link ;" );
		return 1;	
	}
	
	ptrLink = checkLink ( link );
	if( ptrLink == NULL )
	{
		fprintf( stderr , "ERR:freedown->checkLink;" );
		return -1;		
	}

	if( filename != NULL )
	{
		ptrFilename = filename;		
	}
	else
	{
		ptrFilename = getFilenameptr( ptrLink);
	}	
	if( ptrFilename == NULL ) 
	{	
		fprintf( stderr , "ERR:freedown->getFilenamePtr;" );
		return 1;	
	}
	
	dataLen = getDatalen( ptrLink ) ;
	if( dataLen == 0 )
	{	
		fprintf( stderr , "ERR:freedown->getDatalen;" );	
		return -1 ;	
	}

	printf(  FILENAME": %s\n" 
		DATALEN": %lld\n" , ptrFilename , dataLen);

	puts( MAKEFILE );
	file = openfile(  ptrFilename , STR_MOD );
	if( file == NULL )
	{
		fprintf( stderr , "ERR:freedown->openfile;");
		return -1;
	}

	if( (ret = downfile( file , ptrLink , dataLen , nConnections ) ) != 0 ) 
	{ 
		fprintf( stderr , "ERR:freedown->downfile->%d->%s;" , ret ,strerror( ret ) );
	}

	fclose( file );
	if( ret == 0 ) 
	{
		puts( "\n"FINISH );
	}
	else
	{
		puts( "\n"ISERROR );
	}
	return ret;
}
