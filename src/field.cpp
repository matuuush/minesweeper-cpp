#include "field.hpp"

#include <iostream>
#include <vector>
#include "generator.hpp"
#include "player.hpp"
#include "technic.hpp"

// create new field
Field::Field(int w, int h, int m) : width(w), height(h), mines(m) {
    Technic::load_textures(texture);
    for (int x_pos = 0; x_pos < w; ++x_pos) {
        vector<Cell> new_row;
        for (int y_pos = 0; y_pos < h; ++y_pos) {
            new_row.push_back(std::move(Cell(x_pos, y_pos, texture)));
        }
        cells.push_back(std::move(new_row));
    }
    uncovered_cells_left = w * h;
    Player::mines_left = m;
}

// return bool whether mouse points to some place on the field
bool Field::mouse_is_on_the_field(sf::Vector2i& mouse_position) const {
    return mouse_position.x >= 0 && mouse_position.x < width * CELL_SIZE
           && mouse_position.y >= 0 && mouse_position.y < height * CELL_SIZE;
}

// return cell that's pointed to by the mouse
Cell& Field::get_cell_from_mouse_position(sf::Vector2i& mouse_position) {
    int w = mouse_position.x / CELL_SIZE;
    int h = mouse_position.y / CELL_SIZE;
    // cout << w << ' ' << h << endl; // todo show mouse position on the field
    return cells[w][h];
}

// return bool whether the position exists on the field
bool Field::position_exists(int x, int y) {
    if (x < 0 || y < 0 || x + 1 > width || y + 1 > height) {
        return false;
    }
    return true;
}

// return bool whether there is a Cell of a Type on position [x, y] in the field
bool Field::is_thing_on_position(int x, int y, CellType wanted_type) {
    if (x < 0 || y < 0 || x + 1 > width || y + 1 > height) {
        return false;
    }
    return cells[x][y].type == wanted_type;
}

// reveal all the number cells surrounding the clicked value 0 cell
void Field::reveal_plane(Cell& cell) {
    if (cell.type != CellType::number) {
        return;
    }
    const static vector<int> places = {-1, 0, 1};
    for (auto x : places) {
        for (auto y : places) {
            if (is_thing_on_position(cell.x_position + x, cell.y_position + y, CellType::number)) {
                reveal_cell(cells[cell.x_position + x][cell.y_position + y]);
            }
        }
    }
}

// return bool whether the Cell has a neighbour that's revealed
bool Field::has_revealed_neighbour(Cell& current_cell) {
	const static vector<int> places = {-1, 0, 1};
	for (auto x : places) {
		for (auto y : places) {
			if (position_exists(current_cell.x_position + x, current_cell.y_position + y)) {
				Cell& result_cell = cells[current_cell.x_position + x][current_cell.y_position + y];
				if (result_cell.cell_status == CellStatus::revealed) {
					return true;
				}
			}
		}
	}
	return false;
}

// find out whether all mines have been correctly flagged and no
// other cell has been incorrectly accused of containing a mine
bool Field::all_mine_positions_correctly_flagged() {
	if (Player::mines_left != 0) {
		return false;
	}
	for (auto&& row : cells) {
		for (auto&& cell : row) {
			if (cell.type != CellType::mine) {
				continue;
			}
			if (cell.cell_status != CellStatus::flagged) {
				return false;
			}
		}
	}
	return true;
}

// control mechanism deciding whether the player has
// the chance to correctly determine the type of the Cell
bool Field::cell_should_not_be_known(Cell& current_cell) {
	return !has_revealed_neighbour(current_cell)
		&& !Technic::before_first_left_mouse_press
		&& !all_mine_positions_correctly_flagged();
}

// make Cell revealed and do accompanying necessities
void Field::reveal_cell(Cell& current_cell) {
    if (current_cell.cell_status != CellStatus::hidden) {
        return;
    }
	if (cell_should_not_be_known(current_cell)) {
		current_cell.type = CellType::mine;
	}
    current_cell.cell_status = CellStatus::revealed;
    uncovered_cells_left--;
    if (uncovered_cells_left == mines) {
        Player::won_game();
    }
    switch (current_cell.type) {
        case CellType::number:
            if (current_cell.value == 0) {
                reveal_plane(current_cell);
            }
            break;
        case CellType::mine:
            Player::game_over();
            break;
        default:
            break;
    }
}

// make the cell pointed to by the mouse revealed
void Field::reveal_cell(sf::Vector2i& mouse_position) {
    Cell& current_cell = get_cell_from_mouse_position(mouse_position);
    reveal_cell(current_cell);
}

// make the Cell flagged
void Field::flag_cell(Cell& current_cell) {
	switch (current_cell.cell_status) {
		case CellStatus::revealed:
			return;
		case CellStatus::hidden:
			current_cell.cell_status = CellStatus::flagged;
			Player::mines_left--;
			break;
		case CellStatus::flagged:
			current_cell.cell_status = CellStatus::hidden;
			Player::mines_left++;
			break;
	}
}

