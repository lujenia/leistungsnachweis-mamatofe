#ifndef __SUSO_H__
#define __SUSO_H__

#include <array>
#include <vector>
#include <chrono>

enum Mode {
    DEFAULT,
    NAKED,
    HIDDEN,
    BACKTRACKING,
    LAST_RESORT_BACKTRACKING
};

/*!
 * A position contains the coordinates of one cell inside a Sudoku field.
 */
struct Position {
    /*!
     * The x-component of the coordinate. This can also be described as the
     * number of the column.
     */
    int x;
    /*!
     * The y-component of the coordinate. This can also be described as the
     * number of the row.
     */
    int y;
};

/*!
 * A invalid position.
 */
const Position INVALID_POS = {-1, -1};

/*!
 * Metrics contain information about a single solving process.
 */
struct Metrics {
    /*!
     * Number of cells already filled in the loaded Sudoku
     */
    int given;
    /*!
     * Number of cells filled by searching for naked singles
     */
    int solvedNaked;
    /*!
     * Number of cells filled by searching for hidden singles
     */
    int solvedHidden;
    /*!
     * Number of cells filled by the backtracking algorithm.
     */
    int solvedBacktracking;
    /*!
     * The amount of time that was needed to solve the Sudoku
     */
    std::chrono::duration<double, std::milli> duration;
};

std::ostream &operator<<(std::ostream &stream, const Metrics &metrics);

/*!
 * A Sudoku represents the complete field of one Sudoku. It also has functions
 * to solve the Sudoku using different algorithms.
 */
class Sudoku {
private:
    std::array<std::array<int, 9>, 9> field;
    Metrics metrics;

    /*!
     * Returns if a given cell is not filled with a number.
     * @param pos the position to check
     * @return true if the cell is empty, false otherwise
     */
    bool isEmpty(Position pos);

    /*!
     * Writes a number into the Sudoku field at a given position.
     * No checks of the number are performed.
     * @param pos where to insert the number
     * @param num the number to insert
     */
    void insertNumber(Position pos, int num);

    /*!
     * Returns if the sudoku has one or more empty cell to fill.
     * @param pos reference to the position of the next empty cell
     * @return true if at least one cell is empty
     */
    bool getNextEmptyCell(Position &pos);

public:
    /*!
     * Creates a new empty Sudoku.
     */
    Sudoku() = default;

    /*!
     * Get information about how this Sudoku has been solved. This is only
     * filled completely after successfully solving it.
     * @return Metrics about the previous solving process
     */
    Metrics getSolvingMetrics();

    /*!
     * This function returns all numbers that could be placed in the cell at the
     * given position so they do not conflict with the numbers already placed in
     * the field.
     * @param pos the position to check
     * @return all possible numbers for the position
     */
    std::vector<int> validNumbers(Position pos);

    /*!
     * This function reads a file into the sudoku field.
     * @param path the path of the Sudoku file
     * @return true if the Sudoku stored in the field is correct and could be read successfully
     */
    bool loadFromFile(std::string path);

    /*!
     * This function tries to fill cells by searching for naked singles. These
     * are cells where only one number is valid. The function iterates over the
     * whole field and fills in the number when it finds a naked single.
     * @return true if the field has been changed (at least one number filled
     * into a cell), false otherwise
     */
    bool solveNakedSingles() noexcept(false);

    /*!
     * This function tries to fill cells by searching for hidden singles. This
     * is a cell that is the only one in a group, that could contain a specific
     * number. The function iterates over all groups and fills in the numbers
     * when it finds hidden singles inside a group.
     * @return true if the field has been changed (at least one number filled
     * into a cell), false otherwise
     */
    bool solveHiddenSingles() noexcept(false);

    /*!
     * This function can be used to print out the current state of the Sudoku.
     * Cells that are already filled with a number are printed as the number,
     * empty cells show up as zero.
     */
    friend std::ostream &operator<<(std::ostream &stream, Sudoku &sudoku);

    /*!
     * This function can be used to compare two sudoku fields.
     * @param lhs the sudoku to compare with the other sudoku
     * @param rhs other sudoku to compare with the sudoku
     * @return true if the every cell is equal to the same cell in the other sudoku
     */
    friend bool operator==(const Sudoku &lhs, const Sudoku &rhs);

    /*!
     * This function solves the sudoku recursively by using the backtracking algorithm.
     * @return true if the sudoku is solved
     */
    bool solveBacktracking();

    /*!
     * This function checks if the sudoku is solvable.
     * If the sudoku is not solvable, the function will throw an exception.
     */
    void checkSolvability() const noexcept(false);

    /*!
     * This function solves the sudoku with the given algorithms.
     * @param algorithm way to solve the sudoku
     * @return true if sudoku is solved
     */
    void solve(Mode algorithm) noexcept(false);

};

#endif // __SUSO_H__