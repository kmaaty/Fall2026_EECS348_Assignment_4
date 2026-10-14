#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

class Sudoku {
private:
    int grid[9][9];
    long long solutionsFound;

    // Check whether a digit can be placed in a cell.
    bool isSafe(int row, int col, int digit) const {
        // Check the row and column.
        for (int i = 0; i < 9; i++) {
            if (grid[row][i] == digit) {
                return false;
            }

            if (grid[i][col] == digit) {
                return false;
            }
        }

        // Find the top-left cell of the 3x3 block.
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;

        // Check all cells in that 3x3 block.
        for (int r = startRow; r < startRow + 3; r++) {
            for (int c = startCol; c < startCol + 3; c++) {
                if (grid[r][c] == digit) {
                    return false;
                }
            }
        }

        return true;
    }

    // Find the next empty cell.
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

    // Recursive backtracking: find and print ALL solutions.
    void solveAll() {
        int row, col;

        // Base case: no empty cells remain.
        if (!findEmptyCell(row, col)) {
            solutionsFound++;

            cout << "\nSolution #" << solutionsFound << ":\n";
            printBoard();

            return;
        }

        // Try each possible digit.
        for (int digit = 1; digit <= 9; digit++) {
            if (isSafe(row, col, digit)) {
                // Make a choice.
                grid[row][col] = digit;

                // Recursively solve the remaining puzzle.
                solveAll();

                // Backtrack: undo the choice and try another digit.
                grid[row][col] = 0;
            }
        }
    }

public:
    // Constructor: initialize the board.
    Sudoku() {
        solutionsFound = 0;

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                grid[row][col] = 0;
            }
        }
    }

    // Read the puzzle file.
    // Underscores represent empty cells.
    bool loadFromFile(const string& filename) {
        ifstream input(filename);

        if (!input.is_open()) {
            return false;
        }

        int cellCount = 0;
        char ch;

        while (input.get(ch)) {
            // Ignore spaces, tabs, and newlines.
            if (isspace(static_cast<unsigned char>(ch))) {
                continue;
            }

            // Reject files containing more than 81 cells.
            if (cellCount >= 81) {
                input.close();
                return false;
            }

            // Convert the next character into a cell value.
            if (ch == '_') {
                grid[cellCount / 9][cellCount % 9] = 0;
            } else if (ch >= '1' && ch <= '9') {
                grid[cellCount / 9][cellCount % 9] = ch - '0';
            } else {
                input.close();
                return false;
            }

            cellCount++;
        }

        input.close();

        // A Sudoku puzzle must contain exactly 81 cells.
        return cellCount == 81;
    }

    // Print the current Sudoku board.
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

    // Check whether the original clues conflict.
    bool hasValidClues() const {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                int digit = grid[row][col];

                // Empty cells cannot create clue conflicts.
                if (digit == 0) {
                    continue;
                }

                // Check the rest of this row and column.
                for (int i = 0; i < 9; i++) {
                    if (i != col && grid[row][i] == digit) {
                        return false;
                    }

                    if (i != row && grid[i][col] == digit) {
                        return false;
                    }
                }

                // Check the rest of this 3x3 block.
                int startRow = (row / 3) * 3;
                int startCol = (col / 3) * 3;

                for (int r = startRow; r < startRow + 3; r++) {
                    for (int c = startCol; c < startCol + 3; c++) {
                        if ((r != row || c != col) &&
                            grid[r][c] == digit) {
                            return false;
                        }
                    }
                }
            }
        }

        return true;
    }

    // Solve the puzzle and print all solutions.
    void solvePuzzle() {
        solutionsFound = 0;

        solveAll();

        if (solutionsFound == 0) {
            cout << "\nNo solution found.\n";
        } else {
            cout << "\nTotal solutions found: "
                 << solutionsFound << "\n";
        }
    }
};

int main() {
    // Process all five puzzle files.
    for (int i = 1; i <= 5; i++) {
        string filename = "puzzle" + to_string(i) + ".txt";

        cout << "\n=================================\n";
        cout << "File: " << filename << "\n";
        cout << "=================================\n";

        // Create a Sudoku object for this puzzle.
        Sudoku puzzle;

        // Load the puzzle from its file.
        if (!puzzle.loadFromFile(filename)) {
            cout << "Unable to read " << filename
                 << ". Check that the file exists and "
                 << "contains exactly 81 valid cells.\n";
            continue;
        }

        // Print the original puzzle.
        cout << "\nOriginal puzzle:\n";
        puzzle.printBoard();

        // Reject puzzles with conflicting starting clues.
        if (!puzzle.hasValidClues()) {
            cout << "\nNo solution found.\n";
            continue;
        }

        // Find and print every solution.
        puzzle.solvePuzzle();
    }

    return 0;
}