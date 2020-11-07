#ifndef NEUTREEKO_HEADER_GAME_CONTROLLER_H
#define NEUTREEKO_HEADER_GAME_CONTROLLER_H

#include "board.h"
#include "ai.h"

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

static int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

static bool validate_move(game_controller* gc, move_type move) {
    assert(gc != NULL);

    int from_y = move.from[0];
    int from_x = move.from[1];
    int to_y = move.to[0];
    int to_x = move.to[1];
    
    cell_state piece;
    if (is_player_turn(gc)) {
        // player turn
        piece = ((gc->player_turn_parity == 0) ? STATE_BLACK : STATE_WHITE);
    }
    else {
        // cpu turn
        piece = ((gc->player_turn_parity == 1) ? STATE_BLACK : STATE_WHITE);
    }
    
    if (gc->board[from_y][from_x] != piece ||
        gc->board[to_y][to_x] != STATE_EMPTY)
        return false;

    int dy = to_y - from_y;
    int dx = to_x - from_x;

    if (dy != 0 && dx != 0 && abs(dy) != abs(dx))
        return false;

    int sy = sign(dy);
    int sx = sign(dx);
    
    int y = from_y + sy, x = from_x + sx;
    while (y != to_y || x != to_x) {
        if (gc->board[y][x] != STATE_EMPTY)
            return false;
        y += sy;
        x += sx;
    }
    if (gc->board[to_y + sy][to_x + sx] == STATE_EMPTY)
        return false;

    return true;
}

static void print_move(move_type move) {
    int from_y = move.from[0];
    char from_x = 'A' + move.from[1] - 1;
    int to_y = move.to[0];
    char to_x = 'A' + move.to[1] - 1;

    printf("%d%c%d%c\n", from_y, from_x, to_y, to_x);
    
    return;
}

// returns whether parsing succeeded
// if succeeded, `move` is valid pointer to the parsed moving
static bool parse_move(const char* input, move_type* move) {
    // ToDo: implement
  if(strlen(input) != 4)
    return false;
  else{
    int str[4];
    str[0] = input[0] - '0';
    str[1] = input[1] - '@';
    str[2] = input[2] - '0';
    str[3] = input[3] - '@';

    if(str[0] <= 5 && str[0] >= 1 && str[1] <= 5 && str[1] >= 1 && str[2] <= 5 && str[2] >= 1 && str[3] <= 5 && str[3] >= 1)
      return false;
    else{
      move->from[0] = str[0];
      move->from[1] = str[1];
      move->to[0] = str[2];
      move->to[1] = str[3];
      return true;
    }
  }
}

static void move_piece(game_controller* gc, move_type move) {
    gc->board[move.from[0]][move.from[1]] = STATE_EMPTY;

    cell_state piece;
    if ((is_player_turn(gc) && gc->player_turn_parity == 0) ||
        (!is_player_turn(gc) && gc->player_turn_parity == 1))
        piece = STATE_BLACK;
    else
        piece = STATE_WHITE;

    gc->board[move.to[0]][move.to[1]] = piece;
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
