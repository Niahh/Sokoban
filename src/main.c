#include <stdio.h>
#include "array_list.h"
#include "sokoban.h"
#include "stdlib.h"

sokoban* read_file(char* name){
    FILE *myFile;
    myFile = fopen(name, "r");
    int dimx, dimy;
    fscanf(myFile, "%d,", &dimx);
    fscanf(myFile, "%d\n", &dimy);
    int dim = dimx*dimy;
    int* numberArray = malloc(sizeof(int)*dim);
    for (int i = 0; i < dim; i++){
        fscanf(myFile, "%d,", &numberArray[i] );
    }
    fclose(myFile);
    return sokoban_create(numberArray, dimx ,dimy, 0);
}

list* bfs(sokoban* s){
    sokoban_parse_illegals(s);
    sokoban_print(s);
    list* explored = list_init_cap(1000000);
    sokomem* mem = sokomem_empty(0);
    sokoban_explored(s, mem);
    list_add(explored, s);
    for(int i = 0; i<explored->size; i++){
        s = list_at(explored, i);
        if (sokoban_is_final(s)){
            printf("final i : %d\n", i);
            sokoban_print(s);
            break;
        }
        sokoban_add_moves(s, explored, mem, i);
        if (i%100000 == 0) {
            printf("i : %d\n", i);
            printf("final : %d\n\n", explored->size);
        }
    }
    sokoban* prev = s;
    list* sol = list_init();
    list_add(sol, sokoban_clone(prev));
    int nb = 1;
    for (int i = s->prev_id;i!=0;){
        prev = list_at(explored, prev->prev_id);
        sokoban* to_add = sokoban_clone(prev);
        list_add(sol, to_add);
        i = prev->prev_id;
        nb++;
    }
    list_reverse(sol);
    list_free(explored, sokoban_destroy);
    return sol;
}

int main(int argc, char** argv) {
    if (argc >1){
        char* file_name = malloc(11* sizeof(char));
        sprintf(file_name, "soko%d.txt", atoi(argv[1]));
        bfs(read_file(file_name));
    } else {
        printf("please specify number\n");
    }
}


void sokoban_destroy(void * s){
    sokoban* sok = s;
    list_free(sok->boxes, pos_destroy);
    free(s);
}