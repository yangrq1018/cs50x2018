#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    do
    {
        height= get_int("height: ");
    } while (height < 0 || height > 23);
    
    for (int i=0; i<height; i++) {
        for (int j=0; j<height-i-1; j++) {
            printf(" "); // offset
        }
        for (int j=0; j<i+2; j++) {
            printf("#"); // i+2 blocks
        }
        printf("\n");
    }
}