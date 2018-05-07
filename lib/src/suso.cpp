#include <iostream>
#include <set>

#include "suso.h"

std::ostream &operator<<(std::ostream &stream, const Sudoku &sudoku) {
    for (const auto &line : sudoku.field) {
        for (const auto &cell : line) {
            stream << cell << "  ";
        }
        stream << std::endl;
    }
    return stream;
}

std::vector<int> Sudoku::validNumbers(position pos) {
    std::set<int> posNumbers;
    std::vector<int> res;

    for(int i = 0; i<9; i++){
        posNumbers.insert(i+1);
    }

    for(int j = 0; j< 9; j++){
        posNumbers.erase(field[j][pos.y]);
    }

    for(int k = 0; k< 9; k++){
        posNumbers.erase(field[pos.x][k]);
    }

    int xBlock = (pos.x / 3) * 3;
    int yBlock = (pos.y / 3) *3;

    for (int l = 0; l < 3; l++){
        for(int m = 0; m < 3; m++){
            posNumbers.erase(field[xBlock + l][yBlock + m]);
        }
    }

    for (std::set<int>::iterator it=posNumbers.begin(); it!=posNumbers.end(); ++it){
        res.push_back(*it);
    }
    

    return res;
}
