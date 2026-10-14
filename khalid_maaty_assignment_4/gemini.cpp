#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class SudokuBoard {
private:
    std::vector<std::vector<char> > grid;

public:
    SudokuBoard() : grid(9, std::vector<char>(9, '_')) {}

    // Loads the puzzle from a file, reading 81 valid characters ('1'-'9' or '_')
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        char ch;
        int count = 0;
        while (file >> ch && count < 81) {
            if (ch == '_' || (ch >= '1' && ch <= '9')) {
                int r = count / 9;
                int c = count % 9;
                grid[r][c] = ch;
                count++;
            }
        }
        return count == 81;
    }

    // Prints the board formatted neatly to the console
    void print() const {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                std::cout << grid[r][c] << (c == 8 ? "" : " ");
            }
            std::cout << "\n";
        }
    }

    // Finds the next unassigned (empty) cell marked with '_'
    bool findUnassignedLocation(int& row, int& col) const {
        for (row = 0; row < 9; ++row) {
            for (col = 0; col < 9; ++col) {
                if (grid[row][col] == '_') {
                    return true;
                }
            }
        }
        return false;
    }

    // Validates if placing a number at a given cell violates Sudoku rules
    bool isValidPlacement(int row, int col, char num) const {
        // Check row constraints
        for (int c = 0; c < 9; ++c) {
            if (grid[row][c] == num) return false;
        }
        
        // Check column constraints
        for (int r = 0; r < 9; ++r) {
            if (grid[r][col] == num) return false;
        }
        
        // Check 3x3 block constraints
        int startRow = row - (row % 3);
        int startCol = col - (col % 3);
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                if (grid[startRow + r][startCol + c] == num) return false;
            }
        }
        
        return true;
    }

    char getCell(int r, int c) const {
        return grid[r][c];
    }

    void setCell(int r, int c, char val) {
        grid[r][c] = val;
    }
};

class SudokuSolver {
private:
    SudokuBoard initialBoard;
    std::vector<SudokuBoard> solutions;

    // Recursive backtracking method to discover all valid solutions
    void solveRecursive(SudokuBoard& currentBoard) {
        int row, col;
        // Base case: if no unassigned cells remain, a solution is found
        if (!currentBoard.findUnassignedLocation(row, col)) {
            solutions.push_back(currentBoard);
            return;
        }

        // Try digits '1' through '9'
        for (char digit = '1'; digit <= '9'; ++digit) {
            if (currentBoard.isValidPlacement(row, col, digit)) {
                currentBoard.setCell(row, col, digit);
                
                // Recurse with the candidate placed
                solveRecursive(currentBoard);
                
                // Backtrack: reset cell to empty and try the next candidate
                currentBoard.setCell(row, col, '_');
            }
        }
    }

public:
    bool loadPuzzle(const std::string& filename) {
        solutions.clear();
        return initialBoard.loadFromFile(filename);
    }

    void solve() {
        solutions.clear();
        SudokuBoard tempBoard = initialBoard;
        solveRecursive(tempBoard);
    }

    // Displays file name, initial puzzle, and all discovered solutions (or none)
    void displayResults(const std::string& filename) const {
        std::cout << "========================================\n";
        std::cout << "Puzzle File: " << filename << "\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Initial Puzzle:\n";
        initialBoard.print();
        std::cout << "----------------------------------------\n";

        if (solutions.empty()) {
            std::cout << "No solution found.\n";
        } else {
            std::cout << "Found " << solutions.size() << " solution(s):\n";
            for (size_t i = 0; i < solutions.size(); ++i) {
                if (solutions.size() > 1) {
                    std::cout << "Solution " << (i + 1) << ":\n";
                }
                solutions[i].print();
                if (i + 1 < solutions.size()) {
                    std::cout << "----------------------------------------\n";
                }
            }
        }
        std::cout << "========================================\n\n";
    }
};

int main() {
    std::string puzzleFiles[] = {"puzzle1.txt", "puzzle2.txt", "puzzle3.txt", "puzzle4.txt", "puzzle5.txt"};
    
    for (const auto& filename : puzzleFiles) {
        SudokuSolver solver;
        if (solver.loadPuzzle(filename)) {
            solver.solve();
            solver.displayResults(filename);
        } else {
            std::cout << "Error: Could not open or read " << filename << ".\n\n";
        }
    }

    return 0;
}