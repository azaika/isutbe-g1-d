#ifndef NEUTREEKO_HEADER_BOARD_H
#define NEUTREEKO_HEADER_BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum cell_state_tag {
    STATE_EMPTY = 0,
    STATE_WALL = 1,
    STATE_BLACK = 2,
    STATE_WHITE = 4
};

typedef enum cell_state_tag cell_state;

#define FIELD_SIZE (5)
#define BOARD_SIZE (FIELD_SIZE + 2)

typedef int** board_type;

static board_type alloc_board() {
    int* board_raw = (int*)malloc(BOARD_SIZE * BOARD_SIZE * sizeof(int));
    board_type board = (int**)malloc(BOARD_SIZE * sizeof(int*));
    if (!board_raw || !board) {
        perror("failed to allocate gameboard");
        exit(1);
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        board[i] = board_raw + i * BOARD_SIZE;
    }

    return board;
}

static void free_board(board_type board) {
    free(board[0]);
    free(board);
}

static void copy_board(board_type dest, const board_type src) {
    assert(dest != NULL);
    assert(src != NULL);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            dest[i][j] = src[i][j];
        }
    }
}

// generate neutreeko gameboard with sentinel
static board_type generate_board() {
    static int initial_board[BOARD_SIZE][BOARD_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 2, 0, 2, 0, 1},
        {1, 0, 0, 4, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 2, 0, 0, 1},
        {1, 0, 4, 0, 4, 0, 1},
        {1, 1, 1, 1, 1, 1, 1}
    };

    board_type board = alloc_board();

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = initial_board[i][j];
        }
    }
    
    return board;
}

static void print_board(board_type board) {
    for (int i = 1; i <= FIELD_SIZE; ++i) {
        for (int j = 1; j <= FIELD_SIZE; ++j) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

struct move_tag {
    int from[2]; // (y, x)
    int to[2];   // (y, x)
};

typedef struct move_tag move_type;

#endif // NEUTREEKO_HEADER_BOARD_H