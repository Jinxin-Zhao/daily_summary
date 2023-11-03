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
const int MAX_SIZE = 512;

void client(int read_fd,int write_fd){
	size_t len;
	ssize_t n;
	char buff[MAX_SIZE];
	fgets(buff,MAX_SIZE,stdin);
	len = strlen(buff);
	if(buff[len-1] == '\n'){
		len--;
	}
	write(write_fd,buff,len);
	while( (n = read(read_fd,buff,MAX_SIZE)) > 0){
		write(STDOUT_FILENO,buff,n);
	}
}

void server(int read_fd,int write_fd){
	int fd;
	ssize_t n;
	char buff[MAX_SIZE+1];

	if( (n = read(read_fd,buff,MAX_SIZE)) == 0 )
	{
		//err_quit("end of file while reading pathname");
		cout<<"error"<<endl;
	}
	buff[n] = '\0';
	if( (fd = open(buff,O_RDONLY)) < 0 ){
		snprintf(buff+n,sizeof(buff)-n,":can't open,%s\n",strerror(errno));
		n = strlen(buff);
		write(write_fd,buff,n);
	}else{
		while( (n = read(fd,buff,MAX_SIZE)) > 0 ){
			write(write_fd,buff,n);
		}
		close(fd);
	}
}

//half_work communication 
// int main(){
// 	//pipe_1[0]:read pipe_1[1]:write
// 	int pipe_1[2],pipe_2[2];
// 	pid_t child_pid;
// 	pipe(pipe_1);
// 	pipe(pipe_2);


// 	cout<<"pipe_1_read: "<<pipe_1[0] << " pipe_1_write: "<< pipe_1[1]<<endl;
// 	cout<<"pipe_2_read: "<<pipe_2[0] << " pipe_2_write: "<< pipe_2[1]<<endl;
// 	if((child_pid = fork()) == 0){
// 		//child process----client
// 		close(pipe_1[1]);
// 		close(pipe_2[0]);
// 		server(pipe_1[0],pipe_2[1]);
// 		exit(0);
// 	}

// 	close(pipe_1[0]);
// 	close(pipe_2[1]);

// 	client(pipe_2[0],pipe_1[1]);
// 	waitpid(child_pid,NULL,0);
// 	exit(0);
// }

//all_work communication
int main(){
	int fd[2],n;
	char c;
	pid_t child_pid;
	pipe(fd);

	if( (child_pid = fork()) == 0 ){
		sleep(2);
		if( n = read(fd[0],&c,1) != 1){
			cout<<"error"<<endl;
		}
		printf("child read %c\n",c);
		write(fd[0],"c",1);
		exit(0);
	}
	write(fd[1],"p",1);
	sleep(3);
	if( (n = read(fd[1],&c,1)) != 1 ){
		cout<<"parent read error"<<endl;
	}
	printf("parent read %c\n",c);
	return 0;
}
