// brutal force guessing password
#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    if (argc != 2) {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    
    char* hash = argv[1];
    char salt[3];
    salt[0] = hash[0];
    salt[1] = hash[1];
    salt[2] = '\0';
    
    for (int i=0; i<strlen(alphabet); i++) {
        for (int j=0; j<strlen(alphabet); j++) {
            for (int k=0; k<strlen(alphabet); k++) {
                for (int l=0; l<strlen(alphabet); l++) {
                    for (int m=0; m<strlen(alphabet); m++) {
                        char five_d_password[6];
                        five_d_password[0] = alphabet[i];
                        five_d_password[1] = alphabet[j];
                        five_d_password[2] = alphabet[k];
                        five_d_password[3] = alphabet[l];
                        five_d_password[4] = alphabet[m];
                        five_d_password[5] = '\0';
                        
                        // hash password

                        // loop through every subpassword of generated password
                        for (int length=1; length<6; length++) {
                            // move it along the generated password
                            for (int offset=0; offset+length<6; offset++) {
                                char test[length+1]; // add one for null char
                                strncpy(test, five_d_password+offset, length);
                                test[length] = '\0';
                                
                                // char* test = "ROFL";
                                printf("hash: %s, guess pw: %s, guess hash: %s\n", hash, test, crypt(test, salt));
                                if (strcmp(hash, crypt(test, salt)) == 0) {    
                                    printf("%s\n", test);
                                    return 0;
                                }
                            }
                        }



                    }
                }
            }
        }
    }
    
    printf("No password found\n");
}