#include <stdlib.h>
#include <stdio.h>
#include "sokoban.h"
#include "stdbool.h"
#include "array_list.h"

#define AT(x, j, s) s->state[(j)+(x)*(s)->dim.y]
#define AT_DIM(x, j, d) (j+x*d.y)
#define AT_ARR(x, j, y) (j+x*y)

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

sokomem* sokomem_empty(int id){
    sokomem* mem = malloc(sizeof(sokomem));
    mem->position = id;
    mem->children = list_init();
    return mem;
}
// S is a memory
// s2 is an integer.
int sokomem_cmp(void* s2, void* s){
    return ((sokomem*)(s))->position == *((int*)(s2));
}

sokomem* sokomem_has(sokomem* mem, int id, int *has){
    sokomem* child = list_get(mem->children, &id, sokomem_cmp);
    if (child == NULL){
        child = sokomem_empty(id);
        list_add(mem->children, child);
        *has = false;
        return child;
    } else {
        return child;
    }
}

// one by one checks that it exists.
int sokoban_explored(sokoban *s, sokomem* mem){
    int to_ret = true;
    sokomem* starting_point = mem;
    for (int i = 0; i<s->boxes->size; i++){
        pos* box = list_at(s->boxes, i);
        int has;
        starting_point = sokomem_has(starting_point, AT_DIM(box->x, box->y, s->dim), &has);
        if (!has){
            to_ret= false;
        }
    }
    pos box = s->player;
    int has;
    sokomem_has(starting_point, AT_DIM(box.x, box.y, s->dim), &has);
    if (!has){
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
            } else if (AT(i, j, s) & BLOCKED) {
                printf("I");
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
        return !(AT(x, y, s) & BOX || AT(x, y, s) & WALL || AT(x, y, s) & BLOCKED);
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

int pos_compare(void* p1, void* p2){
    pos * pos1 = p1, *pos2 = p2;
    return pos1->x == pos2->x && pos1->y == pos2->y;
}

void box_explore_move(sokoban* s, pos* start, list* explored, int x, int y){
    if (!(AT(start->x-x, start->y-y, s) & WALL) && !(AT(start->x+x, start->y+y, s) & WALL)) {
        pos* p = malloc(sizeof(pos));
        p->x = start->x + x;
        p->y = start->y + y;
        if (!list_contains(explored, (void*)p, (int(*)(void*, void*))pos_compare)) {
            list_add(explored, p);
        }
    }
}

void box_add_moves(sokoban *s, pos* start, list* explored) {
    box_explore_move(s, start, explored, 1, 0);
    box_explore_move(s, start, explored, 0, 1);
    box_explore_move(s, start, explored, 0, -1);
    box_explore_move(s, start, explored, -1, 0);
    sokoban_unapply(s);
}

// run some bfs to check where the boxes can go.
// list of positions
int sokoban_is_legal(sokoban* s, pos* start){
    list* explored = list_init();
    list_add(explored, start);
    for(int i = 0; i<explored->size; i++){
        pos* p = list_at(explored, i);
        if (AT(p->x, p->y, s) & TARGET){
            return 1;
        }
        box_add_moves(s, p, explored);
    }
    list_free(explored, pos_destroy);
    return 0;
}

void pos_destroy(void * p){
    pos* a = p;
    free(a);
}

pos* pos_clone(pos* p){
    pos* new = malloc(sizeof(p));
    new->x = p->x;
    new->y = p->y;
    return new;
}

void sokoban_parse_illegals(sokoban* s){
    pos* p = malloc(sizeof(pos));
    p->x = 0;
    p->y = 0;
    sokoban_is_legal(s, p);
    for (int i = 0; i < s->dim.x; i++) {
        for (int j = 0; j < s->dim.y; j++) {
            pos* p = malloc(sizeof(pos));
            p->x = i;
            p->y = j;
            if (!(AT(i, j, s) & WALL)){
                if (!sokoban_is_legal(s, p)) {
                    AT(i, j, s) += BLOCKED;
                }
            }
        }
    }
}

sokoban* sokoban_clone(sokoban* s){
    sokoban* new = malloc(sizeof(sokoban));
    new->state = s->state;
    new->dim = s->dim;
    new->boxes = list_clone(s->boxes, (void*(*)(void*))pos_clone);
    new->prev_id = 0;
    new->player = s->player;
    return new;
}