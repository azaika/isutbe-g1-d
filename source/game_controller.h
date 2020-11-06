#ifndef NEUTREEKO_HEADER_GAME_CONTROLLER_H
#define NEUTREEKO_HEADER_GAME_CONTROLLER_H

#include "board.h"
#include "ai.h"

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

struct game_controller_tag {
    board_type board;
    int turn;
    int player_turn_parity;
};

typedef struct game_controller_tag game_controller;

static void init_game(game_controller* gc, bool is_player_black) {
    assert(gc != NULL);

    gc->board = generate_board();
    gc->turn = 0;
    gc->player_turn_parity = (is_player_black ? 0 : 1);
}

static void destroy_game(game_controller* gc) {
    free_board(gc->board);
}

static bool is_player_turn(game_controller* gc) {
    assert(gc != NULL);
    return gc->turn % 2 == gc->player_turn_parity;
}

static bool validate_move(game_controller* gc, move_type move) {
    assert(gc != NULL);

    if (is_player_turn(gc)) {
        // player turn

        // ToDo: implement
    }
    else {
        // cpu turn

        // ToDo: implement
    }

    return false;
}

static void print_move(move_type move) {
    int from_y = 6 - move.from[0];
    char from_x = 'A' + move.from[1] - 1;
    int to_y = 6 - move.to[0];
    char to_x = 'A' + move.to[1] - 1;

    printf("%d%c%d%c\n", from_y, from_x, to_y, to_x);
    
    return;
}

// returns whether parsing succeeded
// if succeeded, `move` is valid pointer to the parsed moving
static bool parse_move(const char* input, move_type* move) {
    // ToDo: implement
    return false;
}

static void move_piece(game_controller* gc, move_type move) {
    // ToDo: implement
}

// returns 0 when no one win
//         1 when player wins
//         2 when AI wins
static int check_wins(game_controller* gc) {
    // ToDo: implement    
    return 0;
}

#define TURN_LIMIT (300)

// returns whether the game continues
static bool advance_turn(game_controller* gc) {
    assert(gc != NULL);

    move_type move = {};
    if (is_player_turn(gc)) {
        // player turn

        // ToDo: parse input and validate it
    }
    else {
        // cpu turn
        move = ai_decide_move(gc->board, gc->player_turn_parity == 1);

        print_move(move);
    }

    move_piece(gc, move);

    int wins = check_wins(gc);
    if (wins) {
        printf(wins == 1 ? "You Win\n" : "You Lose\n");
        return false;
    }

    ++gc->turn;
    return gc->turn <= TURN_LIMIT;
}

#endif // NEUTREEKO_HEADER_GAME_CONTROLLER_H