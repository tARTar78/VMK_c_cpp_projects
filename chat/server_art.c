#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define PORTNUM 5555
#define BACKLOG 5
#define BUFLEN 256
#define MSG 2056
#define MSGP 1024
#define MAX_CL 10
#define MAX_USERS 50

struct user
{
        char* username;
        char** privates;
	int pr_us ;
	int admin ;

};

void users_list_f(struct user* mas, char* msg){
	int c = 1, i = 1;
	char dop[MSG];
	char password[BUFLEN];
	printf("Enter password:\n");
	scanf("%s",password);
	while(i != MAX_USERS){
		if(mas[i].username != NULL){
			sprintf(dop,"%d:%s\n",c,mas[i].username);
			if(c == 1){
				strcpy(msg,dop);
			}else{
				strcat(msg,dop);
			}
			c++;
		}
		i++;
	}
}
int send_all(int clients,struct pollfd * fds, char * msg){
	for(int i = 1;i <= clients;++i){
		if(fds[i].fd != -1){
			if(send(fds[i].fd,msg,strlen(msg)+1,0) != strlen(msg)+1){
				perror("error in send\n");//save_buf(fds_cl[clients].fd,buf,BUFLEN,0);
                        	close(fds[i].fd);
                        	fds[i].fd = -1;
		}
		}
	}

}

int n_poll(struct user* mas, char* name){
	for(int i = 1; i <= MAX_USERS; ++i){

		if(mas[i].username!= NULL){
			if(!strcmp(mas[i].username,name)){
				return i;
			}
		}
	}

	return -1;

}

int n_banlist (char** mas, char* name){
	int i = 0;
	if(mas == NULL){
		return -1;
	}
        while(mas[i] != NULL){

             if(!strcmp(mas[i],name)){
			return i;
	     }
	     i++;
        }

        return -1;

}


int save_buf(int fd, char * buf,size_t size, int flags ){
	char bufdop[256];
	int sum = 0;
	int res = recv(fd,&bufdop,size,flags);
	sum += res;
	//printf("%s\n",bufdop);
	if(res < 0){
		return res;
	}
	//printf("OK\n");
	strcpy(buf,bufdop);
	while(bufdop[res-1] != '\0'){
		//strcat(buf,bufdop);
		//printf("OK\n");
		int res = recv(fd,&bufdop,size,flags);
		//printf("%s\n",bufdop)
		if(res < 0){
                	return res;
        	}
		//printf("OK\n");
		sum+=res;
		strcat(buf,bufdop);

	}
	//strcat(buf,bufdop);
	//printf("%s\n",buf);
	return sum;

}

void clear_privates(char** list){
	int c = 0;
	if(list == NULL)
		return;
	while(list[c] != NULL){
		free(list[c]);
		c++;

	}
	free(list);

}

