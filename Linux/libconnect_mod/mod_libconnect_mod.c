/* 
 **  mod_libconnect_mod.c -- Apache sample libconnect_mod module
 **  [Autogenerated via ``apxs -n libconnect_mod -g'']
 **
 **  To play with this sample module first compile it into a
 **  DSO file and install it into Apache's modules directory
 **  by running:
 **
 **    $ apxs -c -i mod_libconnect_mod.c
 **
 **  Then activate it in Apache's apache2.conf file for instance
 **  for the URL /libconnect_mod in as follows:
 **
 **    #   apache2.conf
 **    LoadModule libconnect_mod_module modules/mod_libconnect_mod.so
 **    <Location /libconnect_mod>
 **    SetHandler libconnect_mod
 **    </Location>
 **
 **  Then after restarting Apache via
 **
 **    $ apachectl restart
 **
 **  you immediately can request the URL /libconnect_mod and watch for the
 **  output of this module. This can be achieved for instance via:
 **
 **    $ lynx -mime_header http://localhost/libconnect_mod
 **
 **  The output should be similar to the following one:
 **
 **    HTTP/1.1 200 OK
 **    Date: Tue, 31 Mar 1998 14:42:22 GMT
 **    Server: Apache/1.3.4 (Unix)
 **    Connection: close
 **    Content-Type: text/html
 **
 **    The sample page from mod_libconnect_mod.c
 */
#include <stdio.h>

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "mod_pcap.h"


//数据缓存处理


/* The sample content handler */
static int libconnect_mod_handler(request_rec *r) {
    printf("comming libconnect_mod_handler");
	if (strcmp(r->handler, "libconnect_mod")) {
		return DECLINED;
	}
	/*
	 * FIXME: 1进行post数据获取
	 */

	if ((r->method_number != M_GET) && (r->method_number != M_POST)) {
		return HTTP_METHOD_NOT_ALLOWED;
	}

	char *post = (char *) malloc(sizeof(char) * DFT_BUF_SIZE);
	size_t post_size = DFT_BUF_SIZE;

	if (post == NULL) {
		return HTTP_INTERNAL_SERVER_ERROR;
	}

	memset(post, '\0', post_size);

	int ret = read_post_data(r, &post, &post_size);
	//post 为接收到的post过来的数据

	//进行数据验证 验证失败不接受返回
	if (ret != OK) {
		free(post);
		post = NULL;
		post_size = 0;
		return ret;
	}
	/*
	 * FIXME: 2发包->监听收包->输出
	 */
	pcap_Init("sss");

	//testmain(&post, r);

	/*
	 * FIXME: 3监听收包->输出
	 */
	// a) 输出参数为第一个参数；
	//	b) 如果某个函数需要内部分配内存，则将一个apr_pool_t参数放在最后
	ap_set_content_type(r, "text/html;charset=utf-8");
	ap_set_content_length(r, post_size);

	if (post_size == 0) {
		ap_rputs("no post data found", r);
		return OK;
	}

	return OK;
}

static void libconnect_mod_register_hooks(apr_pool_t *p) {
	ap_hook_handler(libconnect_mod_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA libconnect_mod_module = { STANDARD20_MODULE_STUFF,
		NULL, /* create per-dir    config structures */
		NULL, /* merge  per-dir    config structures */
		NULL, /* create per-server config structures */
		NULL, /* merge  per-server config structures */
		NULL, /* table of config file commands       */
		libconnect_mod_register_hooks /* register hooks                      */
};

