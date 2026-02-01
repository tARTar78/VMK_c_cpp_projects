#include <stdio.h>
#include <setjmp.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "exec_art.h"

extern int c;
extern int cur;

int syntax = 0;

/* Special character counters: > < | & ; ( ) */
#define SPECIAL_CHAR_COUNT 7
char mas[SPECIAL_CHAR_COUNT] = {0,0,0,0,0,0,0};
extern char str[10];
extern char str1[10];
int k = 0;
int dop = 0;
extern char** lst;
extern char* buf;
extern int sizebuf;
extern int sizelist;
extern int curbuf;
extern int curlist;
int check_eof = 0;
int kav = 0;
int resh = 0;
int sl = 0;
char envp_art[BUFSIZ];
void start();
void word();
void greater();
void greater2();
void newline();
void stop();
void amp();
void stck();

void swap(char** s1, char** s2){
	char* s;
	s = *s1;
	*s1 = *s2;
	*s2 = s;
	
}



void sortlist(){
	if(lst == NULL) return;
	for(int i = 0; i < sizelist-2; ++i){
		for(int j = 0; j < sizelist-2-i;++j){
			if(strcmp(lst[j+1],lst[j]) < 0){
				swap(&lst[j],&lst[j+1]);
			}
		}
	}

}
void max_c(char res){
	if(res == '>'){
		mas[0]++;	
	}else if(res == '<'){
		mas[1]++;
	}else if(res == '|'){
		mas[2]++;
        }else if(res == '&'){
		mas[3]++;
        }else if(res == ';'){
		mas[4]++;
        }else if(res == '('){
		mas[5]++;
        }else if(res == ')'){
		mas[6]++;
        }

}
int getsym(){
	int res;
	if(k == 0){
		
		if(fscanf(stdin,"%10[^\n]%n",str,&dop) == EOF){
			return '\0';
		}

		if(dop < 10){
			str[dop] = '\n';
			fscanf(stdin, "%*c");
			//printf("{art}\n");
			//fflush(stdout);	
		}		
		dop = 0;
	}
	
	
	res = str[k];
	max_c(res);
	k = (k+1)%10;
	if((res == '\\') && (sl == 0)) {
		sl = 1;
		res = getsym();
	}else{
		sl = 0;
	}
	if(res == '\"'){
		kav = (kav + 1)%2;
	}
	if((kav == 0) && (res == '#') && (sl == 0)){
		addword();
		while((res = getsym()) != '\0'){}
		termlist();
                printf("\nsize of list == %d\n", sizelist - 1);
                printlist();
                stop();


	}
	return res;

}

int symset(int c){
	return (c!= '\n'&&
		c!=' ' &&
		c!='\t'&&
		c!='>' &&
		c!= '\0'&&
		c!=EOF&&
		c!= '('&&
                c!=')' &&
                c!='<'&&
                c!='|' &&
                c!= '&'&&
		c!=';');
}


void start() {
	if(((c==' ') && (kav == 0))|| c=='\t'){ c = getsym(); start();}
	else if((c == '\0')) {
		termlist();
		check_eof = 1;
		//printf("\nsize of list == %d\n", sizelist - 1);
		//printlist();
		stop();

	}else if(c == '\n'){
			termlist();
			stop();
				
	}else{
		char cprev=c;
		nullbuf();
		if(c != '\"')
			addsym();
		c=getsym();
		if (cprev == '>'){
			greater();
		}else if((cprev == ';')){
			addword();
			start();
		}else if((cprev == '<')){
                        addword();
                        start();
                }else if((cprev == '(')){
                        addword();
                        start();
                }else if((cprev == ')')){
                        addword();
                        start();
                }else if(cprev == '|'){
			stck();
		}else if(cprev == '&'){
			amp();
		}else{
			word();
		}


	}
	return;
	
}

void word(){
	if(symset(c) ||((c == ' ') && (kav == 1))){
		if(c != '\"')
			addsym();
		c=getsym();
		word();
	}else{
		addword();
		start();

	}


}

void greater(){
	if(c=='>'){
		addsym();
		c=getsym();
		greater2();
	}else{
		addword();
		start();
	}

}

void stck(){
        if(c=='|'){
                addsym();
                c=getsym();
                greater2();
        }else{
                addword();
                start();
        }

}
void amp(){
        if(c=='&'){
                addsym();
                c=getsym();
                greater2();
        }else{
                addword();
                start();
        }

}


void greater2(){
	addword();
	start();

}

void newline(){
	clearlist();
	start();

}
/*void max_cc(){
	int m = 0, m_i = 0;
	for(int i = 0; i < 7; ++i){
		if(mas[i] > m){
			m = mas[i];
			m_i = i;

		}

	}

	 if(m_i == 0){
                printf("\ndop_task_answer: >\n");
        }else if(m_i == 1){
                printf("\ndop_task_answer: <\n");
        }else if(m_i == 2){
                printf("\ndop_task_answer: |\n");
        }else if(m_i == 3){
                printf("\ndop_task_answer: &\n");
        }else if(m_i == 4){
                printf("\ndop_task_answer: ;\n");
        }else if(m_i == 5){
                printf("\ndop_task_answer: (\n");
        }else if(m_i == 6){
                printf("\ndop_task_answer: )\n");
        }



}
*/
void stop(){
	if(check_eof == 1){
                exit(0);

	}
	k = 0;
	if(check_lst() || kav == 1){
		if (kav == 1) {
			fprintf(stderr, "syntax error: unclosed quote\n");
		} else {
			fprintf(stderr, "syntax error: unmatched parentheses\n");
		}
		fflush(stderr);
		clearlist();
		return;
	}
        change_env(envp_art,"$SHELL");

	const char* home_env = getenv("HOME");
	if (home_env) {
		strncpy(envp_art, home_env, BUFSIZ - 1);
		envp_art[BUFSIZ - 1] = '\0';
		change_env(envp_art,"$HOME");
	}

	const char* login = getlogin();
	if (login) {
		strncpy(envp_art, login, BUFSIZ - 1);
		envp_art[BUFSIZ - 1] = '\0';
		change_env(envp_art,"$USER");
	}

	snprintf(envp_art,BUFSIZ,"%d",geteuid());
	change_env(envp_art,"$EUID");
	//printlist();
	cur = 0;
	tree t = build_tree(lst);
	//print_tree(t,2);
	execute(t);
	//print_zomb();
	//check_eof = setjmp(ebuf);
	kill_zomb();
	clear_tree(t);
	clearlist();
	//if(check_eof == 1)
                //exit(0);

	//fflush(stdout);
	//max_cc();
	//if(check_eof == 1)
		//exit(0);
}
void child_wait(int code)
{
    (void)code;  /* Signal number not used */
    wait(NULL);
}

int main(int argc, char* argv[], char* envp[]){
		(void)argc;
		(void)envp;
		setpgid(0,0);
		zomb_count = 0;
		signal(SIGINT, SIG_IGN);
                signal(SIGCHLD, child_wait);
                z_list = NULL;
	while(1!=2){
		char dop_s[256];
		getcwd(dop_s,256);
		snprintf(envp_art,BUFSIZ,"%s%s",dop_s,argv[0]+1);
		//strcpy(envp_art, getenv("PWD"));
		//strcpy(dop_s,argv[0] + 1);
		//strcat(envp_art,dop_s);
		printf("$art$ ");
		c = getsym();
		null_list();
		start();
	}
}
