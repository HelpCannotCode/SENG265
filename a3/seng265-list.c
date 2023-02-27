/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"


node_t *new_node(char *text, char *key) {
    assert( text != NULL);
    assert( key != NULL);

    node_t *temp = (node_t *)emalloc(sizeof(node_t));

    strncpy(temp->text, text, LIST_MAXLEN_TEXT);
    strncpy(temp->key, key, LIST_LEN_KEYWORD);
    temp->next = NULL;

    return temp;
}


node_t *add_front(node_t *list, node_t *new) {
    new->next = list;
    return new;
}


node_t *add_end(node_t *list, node_t *new) {
    node_t *curr;

    if (list == NULL) {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return list;
}

node_t *add_lists(node_t *list, node_t *new) {
    node_t *curr;

    if (list == NULL) {
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    return list;
}


node_t *peek_front(node_t *list) {
    return list;
}


node_t *remove_front(node_t *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}

/*inserts at the lexograpically correct spot
*/
node_t *insort(node_t *list, node_t *insert){

    node_t *curr;
    node_t *next_node;
    //if list empty
    if(list == NULL){
        insert -> next = NULL;
        return insert;
    }
    // for list size 1
    if(list->next == NULL){
        if(0 > strncmp(list->key, insert->key, sizeof(curr->key))){
            list->next = insert;
            insert->next = NULL;
            return list;
        }
        else{
            insert->next = list;
            return insert;
        }
    }

    //if list bigger than 1
    curr = list;
    next_node = curr->next;
    if(0 < strncmp(curr->key, insert->key,sizeof(curr->key)))
    {
        insert->next = curr;
        return insert;
    }
    while(next_node->next != NULL){
        if(0 < strncmp(next_node->key, insert->key, sizeof(curr->key))){
            curr->next = insert;
            insert->next = next_node;
            return list;
        }
        else{
            curr = curr -> next;
            next_node = curr->next;
        } 
    }
    if(0 < strncmp(next_node->key, insert->key, sizeof(curr->key))){
            curr->next = insert;
            insert->next = next_node;
            return list;
    }
    else{
        next_node->next = insert;
        insert -> next = NULL; 
    }
    
    return list;
    

}

// if not in list return 0, if in list returns index
int in_list(node_t *list, char *word){
    node_t *curr = NULL;
    char list_word[LIST_LEN_KEYWORD];
     if (list == NULL) {
        return 0;
    }
    
    curr = list;
    while(curr != NULL){
        //printf("text%s - -exxword", curr->text);
        //printf("%s\n", word);
        if(0 == strncmp(word, curr->text, strlen(word))){
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void clean(node_t *list){
    node_t *temp_node = NULL;

    temp_node = list;
    while (temp_node != NULL) {
        assert(temp_node != NULL);
        list = remove_front(list);
        free(temp_node);
        temp_node = list;
    }
   
    assert(list == NULL); 
}

void apply(node_t *list,
           void (*fn)(node_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}
