/*
 * tec_mysql.c
 *
 *  Created on: 2017-9-15
 *      Author: root
 */


#include "tec_maria.h"

#include <stdbool.h>

static inline void _mysql_check(MYSQL * con) {
    fprintf(stderr, "%s\n", mysql_error(con));
   // mysql_close(con);
   // exit(EXIT_FAILURE);
}

bool sql_init(MYSQL *mysql)
{
	/*连接之前，先用mysql_init初始化MYSQL连接句柄*/
	mysql_init(mysql);
	  if (NULL == mysql) {
			fputs("main mysql_init NULL == mysql! error !\n", stderr);
			return false;
		}
	/*使用mysql_real_connect连接服务器,其参数依次为MYSQL句柄，服务器IP地址，
	 登录mysql的用户名，密码，要连接的数据库等*/
	if (!mysql_real_connect(mysql, "localhost", "root", "root", "testdb",
			0, NULL, 0)) {
		_mysql_check(mysql);
		printf("Error connecting to Mysql!\n");
		return false;
		//return 1;
	} else {
		printf("Connected Mysql successful!\n");
		return true;
		//return 0;
	}


}
