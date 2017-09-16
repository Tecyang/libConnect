/*
 * libconnect_mysql.h
 *
 *  Created on: 2017-9-15
 *      Author: root
 */

#ifndef TEC_MYSQL_H_
#define TEC_MYSQL_H_

#include <mysql.h>
#include <stdio.h>
#include <stdbool.h>

/*extern C __cplusplus*/
#ifdef      __cplusplus
extern       "C"{
#endif

/*func*/
bool sql_init(MYSQL* mysql);
void close();

/*func*/

#ifdef      __cplusplus
	}
#endif/*extern C __cplusplus*/

#endif /* TEC_MYSQL_H_ */


