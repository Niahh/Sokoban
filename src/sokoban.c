#include <stdlib.h>
#include <stdio.h>
#include "sokoban.h"
#include "stdbool.h"
#include "array_list.h"

#define AT(x, j, s) s->state[j+x*s->dim.y]
#define AT_DIM(x, j, d) (j+x*d.y)

sokoban *sokoban_create(int *state, int x, int y, int id) {
    sokoban *s = malloc(sizeof(sokoban));
    s->state = state;
    s->dim = (pos) {x, y};
    s->boxes = list_init();
    s->prev_id = id;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (AT(i, j, s) & BOX) {
                pos* box = malloc(sizeof(box));
                box->x = i;
                box->y = j;
                list_add(s->boxes, box);
                AT(i, j, s) ^= BOX;
            } if (AT(i, j, s) & PLAYER) {
                s->player = (pos) {i, j};
                AT(i, j, s) ^= PLAYER;
            }
        }
    }
    return s;
}

void sokoban_apply(sokoban* s){
    for (int i = 0; i<s->boxes->size; i++){
        pos* box = list_at(s->boxes, i);
        AT(box->x, box->y, s) |= BOX;
    }
    AT(s->player.x, s->player.y, s) |= PLAYER;
}

void sokoban_unapply(sokoban* s){
    for (int i = 0; i<s->boxes->size; i++){
        pos* box = list_at(s->boxes, i);
        if (AT(box->x, box->y, s) & BOX) {
            AT(box->x, box->y, s) -= BOX;
        }
    }
    if (AT(s->player.x, s->player.y, s) & PLAYER) {
        AT(s->player.x, s->player.y, s) -= PLAYER;
    }
}

void sokoban_explore_move(sokoban* s, list* explored, int x, int y, sokomem* mem, int id){
    sokoban_apply(s);
    if (move_is_valid(s, s->player.x +x, s->player.y+y)) {
        sokoban_make_move(s, s->player.x + x, s->player.y+y);
        sokoban* copy = sokoban_create(s->state, s->dim.x, s->dim.y, id);
        if (!sokoban_explored(copy, mem)) {
            list_add(explored, copy);
        }
    }
    sokoban_unapply(s);
}

void sokoban_add_moves(sokoban *s, list* explored, sokomem * mem, int id) {
    sokoban_explore_move(s, explored, 1, 0, mem, id);
    sokoban_explore_move(s, explored, 0, 1, mem, id);
    sokoban_explore_move(s, explored, 0, -1, mem, id);
    sokoban_explore_move(s, explored, -1, 0, mem, id);
    sokoban_unapply(s);
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

int sokoban_is_legal(sokoban* s){
    
}

// should only modify the state.
// Only assignment are in the AT() calls
void sokoban_make_move(sokoban *s, int x, int y) {
    if (AT(x, y, s) & BOX) {
        AT(x, y, s) ^= BOX;
        int p_x = x + x - s->player.x;
        int p_y = y + y - s->player.y;
        AT(p_x, p_y, s) |= BOX;
    }
    AT(s->player.x, s->player.y, s) ^= PLAYER;
    AT(x, y, s) |= PLAYER;
}

void sokomem_print(sokomem* mem, int dim, int prof){
    if (mem->exp) {
        for (int i = 0; i < dim; i++) {
            if(mem->son[i]->exp){
                printf("%d at %d\t", i, prof);
                sokomem_print(mem->son[i], dim, prof+1);
            } else {
                printf("ne ");
            }
        }
    } else {
        printf("not explored\n");
    }
    printf("\n");
}

sokomem* sokomem_empty(){
    sokomem* mem = malloc(sizeof(sokomem));
    mem->exp = false;
    return mem;
}
void sokomem_fill(sokomem* mem, int dim){
    mem->exp = true;
    mem->son = malloc(sizeof(sokomem)*dim);
    for (int i = 0;i<dim;i++){
        mem->son[i] = sokomem_empty();
    }
}

// one by one checks that it exists.
int sokoban_explored(sokoban *s, sokomem* mem){
    int to_ret = true;
    int dim = s->dim.x*s->dim.y;
    sokomem* starting_point = mem;
    for (int i = 0; i<s->boxes->size; i++){
        pos* box = list_at(s->boxes, i);
        if (!starting_point->exp){
            sokomem_fill(starting_point, dim);
            to_ret= false;
        }
        starting_point = starting_point->son[AT_DIM(box->x, box->y, s->dim)];
    }
    pos box = s->player;
    if (!starting_point->exp) {
        sokomem_fill(starting_point, dim);
        to_ret= false;
    }
    starting_point = starting_point->son[AT_DIM(box.x, box.y, s->dim)];
    if (!starting_point->exp) {
        sokomem_fill(starting_point, dim);
        to_ret= false;
    }
    return to_ret;
}

int sokoban_is_final(sokoban *s) {
    for (int i = 0; i<s->boxes->size; i++){
        pos* box = list_at(s->boxes, i);
        if (!(AT(box->x, box->y, s) & TARGET)){
            return false;
        }
    }
    return true;
}

int sokoban_hash(sokoban *s) {
    return 0;
}

void sokoban_print(sokoban *s) {
    sokoban_apply(s);
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
    sokoban_unapply(s);
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

// returns 1 if they are the same.
int sokoban_compare(sokoban *s, sokoban *s2){
    for (int i = 0; i<s->boxes->size; i++){
        pos* box1 = list_at(s->boxes, i);
        pos* box2 = list_at(s2->boxes, i);
        if (box1->x != box2->x || box1->y != box2->y){
            return false;
        }
    }
    if (s->player.x != s2->player.x || s->player.y != s2->player.y){
        return false;
    }
    return true;
}