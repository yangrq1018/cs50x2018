#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// define eight bit integer as a BYTE
typedef u_int8_t BYTE;

int write_buffer(BYTE* buffer, FILE* outptr) {
    return fwrite(buffer, 512, 1, outptr);
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

    bool onJPG = false;
    int recoveredCnt = 0;
    char out_filename[9];
    FILE* outptr = NULL;

    while (fread(buffer, 512, 1, inptr) == 1) {
        // check if the head of the buffer indicates the start of a new jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3]>>4) == 0x0e) {
            // are we alreading writting a jpg?
            if (!onJPG) {
                onJPG = true;
                // filename: 00x.jpg, size: 8 bytes, sprintf auto append a null char to the end of the string
                get_filename(out_filename, &recoveredCnt);
                outptr = fopen(out_filename, "w");

                if (write_buffer(buffer, outptr) != 1) {
                    fprintf(stderr, "Cannot write to file %s", out_filename);
                    return 3;
                }

            } else {
                // close the previous file
                fclose(outptr);

                // grab a new file
                get_filename(out_filename, &recoveredCnt);
                outptr = fopen(out_filename, "w");a
                
                // write to new file
                if (write_buffer(buffer, outptr) != 1) {
                    fprintf(stderr, "Cannot write to file %s", out_filename);
                    return 3;
                }
            }
        } else {
            // are we already writing a jpg?
            if (onJPG) {
                // simply write those 512 bytes into outptr
                if (write_buffer(buffer, outptr) != 1) {
                    fprintf(stderr, "Cannot write to file %s", out_filename);
                    return 3;
                }
            }
        }
    }

    // close the last file if any, fclose does not handle nul arguments
    if (!outptr) {
        fclose(outptr);
    }

    fclose(inptr);
}
