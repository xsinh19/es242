#include <iostream>
using namespace std;

const int ROWS = 4;
const int COLS = 5;

enum Player {
    EMPTY = 0,
    RED,
    BLUE,
};

// Function to print the game board
void printBoard(Player board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == RED)
                cout << "R ";
            else if (board[i][j] == BLUE)
                cout << "B ";
            else
                cout << ". ";
        }
        cout << endl;
    }
    cout << "1 2 3 4 5" << endl;
}

// Function to check if a player has won
bool hasWon(Player board[ROWS][COLS], Player player) {
    // Check for horizontal wins
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == player && board[i][j + 1] == player &&
                board[i][j + 2] == player && board[i][j + 3] == player)
                return true;
        }
    }

    // Check for vertical wins
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == player && board[i + 1][j] == player &&
                board[i + 2][j] == player && board[i + 3][j] == player)
                return true;
        }
    }

    // Check for diagonal wins (from top-left to bottom-right)
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] == player && board[i + 1][j + 1] == player &&
                board[i + 2][j + 2] == player && board[i + 3][j + 3] == player)
                return true;
        }
    }

    // Check for diagonal wins (from top-right to bottom-left)
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 3; j < COLS; j++) {
            if (board[i][j] == player && board[i + 1][j - 1] == player &&
                board[i + 2][j - 2] == player && board[i + 3][j - 3] == player)
                return true;
        }
    }

    return false;
}

int main() {
    Player board[ROWS][COLS] = { {EMPTY} };
    Player currentPlayer = RED;

    int column;

    cout << "Connect Four Game" << endl;

    while (true) {
        printBoard(board);

        cout << (currentPlayer == RED ? "Red" : "Blue") << ", enter your move (1-5): ";
        cin >> column;

        // Check if the chosen column is valid
        if (column < 1 || column > 5) {
            cout << "Invalid column. Please choose a column between 1 and 5." << endl;
            continue;
        }

        // Find the first available row in the chosen column
        int row;
        for (row = ROWS - 1; row >= 0; row--) {
            if (board[row][column - 1] == EMPTY) {
                board[row][column - 1] = currentPlayer;
                break;
            }
        }

        // Check for a win
        if (hasWon(board, currentPlayer)) {
            printBoard(board);
            cout << (currentPlayer == RED ? "Red" : "Blue") << " wins! Congratulations!" << endl;
            break;
        }

        // Check for a draw
        bool isFull = true;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j] == EMPTY) {
                    isFull = false;
                    break;
                }
            }
            if (!isFull) break;
        }
        if (isFull) {
            printBoard(board);
            cout << "It's a draw!" << endl;
            break;
        }

        // Switch to the other player
        currentPlayer = (currentPlayer == RED) ? BLUE : RED;
    }

    return 0;
}
