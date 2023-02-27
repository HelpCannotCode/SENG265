#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define LIST_MAXLEN_TEXT 101
#define LIST_LEN_KEYWORD 41

typedef struct node_t {
    char text[LIST_MAXLEN_TEXT];
    char key[LIST_LEN_KEYWORD];
    struct node_t  *next;
} node_t;

node_t *new_node(char *, char *);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
node_t *add_lists(node_t *, node_t *);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
node_t *insort(node_t *, node_t *);// inserts node in lexic order according to key
int     in_list(node_t *, char*); // returns 1 if in list 0 if not
void    clean(node_t*);//frees the mem of list
void    apply(node_t *, void(*fn)(node_t *, void *), void *arg);

#endif
