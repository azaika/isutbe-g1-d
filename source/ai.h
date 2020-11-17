#ifndef NEUTREEKO_HEADER_AI_H
#define NEUTREEKO_HEADER_AI_H

#include "board.h"
#include <stdbool.h>

struct coord_tag {
    int y;
    int x;
};

typedef struct coord_tag coord_type;

// return direction
//          0 1 2
//          3   4
//          5 6 7
static const coord_type ai_direction[8] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    { 0, -1},          { 0, 1},
    { 1, -1}, { 1, 0}, { 1, 1},
};

// write the coordinates of the pieces on `pc`
static void ai_search_piece(coord_type* pc, board_type board, bool is_black) {
    cell_state piece = is_black ? STATE_BLACK : STATE_WHITE;
    int n;

    n = 0;
    for (int i = 1; i <= FIELD_SIZE; ++i) {
        for (int j = 1; j <= FIELD_SIZE; ++j) {
            if (board[i][j] == piece) {
                pc[n].y = i;
                pc[n].x = j;
                ++n;
            }
        }
    }
}

// return whether piece can move from `from` in the direction `dir`
static bool ai_is_movable(board_type board, coord_type from, coord_type dir) {
    if (board[from.y + dir.y][from.x + dir.x] == STATE_EMPTY)
        return true;
    return false;
}

// return the coordinates of piece after move
static coord_type ai_find_to(board_type board, coord_type from, coord_type dir) {
    coord_type to = from;

    while (board[to.y + dir.y][to.x + dir.x] == STATE_EMPTY) {
        to.y += dir.y;
        to.x += dir.x;
    }
    return to;
}

// swap state of c1 and c2 in board
static void ai_swap_piece(board_type board, coord_type c1, coord_type c2) {
    int tmp = board[c1.y][c1.x];
    board[c1.y][c1.x] = board[c2.y][c2.x];
    board[c2.y][c2.x] = tmp;
}

// write move-finished board on `next_board`
static void ai_move_piece(board_type next_board, board_type board, coord_type from, coord_type dir) {
    coord_type to = ai_find_to(board, from, dir);
    copy_board(next_board, board);
    ai_swap_piece(next_board, from, to);
}

// return true when black player wins (if is_black)
//             or when white player wins (if !is_black)
static bool ai_check_win(board_type board, bool is_black) {
    coord_type pc[3];
    ai_search_piece(pc, board, is_black);

    // horizontal
    if (pc[0].y == pc[1].y && pc[1].y == pc[2].y && pc[2].x - pc[0].x == 2)
        return true;
    // vertical
    if (pc[0].x == pc[1].x && pc[1].x == pc[2].x && pc[2].y - pc[0].y == 2)
        return true;
    // diagonal
    if ((pc[1].y - pc[0].y) * (pc[1].x - pc[0].x) *
        (pc[2].y - pc[1].y) * (pc[2].x - pc[1].x) == 1)
        return true;

    return false;
}

// return the number of possible move
static int ai_count_movable(board_type board, bool is_black) {
    cell_state piece = is_black ? STATE_BLACK : STATE_WHITE;
    int n = 0;

    for (int y = 1; y <= FIELD_SIZE; ++y) {
        for (int x = 1; x <= FIELD_SIZE; ++x) {
            if (board[y][x] == piece) {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (board[y + dy][x + dx] == STATE_EMPTY)
                            ++n;
                    }
                }
            }
        }
    }

    return n;
}

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

// return the score of the best move between `alpha` and `beta`
static int ai_alphabeta(board_type board, bool is_black, bool is_ai, int depth, int maxdepth, int alpha, int beta) {
    if (depth >= maxdepth) {
        if (is_ai)
            return ai_count_movable(board, is_black);
        else
            return -ai_count_movable(board, is_black);
    }

    board_type next_board = alloc_board();
    coord_type self_pieces[3];
    coord_type from, dir;
    int score;

    ai_search_piece(self_pieces, board, is_black);

    for (int i = 0; i < 3; ++i) { // for each piece
        from = self_pieces[i];
        for (int d = 0; d < 8; ++d) { // for each direction
            dir = ai_direction[d];
            if (!ai_is_movable(board, from, dir))
                continue;

            ai_move_piece(next_board, board, from, dir);
            if (ai_check_win(next_board, is_black)) {
                free_board(next_board);
                return is_ai ? 100 - depth : -100 + depth;
            }

            score = ai_alphabeta(next_board, !is_black, !is_ai, depth + 1, maxdepth, alpha, beta);
            if (is_ai) {
                alpha = max(alpha, score);
                if (alpha >= beta) {
                    free_board(next_board);
                    return alpha;
                }
            }
            else {
                beta = min(beta, score);
                if (alpha >= beta) {
                    free_board(next_board);
                    return beta;
                }
            }
        }
    }

    free_board(next_board);
    return is_ai ? alpha : beta;
}

