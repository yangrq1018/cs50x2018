#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./caesar.c k\n");
        return 1;
    }
    
    // get plaintext
    int key = atoi(argv[1]);
    string s = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i=0; i<strlen(s); i++) {
        if (isalpha(s[i])) {
            if (isupper(s[i])) {
                printf("%c", ((s[i] - 'A') + key) % 26 + 'A');
            } else {
                printf("%c", ((s[i] - 'a') + key) % 26 + 'a');
            }
        } else {
            printf("%c", s[i]);
        }
    }
    printf("\n");
}