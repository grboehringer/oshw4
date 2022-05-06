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

char char_array[ARRAY_SIZE][STRING_SIZE];
int min_vals[ARRAY_SIZE] = {0};	

void* read_file(const char* filename){
    if (filename == NULL) return NULL;
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
        for (i = id; i < ARRAY_SIZE; i += nthreads){
            int j;
            char min = ASCII_MAX + 1;
            for (int j = 0; j < STRING_SIZE && char_array[i][j] != '\0'; j++){
                char curr = char_array[i][j];
                if (curr < min && curr >= ASCII_MIN && curr <= ASCII_MAX) min = curr;
            }
            if (min > ASCII_MAX) break;
            min_vals[i] = (int)min;
        }
        for (; i < ARRAY_SIZE; i += nthreads){
            min_vals[i] = 0;
        }
    }
    return min_vals;
}

void print_mins(){
    int i;
    if (char_array == NULL) return;
    for (int i = 0; i < ARRAY_SIZE; i++){
        if (min_vals[i] == 0) break;
        printf("%d: %d\n", i, (int)min_vals[i]);
    }
}

double time_diff(struct timeval recent, struct  timeval last){
    double t = (recent.tv_sec - last.tv_sec) * 1000;
    t += (recent.tv_usec - last.tv_usec) / 1000;
    return t;
}

int main(int argc, char* argv[]){
    struct timeval tstart, tread, tmins, tprint;
    double elapsedTime;
    gettimeofday(&tstart, NULL);
    if (argv == NULL) return 1;
    if (argc != 2){
        printf("There should be exactly 1 argument\n");
        return 1;
    }
    const char* filename = argv[1];
    if (read_file(filename) == NULL) return 1;
    gettimeofday(&tread, NULL);
    if (get_mins() == NULL) return 1;
    gettimeofday(&tmins, NULL);
    print_mins();
    gettimeofday(&tprint, NULL);
    printf("Time to read file: %fms\n", time_diff(tread, tstart));
    printf("Time to get mins: %fms\n", time_diff(tmins, tread));
    printf("Time to print: %fms\n", time_diff(tprint, tmins));
    printf("Total elapsed time: %fms\n", time_diff(tprint, tstart));
}
