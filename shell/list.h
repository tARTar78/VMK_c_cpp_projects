#ifndef LIST_H
#define LIST_H

/* Buffer and list size constants */
#define INITIAL_BUF_SIZE 64
#define INITIAL_LIST_SIZE 32
#define INPUT_CHUNK_SIZE 16

/* Function declarations */
void clearlist(void);
void null_list(void);
void termlist(void);
void nullbuf(void);
void addsym(void);
void addword(void);
void printlist(void);
void change_env(char* str1, char* str2);
int check_lst(void);

#endif /* LIST_H */

