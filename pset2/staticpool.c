#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>

int main(void) {
    char* salt = "sa";
    char* key = "password";
    char* hash1 = crypt(key, salt);
    
    // get a new key
    char* another_key = "another key";
    char* hash2 = crypt(another_key, salt);
    
    // print both pointer out
    printf("%s, %s\n", hash1, hash2);
    /*
    output: saegbqMiMr4O6, saegbqMiMr4O6
    it turns out both hashed passwords are the same, while they should not.
    this is caused by crypt returns a char array pointer to an array stored in static area, which
    is rewritten every time crypt is called.
    To preserve the password hash generated for later use, use strcpy to copy it out.
    */
}