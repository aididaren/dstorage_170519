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


int main(int argc, char *argv[])
{
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1)
    {
    	perror("pipe error");
    	exit(1);
    }
    
    pid_t pid = fork();
    if(pid == -1)
    {
    	perror("fork error");
    	exit(1);
    }
    
    
    if(pid > 0)
    {
    	close(fd[1]);//关闭写端
    	sleep(1);
    	char buf[1024] = {0};
    	ret = read(fd[0], buf, sizeof(buf));
    	if(ret == 0)
    	{
    		perror("read error");
    		exit(1);
    	}
    	printf("fileid = [%s]\n", buf);
    	
    	wait(NULL);
    	exit(1);
    	
    }
    else if(pid == 0)
    {
    	close(fd[0]);//关闭读端
    	dup2(fd[1], STDOUT_FILENO);
    	execl("/home/itheima/fastdfs-5.05/client/fdfs_upload_file", "myupload", "/etc/fdfs/client.conf", "main.c", NULL);
    	exit(1);
    }
	
    return 0;
}