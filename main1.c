#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "fdfs_api.h"

int main(int argc, char *argv[])
{
		int ret = 0;
		
		char *filename = "abc.txt";
		char file_id[128] = {0};
		
    ret = upload_filename(filename, file_id);
		if(ret != 0)
		{
			printf("upload error\n");
			exit(1);
		}
		printf("fileid = [%s]\n", file_id);
	
    return 0;
}