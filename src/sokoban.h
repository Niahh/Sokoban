//
// Created by laurent on 21/03/2018.
//

#ifndef SOKOBAN_SOKOBAN_H
#define SOKOBAN_SOKOBAN_H

#include "array_list.h"
typedef struct pos {
    int x;
    int y;
} pos;

enum move {
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8,
};

enum cell {
    WALL = 8,
    BOX = 1,
    TARGET = 4,
    PLAYER = 2,
    FREE = 0
};

typedef struct sokoban {
    int *state;
    pos player;
    pos dim;
} sokoban;

sokoban *sokoban_create(int *state, int x, int y);

void sokoban_add_moves(sokoban *sokobans, list* explored);

int sokoban_compare(sokoban *s1, sokoban *s2);

sokoban *sokoban_clone(sokoban *sokoban);

void sokoban_make_move(sokoban *sokoban, int x, int y);

int sokoban_is_final(sokoban *sokoban);

int sokoban_hash(sokoban *sokoban);

void sokoban_print(sokoban *sokoban);

int move_is_valid(sokoban *s, int mv_x, int mv_y);


#endif //SOKOBAN_SOKOBAN_H
