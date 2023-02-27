/*
 * concord3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Fall 2022.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"

#define MAX_LEN_KEYWORD 41
#define MAX_LEN_LINE 101

void print_word(node_t *node, void *arg)
{
    char *format = (char *)arg;
    printf(format, node->text);
}

/*
 * HI TA or zastre
 */

void get_input(node_t **lines_head, node_t **exclusion_head){
    node_t *head = NULL;
    node_t *no_head = NULL;

    char empty[] = {""};
    char start[] = {"''''\n"};
    char mid[] = {"\"\"\"\"\n"};
    char line[MAX_LEN_LINE];

    int in_exclusion = 0;
    int in_lines_to_index = 0; 

    //FILE *yes = fopen("tests/in03.txt", "r");

    if (fgets(line, sizeof(line),stdin)!= NULL){
        if(atoi(line) == 1 ){
            printf("Input is version 1, concord3 expected version 2");
            return;
        }
    }

    while( fgets(line, sizeof(line),stdin) != NULL)
    {   
        if(0 == strncmp(start,line, sizeof(line))){
            in_exclusion = 1;
            continue;
        }
        if(0 == strncmp(mid,line, sizeof(line))){
            in_exclusion = 0;
            in_lines_to_index = 1;
            continue;
        }
        if(in_exclusion == 1){
            int location = strcspn(line,"\n"); 
            line[location] ='\0'; 
            head = add_end(head,new_node(line,empty));
        }
        if(in_lines_to_index == 1){
            int location = strcspn(line,"\n"); 
            line[location] ='\0'; 
            no_head = add_end(no_head,new_node(line,empty));
        }
    }
    *lines_head = no_head;
    *exclusion_head = head;
}

//uppercases a full string
void upper(char word[MAX_LEN_KEYWORD])
{
    for(int i = 0; word[i]!= '\0';i++)
    {
        word[i] = toupper(word[i]);
    }
}

//lowercases a full string
void lower(char word[MAX_LEN_KEYWORD])
{
    for(int i = 0; word[i]!= '\0';i++)
    {
        word[i] = tolower(word[i]);
    }
}

// make a string containing formated text and keyword and return it
void create_format_line(node_t *tokenized_line, node_t *word_in_line, char *return_sentence){
    node_t *curr = tokenized_line;
    int size_before = 0; 
    int size_space = 1;
    int total_size = 0;
    char space = ' ';
    char sentence[MAX_LEN_LINE];
    char indexed_word[MAX_LEN_KEYWORD];

    strncpy(indexed_word ,word_in_line->text, sizeof(word_in_line->text));
    upper(indexed_word);

    // get char count before the indexed word
    while(0 != strncmp(curr->text, word_in_line->text, sizeof(word_in_line->text))){
        size_before += strlen(curr->text);
        size_before += size_space;  
        curr = curr-> next;
    }
    // skip words until char count is under 20 (the cut off for front)
    curr = tokenized_line;
    while (size_before > 20){
        size_before -= strlen(curr->text);
        size_before -= size_space;
        curr = curr->next;
    }

    int spaces_before  = 9 + (20 - size_before);// add spaces before sentence
    for(int i = 0 ; i < spaces_before; i++){
        sentence[i] = space;
    }

    int index = spaces_before;
    total_size = total_size + spaces_before;
    
    while(curr != NULL){// keep adding words until we can't no more
        total_size = total_size + strlen(curr->text);
        total_size = total_size + size_space;   
        if(total_size > 61){//wouldn't work in while loop for some reason?
            break;//makes sure the sentence is not larger than the column lim
        }
        if(0 != strncmp(curr->text, word_in_line->text, strlen(word_in_line->text))){
            for(int i = 0; i<strlen(curr->text); i++)
                {
                sentence[index] = curr->text[i];
                index++;
                }  
        }
        else{
            for(int i = 0; i<strlen(curr->text); i++)
                {
                sentence[index] = indexed_word[i];
                index++;
                }  
        } 
        sentence[index] = space;
        index += size_space;
        curr = curr->next;
    }
    sentence[index-1] = '\0';
    strncpy(return_sentence, sentence, strlen(sentence));

}


