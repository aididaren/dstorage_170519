#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "redis_op.h"
#include "make_log.h"
#include "fdfs_api.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT "6379"


//完成读取当前redis服务器都有哪些文件，并将每个文件的信息，存储log日志中
int foreach_redis(redisContext *conn)
{
	if (conn == NULL) {
		return -1;
	}
	int ret = 0;
	
	redisReply *reply = NULL;
	char *files = (char *)malloc(1024*1024);
	
	
	reply = redisCommand(conn, "KEYS *");
	if(reply == NULL)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "KEYS * error");
		ret = -1;
		return ret;
	}

	
	printf("-------------\n");
	
	FILE* fd = fopen("./redis_key.txt", "r+");
	char *buf[reply->elements];
	int i = 0;
	for(int n = 0; n < reply->elements; ++n )
	{
		buf[n] = (char *)malloc(1024);
		fgets(buf[n], 1024, fd);
		i = strlen(buf[n]);
		if(buf[n][i-1] == '\n')
		{
			buf[n][i-1] = 0;
		}
		printf("%s\n", buf[n]);

/*
		char *each_key = buf[n];
		char *value = (char *)malloc(1024);


		if(rop_get_string(conn, each_key, value) != 0)
		{
			printf("------------");
			LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "STRING key:%s, value:%s", each_key, value);
			continue;
		}
		else if(redisCommand(conn, "HGETALL %s", each_key) != NULL)
		{
			reply = redisCommand(conn, "HGETALL %s", each_key);
			printf("%s", reply->elements);
			
			for(int k = 0; k < reply->elements; ++k)
			{				
				printf("%s", reply->str);
				reply = *(reply->element);
			}
			continue;
			
		}
		else if(redisCommand(conn, "LRANG %s %d %d", each_key, 0, -1) != NULL)
		{
		
			reply = redisCommand(conn, "LRANG %s %d %d", each_key, 0, -1);
			printf("%s", reply->elements);
			continue;
		}

		
	}
	
	*/


	
	
	

	freeReplyObject(reply);
	return ret;
}



int main(int argc, char *argv[])
{
	int ret = 0;
	redisContext *conn = NULL;
	
	char *key = "my_list";
	
	time_t t;
	t = time(NULL);
	
	char *time = ctime(&t);
	
	
	
	
	//创建一个连接句柄
	conn = rop_connectdb_nopwd(SERVERIP, SERVERPORT);
	if(conn == NULL)
	{
		LOG("test", "redis_op", "conn db error");
		ret = -1;
		goto END;
	}

	//链表中插入一条数据
	ret = rop_list_push(conn, key, "aaa");
	if(ret != 0)
	{
		LOG("test", "redis_op", "lpush %s error", key);
		goto END;
	}
	
	printf("----------test----------\n");
	//插入字符串
	ret = rop_set_string(conn, "name", "gailun");
	if(ret != 0)
	{
		LOG("test", "redis_op", "set %s error", key);
		goto END;
	}
	printf("set name succ!\n");
	
	char *value = (char *)malloc(128);
	//获取字符串
	ret = rop_get_string(conn, "name", value);
	if(ret != 0)
	{
		LOG("test", "redis_op", "get %s error", key);
		goto END;
	}
	printf("get key:%s value:%s succ!\n", key, value);
	free(value);
	
	
	//h插入字符串
	ret = rop_hset_string(conn, "hash", "name", "bufer");
	if(ret != 0)
	{
		LOG("test", "redis_op", "set %s error", key);
		goto END;
	}
	printf("set name succ!\n");
	
	char *value1 = (char *)malloc(128);
	//h获取字符串
	ret = rop_hget_string(conn, "hash", "name", value1);
	if(ret != 0)
	{
		LOG("test", "redis_op", "get %s error", key);
		goto END;
	}
	printf("get key:%s value:%s succ!\n", key, value1);
	free(value1);
	printf("----------test----------\n");
	
	//-------------------------
	//fopen创建或打开一个文件
	
	char *filename = "./redis_list.txt";
	/*ret = fopen(filename, O_RDWR | O_CREAT, 0644);
	if(ret != 0)
	{
		
	}
	*/
	//或者手动创建一个文件
	
	 char file_id[FILE_ID_LEN] = {0};
	//通过fdfs接口上传本地文件,得到fileid
	ret = fdfs_upload_by_filename(filename, file_id);
	if(ret != 0)
	{
		LOG("test", "redis_op", "fdfs_upload_fileid error");
		goto END;
	}
	
	
	//组织列表
	
	char *key_attr = "fileAttribute";
	//创建fileAttribute 的fileid

		ret = rop_hset_string(conn, key_attr, "fileid", file_id);
		if(ret != 0)
		{
			LOG("test", "redis_op", "rop_hset_string fileid error");
			goto END;
		}
	

	//创建fileAttribute 的time
	
		ret = rop_hset_string(conn, key_attr, "time", time);
		if(ret != 0)
		{
			LOG("test", "redis_op", "rop_hset_string time error");
			goto END;
		}
	
	
	//创建fileAttribute 的belongs
	
		ret = rop_hset_string(conn, key_attr, "belongs", "itheima");
		if(ret != 0)
		{
			LOG("test", "redis_op", "rop_hset_string belongs error");
			goto END;
		}

	
	//创建fileAttribute 的downloads
	
		ret = rop_hset_string(conn, key_attr, "downloads", "1");
		if(ret != 0)
		{
			LOG("test", "redis_op", "rop_hset_string downloads error");
			goto END;
		}
	
	
	//将fileid存入到 FILE_INFO_LIST列表中

		ret = rop_set_string(conn, "FILE_INFO_LIST", file_id);
		if(ret != 0)
		{
			LOG("test", "redis_op", "rop_hset_string fileid error");
			goto END;
		}
	
	printf("fileid存入到 FILE_INFO_LIST\n");
	
	//----------------------------
	//读取当前redis服务器所有文件
	ret = foreach_redis(conn);
	if(ret != 0)
	{
		printf("foreach_redis error!\n");
	}
	
	
	
	
	
	
	
	
	//断开连接
	if(conn != NULL)
	{
		rop_disconnect(conn);
	}
	
	
	
	
	
END:
	
	return ret;
}