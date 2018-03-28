#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    
    string key_string = argv[1];
    for (int i=0; i<strlen(key); i++) {
        if (!isalpha(key[i])) {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }
    
    string s = get_string("plaintext: ");
    printf("ciphertext: ");
    int key_index = 0;
    
    for (int i=0; i<strlen(s); i++) {
        if (isalpha(s[i])) {
            int key_delta = tolower(key[key_index % strlen(key)]) - 'a'
            
            if (isupper(s[i])) {
                printf("%c", ((s[i] - 'A' + key_delta) % 26 + 'A'));
                key_index++;
            } else {
                printf("%c", ((s[i] - 'a' + key_delta) % 26 + 'a'));
                key_index++;
            }
        } else {
            printf("%c", s[i]);
        }
    }
    printf("\n");
    return 0;
}