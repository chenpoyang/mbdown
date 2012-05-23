#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql/mysql.h>

#define HOST        "127.0.0.1"
#define USER        "young"
#define PASSWORD    "10"
#define DATABASE    "test"

/* connect mysql */
MYSQL * connect_mysql(const char *host, 
        const char *user, const char *pwd, const char *db)
{
    MYSQL *mysql = NULL;

    mysql = mysql_init(mysql);
    if (mysql == NULL)
    {
        printf("failed to initialise mysql!\r\n");
        return NULL;
    }
    
    if (!mysql_real_connect(mysql, host, user, pwd, db, 3306, NULL, 0))
    {
        fprintf(stderr, "Error: %s\r\n", mysql_error(mysql));
        return NULL;
    }
    
    return mysql;
}

/* sql request */
MYSQL_RES * query_sql(MYSQL *mysql, const char *sql)
{
    int ret;
    MYSQL_RES * res;
    
    ret = mysql_real_query(mysql, sql, strlen(sql));
    if (ret != 0)
    {
        printf("mysql_real_query error!\r\n");
        return NULL;
    }

    res = mysql_store_result(mysql);
    if (res == NULL)
    {
        printf("empty result!\r\n");
    }

    return res;
}

int main(int argc, char *argv[])
{
    char *sql = "show databases;";
    MYSQL *mysql = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    int i, len;

    mysql = connect_mysql(HOST, USER, PASSWORD, DATABASE);
    if (mysql == NULL)
    {
        printf("failed to link mysql!\r\n");
        return -1;
    }

    res = query_sql(mysql, sql);
    if (res == NULL)
    {
        printf("failed to query mysql!\r\n");
        return -1;
    }

    len = (int)mysql_num_rows(res);
    for (i = 0; i < len; ++i)
    {
        row = mysql_fetch_row(res);
        puts(row[0]);
    }
    mysql_free_result(res);
    mysql_close(mysql);

    return 0;
}
