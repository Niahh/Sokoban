#include <stdio.h>
#include "array_list.h"
#include "sokoban.h"

void bfs(){
    int state[64] = {WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,
                     WALL,FREE,FREE,TARGET,FREE,WALL,WALL,WALL,
                     WALL,FREE,FREE,FREE,FREE,WALL,WALL,WALL,
                     WALL,FREE,WALL,BOX,BOX,TARGET,FREE,WALL,
                     WALL,TARGET,FREE,FREE,WALL,WALL,FREE,WALL,
                     WALL,PLAYER,BOX,FREE,WALL,WALL,FREE,WALL,
                     WALL,WALL,WALL,FREE,FREE,FREE,FREE,WALL,
                     WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL};
    sokoban *s = sokoban_create(&state, 8, 8);
    sokoban_print(s);
    list* explored = list_init();
    list_add(explored, s);
    for(int i = 0; i<explored->size; i++){
        s = list_at(explored, i);
        if (sokoban_is_final(s)){
            printf("final\n");
            sokoban_print(s);
            break;
        }
        sokoban_add_moves(s, explored);
        //printf("result : %d \n", explored->size);
        //sokoban_print(s);
    }
}

int main() {
    struct array_list *a = list_init();
    bfs();
}

