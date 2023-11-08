#include <cstdio>
#include <vector>
#include <array>
#include <algorithm>
#include <tuple>

#include "queue.hpp"

struct Board {
    int elements[3][3];
};

long order(const Board& b) {
    long o = 0;
    int power = 1;
    for (int r = 2; r >= 0; --r) {
        for (int c = 2; c >= 0; --c) {
            o += b.elements[r][c] * power;
            power *= 7;
        }
    }
    return o;
}

void readBoard(Board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            scanf("%d", &b.elements[r][c]);
        }
    }
}

std::tuple<int, int> findSpace(const Board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (b.elements[r][c] == 0) return { r, c };
        }
    }
    assert(0);
    return { 0, 0 };
}

Board moveUp(const Board& b) {
    const std::tuple<int, int> space = findSpace(b);
    const int r = std::get<0>(space);
    const int c = std::get<1>(space);
    if (r == 2) return b;
    Board updatedBoard = b;
    switch (updatedBoard.elements[r + 1][c]) {
        case 1: updatedBoard.elements[r][c] = 3; break;
        case 2: updatedBoard.elements[r][c] = 2; break;
        case 3: updatedBoard.elements[r][c] = 6; break;
        case 4: updatedBoard.elements[r][c] = 4; break;
        case 5: updatedBoard.elements[r][c] = 1; break;
        case 6: updatedBoard.elements[r][c] = 5; break;
    }
    updatedBoard.elements[r + 1][c] = 0;
    return updatedBoard;
}

Board moveDown(const Board& b) {
    const std::tuple<int, int> space = findSpace(b);
    const int r = std::get<0>(space);
    const int c = std::get<1>(space);
    if (r == 0) return b;
    Board updatedBoard = b;
    switch (updatedBoard.elements[r - 1][c]) {
        case 1: updatedBoard.elements[r][c] = 5; break;
        case 2: updatedBoard.elements[r][c] = 2; break;
        case 3: updatedBoard.elements[r][c] = 1; break;
        case 4: updatedBoard.elements[r][c] = 4; break;
        case 5: updatedBoard.elements[r][c] = 6; break;
        case 6: updatedBoard.elements[r][c] = 3; break;
    }
    updatedBoard.elements[r - 1][c] = 0;
    return updatedBoard;
}

Board moveLeft(const Board& b) {
    const std::tuple<int, int> space = findSpace(b);
    const int r = std::get<0>(space);
    const int c = std::get<1>(space);
    if (c == 2) return b;
    Board updatedBoard = b;
    switch (updatedBoard.elements[r][c + 1]) {
        case 1: updatedBoard.elements[r][c] = 2; break;
        case 2: updatedBoard.elements[r][c] = 6; break;
        case 3: updatedBoard.elements[r][c] = 3; break;
        case 4: updatedBoard.elements[r][c] = 1; break;
        case 5: updatedBoard.elements[r][c] = 5; break;
        case 6: updatedBoard.elements[r][c] = 4; break;
    }
    updatedBoard.elements[r][c + 1] = 0;
    return updatedBoard;
}

Board moveRight(const Board& b) {
    const std::tuple<int, int> space = findSpace(b);
    const int r = std::get<0>(space);
    const int c = std::get<1>(space);
    if (c == 0) return b;
    Board updatedBoard = b;
    switch (updatedBoard.elements[r][c - 1]) {
        case 1: updatedBoard.elements[r][c] = 4; break;
        case 2: updatedBoard.elements[r][c] = 1; break;
        case 3: updatedBoard.elements[r][c] = 3; break;
        case 4: updatedBoard.elements[r][c] = 6; break;
        case 5: updatedBoard.elements[r][c] = 5; break;
        case 6: updatedBoard.elements[r][c] = 2; break;
    }
    updatedBoard.elements[r][c - 1] = 0;
    return updatedBoard;
}

bool isSame(const Board& a, const Board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (a.elements[r][c] != b.elements[r][c]) return false;
        }
    }
    return true;
}

enum Move { LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4 };

/*
 * Return a shortest path from src to dest.
 */
#define MAX_ORDER (40353600)
std::vector<int> solve(const Board& src, const Board& dest) {
    queue<Board, MAX_ORDER> q;
    int visited[MAX_ORDER];
    Board parent[MAX_ORDER];

    enqueue(q, src);
    visited[order(src)] = LEFT;

    while (!is_empty(q)) {
        Board u = dequeue(q);
        if (isSame(u, dest)) {
            /* Return the moves to get to u from src. */
            std::vector<int> moves;
            Board c = u;
            int o = order(c);
            while (!isSame(c, src)) {
                moves.push_back(visited[o]);
                c = parent[o];
                o = order(c);
            }
            std::reverse(moves.begin(), moves.end());
            return moves;
        }

        Board a = moveUp(u);
        Board b = moveDown(u);
        Board c = moveLeft(u);
        Board d = moveRight(u);

        int aOrder = order(a);
        int bOrder = order(b);
        int cOrder = order(c);
        int dOrder = order(d);

        if (!visited[aOrder]) {
            visited[aOrder] = UP;
            parent[aOrder] = u;
            enqueue(q, a);
        }
        if (!visited[bOrder]) {
            visited[bOrder] = DOWN;
            parent[bOrder] = u;
            enqueue(q, b);
        }
        if (!visited[cOrder]) {
            visited[cOrder] = LEFT;
            parent[cOrder] = u;
            enqueue(q, c);
        }
        if (!visited[dOrder]) {
            visited[dOrder] = RIGHT;
            parent[dOrder] = u;
            enqueue(q, d);
        }
    }

    printf("\nBoard is not solvable\n");
    return { 0 };
}

void printMoves(const std::vector<int>& moves) {
    for (auto m : moves) {
        switch (m) {
            case LEFT: printf("L "); break;
            case RIGHT: printf("R "); break;
            case UP: printf("U "); break;
            case DOWN: printf("D "); break;
        }
    }
    printf("\n");
}

int main() {
    Board src, dest;

    readBoard(src);
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            dest.elements[r][c] = 1;
        }
    }
    dest.elements[1][1] = 0;

    auto moves = solve(src, dest);
    printMoves(moves);

    return 0;
}
