// Pierre Bertin 
// Samy Ouastani
// Laurent Toson

#ifndef SOKOBAN_SOKOBAN_H
#define SOKOBAN_SOKOBAN_H

#include "array_list.h"
typedef struct pos {
    int x;
    int y;
} pos;

enum cell {
    BLOCKED = 16,
    WALL = 8,
    TARGET = 4,
    PLAYER = 2,
    BOX = 1,
    FREE = 0
};

typedef struct sokomem sokomem;

/**
 * A tree to memorize the explored nodes.
 * Each level represents the x/y position of a box/player.
 */
struct sokomem{
    // checks if the node exist
    int position;
    // the list of child nodes.
    list* children;
};
// Creates an empty node.
sokomem* sokomem_empty(int id);

/**
 * The struct containing the sokoban, contains a pointer to the map, the positions of the player and boxes.
 * Also contains an index to the previous state in the list of explored nodes.
 */
typedef struct sokoban {
    int *state;
    list* boxes;
    pos player;
    pos dim;
    int prev_id;
} sokoban;

/**
 * Register the sokoban instance in the memory, returns 1 if it exists, 0 otherwise.
 * @param s : the sokoban instance.
 * @param mem : the memory of explored states.
 * @return
 */
int sokoban_explored(sokoban *s, sokomem* mem);

/**
 * Creates a new state.
 * @param state : the map with the applied positions.
 * @param x : the x size of the map
 * @param y : the y size of the map.
 * @param id : the id of the previous move.
 * @return the new state.
 */
sokoban *sokoban_create(int *state, int x, int y, int id);

/**
 * Explores a move and add the resulting state to the list if it did not exist.
 * @param s : the starting state
 * @param explored : the list of explored instances.
 * @param x : the xposition to move to.
 * @param y : the yposition to move to.
 * @param mem : the memory of explored moves.
 * @param id : the id of the state in the list.
 */
void sokoban_explore_move(sokoban* s, list* explored, int x, int y, sokomem* mem, int id);
/**
 * Apply the positions of the boxes and player to the map
 * @param sokoban : the sokoban ot apply to the map.
 */
void sokoban_apply(sokoban* sokoban);
/**
 * Remove the positions of the boxes and player to the map
 * @param sokoban : the sokoban ot remove to the map.
 */
void sokoban_unapply(sokoban* sokoban);

/**
 * Adds the possibles moves to the list of explored nodes and the memory.
 * @param sokobans : the starting state.
 * @param explored : the list of explored states.
 * @param mem : the memory of explored states.
 * @param id : the id of the starting state in the list.
 */
void sokoban_add_moves(sokoban *sokobans, list* explored, sokomem* mem, int id);

/**
 * Make the given move.
 * @param sokoban : the starting state.
 * @param x : the postiion to move to
 * @param y : the postiion to move to
 */
void sokoban_make_move(sokoban *sokoban, int x, int y);

/**
 * Returns true if the state is a winning state.
 * @param sokoban : the sokoban state.
 * @return True if all boxes are on the target, false otherwise.
 */
int sokoban_is_final(sokoban *sokoban);

/**
 * Pretty prints the sokoban to the screen.
 * @param sokoban : the state.
 */
void sokoban_print(sokoban *sokoban);

/**
 * Checks if a move is legal.
 * @param s : the sokoban state.
 * @param mv_x : the direction in wich the move is made on the x axis
 * @param mv_y : the direction in wich the move is made on the y axis
 * @return True if the move is possible, else false.
 */
int move_is_valid(sokoban *s, int mv_x, int mv_y);

/**
 * Parses the positions at wich a box can not be put.
 * Run bfs from all positions to check if the box can go to a target.
 * @param s : the sokoban state.
 */
void sokoban_parse_illegals(sokoban* s);

/**
 * Checks if a box at the given position can go to a target.
 * @param s : the sokoban state.
 * @param start : the starting position of the box.
 * @return true if the box can reach a target.
 */
int sokoban_is_legal(sokoban* s, pos* start);

/**
 * Compare two positions
 * @param p1 : the first position
 * @param p2 : the second position
 * @return true if the positions are the same, false otherwise.
 */
int pos_compare(void* p1, void* p2);

/**
 * Clone the position and returns a new one.
 * @param p : the position to clone.
 * @return the cloned position.
 */
pos* pos_clone(pos* p);

/**
 * Returns a clone of the sokoban.
 *
 * /!\ This will not clone the state.
 *
 * @param s : the given sokoban.
 * @return The cloned  state.
 */
sokoban* sokoban_clone(sokoban* s);

/**
 * destroys a position.
 * @param p : the position to destroy.
 */
void pos_destroy(void * p);

/**
 * destroys a sokoban state.
 * @param s : the state to destroy, the map wont be destroyed.
 */
void sokoban_destroy(void * s);
#endif //SOKOBAN_SOKOBAN_H
