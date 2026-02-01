#ifndef TREE_ART_H
#define TREE_ART_H

#define _POSIX_C_SOURCE 200809L

#include "list.h"
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <stdio.h>

/* Maximum number of arguments per command */
#define MAX_ARGS 256

/* Path buffer size */
#define PATH_BUF_SIZE 256

enum type_of_next{
    NXT, AND, OR, CONV   /* виды связей соседних команд в списке команд */
};

struct cmd_inf {
    char ** argv; // список из имени команды и аргументов
    char *infile; // переназначенный файл стандартного ввода
    char *outfile; // переназначенный файл стандартного вывода
    int append;
    int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме
    struct cmd_inf* psubcmd; // команды для запуска в дочернем shell
    //struct cmd_inf* pipe; // следующая команда после "|"
    struct cmd_inf* next; // следующая после ";" (или после "&")
    enum type_of_next type;// связь со следующей командой через ; или && или ||
};

typedef struct cmd_inf *tree;
typedef struct cmd_inf node;

void print_tree(tree t, int i);
tree build_tree(char** lst );
void clear_tree(tree t);

#endif /* TREE_ART_H */