//takes in sentence to index and exclusion words returns a list of 
//sentences that have been formatted
void format_lines(node_t *line, node_t *exclusion_head, node_t **final_head){
    node_t *formatted_sentences = NULL;
    node_t *tokenized_sentence = NULL;
    node_t *curr = NULL;
    node_t *temp = NULL;
    char *token; 
    char empty[] = {""};
    char temp_sent[MAX_LEN_LINE];
    char formatted_line[MAX_LEN_LINE];
    char keyword[MAX_LEN_KEYWORD];
    char compared_token[MAX_LEN_KEYWORD];

    strncpy(temp_sent, line->text, sizeof(line->text));
    
// tokenizes the sentence
    token = strtok(temp_sent," ");
    while(token != NULL){
        temp = new_node(token, empty);
        tokenized_sentence = add_end(tokenized_sentence, temp);
        token = strtok(NULL, " ");
    }

// go through the tokenized sentence and make the sentence
    curr = tokenized_sentence;
    while(curr != NULL){
        strncpy(compared_token,curr->text,sizeof(curr->text));
        lower(compared_token);
        if(0 != in_list(exclusion_head,curr->text) || 0 != in_list(exclusion_head,compared_token))
        {   
            // do nothing if either the og word or the lowered word is in the 
            // exclusion list
        }
        else{
            memset(formatted_line,0,sizeof(formatted_line));//clear formatted line
            memset(keyword,0,sizeof(keyword));//clear keyword
            create_format_line(tokenized_sentence, curr, formatted_line);
            strncpy(keyword, curr->text, strlen(curr->text));
            upper(keyword);// get copy of capitalized keyword

            temp = new_node(formatted_line,keyword);
            formatted_sentences = add_end(formatted_sentences,temp);
            
        }
        curr = curr->next;
    }
    *final_head = formatted_sentences;
    clean(tokenized_sentence);
    
}

// makes the list of indexed sentences sorts it then returns it to main
void make_list(node_t *lines_head, node_t *exclusion_head, node_t **final_head){
    node_t *lines_to_add = NULL;
    node_t *curr = lines_head;
    node_t *curr2 = NULL;
    node_t *temp_node = NULL;
    node_t *to_insert = NULL;
    node_t *temp_final = NULL;


    // go through the list of lines and add the formatted lines to a unsorted linked list
    while(curr->next != NULL){
        format_lines(curr, exclusion_head, &temp_node);
        lines_to_add = add_lists(lines_to_add,temp_node);    
        curr = curr->next;
    }
    format_lines(curr, exclusion_head, &temp_node);
    lines_to_add = add_lists(lines_to_add, temp_node);

    //use insort to insert sort the linked list
    curr = lines_to_add;
    while (curr->next != NULL) {
        to_insert = curr;
        curr = curr->next; 
        temp_final = insort(temp_final, to_insert);      
    }
    temp_final = insort(temp_final,curr);

    *final_head = temp_final;
} 



int main(int argc, char *argv[])
{
    char *exception_file = NULL;
    char *input_file = NULL;
    int i;  

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i+i < argc) {
            exception_file = argv[++i];
        } else {
            input_file = argv[i];
        }
    }

    node_t *lines_head = NULL;
    node_t *exclusion_head = NULL;
    node_t *final_head = NULL;

    get_input(&lines_head, &exclusion_head);
    make_list(lines_head, exclusion_head, &final_head);

    apply(final_head, print_word, "%s\n");
    //apply(lines_head, print_word, "--> %s\n");
    //apply(exclusion_head, print_word, "--> %s\n");

    clean(lines_head);
    clean(exclusion_head);
    clean(final_head);  


    exit(0);
}
