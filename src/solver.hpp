#ifndef MINESWEEPER_SOLVER_HPP
#define MINESWEEPER_SOLVER_HPP

#include "field.hpp"

namespace Solver {

    bool can_solve(int ordering_n, Field& field, Cell& initial_cell);
}

#endif //MINESWEEPER_SOLVER_HPP
