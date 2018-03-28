// Implements a dictionary's functionality
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

#define HASH_BIN_NUM 1000000

// define the node in linked list
typedef struct node {
    char word[LENGTH+1];
    struct node* next;
} node;


unsigned long
hash(const char *str)
{
    unsigned long hash = 5381;

    for (int i=0, n=strlen(str); i<n; i++) {
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    }    

    return hash % HASH_BIN_NUM;
}
// int hash(const char* word) { // use constant to make sure the word will not be altered
//     int hash = 0;
//     int n;
//     for (int i = 0; word[i] != '\0'; i++)
//     {
//         // alphabet case
//         if(isalpha(word[i]))
//             n = word [i] - 'a' + 1;
        
//         // comma case
//         else
//             n = 27;
            
//         hash = ((hash << 3) + n) % HASH_BIN_NUM;
//     }
//     return hash;
// }

// the hash table
// each bin keeps a reference to the head of a linked list
node* hash_table[HASH_BIN_NUM];

// Returns true if word is in dictionary else false
bool check(const char *word)
{ 
    char test_word[strlen(word)+1];
    strcpy(test_word, word);

    // lower the world
    for (int i=0; i<strlen(test_word); i++) {
        test_word[i] = tolower(test_word[i]);
    }

    int hash_value = hash(test_word);

    node* cursor;
    if (hash_table[hash_value]) {
        cursor = hash_table[hash_value];
        while (cursor) {
            if (strcmp(cursor->word, test_word) == 0) {
                return true;
            }
            cursor = cursor->next;
        }
        return false;
    } else {
    return false;
    }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // initialize hash_table to nulls
    for (int i=0; i<HASH_BIN_NUM; i++) {
        hash_table[i] = NULL;
    }
    
    // try to open the dictionary file
    FILE* dict_handler = fopen(dictionary, "r");
    if (!dict_handler) {
        printf("Cannot open %s\n", dictionary);
        return false;
    }

    // temporary storage for the word per line parsed out
    // need tor reserver +2, one for \0 and one for \n, because fgets preserves the newline char
    char word[LENGTH+2];

    // parse the dictionary line by line
    // fgets return NULL when EOF is reached or an error occurs
    while (fgets(word, LENGTH+2, dict_handler) != NULL) {
        // remove newline from word
        for (int i=0; i<strlen(word); i++) {
            if (word[i] == '\n')
                word[i] = '\0';
        }
        
        // hash the word first
        int hash_index = hash(word);

        // make a new new
        node* temp = malloc(sizeof(node));
        // copy string from word into the node
        strcpy(temp->word, word);
        temp->next = NULL;

        // append the node into the hash table
        if (hash_table[hash_index]) {
            node* cursor = hash_table[hash_index];
            while (cursor->next != NULL) {
                cursor = cursor->next;
            }
            // let the last node remember temp
            cursor->next = temp;
        } else {
            hash_table[hash_index] = temp;
        }
    }

    if (!feof(dict_handler)) {
        printf("Dictionary loading has IO Error, abort\n");
        return false;
    }

    return true;
    
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    unsigned int count = 0;
    node* cursor;
    for (int i=0; i<HASH_BIN_NUM; i++) {
        if (hash_table[i]) {
            cursor = hash_table[i];
            while (cursor) {
                count++;
                cursor = cursor->next;
            }
        }
    }

    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i=0; i<HASH_BIN_NUM; i++) {
        if (hash_table[i]) {
            node* cursor = hash_table[i];
            node* next;
            while (cursor) {
                // remember the next node so we don't lose the rest of the list
                next = cursor->next;
                // free the node under cusor
                free(cursor);
                cursor = next;
            }
        }
    }
    return true;
}