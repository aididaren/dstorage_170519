#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "hiredis.h"


int main(int argc, char *argv[])
{
		redisContext *conn = NULL;
		
		conn = redisConnect("127.0.0.1", 6379);
		if(conn == NULL || conn->err)
		{
			if(conn){
				printf("ERROR:%s\n", conn->errstr);
			}
			else 
			{
				printf("Can't allocate redis context\n");
			}    
		}
		
		redisReply *reply = NULL;
		reply = redisCommand(conn, "SET %s %s", "test_string", "hello");
		if(reply == NULL)
		{
			printf("set error\n");
			goto END;
		}
		freeReplyObject(reply);
		printf("set %s succ!\n", "test_string");

		reply = redisCommand(conn, "GET %s", "test_string");
		if(reply == NULL)
		{
			printf("get error\n");
			goto END;
		}
		freeReplyObject(reply);
		printf("values = %s, len = %d \n", reply->str, (int)reply->len);
	
		char *list_key = "my_test_list";
		int list_len = 0;
		
		reply = redisCommand(conn, "lpush %s %s", list_key, "cpp");
		if (reply == NULL) {
        printf("lpush error\n");
        goto END;
    }
    list_len = reply->integer;
    freeReplyObject(reply);

    reply = redisCommand(conn, "lpush %s %s", list_key, "python");
    if (reply == NULL) {
        printf("lpush error\n");
        goto END;
    }
    list_len = reply->integer;
    freeReplyObject(reply);
    
    printf("lpush succ len = %d\n", list_len);
	
END:
		if(reply != NULL)	freeReplyObject(reply);
		
		if(conn != NULL)
		{
			redisFree(conn);
		}		
		
		
    return 0;
}