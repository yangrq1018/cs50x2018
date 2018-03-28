#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// define eight bit integer as a BYTE
typedef uint8_t BYTE;

void write_buffer(BYTE* buffer, FILE* outptr) {
    int outcome = fwrite(buffer, 512, 1, outptr);
    if (outcome != 1) {
        fprintf(stderr, "Cannot write to out file.");
        exit(3);
    }
}

void get_filename(char* filename, int* recoveredCnt) {
    sprintf(filename, "%03i.jpg", *recoveredCnt);
    (*recoveredCnt)++;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./recover filename-of-forensic-image");
        return 1;
    }

    // byte array to store a block of 512 bytes memory read from card
    BYTE buffer[512];

    // try to open the image
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL) {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 2;
    }
    
    int recoveredCnt = 0;
    char out_filename[9];
    FILE* outptr = NULL;

    while (fread(buffer, 512, 1, inptr) == 1) {
        // check if the head of the buffer indicates the start of a new jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3]>>4) == 0x0e) {
            // close last file if any
            if (outptr) {
                fclose(outptr);
            }

            // grab a new file
            get_filename(out_filename, &recoveredCnt);
            outptr = fopen(out_filename, "w");
                
            // write to new file
            write_buffer(buffer, outptr);
 
        } else {
            // are we already writing a jpg?
            if (outptr) {
                // simply write those 512 bytes into outptr
                write_buffer(buffer, outptr);
            }
        }
    }

    // close the last file if any, fclose does not handle nul arguments
    if (!outptr) {
        fclose(outptr);
    }

    fclose(inptr);
}
