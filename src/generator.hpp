#ifndef MINESWEEPER_GENERATOR_HPP
#define MINESWEEPER_GENERATOR_HPP

#include <vector>
#include "cell.hpp"
#include "field.hpp"

using namespace std;

namespace Generator {
    Field start_generating(Field field, Cell& initial_cell);
}

#endif //MINESWEEPER_GENERATOR_HPP
