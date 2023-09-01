#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "struct3.h"
#define SIZE 16
int c;
extern int syntax;
char str[10];
char str1[10];
char** lst;
char* buf;
int sizebuf;
int sizelist;
int curbuf;
int curlist;
extern char* home;
extern char* path_home;
void clearlist(){

	int i;
        sizelist=0;
        curlist=0;
        if (lst == NULL) return;
        for(int i = 0; lst[i] != NULL; ++i){
		//printf("%s\n",lst[i]);
	
                free(lst[i]);
	
	}

        free(lst);
        lst = NULL;


}

void null_list(){
        sizelist = 0;
        curlist =0;
        lst=NULL;
}

void termlist(){
        if(lst==NULL) return;
        if(curlist > sizelist-1)
                lst=realloc(lst,(sizelist=curlist+1)*sizeof(*lst));

        lst[curlist] = NULL;
        lst = realloc(lst,(sizelist=curlist+1)*sizeof(*lst));
}

void nullbuf(){
        buf = NULL;
        sizebuf = 0;
        curbuf = 0;

}
void addsym(){
        if(curbuf>sizebuf-1)
                buf = realloc(buf, sizebuf+=SIZE);

        buf[curbuf++]=c;

}

void addword(){
        if (curbuf > sizebuf-1)
                buf = realloc(buf, sizebuf+=1);

        buf[curbuf++]='\0';
        buf=realloc(buf,sizebuf=curbuf);

        if(curlist>sizelist-1)
                lst=realloc(lst, (sizelist+=SIZE)*sizeof(*lst));
        lst[curlist++]=buf;

}

void printlist(){
        int i;
        if(lst == NULL) return;

        for(int i=0; i < sizelist-1;i++)
                printf("%s\n", lst[i]);
}
int check_lst(){
	int sc1 = 0, sc2 = 0, kav1 = 0, kav2 = 0;
	int i;
        if(lst == NULL) return 0;

        for(int i=0; i < sizelist-1;i++){
                if(strcmp(lst[i],"(") == 0){
                       sc1++;
                }
		if(strcmp(lst[i],")") == 0){
                        sc2++;
                }
		if(strcmp(lst[i],"\"") == 0){
                        kav1++;
                }
		if((sc1 < sc2 )){
			return 1;
		}	
	}
	if((sc1 != sc2) || (kav1 % 2 == 1)){
		return 1;
	}
	return 0;



}
void change_env(char* str1,char* str2)
{
	int i;
        if(lst == NULL) return;

        for(int i=0; i < sizelist-1;i++)
		if(strcmp(lst[i],str2 ) == 0){
			lst[i] = realloc(lst[i],sizeof(char)*(strlen(str1) + 1));
			strcpy(lst[i], str1);	
		}
	

}


