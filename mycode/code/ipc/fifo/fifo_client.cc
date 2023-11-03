#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main(){
	int fd = open("mypipe",O_WRONLY);
	if(fd < 0){
		perror("open");
		return 1;
	}
	char buf[1024] = {0};
	while(1){
		cout<<"please enter #";
		ssize_t s  = read(0,buf,sizeof(buf)-1);
		if(s > 0){
			buf[s] = 0;
			write(fd,buf,strlen(buf));
		}else if(s == 0){
			cout<<"read finish";
			return 0;
		}else{
			perror("read");
			return 2;
		}
	}
	close(fd);
	return 0;
}
