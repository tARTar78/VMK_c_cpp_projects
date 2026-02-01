#define _POSIX_C_SOURCE 200809L
#include "tree_art.h"
#include "list.h"

extern char** lst;
extern int syntax;
int cur = 0;

/* Forward declarations */
tree make_cmd(void);  /* создает дерево из одного элемента, обнуляет все поля */
void make_bgrnd(tree t); /* устанавливает поле backgrnd=1 во всех командах конвейера t */
void add_arg(tree t, int i);

/* Helper function to handle allocation failure */
static void tree_alloc_error(const char* func_name) {
    fprintf(stderr, "Memory allocation failed in %s\n", func_name);
    exit(1);
}


void clear_tree(tree t){
	if( t == NULL){

	}else{
		int c =0;
		if(t->argv != NULL){
			while(t->argv[c] != NULL){
				free(t->argv[c]);
				c++;
			}
			free(t->argv);

		}
		free(t->infile);
		free(t->outfile);
		clear_tree(t->psubcmd);
		//clear_tree(t->pipe);
		clear_tree(t->next);
		free(t);
	}



}


void make_shift(int n){
    while(n--)
        putc(' ', stderr);
}

void print_argv(char **p, int shift){
    char **q=p;
    if(p!=NULL){
        while(*p!=NULL){
             make_shift(shift);
             fprintf(stderr, "argv[%d]=%s\n",(int) (p-q), *p);
             p++;
        }
    }
}

void print_tree(tree t, int shift){
    char **p;
    if(t==NULL)
        return;
    p=t->argv;
    if(p!=NULL)
        print_argv(p, shift);
    else{
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }
    make_shift(shift);
    if(t->infile==NULL)
        fprintf(stderr, "infile=NULL\n");
    else
        fprintf(stderr, "infile=%s\n", t->infile);
    make_shift(shift);
    if(t->outfile==NULL)
        fprintf(stderr, "outfile=NULL\n");
    else
        fprintf(stderr, "outfile=%s\n", t->outfile);
    make_shift(shift);
    fprintf(stderr, "append=%d\n", t->append);
    make_shift(shift);
    fprintf(stderr, "background=%d\n", t->backgrnd);
    make_shift(shift);
    fprintf(stderr, "type=%s\n", t->type==NXT?"NXT": t->type==OR?"OR":t->type==AND?"AND":"CONV" );
    make_shift(shift);
    if(t->psubcmd==NULL)
        fprintf(stderr, "psubcmd=NULL \n");
    else{
        fprintf(stderr, "psubcmd---> \n");
        print_tree(t->psubcmd, shift+5);
    }
    make_shift(shift);
    /*if(t->pipe==NULL)
        fprintf(stderr, "pipe=NULL \n");
    else{
        fprintf(stderr, "pipe---> \n");
        print_tree(t->pipe, shift+5);
    }*/
    make_shift(shift);
    if(t->next==NULL)
        fprintf(stderr, "next=NULL \n");
    else{
        fprintf(stderr, "next---> \n");
        print_tree(t->next, shift+5);
    }
}

