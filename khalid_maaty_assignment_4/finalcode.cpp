// file: finalcode.cpp
// Description: Object-oriented recursive Sudoku solver that prints every solution
// Outputs: Terminal
// Collaborators: ChatGPT
// Sources for the code: ChatGPT
// Name: Khalid Maaty
// Creation date: 10/13/26
// Revision date: 
// Revisions:

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

// chatGPT authored, edited by me
// holds the 9x9 grid and everything that touches it
class SudokuBoard {
private:
    int grid[9][9];
    string errorMessage;

public:
    // start with an empty board and no error
    SudokuBoard() { 
        errorMessage = "";
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                grid[row][col] = 0;
            }
        }
    }

    string getError() const { 
        return errorMessage;
    }

    // lets the solver place a digit or undo one
    void setCell(int row, int col, int value) {
        grid[row][col] = value;
    }

    // returns true if the digit is not already in the same row, column, or 3x3 block
    bool isSafe(int row, int col, int digit) const {
        for (int i = 0; i < 9; i++) {
            if (grid[row][i] == digit) {
                return false;
            }
            if (grid[i][col] == digit) {
                return false;
            }
        }

        // find the top-left corner of the 3x3 block this cell is in
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;

        // check every cell in that block
        for (int r = startRow; r < startRow + 3; r++) {
            for (int c = startCol; c < startCol + 3; c++) {
                if (grid[r][c] == digit) {
                    return false;
                }
            }
        }
        return true;
    }

    // finds the first blank cell and returns its position through row and col, false if the board is full
    bool findEmptyCell(int& row, int& col) const {
        for (row = 0; row < 9; row++) {
            for (col = 0; col < 9; col++) {
                if (grid[row][col] == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    // reads a puzzle file, and if it is not valid it saves the exact reason in errorMessage
    bool loadFromFile(const string& filename) {
        errorMessage = "";
        ifstream input(filename);

        if (!input.is_open()) {
            errorMessage = "the file could not be opened (missing or unreadable)";
            return false;
        }

        int cellCount = 0;
        char ch;
        while (input.get(ch)) {
            // spaces, tabs and newlines are not cells
            if (isspace(static_cast<unsigned char>(ch))) {
                continue;
            }

            if (cellCount >= 81) {
                errorMessage = "the file contains more than 81 cells";
                input.close();
                return false;
            }

            // turn the character into a cell value, anything other than _ or 1-9 is rejected
            if (ch == '_') {
                grid[cellCount / 9][cellCount % 9] = 0;
            } else if (ch >= '1' && ch <= '9') {
                grid[cellCount / 9][cellCount % 9] = ch - '0';
            } else {
                errorMessage = string("the file contains the invalid character '") + ch + "'";
                input.close();
                return false;
            }
            cellCount++;
        }
        input.close();

        if (cellCount != 81) {
            errorMessage = "the file contains " + to_string(cellCount) + " cells instead of 81";
            return false;
        }
        return true;
    }

    // prints the board with lines around each 3x3 block
    void printBoard() const {
        cout << "+-------+-------+-------+\n";
        for (int row = 0; row < 9; row++) {
            cout << "| ";
            for (int col = 0; col < 9; col++) {
                if (grid[row][col] == 0) {
                    cout << "_ ";
                } else {
                    cout << grid[row][col] << " ";
                }
                if ((col + 1) % 3 == 0) {
                    cout << "| ";
                }
            }
            cout << "\n";
            if ((row + 1) % 3 == 0) {
                cout << "+-------+-------+-------+\n";
            }
        }
    }

    // checks the starting clues against each other so a puzzle that already breaks the rules is rejected before searching
    bool hasValidClues() const {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                int digit = grid[row][col];

                // blank cells cannot conflict
                if (digit == 0) {
                    continue;
                }

                for (int i = 0; i < 9; i++) {
                    if (i != col && grid[row][i] == digit) {
                        return false;
                    }
                    if (i != row && grid[i][col] == digit) {
                        return false;
                    }
                }

                // checks for same digit somewhere else in this block
                int startRow = (row / 3) * 3;
                int startCol = (col / 3) * 3;
                for (int r = startRow; r < startRow + 3; r++) {
                    for (int c = startCol; c < startCol + 3; c++) {
                        if ((r != row || c != col) && grid[r][c] == digit) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
};


// chatgpt authored, edited by me
// runs the recursive backtracking search on the board
class SudokuSolver {
private:
    SudokuBoard board;          // separate copy of the puzzle that the search fills in
    long long solutionsFound;

    // fill the next blank, recurse, then undo it and try the next digit
    void solveAll() {
        int row, col;

        // no blanks left, so the board is a solution, print it right away instead of storing it
        if (!board.findEmptyCell(row, col)) {
            solutionsFound++;
            cout << "\nSolution #" << solutionsFound << ":\n";
            board.printBoard();
            return;
        }

        // trys each digit in the blank cell
        for (int digit = 1; digit <= 9; digit++) {
            if (board.isSafe(row, col, digit)) {
                board.setCell(row, col, digit);
                solveAll();
                board.setCell(row, col, 0);
            }
        }
    }

public:
    // copies the loaded board so the original stays unchanged
    SudokuSolver(const SudokuBoard& startBoard) {
        board = startBoard;
        solutionsFound = 0;
    }

    // run the search, then print the number of solutions or "No solution found."
    void solvePuzzle() {
        solutionsFound = 0;
        solveAll();
        if (solutionsFound == 0) {
            cout << "\nNo solution found.\n";
        } else {
            cout << "\nTotal solutions found: " << solutionsFound << "\n";
        }
    }
};

// chatGPT authored, edited by me
int main() {
    // goes through all puzzle files
    for (int i = 1; i <= 5; i++) {
        string filename = "puzzle" + to_string(i) + ".txt";

        cout << "\n=================================\n";
        cout << "File: " << filename << "\n";
        cout << "=================================\n";

        // load the file, if it is invalid print the exact reason and skip to the next file
        SudokuBoard board;
        if (!board.loadFromFile(filename)) {
            cout << "Unable to read " << filename << ": " << board.getError() << ".\n";
            continue;
        }

        // print the puzzle from the file
        cout << "\nOriginal puzzle:\n";
        board.printBoard();

        // clues that already conflict means there is no solution, so skip the search
        if (!board.hasValidClues()) {
            cout << "\nNo solution found.\n";
            continue;
        }

        // find and print every solution
        SudokuSolver solver(board);
        solver.solvePuzzle();
    }
    return 0;
}