int main(){
	struct user* users_list = (struct user*)calloc(MAX_USERS+1,sizeof(struct user));
	char** banlist = NULL;
	char password[BUFLEN];
	printf("Password_server:\n");
	scanf("%s",password);
	int end = 0,client_len, serv,clients = 0, len, max_clients, events, dop_socket,bn = 0;
	char buf[BUFLEN];
	char help[MSG] = "\\help\n\\quit\n\\users\n\\private\n\\privates\n";
	memset(buf,'\0',BUFLEN);
	char chat_msg[MSG];
	struct pollfd * fds_cl, * fds_temp;
	if((serv = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0){
		perror("error in socket\n");
		exit(1);

	}
	struct sockaddr_in serv_adr = {0}, client_adr = {0};
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = INADDR_ANY;
	serv_adr.sin_port = htons(PORTNUM);
	if(bind(serv,(struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0){
		perror("error in bind\n");
		exit(1);
	}

	if(listen(serv,BACKLOG) < 0){
		perror("error in listen\n");
		exit(1);
	}
	max_clients = MAX_CL;
	clients = 0;
	fds_cl = (struct pollfd *)calloc(max_clients + 1, sizeof(struct pollfd));
	if(fds_cl == NULL){
		perror("error in calloc\n");
		exit(1);
	}
	fds_cl[0].fd = serv;
	fds_cl[0].events = POLLIN | POLLERR | POLLPRI | POLLOUT;
    	fds_cl[0].revents = 0;
	while(1){
		events = poll(fds_cl,clients + 1,100);
		if(events == -1){
			perror("error in poll\n");
			exit(1);
		}
		if(events == 0)
			continue;
		if(fds_cl[0].revents){
			dop_socket = accept(serv,NULL,NULL);
			//if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                        //perror("error in send\n");
                                                //}			
			if(dop_socket < 0){
				perror("error in accept\n");
				exit(1);
			}

			clients++;
			if(clients >= max_clients){
				max_clients += MAX_CL;
				fds_temp = fds_cl;
				fds_cl = realloc(fds_cl,sizeof(struct pollfd) * (max_clients + 1));
				if(fds_cl == NULL){
					perror("error in realloc\n");
					free(fds_temp);
					exit(1);
				}

			}

			fds_cl[clients].fd = dop_socket;
			fds_cl[clients].events = POLLIN | POLLERR | POLLPRI | POLLHUP;
            		//fds_cl[clients].revents = 0;
			fds_cl[0].revents = 0;
			len = save_buf(fds_cl[clients].fd,buf,BUFLEN,0);//recv(fds_cl[clients].fd,&buf,BUFLEN,0);
                        //buf[len] = '\0';
			if(n_banlist(banlist,buf) == -1){

			
			fds_cl[clients].revents = 0;
			users_list[clients].admin = 0;
			users_list[clients].username = (char*)calloc(BUFLEN,sizeof(char));
			
			strcpy(users_list[clients].username,buf);
			//memset(chat_msg,'\0',MSG);
			sprintf(chat_msg,"USER %s entered the chat",users_list[clients].username);
			chat_msg[strlen(chat_msg)] = '\0';
			//printf("[%s]\n",chat_msg);
			//printf();
			printf("USER %s entered the chat\n",users_list[clients].username);
			fflush(stdout);
			//sprintf(chat_msg,"%s:%s\n",users_list[clients],buf);
			send_all(clients,fds_cl,chat_msg);
			}else{
						sprintf(chat_msg,"BAN");
                                                chat_msg[strlen(chat_msg)] = '\0';
                                                if(send(fds_cl[clients].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                        perror("error in send\n");
                                                        close(fds_cl[clients].fd);
                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                        fds_cl[clients].fd = -1;
                                                        free(users_list[clients].username);
                                                        users_list[clients].username = NULL;
                                                        clear_privates(users_list[clients].privates);
                                                }
						close(fds_cl[clients].fd);
                                                //shutdown(fds_cl[i].fd,SHUT_WR);
                                                fds_cl[clients].fd = -1;
                                                free(users_list[clients].username);
                                                users_list[clients].username = NULL;
                                                clear_privates(users_list[clients].privates);

			}	
		
		}

		for(int i = 1; i <= clients; ++i){
			if(fds_cl[i].revents){

				len = save_buf(fds_cl[i].fd,buf,BUFLEN,0);//recv(fds_cl[i].fd,&buf,BUFLEN,0);
				//buf[len] = '\0';
				if(len < 0 ){
					perror("error in recv\n");
					close(fds_cl[i].fd);
                    			fds_cl[i].fd = -1;
					free(users_list[i].username);
                                        users_list[i].username = NULL;
                                        clear_privates(users_list[i].privates);

				}else if(len > 0){
					//printf("%s\n",buf);
					if(!strcmp(buf,"\\quit")){
						sprintf(chat_msg,"%s: %s",users_list[i].username,"left the chat");
                                                //strcat(chat_msg,"\0");
						chat_msg[strlen(chat_msg)] = '\0';
                                                printf("%s\n",chat_msg);
                                                send_all(clients,fds_cl,chat_msg);
						//sprintf(chat_msg,"\\END");
						//chat_msg[strlen(chat_msg)] = '\0';
						//printf("%s\n",chat_msg);
						//printf("cl - %d\ni - %:set nu
						//d\n",clients,i);
						//if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                        				//perror("error in send\n");
                				//}
						//sleep(1);
						close(fds_cl[i].fd);
						//shutdown(fds_cl[i].fd,SHUT_WR);
                                        	fds_cl[i].fd = -1;
						free(users_list[i].username);
						users_list[i].username = NULL;
						clear_privates(users_list[i].privates);
						
					}else if(!strcmp(buf,"\\admin")){
						sprintf(chat_msg,"Enter password:");
						chat_msg[strlen(chat_msg)] = '\0';
						if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                        perror("error in send\n");
                                                        close(fds_cl[i].fd);
                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                        fds_cl[i].fd = -1;
                                                        free(users_list[i].username);
                                                        users_list[i].username = NULL;
                                                        clear_privates(users_list[i].privates);
                                                }
						len = save_buf(fds_cl[i].fd,buf,BUFLEN,0);
						if(len < 0 ){
                                        		perror("error in recv\n");
                                        		close(fds_cl[i].fd);
                                        		fds_cl[i].fd = -1;
							free(users_list[i].username);
                                                        users_list[i].username = NULL;
                                                        clear_privates(users_list[i].privates);

                                		}else if(len > 0){
							if(!strcmp(buf,password)){
								sprintf(chat_msg,"Hello,admin!");
                                                		chat_msg[strlen(chat_msg)] = '\0';
                                                		if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                        		perror("error in send\n");
                                                        		close(fds_cl[i].fd);
                                                        		//shutdown(fds_cl[i].fd,SHUT_WR);
                                                        		fds_cl[i].fd = -1;
                                                        		free(users_list[i].username);
                                                        		users_list[i].username = NULL;
                                                        		clear_privates(users_list[i].privates);
                                                		}
								users_list[i].admin = 1;

							}else{
								sprintf(chat_msg,"Wrong password");
                                                                chat_msg[strlen(chat_msg)] = '\0';
                                                                if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[i].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[i].fd = -1;
                                                                        free(users_list[i].username);
                                                                        users_list[i].username = NULL;
                                                                        clear_privates(users_list[i].privates);
                                                                }

							}
						}
						
					}else if(!strncmp(buf,"\\nick ",6)){
						if(users_list[i].admin == 1){
							char newname[MSGP];
							char name[MSG];
							int cnt = 0, j = 6;
							for(; buf[j] != ' '; ++j){
								name[cnt] = buf[j];
								cnt++;
							}
							name[cnt] = '\0';
							j++;
							cnt = 0;
							for(; j < strlen(buf); ++j){
                                                                newname[cnt] = buf[j];
                                                                cnt++;
                                                        }
							newname[cnt] = '\0';
							cnt = n_poll(users_list,name);
							strcpy(users_list[cnt].username,newname);
							sprintf(chat_msg,"you are %s now", newname);
                                                        chat_msg[strlen(chat_msg)] = '\0';
                                                        if(send(fds_cl[cnt].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[cnt].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[cnt].fd = -1;
                                                                        free(users_list[cnt].username);
                                                                        users_list[cnt].username = NULL;
                                                                        clear_privates(users_list[cnt].privates);
                                                                }

						}else{
								sprintf(chat_msg,"you aren't an admin");
                                                                chat_msg[strlen(chat_msg)] = '\0';
                                                                if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[i].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[i].fd = -1;
                                                                        free(users_list[i].username);
                                                                        users_list[i].username = NULL;
                                                                        clear_privates(users_list[i].privates);
                                                                }


						}

                                        }else if(!strncmp(buf,"\\kick ",6)){
						if(users_list[i].admin == 1){
                                                        char msg[MSGP];
                                                        char name[MSG];
                                                        int cnt = 0, j = 6;
                                                        for(; buf[j] != ' '; ++j){
                                                                name[cnt] = buf[j];
                                                                cnt++;
                                                        }
                                                        name[cnt] = '\0';
                                                        j++;
                                                        cnt = 0;
                                                        for(; j < strlen(buf); ++j){
                                                                msg[cnt] = buf[j];
                                                                cnt++;
                                                        }
                                                        msg[cnt] = '\0';
                                                        cnt = n_poll(users_list,name);
                                                        sprintf(chat_msg,"kick: %s", msg);
                                                        chat_msg[strlen(chat_msg)] = '\0';
							if(cnt != -1){
                                                        	if(send(fds_cl[cnt].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[cnt].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[cnt].fd = -1;
                                                                        free(users_list[cnt].username);
                                                                        users_list[cnt].username = NULL;
                                                                        clear_privates(users_list[cnt].privates);
                                                         	}
							}else{
								sprintf(chat_msg,"Wrong username");
								if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[cnt].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[cnt].fd = -1;
                                                                        free(users_list[cnt].username);
                                                                        users_list[cnt].username = NULL;
                                                                        clear_privates(users_list[cnt].privates);
								}

							}
							 sprintf(chat_msg,"%s: left the chat",users_list[cnt].username);
                                                         chat_msg[strlen(chat_msg)] = '\0';
                                                         send_all(clients,fds_cl,chat_msg);
							 close(fds_cl[cnt].fd);
                                                         //shutdown(fds_cl[i].fd,SHUT_WR);
                                                         fds_cl[cnt].fd = -1;
                                                         free(users_list[cnt].username);
                                                         users_list[cnt].username = NULL;
                                                         clear_privates(users_list[cnt].privates);
							 //sprintf(chat_msg,"%s: left the chat",users_list[cnt].username);
							 //chat_msg[strlen(chat_msg)] = '\0';
							 //send_all(clients,fds_cl,chat_msg);


                                                }else{
                                                                sprintf(chat_msg,"you aren't an admin");
                                                                chat_msg[strlen(chat_msg)] = '\0';
                                                                if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[i].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[i].fd = -1;
                                                                        free(users_list[i].username);
                                                                        users_list[i].username = NULL;
                                                                        clear_privates(users_list[i].privates);
                                                                }


                                                }



                                        }else if(!strncmp(buf,"\\ban ",5)){
						if(users_list[i].admin == 1){
                                                        char msg[MSGP];
                                                        char name[MSG];
                                                        int cnt = 0, j = 5;
                                                        for(; buf[j] != ' '; ++j){
                                                                name[cnt] = buf[j];
                                                                cnt++;
                                                        }
                                                        name[cnt] = '\0';
                                                        j++;
                                                        cnt = 0;
                                                        for(; j < strlen(buf); ++j){
                                                                msg[cnt] = buf[j];
                                                                cnt++;
                                                        }
                                                        msg[cnt] = '\0';
                                                        cnt = n_poll(users_list,name);
                                                        sprintf(chat_msg,"ban: %s", msg);
                                                        chat_msg[strlen(chat_msg)] = '\0';
							if(cnt != -1){
                                                        	if(send(fds_cl[cnt].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[cnt].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[cnt].fd = -1;
                                                                        free(users_list[cnt].username);
                                                                        users_list[cnt].username = NULL;
                                                                        clear_privates(users_list[cnt].privates);
                                                        	}
							}else{
								sprintf(chat_msg,"Wrong username");
								if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[cnt].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[cnt].fd = -1;
                                                                        free(users_list[cnt].username);
                                                                        users_list[cnt].username = NULL;
                                                                        clear_privates(users_list[cnt].privates);
                                                        	}

							}
							if(banlist == NULL){
								bn = 1;
								banlist = (char**)calloc(bn,sizeof(char*));
								banlist[bn-1] = (char*)calloc(MSG,sizeof(char));
								strcpy(banlist[bn-1],name);
							}else{
								bn++;
								banlist = realloc(banlist,sizeof(char*)*bn);
                                                                banlist[bn-1] = (char*)calloc(MSG,sizeof(char));
								strcpy(banlist[bn-1],name);

							}
							sprintf(chat_msg,"%s: left the chat",users_list[cnt].username);
                                                        chat_msg[strlen(chat_msg)] = '\0';
                                                        send_all(clients,fds_cl,chat_msg);
							close(fds_cl[cnt].fd);
                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                        fds_cl[cnt].fd = -1;
							free(users_list[cnt].username);
                                                        users_list[cnt].username = NULL;
                                                        clear_privates(users_list[cnt].privates);
							//sprintf(chat_msg,"%s: left the chat",users_list[cnt].username);
                                                        //chat_msg[strlen(chat_msg)] = '\0';
                                                        //send_all(clients,fds_cl,chat_msg);

                                                }else{
                                                                sprintf(chat_msg,"you aren't an admin");
                                                                chat_msg[strlen(chat_msg)] = '\0';
                                                                if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[i].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[i].fd = -1;
                                                                        free(users_list[i].username);
                                                                        users_list[i].username = NULL;
                                                                        clear_privates(users_list[i].privates);
                                                                }


                                                }


                                        }else if(!strncmp(buf,"\\shutdown ",10)){
						if(users_list[i].admin == 1){
							sprintf(chat_msg,"shutdown: ");
							strcat(chat_msg,buf+10);
							send_all(clients,fds_cl,chat_msg);
							end = 1;
							printf("END\n");
							break;
						}
                                        }else if(!strcmp(buf,"\\users")){
						users_list_f(users_list,chat_msg);
						chat_msg[strlen(chat_msg)] = '\0';
						if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                        perror("error in send\n");
							close(fds_cl[i].fd);
                                                	//shutdown(fds_cl[i].fd,SHUT_WR);
                                                	fds_cl[i].fd = -1;
                                               		free(users_list[i].username);
                                                	users_list[i].username = NULL;
                                                	clear_privates(users_list[i].privates);
						}
					}else if(!strcmp(buf,"\\help")){
						if(send(fds_cl[i].fd,help,strlen(help)+1,0) != strlen(help)+1){
                                                        perror("error in send\n");
							close(fds_cl[i].fd);
                                                	//shutdown(fds_cl[i].fd,SHUT_WR);
                                                	fds_cl[i].fd = -1;
                                                	free(users_list[i].username);
                                                	users_list[i].username = NULL;
                                                	clear_privates(users_list[i].privates);
                                                }
					}else if(!strcmp(buf,"\\privates")){
                                                        char priv[MSG];
                                                        char dopp[MSG];
                                                        //printf("cooooool\n");
                                                        if(users_list[i].privates == NULL){
                                                                sprintf(priv,"No privates");
                                                                priv[strlen(priv)] = '\0';
                                                                if(send(fds_cl[i].fd,priv,strlen(priv)+1,0) != strlen(priv)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[i].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[i].fd = -1;
                                                                        free(users_list[i].username);
                                                                        users_list[i].username = NULL;
                                                                        clear_privates(users_list[i].privates);
                                                                }

                                                        }else{

                                                                sprintf(priv,"your privates:\n1:%s\n",(users_list[i].privates)[0]);
                                                                for(int j = 1; j < users_list[i].pr_us;++j){
                                                                        sprintf(dopp,"%d: %s\n",j+1,(users_list[i].privates)[j]);
                                                                        strcat(priv,dopp);
                                                                }
                                                                priv[strlen(priv)] = '\0';
                                                                if(send(fds_cl[i].fd,priv,strlen(priv)+1,0) != strlen(priv)+1){
                                                                        perror("error in send\n");
                                                                        close(fds_cl[i].fd);
                                                                        //shutdown(fds_cl[i].fd,SHUT_WR);
                                                                        fds_cl[i].fd = -1;
                                                                        free(users_list[i].username);
                                                                        users_list[i].username = NULL;
                                                                        clear_privates(users_list[i].privates);
                                                                }


                                                        }

                                        }else if(!strncmp(buf,"\\private ",9)){
						char name[BUFLEN];
						char pr_msg[MSGP];
						int count = 0, j = 9;
						for(; buf[j] != ' '; j++){
							name[count] = buf[j];
							count++;
						}
						j++;
						name[count]='\0';
						count = 0;
						for(;j < strlen(buf);j++){
							pr_msg[count] = buf[j];
							count++;
						}
						pr_msg[count] ='\0';
						sprintf(chat_msg,"from %s to %s: %s",users_list[i].username,name,pr_msg);
						count = n_poll(users_list,name);
						pr_msg[strlen(pr_msg)] = '\0';
						if(count != -1){
							printf("%s\n",chat_msg);
							if(send(fds_cl[count].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                        	perror("error in send\n");
                                                	}
							if(send(fds_cl[i].fd,chat_msg,strlen(chat_msg)+1,0) != strlen(chat_msg)+1){
                                                                perror("error in send\n");
                                                        }

							if(users_list[i].privates == NULL){
                                                                users_list[i].pr_us = 1;
                                                                users_list[i].privates = (char**)calloc(users_list[i].pr_us,sizeof(char*));
                                                                (users_list[i].privates)[users_list[i].pr_us - 1] = (char*)calloc(BUFLEN,sizeof(char));
                                                                strcpy((users_list[i].privates)[users_list[i].pr_us - 1],name);
                                                        }else{
                                                                users_list[i].pr_us++;
                                                                users_list[i].privates = realloc(users_list[i].privates,sizeof(char*)*users_list[i].pr_us);
                                                                (users_list[i].privates)[users_list[i].pr_us - 1] = (char*)calloc(BUFLEN,sizeof(char));
                                                                strcpy((users_list[i].privates)[users_list[i].pr_us - 1],name);

                                                        }


						}else{
							sprintf(pr_msg,"Wrong username");
							pr_msg[strlen(pr_msg)] = '\0';
							if(send(fds_cl[i].fd,pr_msg,strlen(pr_msg)+1,0) != strlen(pr_msg)+1){
                                                                perror("error in send\n");
								close(fds_cl[i].fd);
                                                		//shutdown(fds_cl[i].fd,SHUT_WR);
                                                		fds_cl[i].fd = -1;
                                                		free(users_list[i].username);
                                                		users_list[i].username = NULL;
                                                		clear_privates(users_list[i].privates);
                                                        }
						}

						
					}/*else if(!strcmp(buf,"\\privates")){
							char priv[MSG];
							char dopp[MSG];
							printf("cooooool\n");
							if(users_list[i].privates == NULL){
								sprintf(priv,"No privates");
								priv[strlen(priv)] = '\0';
                                                                if(send(fds_cl[i].fd,priv,strlen(priv)+1,0) != strlen(priv)+1){
                                                                        perror("error in send\n");
									close(fds_cl[i].fd);
                                                			//shutdown(fds_cl[i].fd,SHUT_WR);
                                                			fds_cl[i].fd = -1;
                                                			free(users_list[i].username);
                                                			users_list[i].username = NULL;
                                                			clear_privates(users_list[i].privates);
                                                                }

							}else{
								
								sprintf(priv,"your privates:\n1:%s\n",(users_list[i].privates)[0]);
								for(int j = 1; j < users_list[i].pr_us;++j){
									sprintf(dopp,"%d: %s\n",j+1,(users_list[i].privates)[j]);
									strcat(priv,dopp);
								}
								priv[strlen(priv)] = '\0';
								if(send(fds_cl[i].fd,priv,strlen(priv)+1,0) != strlen(priv)+1){
                                                                	perror("error in send\n");
									close(fds_cl[i].fd);
                                                			//shutdown(fds_cl[i].fd,SHUT_WR);
                                                			fds_cl[i].fd = -1;
                                                			free(users_list[i].username);
                                                			users_list[i].username = NULL;
                                                			clear_privates(users_list[i].privates);
                                                        	}


							}	
						
					}*/else{
						printf("%s: %s\n",users_list[i].username,buf);
						sprintf(chat_msg,"%s: %s",users_list[i].username,buf);
						//strcat(chat_msg,"\0");
						//printf("[%s]\n", chat_msg);
                        			send_all(clients,fds_cl,chat_msg);
					}


				}

			}
			fds_cl[i].revents = 0;
			

		}
		 if(end)
                    break;

	}
	close(fds_cl[0].fd);
        for(int i = 1; i <= clients; ++i){
                          close(fds_cl[i].fd);
                          //shutdown(fds_cl[i].fd,SHUT_WR);
                          fds_cl[i].fd = -1;
                          free(users_list[i].username);
                          users_list[i].username = NULL;
                          clear_privates(users_list[i].privates);
                          }
	clear_privates(banlist);
        free(users_list);
	free(fds_cl);
	return 0;
}
