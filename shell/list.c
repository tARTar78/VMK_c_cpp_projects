#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int c;
extern int syntax;
char str[INPUT_CHUNK_SIZE];
char str1[INPUT_CHUNK_SIZE];
char** lst;
char* buf;
int sizebuf;
int sizelist;
int curbuf;
int curlist;
extern char* home;
extern char* path_home;

/* Helper function to handle allocation failure */
static void alloc_error(const char* func_name) {
    fprintf(stderr, "Memory allocation failed in %s\n", func_name);
    exit(1);
}
void clearlist(){
        sizelist = 0;
        curlist = 0;
        if (lst == NULL) return;
        for(int i = 0; lst[i] != NULL; ++i) {
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
        if(curlist > sizelist-1) {
                char** tmp = realloc(lst, (curlist+1) * sizeof(*lst));
                if (!tmp) alloc_error("termlist");
                lst = tmp;
                sizelist = curlist + 1;
        }

        lst[curlist] = NULL;
        char** tmp = realloc(lst, (curlist+1) * sizeof(*lst));
        if (!tmp) alloc_error("termlist");
        lst = tmp;
        sizelist = curlist + 1;
}

void nullbuf(){
        buf = NULL;
        sizebuf = 0;
        curbuf = 0;

}
void addsym(){
        if(curbuf > sizebuf-1) {
                char* tmp = realloc(buf, sizebuf + INPUT_CHUNK_SIZE);
                if (!tmp) alloc_error("addsym");
                buf = tmp;
                sizebuf += INPUT_CHUNK_SIZE;
        }

        buf[curbuf++] = c;
}

void addword(){
        if (curbuf > sizebuf-1) {
                char* tmp = realloc(buf, sizebuf + 1);
                if (!tmp) alloc_error("addword");
                buf = tmp;
                sizebuf += 1;
        }

        buf[curbuf++] = '\0';
        char* tmp = realloc(buf, curbuf);
        if (!tmp) alloc_error("addword");
        buf = tmp;
        sizebuf = curbuf;

        if(curlist > sizelist-1) {
                char** tmp_lst = realloc(lst, (sizelist + INITIAL_LIST_SIZE) * sizeof(*lst));
                if (!tmp_lst) alloc_error("addword");
                lst = tmp_lst;
                sizelist += INITIAL_LIST_SIZE;
        }
        lst[curlist++] = buf;
}

void printlist(){
        if(lst == NULL) return;

        for(int i = 0; i < sizelist-1; i++) {
                printf("%s\n", lst[i]);
        }
}
int check_lst(){
        int sc1 = 0, sc2 = 0, kav1 = 0;
        if(lst == NULL) return 0;

        for(int i = 0; i < sizelist-1; i++) {
                if(strcmp(lst[i], "(") == 0) {
                        sc1++;
                }
                if(strcmp(lst[i], ")") == 0) {
                        sc2++;
                }
                if(strcmp(lst[i], "\"") == 0) {
                        kav1++;
                }
                if(sc1 < sc2) {
                        return 1;
                }
        }
        if((sc1 != sc2) || (kav1 % 2 == 1)) {
                return 1;
        }
        return 0;
}
void change_env(char* str1, char* str2)
{
        if(lst == NULL) return;

        for(int i = 0; i < sizelist-1; i++) {
                if(strcmp(lst[i], str2) == 0) {
                        char* tmp = realloc(lst[i], strlen(str1) + 1);
                        if (!tmp) alloc_error("change_env");
                        lst[i] = tmp;
                        strcpy(lst[i], str1);
                }
        }
}


