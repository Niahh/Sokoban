#include <stdio.h>
#include "array_list.h"
#include "sokoban.h"

void bfs(){
    /*int state[25] = {
                     WALL,WALL,WALL,WALL,WALL,
                     WALL,PLAYER,BOX,TARGET,WALL,
                     WALL,BOX,FREE,FREE,WALL,
                     WALL,TARGET,FREE,FREE,WALL,
                     WALL,WALL,WALL,WALL,WALL};*/
    /*int state[64] = {WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,
                     WALL,FREE,FREE,TARGET,FREE,WALL,WALL,WALL,
                     WALL,FREE,FREE,FREE,FREE,WALL,WALL,WALL,
                     WALL,FREE,WALL,BOX,BOX,TARGET,FREE,WALL,
                     WALL,TARGET,FREE,FREE,WALL,WALL,FREE,WALL,
                     WALL,PLAYER,BOX,FREE,WALL,WALL,FREE,WALL,
                     WALL,WALL,WALL,FREE,FREE,FREE,FREE,WALL,
                     WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL};*//*
    int state[73] = {8,8,8,8,8,8,8,8,8,
                     8,0,0,0,0,8,8,8,8,
                     8,0,1,1,1,8,8,8,8,
                     8,0,0,8,4,4,8,8,8,
                     8,8,0,0,4,4,1,0,8,
                     8,8,0,2,0,0,0,0,8,
                     8,8,8,8,8,8,8,8,8};*/
    int state[64] = {
            8,8,8,8,8,8,8,8,
            8,8,8,4,8,8,8,8,
            8,8,8,0,8,8,8,8,
            8,8,8,1,0,1,4,8,
            8,4,0,1,2,8,8,8,
            8,8,8,8,1,8,8,8,
            8,8,8,8,4,8,8,8,
            8,8,8,8,8,8,8,8};
    sokoban *s = sokoban_create((int*)&state, 8, 8, 0);
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

int main() {
    bfs();
}

