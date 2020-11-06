#include <stdio.h>

#include "ai.h"
#include "board.h"
#include "game_controller.h"

#include <stdbool.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        perror("too many or too short arguments");
        return 0;
    }
    
    game_controller gc;
    if (!strcmp(argv[1], "0")) {
        init_game(&gc, true);
    }
    else if (!strcmp(argv[1], "1")) {
        init_game(&gc, false);
    }
    else {
        perror("argument must be 0 or 1");
        return 0;
    }

    // for debugging
    printf("init success!\n");

    while (advance_turn(&gc)) {
        // for debugging
        print_board(gc.board);
        printf("\n");
    }

    destroy_game(&gc);
    
    return 0;
}