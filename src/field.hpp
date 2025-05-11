#ifndef MINESWEEPER_FIELD_HPP
#define MINESWEEPER_FIELD_HPP

#include "cell.hpp"

using namespace std;

struct Field {
    int width; // cells
    int height; // cells
    int mines;
    int uncovered_cells_left;
    vector<vector<Cell>> cells;
    sf::Texture texture;

    Field(int w, int h, int m);
    bool mouse_is_on_the_field(sf::Vector2i& mouse_position) const;
    Cell& get_cell_from_mouse_position(sf::Vector2i& mouse_position);
    bool position_exists(int x, int y);
    bool is_thing_on_position(int x, int y, CellType wanted_type);
    void reveal_plane(Cell& cell);
    void reveal_cell(Cell& current_cell);
    void reveal_cell(sf::Vector2i& mouse_position);
	void flag_cell(Cell& current_cell);
    void flag_cell(sf::Vector2i& mouse_position);
    void draw(sf::RenderWindow& window, sf::Vector2i& mouse_position);
    void start_generation(sf::Vector2i& mouse_position);
	vector<Cell*> unrevealed_neighbourhood(Cell& cell);
	vector<Cell*> neighbourhood(Cell& cell);
    size_t unrevealed_neighbours_count(Cell& cell);
    void flag_unrevealed_neighbours_and_lower_remaining_mines(Cell& cell);
    void lower_remaining_mines_count_around(Cell& cell);
private:
    void draw_highlighted_cell(sf::Vector2i& mouse_position);
	bool has_revealed_neighbour(Cell& current_cell);
	bool cell_should_not_be_known(Cell& current_cell);
	bool all_mine_positions_correctly_flagged();
};

struct UnrevealedNeighbourhoodsComparison {
	Cell* cell1;
	Cell* cell2;
	// the unrevealed neighbourhood of cell 1 is a subset
	// of the unrevealed neighbourhood of cell 2
	bool is_subset = true;
	// the size of the unrevealed neighbourhood of cells
	size_t cell1_hood_size;
	size_t cell2_hood_size;

	UnrevealedNeighbourhoodsComparison(Field& field, Cell& cell1, Cell& cell2);
};

#endif //MINESWEEPER_FIELD_HPP