// make the Cell pointed to by the mouse flagged
void Field::flag_cell(sf::Vector2i& mouse_position) {
	Cell& current_cell = get_cell_from_mouse_position(mouse_position);
	flag_cell(current_cell);
}

// draw highlighted cell if mouse points to the cell
void Field::draw_highlighted_cell(sf::Vector2i& mouse_position) {
    Cell& current_cell = get_cell_from_mouse_position(mouse_position);
    current_cell.draw(true);
}

// draw all cells of the field
void Field::draw(sf::RenderWindow& window, sf::Vector2i& mouse_position) {
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {
            Cell& current_cell = cells[w][h];
            current_cell.draw(false);
            window.draw(current_cell.sprite);
        }
    }
    if (mouse_is_on_the_field(mouse_position)) {
        draw_highlighted_cell(mouse_position);
		Cell& current_cell = get_cell_from_mouse_position(mouse_position);
		window.draw(current_cell.sprite);
    }
}

// start generating the field after the first click somewhere on the field
void Field::start_generation(sf::Vector2i& mouse_position) {
    auto& current_cell = get_cell_from_mouse_position(mouse_position);
    current_cell.value = 0;
    current_cell.type = CellType::number;

    cells = Generator::start_generating(*this, current_cell).cells;
}

// return list of neighbours of a Cell in a field
vector<Cell*> Field::neighbourhood(Cell& cell) {
	vector<Cell*> result;
	const static vector<int> places = {-1, 0, 1};
	for (auto x : places) {
		for (auto y : places) {
			if (position_exists(cell.x_position + x, cell.y_position + y)) {
				Cell& result_cell = cells[cell.x_position + x][cell.y_position + y];
				result.push_back(&result_cell);
			}
		}
	}
	return result;
}

// return list of unrevealed neighbours of a Cell in a field
vector<Cell*> Field::unrevealed_neighbourhood(Cell& cell) {
	vector<Cell*> result;
	const static vector<int> places = {-1, 0, 1};
	for (auto x : places) {
		for (auto y : places) {
			if (position_exists(cell.x_position + x, cell.y_position + y)) {
				Cell& result_cell = cells[cell.x_position + x][cell.y_position + y];
				if (result_cell.cell_status == CellStatus::hidden) {
					result.push_back(&result_cell);
				}
			}
		}
	}
	return result;
}

// return the amount of unrevealed neighbours of a Cell in a field
size_t Field::unrevealed_neighbours_count(Cell& cell) {
    return unrevealed_neighbourhood(cell).size();
}

// lower the counts of remaining mines of the neighbouring cells of the Cell
void Field::lower_remaining_mines_count_around(Cell& cell) {
    const static vector<int> places = {-1, 0, 1};
    for (auto x : places) {
        for (auto y : places) {
            if (position_exists(cell.x_position + x, cell.y_position + y)) {
                Cell& result_cell = cells[cell.x_position + x][cell.y_position + y];
                if (result_cell.type == CellType::number) {
                    result_cell.remaining_mines--;
                }
            }
        }
    }
}

// flag all unrevealed neighbours of the Cell and lower
// the counts of remaining mines of the neighbouring cells
void Field::flag_unrevealed_neighbours_and_lower_remaining_mines(Cell& cell) {
    const static vector<int> places = {-1, 0, 1};
    for (auto x : places) {
        for (auto y : places) {
            if (position_exists(cell.x_position + x, cell.y_position + y)) {
                Cell& result_cell = cells[cell.x_position + x][cell.y_position + y];
                if (result_cell.cell_status == CellStatus::hidden) {
					flag_cell(result_cell);
                    result_cell.solver_status = SolverStatus::closed;
                    lower_remaining_mines_count_around(result_cell);
                }
            }
        }
    }
}

// return bool whether the List contains the Cell in a field
static bool list_contains_cell(vector<Cell*>& list, Cell*& cell) {
	for (auto&& list_cell : list) {
		if (cell == list_cell) {
			return true;
		}
	}
	return false;
}

// return special struct used for comparing the unrevealed neighbourhoods of 2 cells
// used in the solver for applying some specific rules for revealing cells
UnrevealedNeighbourhoodsComparison::UnrevealedNeighbourhoodsComparison(Field& field, Cell& current_cell, Cell& neighbour_cell) : cell1(&current_cell), cell2(&neighbour_cell) {
	vector<Cell*> cell1_unrevealed_neighbourhood = field.unrevealed_neighbourhood(current_cell);
	vector<Cell*> cell2_unrevealed_neighbourhood = field.unrevealed_neighbourhood(neighbour_cell);
	for (auto&& n_cell : cell1_unrevealed_neighbourhood) {
		if (!list_contains_cell(cell2_unrevealed_neighbourhood, n_cell)) {
			is_subset = false;
			break;
		}
	}
	cell1_hood_size = cell1_unrevealed_neighbourhood.size();
	cell2_hood_size = cell2_unrevealed_neighbourhood.size();
}