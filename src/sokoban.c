#include <stdlib.h>
#include <stdio.h>
#include "sokoban.h"
#include "stdbool.h"
#include "array_list.h"

#define AT(x, j, s) s->state[j+x*s->dim.y]

sokoban *sokoban_create(int *state, int x, int y) {
    sokoban *s = malloc(sizeof(sokoban));
    s->state = state;
    s->dim = (pos) {x, y};
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (AT(i, j, s) & PLAYER) {
                s->player = (pos) {i, j};
            }
        }
    }
    return s;
}

void sokoban_add_moves(sokoban *s, list* explored) {
    //printf("base\n");
    //sokoban_print(s);
    if (move_is_valid(s, s->player.x + 1, s->player.y)) {
        sokoban* copy = sokoban_clone(s);
        sokoban_make_move(copy, s->player.x + 1, s->player.y);
        if (!list_contains(explored, copy, sokoban_compare)) {
            //printf("ok down\n");
            //sokoban_print(copy);
            list_add(explored, copy);
        }
    }
    if (move_is_valid(s, s->player.x - 1, s->player.y)) {
        sokoban* copy = sokoban_clone(s);
        sokoban_make_move(copy, s->player.x - 1, s->player.y);
        if (!list_contains(explored, copy, sokoban_compare)) {
            //printf("ok up\n");
            //sokoban_print(copy);
            list_add(explored, copy);
        }
    }
    if (move_is_valid(s, s->player.x, s->player.y + 1)) {
        sokoban* copy = sokoban_clone(s);
        sokoban_make_move(copy, s->player.x, s->player.y + 1);
        if (!list_contains(explored, copy, sokoban_compare)) {
            //printf("ok right\n");
            //sokoban_print(copy);
            list_add(explored, copy);
        }
    }
    if (move_is_valid(s, s->player.x, s->player.y - 1)) {
        sokoban* copy = sokoban_clone(s);
        sokoban_make_move(copy,s->player.x, s->player.y - 1);
        if (!list_contains(explored, copy, sokoban_compare)) {
            //printf("ok left\n");
            //sokoban_print(copy);
            list_add(explored, copy);
        }
    }
}

sokoban *sokoban_clone(sokoban *s) {
    sokoban *new = malloc(sizeof(sokoban));
    new->dim = s->dim;
    new->player = s->player;
    new->state = malloc(sizeof(int) * s->dim.x * s->dim.y);
    for (int i = 0; i < s->dim.x; i++) {
        for (int j = 0; j < s->dim.y; j++) {
            AT(i, j, new) = AT(i, j, s);
        }
    }
    return new;
}

void sokoban_make_move(sokoban *s, int x, int y) {
    if (AT(x, y, s) & BOX) {
        AT(x, y, s) ^= BOX;
        int p_x = x + x - s->player.x;
        int p_y = y + y - s->player.y;
        AT(p_x, p_y, s) |= BOX;
    }
    AT(x, y, s) |= PLAYER;
    AT(s->player.x, s->player.y, s) ^= PLAYER;
    s->player = (pos){x, y};
}

int sokoban_is_final(sokoban *s) {
    for (int i = 0; i < s->dim.x; i++) {
        for (int j = 0; j < s->dim.y; j++) {
            if (AT(i, j, s) & BOX){
                if (!(AT(i, j, s) & TARGET)){
                    return false;
                }
            }
        }
    }
    return true;
}

int sokoban_hash(sokoban *s) {

}

void sokoban_print(sokoban *s) {
    for (int i = 0; i < s->dim.x; i++) {
        for (int j = 0; j < s->dim.y; j++) {
            if (AT(i, j, s) & PLAYER) {
                printf("P");
            } else if (AT(i, j, s) & BOX) {
                printf("B");
            } else if (AT(i, j, s) & TARGET) {
                printf("T");
            } else if (AT(i, j, s) & WALL) {
                printf("W");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int move_is_valid(sokoban *s, int mv_x, int mv_y) {
    if (mv_x < 0 || mv_x >= s->dim.x || mv_y < 0 || mv_y >= s->dim.y || AT(mv_x, mv_y, s) == WALL) {
        return 0;
    } else if (AT(mv_x, mv_y, s) & BOX) {
        int x = mv_x + mv_x - s->player.x;
        int y = mv_y + mv_y - s->player.y;
        return !(AT(x, y, s) & BOX || AT(x, y, s) & WALL);
    } else {
        return 1;
    }
}


int sokoban_compare(sokoban *s, sokoban *s2){
    for (int i = 0; i < s->dim.x; i++) {
        for (int j = 0; j < s->dim.y; j++) {
            if (AT(i, j, s) != AT(i, j, s2)){
                return false;
            }
        }
    }
    return true;
}