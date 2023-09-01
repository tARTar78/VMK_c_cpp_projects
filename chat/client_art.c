#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define PORTNUM 5555
#define BACKLOG 5
#define BUFLEN 256

int save_buf(int fd, char * buf,size_t size, int flags ){
        char bufdop[256];
        int sum = 0;
        int res = recv(fd,&bufdop,size,flags);
        sum += res;
	//printf("OK\n");
        if(res < 0){
                return res;
        }
	strcpy(buf,bufdop);
	//printf("[%s]\n",bufdop);
        while(bufdop[res-1] != '\0'){
                //strcat(buf,bufdop);
                int res = recv(fd,&bufdop,size,flags);
		//printf("OK\n");
                if(res < 0){
                        return res;
                }
                sum+=res;
		strcat(buf,bufdop);

        }
        //strcat(buf,bufdop);
	//printf("sbf - %d\n",sum);
	//printf("OKkkk:%s\n",buf);
        return sum;

}

int main(int argc, char* argv[]){
	signal(SIGINT,SIG_DFL);
        int client, res;
	char buf[BUFLEN];
	memset(buf,'\0',BUFLEN);
	char user_name[BUFLEN];
	char sys_str[BUFLEN];
	char msg[BUFLEN];
        if((client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0){
                perror("error in socket\n");
                exit(1);

        }
	struct sockaddr_in adr = {0};
	adr.sin_family = AF_INET;
	adr.sin_port = htons(PORTNUM);
	if((res = inet_pton(AF_INET,"127.0.0.1",&adr.sin_addr)) == 0){
		printf("bad addres\n");
		exit(1);
	}else if(res < 0){
		perror("error in inet_pton\n");
		exit(1);
	}
	if(connect(client,(struct sockaddr *)&adr, sizeof(adr)) < 0){
		perror("error in connect");
		exit(1);
	}
	printf("enter USER name: ");
	scanf("%s",user_name);
	user_name[strlen(user_name)] = '\0';
	strcpy(sys_str,user_name);
	strcat(sys_str,": left the chat");
	//user_name[strlen(user_name)+1] = '\0';
	//printf("[%s]\n",user_name);
	if(send(client,user_name,strlen(user_name)+1,0) != strlen(user_name)+1){
		perror("error in send\n");
		exit(1);
	}
	pid_t pid = fork();
	if(pid < 0){
                perror("error in fork\n");
                exit(1);
        }
	if(pid != 0){
		while(1){
			if((res = save_buf(client,buf,BUFLEN,0)) < 0/*recv(client,&buf,BUFLEN,0))<0*/){
                		perror("error in recv\n");
                		exit(1);
        		}else if(res > 0){
				//printf("OK\n");
				//buf[res] = '\0';
				printf("%s\n",buf);
                                fflush(stdout);
				//printf("%s",buf);
                                //fflush(stdout);
				if((strcmp(buf,sys_str) == 0) || (!strcmp(buf,"BAN")) || (!strncmp(buf,"kick: ",6)) || (!strncmp(buf,"ban: ",5)) || (!strncmp(buf,"shutdown: ",10))){
					printf("cool\n");
					kill(pid,SIGINT);
					exit(0);
				}
				//printf("%s\n",buf);
				//fflush(stdout);
			}
			//printf("%d\n",res);
		}
	}
	fgets(msg,BUFLEN,stdin);
	while(1){
		//sleep(1);
		//printf("enter message: ");
		//printf("ssssss\n");
		//sleep(60);
        	//fscanf(stdin,"%[^\n]",msg);
		fgets(msg,BUFLEN,stdin);
		//msg[strlen(msg)] = '#';
		msg[strlen(msg)-1] = '\0';
       		if(send(client,msg,strlen(msg)+1,0) != strlen(msg)+1){
                	perror("error in send\n");
                	exit(1);
		}
	}
	shutdown(client,1);
	close(client);
	return 0;
}
