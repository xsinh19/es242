#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

typedef char board_t[15];
typedef char player_t;

int randrange(int min, int max) {
    return min + rand() % (max - min + 1);
}

int find_max_index(int array[], int size) {
    if (size <= 0) {
        return -1; // Handle an invalid size
    }

    int maxIndex = 0;

    for (int i = 1; i < size; i++) {
        if (array[i] > array[maxIndex]) {
            maxIndex = i; // Update maxIndex when a larger element is found
        }
    }

    return maxIndex;
}

int find_index(int array[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (array[i] == target) {
            return i;
        }
    }
    return -1;
}

int has_won(board_t board, player_t player) {
    // Check if the player has formed a triangle.

    int triangles[20][3] = {
        {0, 1, 5}, {0, 2, 6}, {0, 3, 7}, {0, 4, 8}, {1, 2, 9},
        {1, 3, 10}, {1, 4, 11}, {2, 3, 12}, {2, 4, 13}, {3, 4, 14},
        {5, 6, 9}, {5, 7, 10}, {5, 8, 11}, {6, 7, 12},
        {6, 8, 13}, {7, 8, 14}, {9, 10, 12}, {9, 11, 13}, {10, 11, 14}, {12, 13, 14}
    };

    for (int i = 0; i < 20; i++) {
        int a = triangles[i][0];
        int b = triangles[i][1];
        int c = triangles[i][2];

        player_t other_player = (player == RED) ? BLUE : RED;

        if (board[a] == other_player && board[b] == other_player && board[c] == other_player) {
            return 1;
        }
    }

    // The player has not formed a triangle.
    return 0;
}

int is_full(board_t board) {
    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            return 0;
        }
    }

    // The board is full.
    return 1;
}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for a loss, 0 for a draw, 1 for a win. */
} move_t;

move_t best_move(board_t board, player_t player) {
    // If the player has won, return a move with a score of 1.
    if (has_won(board, player)) {
        move_t best_move_val;
        best_move_val.line = -1;
        best_move_val.score = 1;
        return best_move_val;
    }

    // If the board is full, return a move with a score of 0.
    if (is_full(board)) {
        move_t best_move_val;
        best_move_val.line = -1;
        best_move_val.score = 0;
        return best_move_val;
    }

    // Try all possible moves and choose the best one.
    int best_move_line = -1;
    int best_move_score = -1;
    int losing_moves_score[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            // Temporarily assign the best move as the current test move. If it turns out to be a winning move, break out of the loop.
            best_move_line = i;

            // Make the move.
            board[i] = player;

            player_t opponent = (player_t) player == RED ? BLUE : RED;

            // Compute the score for this move.
            move_t move_op = best_move(board, opponent);
            int score = move_op.score;

            if (has_won(board, opponent)) {
                losing_moves_score[i] = -1;
            } else {
                losing_moves_score[i] = 1;
            }

            // Undo the move.
            board[i] = NO;

            // Update the best move if necessary.
            if (score == -1) {
                best_move_score = 1;
                break;
            }
        }
    }

    // Return the best move.
    move_t best_move_val;
    best_move_val.line = best_move_line;
    best_move_val.score = best_move_score;

    if (best_move_score == -1) {
        best_move_val.line = find_max_index(losing_moves_score, 15);
    }

    return best_move_val;
}

void print_board(board_t board) {
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
            case RED: printf("R"); break;
            case BLUE: printf("B"); break;
            default: printf("_"); break;
        }
    }
    printf("\n");
}

int main() {
    // Initialize the board.
    board_t board;
    for (int i = 0; i < 15; i++) {
        board[i] = NO;
    }

    // Initialize the player.
    player_t user_choice = NO;
    char player_input;

    while (1) {
        // Get the player's choice.
        printf("Do you want to play first (R) or second (B)? ");
        scanf(" %c", &player_input);

        // Clear the input buffer to remove the newline character
        while ((getchar()) != '\n');

        // Convert the player's choice to the appropriate player type.
        if (player_input == 'r' || player_input == 'R') {
            user_choice = RED;
            break;
        } else if (player_input == 'b' || player_input == 'B') {
            user_choice = BLUE;
            break;
        } else {
            // Print an error message and ask the player to enter a valid choice.
            printf("Invalid player choice.\n\n");
        }
    }

    player_t computer_choice = (user_choice == RED) ? BLUE : RED;

    // Determine the winner.
    player_t winner = NO;

    if (user_choice == BLUE) {
        int line;
        line = randrange(0, 14);
        board[line] = computer_choice;

        // Print the board.
        printf("Computer's Move: %d\n", possible_lines[line]);
        print_board(board);
        printf("\n\n");
    }

    // Play the game until it is over.
    while (!is_full(board)) {
        int move;
        int vertex_input;

        while (1) {
            printf("Enter your move (e.g., 12, 13, 14): ");
            scanf("%d", &vertex_input);
            move = find_index(possible_lines, 15, vertex_input);

            if (move == -1) {
                printf("Invalid input. Please enter the smaller vertex number first. (e.g., 12, 13, 14)\n\n");
            } else if (board[move] != NO) {
                printf("Joined vertices cannot be reconnected. Specify two non-joined vertices.\n\n");
            } else {
                break;
            }
        }

        board[move] = user_choice;

        if (has_won(board, user_choice)) {
            print_board(board);
            winner = user_choice;
            break;
        }

        if (has_won(board, computer_choice)) {
            print_board(board);
            winner = computer_choice;
            break;
        }

        move_t best_move_val = best_move(board, computer_choice);

        // Make the move
        board[best_move_val.line] = computer_choice;

        printf("Computer's Move: %d\n", possible_lines[best_move_val.line]);
        print_board(board);
        printf("\n\n");

        if (has_won(board, computer_choice)) {
            winner = computer_choice;
            break;
        }
        if (has_won(board, user_choice)) {
            print_board(board);
            winner = user_choice;
            break;
        }
    }

    printf("\n");

    // Print the winner
    switch (winner) {
        case RED:
            if (user_choice == RED) {
                printf("You win! (Red)");
            }
            if (user_choice == BLUE) {
                printf("Computer wins! (Red)");
            }
            break;
        case BLUE:
            if (user_choice == BLUE) {
                printf("You win! (Blue)");
            }
            if (user_choice == RED) {
                printf("Computer wins! (Blue)");
            }
            break;
        case NO:
            printf("It's a draw!\n");
            break;
    }

    return 0;
}
