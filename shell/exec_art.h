#include <stdio.h>
#include "tree_art.h"
#include "list.h"
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#ifndef EXEC_ART_H
#define EXEC_ART_H
jmp_buf ebuf;
typedef struct zombie_str{

	pid_t pidz;
	struct zombie_str* next;

}zomb_t;

zomb_t* z_list;
int zomb_count;

void execute(tree t);
void print_zomb();
void kill_zomb();

#endif
