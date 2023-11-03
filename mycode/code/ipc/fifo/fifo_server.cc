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
	umask(0);
	if(mkfifo("mypipe",0644) < 0){
		perror("mkfifo");
		return 1;
	}
	int fd = open("mypipe",O_RDONLY);
	if(fd < 0){
		perror("open");
		return 2;
	}
	char buf[1024] = {0};
	while(1){
		std::cout<<"please wait...\n";
		ssize_t s = read(fd,buf,sizeof(buf)-1);
		if(s < 0){
			perror("read");
			return 3;
		}else if(s == 0){
			std::cout<<"client is quit\n";
			return 0;
		}else{
			buf[s-1] = 0;
			std::cout<<"client says #  "<<buf<<std::endl;
		}
	}
	close(fd);
	return 0;
}