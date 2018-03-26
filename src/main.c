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

void bfs(sokoban* s){
    sokoban_print(s);
    list* explored = list_init();
    sokomem* mem = sokomem_empty();
    sokoban_explored(s, mem);
    list_add(explored, s);
    for(int i = 0; i<explored->size; i++){
        s = list_at(explored, i);
        if (sokoban_is_final(s)){
            printf("final\n");
            sokoban_print(s);
            break;
        }
        sokoban_add_moves(s, explored, mem, i);
        /*if (i%10000) {
            printf("i : %d\n", i);
            printf("final : %d\n\n", explored->size);
        }*/
    }
    sokoban* prev = s;
    int nb = 1;
    for (int i = s->prev_id;i!=0;){
        prev = list_at(explored, prev->prev_id);
        i = prev->prev_id;
        printf("\n");
        sokoban_print(prev);
        nb++;
    }
    printf("in %d", nb);
}

void create(){/*
    int state[64] = {WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,
                     WALL,FREE,FREE,TARGET,FREE,WALL,WALL,WALL,
                     WALL,FREE,FREE,FREE,FREE,WALL,WALL,WALL,
                     WALL,FREE,WALL,BOX,BOX,TARGET,FREE,WALL,
                     WALL,TARGET,FREE,FREE,WALL,WALL,FREE,WALL,
                     WALL,PLAYER,BOX,FREE,WALL,WALL,FREE,WALL,
                     WALL,WALL,WALL,FREE,FREE,FREE,FREE,WALL,
                     WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL};*/
    int state[73] = {8,8,8,8,8,8,8,8,8,
            8,0,0,0,0,8,8,8,8,
            8,0,1,1,1,8,8,8,8,
            8,0,0,8,4,4,8,8,8,
            8,8,0,0,4,4,1,0,8,
            8,8,0,2,0,0,0,0,8,
            8,8,8,8,8,8,8,8,8};
    sokoban *s = sokoban_create((int*)&state, 7, 9, 0);
    sokoban_print(s);
    sokoban_apply(s);
    sokoban_print(s);
    sokoban_add_moves(s, list_init(), sokomem_empty(), 0);
    sokoban * copy = sokoban_create(s->state, s->dim.x, s->dim.y, 0);
    sokoban_compare(copy, s);
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