// write maximum score index on `maxi` and `maxd` and return maximum score
static int ai_find_maxscore_index(int* maxi, int* maxd, int score_array[3][8]) {
    int maxscore = score_array[0][0];
    *maxd = 0;
    *maxi = 0;
    for (int i = 0; i < 3; ++i) {
        for (int d = 0; d < 8; ++d) {
            if (score_array[i][d] > maxscore) {
                maxscore = score_array[i][d];
                *maxi = i;
                *maxd = d;
            }
        }
    }

    return maxscore;
}

// write move from `from` to `to` on `move`
static void ai_write_move(move_type* move, coord_type from, coord_type to) {
    move->from[0] = from.y;
    move->from[1] = from.x;
    move->to[0] = to.y;
    move->to[1] = to.x;
}

// first step of alphabeta
// return whether ai wins or not
static bool ai_alphabeta_init(move_type* move, int score_array[3][8], board_type board, bool is_black, int maxdepth) {
    board_type next_board = alloc_board();
    coord_type self_pieces[3];
    coord_type from, dir;
    int score;

    ai_search_piece(self_pieces, board, is_black);

    for (int i = 0; i < 3; ++i) { // for each piece
        from = self_pieces[i];
        for (int d = 0; d < 8; ++d) { // for each direction
            dir = ai_direction[d];
            if (score_array[i][d] < -30) // immovable or ai loses
                continue;

            ai_move_piece(next_board, board, from, dir);
            if (ai_check_win(next_board, is_black)) {
                score_array[i][d] = 100;
                continue;
            }

            score = ai_alphabeta(next_board, !is_black, false, 1, maxdepth, -110, 110);
            score_array[i][d] = score; // update score_array

            if (score > 30) {
                i = 2;
                break;
            }
        }
    }

    free_board(next_board);

    int maxscore, maxi, maxd;
    maxscore = ai_find_maxscore_index(&maxi, &maxd, score_array);

    from = self_pieces[maxi];
    dir = ai_direction[maxd];
    coord_type to = ai_find_to(board, from, dir);
    ai_write_move(move, from, to);

    return maxscore > 30; // whether ai wins or not
}

#define IMMOVABLE (-999)

// write IMMOVABLE(immovable) or 0(movable) in `score_array`
static void ai_initialize_score_array(int score_array[3][8], board_type board, bool is_black) {
    coord_type self_pieces[3];
    coord_type from, dir;

    ai_search_piece(self_pieces, board, is_black);

    for (int i = 0; i < 3; ++i) {
        from = self_pieces[i];
        for (int d = 0; d < 8; ++d) {
            dir = ai_direction[d];
            if (!ai_is_movable(board, from, dir))
                score_array[i][d] = IMMOVABLE;
            else
                score_array[i][d] = 0;
        }
    }
}

static move_type ai_decide_move(board_type board, bool is_black) {
    int score_array[3][8];
    move_type move;
    int depthlimit = 9; // maximum depth ai can search
    bool ai_wins;

    if (is_black) {
        board_type initial_board = generate_board();

        bool is_same = true;
        for (int i = 1; i <= FIELD_SIZE; ++i) {
            for (int j = 1; j <= FIELD_SIZE; ++j) {
                if (board[i][j] != initial_board[i][j]) {
                    is_same = false;
                    i = j = FIELD_SIZE + 1;
                }
            }
        }

        free_board(initial_board);

        move.from[0] = 1;
        move.from[1] = 2;
        move.to[0] = 1;
        move.to[1] = 3;

        if (is_same) {
            return move;
        }
    }


    ai_initialize_score_array(score_array, board, is_black);

    for (int maxdepth = 1; maxdepth <= depthlimit; ++maxdepth) {
        ai_wins = ai_alphabeta_init(&move, score_array, board, is_black, maxdepth);
        if (ai_wins)
            break;
    }

    return move;
}

#endif // NEUTREEKO_HEADER_AI_H