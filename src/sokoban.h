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
    BLOCKED = 16,
    WALL = 8,
    TARGET = 4,
    PLAYER = 2,
    BOX = 1,
    FREE = 0
};

typedef struct sokomem sokomem;

// a tree to store explored nodes.
struct sokomem{
    // checks if the node exist
    int position;
    // the list of child nodes.
    list* children;
};

sokomem* sokomem_empty(int id);

void sokomem_print(sokomem* mem, int dim, int prof);

typedef struct sokoban {
    int *state;
    list* boxes;
    pos player;
    pos dim;
    int prev_id;
} sokoban;

void sokomem_fill(sokomem* mem, int dim, sokoban* s, int is_box);

int box_compare(pos* b1, pos* b2);

int sokoban_explored(sokoban *s, sokomem* mem);

sokoban *sokoban_create(int *state, int x, int y, int id);

void sokoban_explore_move(sokoban* s, list* explored, int x, int y, sokomem* mem, int id);

void sokoban_apply(sokoban* sokoban);

void sokoban_add_moves(sokoban *sokobans, list* explored, sokomem* mem, int id);

int sokoban_compare(sokoban *s1, sokoban *s2);

sokoban *sokoban_clone(sokoban *sokoban);

void sokoban_make_move(sokoban *sokoban, int x, int y);

int sokoban_is_final(sokoban *sokoban);

int sokoban_hash(sokoban *sokoban);

void sokoban_print(sokoban *sokoban);

int move_is_valid(sokoban *s, int mv_x, int mv_y);

void sokoban_parse_illegals(sokoban* s);

int sokoban_is_legal(sokoban* s, pos* start);

int pos_compare(void* p1, void* p2);

void pos_destroy(void * p);
#endif //SOKOBAN_SOKOBAN_H
