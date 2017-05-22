
CC=gcc
CPPFLAGS= -I./include -I/usr/include/fastdfs -I/usr/include/fastcommon -I /usr/local/include/hiredis/
CFLAGS=-Wall 
LIBS= -lfdfsclient -lfastcommon -lhiredis

main=./main
test_hiredis = ./test/test_hiredis
test_redisop = ./test/redis_op_test

target=$(main) $(test_hiredis) $(test_redisop)


ALL:$(target)


#生成所有的.o文件
%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#main程序
$(main):./main.o ./fdfs_api.o make_log.o 
	$(CC) $^ -o $@ $(LIBS)
	
#test_hiredis程序
$(test_hiredis):./test/test_hiredis.o
	$(CC) $^ -o $@ $(LIBS)
	
#redis_op_test程序
$(test_redisop):./test/redis_op_test.o make_log.o redis_op.o fdfs_api.o
	$(CC) $^ -o $@ $(LIBS)



#clean指令

clean:
	-rm -rf ./*.o $(target) ./test/*.o

distclean:
	-rm -rf ./*.o $(target) ./test/*.o

#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
