#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

enum {
    EMPTY = 0,
    RED,
    BLUE,
};

typedef char board_t[4][5];
typedef char player_t;

int has_won(board_t board, player_t player) {
    // Check for a win horizontally
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row][col + 1] == player &&
                board[row][col + 2] == player &&
                board[row][col + 3] == player) {
                return 1;
            }
        }
    }

    // Check for a win vertically
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 1; row++) {
            if (board[row][col] == player &&
                board[row + 1][col] == player &&
                board[row + 2][col] == player &&
                board[row + 3][col] == player) {
                return 1;
            }
        }
    }

    // Check for a win diagonally (top-left to bottom-right)
    for (int row = 0; row < 1; row++) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row + 1][col + 1] == player &&
                board[row + 2][col + 2] == player &&
                board[row + 3][col + 3] == player) {
                return 1;
            }
        }
    }

    // Check for a win diagonally (top-right to bottom-left)
    for (int row = 0; row < 1; row++) {
        for (int col = 3; col < 5; col++) {
            if (board[row][col] == player &&
                board[row + 1][col - 1] == player &&
                board[row + 2][col - 2] == player &&
                board[row + 3][col - 3] == player) {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(board_t board) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            if (board[row][col] == EMPTY) {
                return 0;  // Not full
            }
        }
    }
    return 1;  // Full
}

typedef struct {
    int col;
    int score;
} move_t;

int can_win(board_t board, int col, player_t player) {
    int row;
    for (row = 3; row >= 0; row--) {
        if (board[row][col] == EMPTY) {
            board[row][col] = player;
            if (has_won(board, player)) {
                board[row][col] = EMPTY;
                return 1;
            }
            board[row][col] = EMPTY;
        }
    }
    return 0;
}

move_t best_move(board_t board, player_t player) {
    move_t move;
    move.col = -1;
    move.score = -1;

    for (int col = 0; col < 5; col++) {
        if (can_win(board, col, player)) {
            move.col = col;
            return move;
        }
    }

    for (int col = 0; col < 5; col++) {
        if (can_win(board, col, (player == RED) ? BLUE : RED)) {
            move.col = col;
            return move;
        }
    }

    while (move.col == -1) {
        int random_col = rand() % 5;
        for (int row = 3; row >= 0; row--) {
            if (board[row][random_col] == EMPTY) {
                move.col = random_col;
                break;
            }
        }
    }
    return move;
}

void print_board(board_t board) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            char symbol;
            if (board[row][col] == RED) {
                symbol = 'R';
            } else if (board[row][col] == BLUE) {
                symbol = 'B';
            } else {
                symbol = '-';
            }
            printf("%c ", symbol);
        }
        printf("\n");
    }
    printf("1 2 3 4 5\n"); // Column numbers
}

int main() {
    board_t board = {{EMPTY}};
    player_t userColor, compColor;
    int userTurn = 0;

    printf("Welcome to Connect 4!\n");

    printf("Choose your color (R or B): ");
    char userColorChar;
    scanf(" %c", &userColorChar);

    if (userColorChar == 'R' || userColorChar == 'r') {
        userColor = RED;
        compColor = BLUE;
    } else {
        userColor = BLUE;
        compColor = RED;
        userTurn = 1; // Computer goes first if user chooses Blue
    }

    print_board(board);

    while (1) {
        if (userTurn) {
            int userMove;
            printf("Your move (1-5): ");
            scanf("%d", &userMove);

            // Check if the selected column is valid
            if (userMove < 1 || userMove > 5) {
                printf("Invalid move. Please choose a column between 1 and 5.\n");
                continue;
            }

            int col = userMove - 1;
            int row;
            for (row = 3; row >= 0; row--) {
                if (board[row][col] == EMPTY) {
                    board[row][col] = userColor;
                    break;
                }
            }

            if (row == -1) {
                printf("Column is full. Try another one.\n");
                continue;
            }

            userTurn = 0;
        } else {
            // Computer's turn
            move_t compMove = best_move(board, compColor);
            int col = compMove.col;
            int row;
            for (row = 3; row >= 0; row--) {
                if (board[row][col] == EMPTY) {
                    board[row][col] = compColor;
                    break;
                }
            }

            printf("Computer's move:\n");
            print_board(board);

            if (has_won(board, compColor)) {
                printf("Computer wins!\n");
                break;
            }

            userTurn = 1;
        }

        print_board(board);

        if (has_won(board, userColor)) {
            printf("You win!\n");
            break;
        }

        if (is_full(board)) {
            printf("It's a draw!\n");
            break;
        }
    }

    return 0;
}
