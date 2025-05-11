#include "solver.hpp"

#include <iostream>
#include <memory>
#include <queue>

// add untouched neighbouring cells to queue
static void add_neighbours_to_queue(Field& field, Cell& current_cell, queue<Cell*>& queue) {
    const static vector<int> places = {-1, 0, 1};
    for (auto x : places) {
        for (auto y : places) {
            if (field.position_exists(current_cell.x_position + x, current_cell.y_position + y)) {
                Cell& result_cell = field.cells[current_cell.x_position + x][current_cell.y_position + y];
                if (result_cell.solver_status == SolverStatus::none) {
                    result_cell.solver_status = SolverStatus::opened;
                    queue.push(&result_cell);
                }
            }
        }
    }
}

// reveal all neighbouring number cells
static void reveal_while_possible(Cell& current_cell, Field& field) {
    const static vector<int> places = {-1, 0, 1};
    for (auto x : places) {
        for (auto y : places) {
            if (field.position_exists(current_cell.x_position + x, current_cell.y_position + y)) {
                Cell& result_cell = field.cells[current_cell.x_position + x][current_cell.y_position + y];
                if (result_cell.type == CellType::number) {// && result_cell.remaining_mines == 0) {
                    current_cell.cell_status = CellStatus::revealed;
                    // return;
                }
            }
        }
    }
}

// set the wanted_status to unrevealed cells belonging to the set difference of neighbourhoods of cell1 and cell2
static void set_set_difference_of_cell2_unr_n_and_cell1_unr_n_to(CellStatus wanted_status, Field& field, Cell& cell1, Cell& cell2) {
	vector<Cell*> cell1_unrevealed_neighbourhood = field.unrevealed_neighbourhood(cell1);
	vector<Cell*> cell2_unrevealed_neighbourhood = field.unrevealed_neighbourhood(cell2);
	for (auto&& n_cell : cell2_unrevealed_neighbourhood) {
		if (!n_cell->is_included_in(cell1_unrevealed_neighbourhood)) {
			// n_cell->solver_status = SolverStatus::closed;
			if (wanted_status == CellStatus::flagged) {
				field.flag_cell(*n_cell);
				field.lower_remaining_mines_count_around(*n_cell);
			} else {
				n_cell->cell_status = wanted_status;
			}
		}
	}

}

// find out whether some cells can be unrevealed based on the rules
static void apply_rules_on_cell(Cell& current_cell, Field& field, queue<Cell*>& solver_queue) {
    if (current_cell.type == CellType::number) {
        reveal_while_possible(current_cell, field);
        if (current_cell.value == 0 || current_cell.remaining_mines == 0) {
			// rule 1: reveal all neighbours because there is no mine around the cell
            current_cell.solver_status = SolverStatus::closed;
            add_neighbours_to_queue(field, current_cell, solver_queue);
        } else if (field.unrevealed_neighbours_count(current_cell) == current_cell.remaining_mines) {
			// rule 2: if there is as many mines as cells remaining around a cell -> flag all remaining neighbouring cells
            field.flag_unrevealed_neighbours_and_lower_remaining_mines(current_cell);
        } else {
            auto current_cell_neighbourhood = field.neighbourhood(current_cell);
            for (auto&& neighbour_cell : current_cell_neighbourhood) {
				UnrevealedNeighbourhoodsComparison neighbourhood_info(field, current_cell, *neighbour_cell);
				if (neighbourhood_info.is_subset) {
					if (current_cell.remaining_mines == neighbour_cell->remaining_mines) {
						// rule 3: raising scope doesn't raise number of mines -> new scope part is safe
						set_set_difference_of_cell2_unr_n_and_cell1_unr_n_to(CellStatus::revealed, field, current_cell, *neighbour_cell);
					} else if (neighbour_cell->remaining_mines - current_cell.remaining_mines == neighbourhood_info.cell2_hood_size - neighbourhood_info.cell1_hood_size) {
						// rule 4: raising scope with X cells raises number of expected mines with X -> flag the new scope part
						set_set_difference_of_cell2_unr_n_and_cell1_unr_n_to(CellStatus::flagged, field, current_cell, *neighbour_cell);
					}
				}
            }
        }
    }
}

// return bool whether the field is solvable
// int ordering_n used during debugging
bool Solver::can_solve(int ordering_n, Field& field, Cell& initial_cell) {
    queue<Cell*> solver_queue;
    solver_queue.push(&initial_cell);
    // field.reveal_cell(initial_cell);
    // while (!solver_queue.empty()) {
	int last_changed_i = 0; // used during debugging
	// the most I saw needed was 3233 (for 30x 20y 120m)
	// most of the loops needed were less than 2100
	int limit = 2100;
    for (int i = 0; i < limit; ++i) {
		if (solver_queue.empty()) {
			break;
		}
        Cell* current_cell = solver_queue.front();
        solver_queue.pop();
        apply_rules_on_cell(*current_cell, field, solver_queue);
        if (current_cell->solver_status != SolverStatus::closed) {
            solver_queue.push(current_cell);
        } else {
			field.uncovered_cells_left--;
			last_changed_i = i;
		}
    }
	// cout << ordering_n << ". last changed on: " << last_changed_i << endl;
	if (field.uncovered_cells_left == field.mines) {
		// cout << "ran until: " << limit << endl;
		// cout << "uncovered cells left: " << field.uncovered_cells_left << endl;
		// cout << "total mines: " << field.mines << endl;
		return true;
	}
	return false;
}