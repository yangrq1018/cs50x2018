// Helper functions for music

#include <math.h>
#include <cs50.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // copy fraction into temp
    char temp[strlen(fraction)+1];
    for (int i=0; i<strlen(fraction); i++) {
        temp[i] = fraction[i];
    }
    temp[strlen(fraction)] = '\0';
    
    // tokenize temp
    int numerator = atoi(strtok(temp, "/")); // throw away the first token numerator
    int denominator = atoi(strtok(NULL, "/"));
    
    
    return (int)round( (numerator/(double)denominator) / (1.0/8) );
}

// Calculates frequency (in Hz) of a note
/*
The frequency of a note is determined by its distance to A4, denoting distance as n
2**(n/12) * 440;

[C, #b, D, #b, E, F, #b, G, #b, A, #b, B]
[-9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2]

note is a string representation a la "C4" or "C#4";
*/
int frequency(string note)
{
    int n = find_distance_to_A4(note);
    // printf("n: %d\n", n);
    // printf("%f\n", pow(2, n/12.0)*440);
    return (int)round(pow(2, n/12.0)*440);
}


int find_distance_to_A4(char* note) {
    // parse note string
    bool has_accidental;
    char accidental, tune;
    int octave;
    for (int i=0; i<strlen(note); i++) {
        if (note[i] == '#') {
            has_accidental = true;
            accidental = '#';
        } else if (note[i] == 'b') {
            has_accidental = true;
            accidental = 'b';
        } else if (isalpha(note[i])) {
            tune = note[i];
        } else {
            octave = note[i] - '0';
        }
    }
    
    int distance;
    if (tune == 'C') {
        distance = -9;
    } else if (tune == 'D') {
        distance = -7;
    } else if (tune == 'E') {
        distance = -5;
    } else if (tune == 'F') {
        distance = -4;
    } else if (tune == 'G') {
        distance = -2;
    } else if (tune == 'A') {
        distance = 0;
    } else if (tune == 'B') {
        distance = 2;
    } else {
        printf("Invalid tune char %c\n", tune);
        exit(1);
    }
    
    if (has_accidental) {
        distance += (accidental=='#')? 1:-1;
    }
    
    distance += (octave - 4)*12;
    
    return distance;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    return strlen(s) == 0;
}


// int main(int argc, char** argv) {
//     printf("%d\n", find_distance_to_A4(argv[1]));
// }
