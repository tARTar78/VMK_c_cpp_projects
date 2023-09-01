#include "exec_art.h"

//extern jmp_buf ebuf;

void addzomb(pid_t pid){
	zomb_t* zn = malloc(sizeof(zomb_t));
	zn->pidz = pid;
	zn->next = z_list;
	z_list = zn;	


}

tree t_ex = NULL;

void print_zomb(){
	if(z_list == NULL){
		printf("NULL\n");
		fflush(stdout);
	}else{
		zomb_t* d = z_list;
		while(d != NULL){
			printf("%ld\n", (long)z_list->pidz);
			d = d->next;
		}
		
	}

}
void execute(tree t);
int pipe_exec(tree t);
int cd(int argc, char** argv);
void proc(tree t, int back, pid_t pgid){

	if(back){
		signal(SIGINT, SIG_IGN);
	}else{
		signal(SIGINT, SIG_DFL);
	}
	if(t->infile != NULL){
		int f = open(t->infile,O_RDONLY);
		if(f == -1){
			fprintf(stderr,"Error in open\n");
			exit(1);
		}else
			dup2(f,0);
		close(f);

	}
	if(t->outfile != NULL){
		int f;
		if(t->append){
			f = open(t->outfile, O_WRONLY|O_APPEND|O_CREAT, 0664);
		}else{
			f = open(t->outfile, O_WRONLY|O_TRUNC|O_CREAT, 0664);
		}
		if(f == -1){
			fprintf(stderr,"Error in open\n");
			exit(1);
		}else
			dup2(f,1);
		close(f);

	}

	if(back){
		
		
		//addzomb(getpid());
		//print_zomb();
		//setpgid(0, pgid);
		int f = open("/dev/null",O_RDONLY);
		dup2(f,0);


	}
	if(t->argv == NULL){
		execute(t->psubcmd);
		exit(0);
	}else{
		//printf("art\n");
		execvp(t->argv[0],t->argv);
		fprintf(stderr,"%s: command not found\n", t->argv[0]);
		exit(1);
	}

}
/*void exec_back(tree t){
	
	 int status = 0;
        status = pipe_exec(t);
        if(t == NULL){
                exit(0);
        }
        tree last;
        while((t->type == CONV)&&(t != NULL)){
                   t = t->next;
        }
        last = t;
        if((t != NULL)&&(t->backgrnd))
               t = t->next;
               
        while((t != NULL) && (t->backgrnd)){
                if(last->type == OR){
                        if(status){
                                status = pipe_exec(t);
                        }

                }else if(last->type == AND){
                        if(!status){
                                status = pipe_exec(t);
                        }

                }else{


                        status = pipe_exec(t);

                }
                
                        while((t->type == CONV)&&(t != NULL)){
                                t = t->next;
                        }
                        if((t != NULL)&&(t->backgrnd))
                                t = t->next;	
	}
	exit(0);
}*/

int pipe_exec(tree t){
	if(t == NULL){
		return 0;

	}
	tree c = t;
	pid_t pid, pgid;
	int fd[2], fd_temp, stat = 0, frk = 0;
	fd_temp = -1;
	if(t->backgrnd){
		
		if(getpgrp() != getpid()){

			setpgid(0, 0);
			frk++;

		}else if((pid = fork()) == 0){
				pipe_exec(t);
		}
			
			else{
				addzomb(pid);
				return 0;
			}
		

	}

	int check = 0;
	while((c != NULL)&&(check == 0)){
		if(c->type != CONV){
			check++;
		}
		if(c->argv && !strcmp(c->argv[0],"exit")){
			//printf("exit\n");
			clearlist();
			clear_tree(t_ex);
			exit(0);
			//longjmp(ebuf,1);
			
		}
		
		 if (c->argv && !strcmp(c->argv[0], "cd"))   
        	{	
			int argc = 0, k = 0;
			while(c->argv[k] != NULL){
				argc++;
				k++;
			}
            		if (fd_temp != -1)
                		close(fd_temp);
            		if ((c->next != NULL)&&(c->type == CONV))
            		{
                		if (pipe(fd) < 0)
                    			exit(1);
                		fd_temp = fd[0];
                		close(fd[1]);
            	}
            return cd(argc, c->argv);
        }
		if(c == t){
			if((c->next != NULL)&&(check == 0)){
				if(pipe(fd)<0)
					exit(1);
				if((pid = fork()) == 0){

					dup2(fd[1],1);
					close(fd[0]);
					close(fd[1]);
					proc(c,t->backgrnd,pgid);

				}else if(t->backgrnd)
					addzomb(pid);
				fd_temp = fd[0];
				close(fd[1]);
			}
			else if((pid = fork()) == 0)
				proc(c,t->backgrnd,pgid);
			else if(t->backgrnd)
				addzomb(pid);

		}else if((check == 1)||(c->next == NULL)){
			if((pid = fork()) == 0){
				dup2(fd_temp,0);
				close(fd_temp);
				proc(c,t->backgrnd,pgid);

			}
			else if(t->backgrnd)
				addzomb(pid);
			close(fd_temp);


		}else{
			if(pipe(fd) < 0)
				exit(1);
			if((pid = fork()) == 0){
				dup2(fd[1],1);
				dup2(fd_temp,0);
				close(fd[1]);
				close(fd[0]);
				close(fd_temp);
				proc(c,t->backgrnd,pgid);

			}else if(t->backgrnd)
				addzomb(pid);
			close(fd_temp);
			fd_temp = fd[0];

		}
		c = c->next;
	}
	if(!t->backgrnd || frk){
		waitpid(pid,&stat,0);
		while(wait(NULL) != -1);
	}
	if(frk){
		exit(stat);
	}
	return stat;
	
	
}


int cd(int argc, char **argv)      
{
	char *s;
	if (argc == 1)
    {
		s = getenv("HOME");
		if (s == NULL)
        {
            fprintf(stderr, "Home?\n");
            return 1;
        }
		else
            chdir(s);
	}
	else if (argc > 2)
    {
        fprintf(stderr, "too much arguments\n");
        return 2;
    }
    else if (chdir(argv[1]))
    {
        fprintf(stderr,"error in cd\n");
        return 3;
    }
    return 0;
}

void execute(tree t){
	if(t_ex == NULL)
		t_ex = t;	
	int status = 0;
	status = pipe_exec(t);
	if(t == NULL){
		return ;
	}
	tree last;
	/*if(t->backgrnd){
                        while((t->backgrnd)&&(t != NULL))
                                t = t->next;
			execute(t);

			return;
                }else{*/
                        while((t->type == CONV)){
                                t = t->next;
				if(t == NULL)
					break;
                        }
			last = t;	
                        if(t != NULL)
                                t = t->next;
                

	while(t != NULL){
		if(last->type == OR){
			if(status){
				status = pipe_exec(t);
			}

		}else if(last->type == AND){
			if(!status){
				status = pipe_exec(t);
			}

		}else{


			status = pipe_exec(t);

		}
		/*if(t->backgrnd){
			while((t->backgrnd)&&(t != NULL))
				t = t->next;
			execute(t);
			return;
		}else{*/
			while((t->type == CONV)){
				
				if(t == NULL)
                                        break;
				t = t->next;
                        }
		                

			
			if(t != NULL)
				t = t->next;
	}


}


void kill_zomb(){

	int status;
	while(z_list != NULL){

		waitpid(z_list->pidz,&status,0);
		printf("[%ld] done\n", (long)z_list->pidz);
		fflush(stdout);
		zomb_t* dop = z_list;
		z_list = z_list->next;
		free(dop);


	}

}