tree build_tree(char** lst){
	typedef enum {begin, conv, conv1, in, in1, out, out1, backgrnd, sc ,end } graf;
	graf G = begin;
	int ch = 1;
	if( lst == NULL){
		return NULL;
	}
	//printf("%s\n",lst[i]);
	tree s, c, p, f_conv;
	for(;; ++cur){
		switch(G){
			case begin:
				//printf("{art}b %s \n", lst[i]);
        			//fflush(stdout);

				if(lst[cur] != NULL){
					if(lst[cur][0] == '('){
						s = c = make_cmd();
                                                p = c;
						G = sc;
						f_conv = c;
						ch++;
					}else{
						s = c = make_cmd();
						add_arg(c,cur);
						p = c;
						G = conv;
						f_conv = c;
						ch++;
					}		
				}else{
					G = end;
				}
				continue;
			case conv:

				 //printf("{art}c %s \n", lst[i]);
                                //fflush(stdout);

				if(ch == 1){
					f_conv = c;
				}

				if(lst[cur] == NULL){
                                        G = end;
                                         //printf("{art}\n");
                                       // fflush(stdout);

                           
				}else if(strcmp(lst[cur] ,"&") == 0){
					G = backgrnd;
					ch = 1;
					c->type = NXT;
				}else if(lst[cur][0] == ';'){
					G = backgrnd;
                                       	ch = 1;
					c->type = NXT;
				}else if(strcmp(lst[cur],"&&") == 0){
					G = backgrnd;
                                       
					c->type = AND;
				}else if(strcmp(lst[cur], "||") == 0){
					G = backgrnd;
                                       
					c->type = OR;
				}else if(lst[cur][0] == '>'){
					G = out;
					if(strcmp(lst[cur],">>") == 0)
						c->append = 1;
				}else if(lst[cur][0] == '<'){
					G = in;
				}else if(strcmp(lst[cur],"|") == 0){
					G = conv1;
					//printf("art\n");
					c->type = CONV;
				}else if(lst[cur][0] == ')'){
					//printf("%s\n",lst[i]);
					G = end;
					cur--;
					
				}else{
					//printf("ddd");
					add_arg(c,cur);
				}
				continue;
			case conv1:
				 //printf("{art}c1 %s \n", lst[i]);
                                //fflush(stdout);
				if(strcmp(lst[cur],"(") == 0){
                                                c = make_cmd();
                                                G = sc;
						p->next = c;
						p = c;
				}else{
					ch++;
					c = make_cmd();
					add_arg(c,cur);
					p->next = c;
					p = c;
					G = conv;
					//printf("%s\n",lst[i]);
				}
				continue;
			case sc:
				//printf("%s\n",lst[i]);
				//print_tree(s,2);
				c->psubcmd = build_tree(lst);
					
				//printf("%s\n",lst[i]);
				G = conv;
				continue;
			case in:
				c->infile = strdup(lst[cur]);
				if (!c->infile) tree_alloc_error("build_tree:in");
				G = in1;
				cur--;
				continue;
			case in1:
				G = conv;
				continue;
			case out:
				c->outfile = strdup(lst[cur]);
				if (!c->outfile) tree_alloc_error("build_tree:out");
				G = out1;
				cur--;
				continue;
			case out1:
				G = conv;
				continue;
			case backgrnd:
				//printf("art\n");
				fflush(stdout);
				if(lst[cur] == NULL){
					if(strcmp(lst[cur-1],"&") == 0) 
						make_bgrnd(f_conv);
					G = end;
				}else{
					if(strcmp(lst[cur-1],"&") == 0)
						make_bgrnd(f_conv);
					if(lst[cur][0] == '('){
                                                c = make_cmd();
						p->next = c;
                                                p = c;
                                                G = sc;
					}else{
						c = make_cmd();
                                		add_arg(c,cur);
                                		p->next = c;
                                		p = c;
                                		G = conv;
					}


				}	

				continue;
			case end:
				c->type = NXT;
				//printf("art\n");
				return s;



		}

	}

}
void make_bgrnd(tree t)
{
	tree q = t;
	while(q != NULL){
		q->backgrnd = 1;
		//q = q->pipe;
		if(q->type == NXT){
			return;
		}
		q = q->next;
		
	}

}

tree make_cmd(void){
	tree res = malloc(sizeof(struct cmd_inf));
	if (!res) tree_alloc_error("make_cmd");

	res->argv = NULL;
	res->backgrnd = 0;
	res->infile = NULL;
	res->outfile = NULL;
	res->append = 0;
	res->psubcmd = NULL;
	res->next = NULL;
	return res;
}

void add_arg(tree t, int i){
	if(t->argv == NULL){
		t->argv = calloc(MAX_ARGS, sizeof(char*));
		if (!t->argv) tree_alloc_error("add_arg:calloc");
		t->argv[0] = strdup(lst[i]);
		if (!t->argv[0]) tree_alloc_error("add_arg:strdup");
	}else{
		int c = 0;
		while(t->argv[c] != NULL){
			c++;
		}
		if (c >= MAX_ARGS - 1) {
			fprintf(stderr, "Too many arguments (max %d)\n", MAX_ARGS - 1);
			return;
		}
		t->argv[c] = strdup(lst[i]);
		if (!t->argv[c]) tree_alloc_error("add_arg:strdup");
	}
}

