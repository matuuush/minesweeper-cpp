#include "generator.hpp"

#include <algorithm>
#include <random>
#include "solver.hpp"
#include "player.hpp"

// return cells around the cell that was clicked on
// first during the time the field was still empty
// the returned cells have to be number cells
static vector<Cell*> get_unused_cells(Field& field, Cell& initial_cell) {
    vector<Cell*> unused_cells;
    for (auto&& row : field.cells) {
        for (auto&& current_cell : row) {
            if (current_cell.x_position <= initial_cell.x_position + 1
                && current_cell.x_position >= initial_cell.x_position - 1
                && current_cell.y_position <= initial_cell.y_position + 1
                && current_cell.y_position >= initial_cell.y_position - 1) {
                // cell is too close to the initial cell - do nothing
            } else {
                unused_cells.push_back(&current_cell);
            }
        }
    }
    return unused_cells;
}

// return bool whether the mine is on position [x, y] on a field
static bool is_mine_on_position(int x, int y, Field& field) {
    if (x < 0 || y < 0 || x + 1 > field.width || y + 1 > field.height) {
        return false;
    }
    return field.cells[x][y].type == CellType::mine;
}

// decide the final number of the cell
static void evaluate_cell(Cell& cell, Field& field) {
    if (cell.type != CellType::unknown) {
        return;
    }
    cell.type = CellType::number;
    cell.value = 0;
    cell.remaining_mines = 0;
    const static vector<int> places = {-1, 0, 1};
    for (auto x : places) {
        for (auto y : places) {
            if (is_mine_on_position(cell.x_position + x, cell.y_position + y, field)) {
                cell.value++;
                cell.remaining_mines++;
            }
        }
    }
}

// generate a field layout based on randomness
static void generate(Field& field, vector<Cell*>& unused_cells) {
    std::shuffle(std::begin(unused_cells), std::end(unused_cells), std::random_device());
    for (int i = 0; i < field.mines; ++i) {
        unused_cells[i]->type = CellType::mine;
    }
    for (auto&& row : field.cells) {
        for (auto&& current_cell : row) {
            evaluate_cell(current_cell, field);
        }
    }
}

// generate fields until it is solvable
// the field copy is intentional so that we can change the
// field multiple times without modifying the original field
Field Generator::start_generating(Field field, Cell& initial_cell) {

	int i = 1;
	while (true) {
		Field field_copy = field;
		vector<Cell*> unused_cells = get_unused_cells(field_copy, initial_cell);
		generate(field_copy, unused_cells);
		if (Solver::can_solve(i, field_copy, field_copy.cells[initial_cell.x_position][initial_cell.y_position])) {
			field.cells = field_copy.cells;

			// comment the for cycle for the solver showcase
			for (auto&& row : field.cells) {
				for (auto&& cell : row) {
					cell.cell_status = CellStatus::hidden;
				}
			}
			Player::mines_left = field.mines;
			break;
		}
		// degenerate
		for (auto&& cell_ptr : unused_cells) {
			cell_ptr->type = CellType::unknown;
		}
		++i;
	}
    // generated field is valid in this point
    return field;
}