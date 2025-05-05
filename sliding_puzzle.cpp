#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// Function to print the puzzle board
void printBoard(const vector<int>& board, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int val = board[i * size + j];
            if (val == 0) {
                cout << "   "; // Represent empty space
            } else {
                cout << val << '\t';
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Function to check if the puzzle is solved
bool isSolved(const vector<int>& board, int size) {
    for (int i = 0; i < size * size - 1; ++i) {
        if (board[i] != i + 1) {
            return false;
        }
    }
    return board[size * size - 1] == 0;
}

// Function to find the position of the empty tile (0)
int findEmptyTile(const vector<int>& board) {
    for (size_t i = 0; i < board.size(); ++i) {
        if (board[i] == 0) {
            return static_cast<int>(i);
        }
    }
    return -1; // Should never happen
}

// Function to move a tile
bool moveTile(vector<int>& board, int size, int tile) {
    int emptyPos = findEmptyTile(board);
    int tilePos = -1;

    for (size_t i = 0; i < board.size(); ++i) {
        if (board[i] == tile) {
            tilePos = static_cast<int>(i);
            break;
        }
    }

    if (tilePos == -1) {
        cout << "Invalid move: Tile not found." << endl;
        return false;
    }

    // Check if the tile is adjacent to the empty tile
    int emptyRow = emptyPos / size;
    int emptyCol = emptyPos % size;
    int tileRow = tilePos / size;
    int tileCol = tilePos % size;

    if ((abs(emptyRow - tileRow) + abs(emptyCol - tileCol)) != 1) {
        cout << "Invalid move: Tile not adjacent to empty space." << endl;
        return false;
    }

    // Swap the tile and the empty tile
    swap(board[emptyPos], board[tilePos]);
    return true;
}

int main() {
    int size;
    cout << "Enter the size of the puzzle (e.g., 3 for a 3x3 puzzle): ";
    cin >> size;

    int numTiles = size * size;
    vector<int> board(numTiles);

    // Initialize the board with numbers 1 to size*size - 1 and 0 for the empty tile
    for (int i = 0; i < numTiles - 1; ++i) {
        board[i] = i + 1;
    }
    board[numTiles - 1] = 0;

    // Shuffle the board
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(board.begin(), board.end(), default_random_engine(seed));

    cout << "Welcome to the " << size << "x" << size << " Sliding Puzzle!" << endl;
    cout << "Use the numbers to slide tiles into the empty space (0)." << endl;
    cout << "Enter the number of the tile you want to move." << endl;

    while (!isSolved(board, size)) {
        printBoard(board, size);

        int tileToMove;
        cout << "Enter tile to move (or 0 to exit): ";
        cin >> tileToMove;

        if (tileToMove == 0) {
            cout << "Exiting game." << endl;
            break;
        }

        if (moveTile(board, size, tileToMove)) {
            // Move was successful
        }

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(256, '\n');
        }
    }

    if (isSolved(board, size)) {
        cout << "Congratulations! You solved the puzzle!" << endl;
        printBoard(board, size);
    }

    return 0;
}

