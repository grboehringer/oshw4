#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#define NUM_THREADS 4

#define ARRAY_SIZE 2000000
#define STRING_SIZE 16
#define ALPHABET_SIZE 26

#define ASCII_MIN 0x20
#define ASCII_MAX 0x7e

pthread_mutex_t mutexsum;			// mutex for char_counts

char char_array[ARRAY_SIZE][STRING_SIZE];
int min_vals[ARRAY_SIZE] = {0};	

void* read_file(const char* filename){
    if (filename == NULL) return 1;
    FILE* fp;
    fp = fopen(filename, "r");
    if (fp == NULL) return NULL;
    int i;
    for (i = 0; i < ARRAY_SIZE && fgets(char_array[i], STRING_SIZE, fp) != NULL; i++){
        puts(char_array[i]);
    }
    fclose(fp);
    return char_array;
}

void* get_mins(){
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        int i;
        for (int i = id; i < ARRAY_SIZE; i += nthreads){
            int j;
            char min = 0;
            for (int j = id; j < STRING_SIZE && char_array[i][j] != '\0'; j++){
                char curr = char_array[i][j];
                if (curr < min && curr > ASCII_MIN && curr < ASCII_MAX) min = curr;
            }
            min_vals[i] = min;
        }
    }
    return min_vals;
}

void print_mins(){
    int i;
    if (char_array == NULL) return;
    for (int i = 0; i < ARRAY_SIZE; i++){
        if (min_vals[i] == 0) break;
        printf("%d: %d\n", i, min_vals[i]);
    }
}

int main(){
    char* filename = "example.txt";
    if (readfile(filename) == NULL) return 1;
    if (get_mins() == NULL) return 1;
    print_mins();
}
