#ifndef MINESWEEPER_CELL_HPP
#define MINESWEEPER_CELL_HPP

#include <SFML/Graphics.hpp>

using namespace std;

constexpr int CELL_TEXTURE_SIZE = 128;
constexpr int CELL_SIZE = 32; // pixels

enum class SolverStatus { none, opened, closed };
enum class CellStatus { hidden, flagged, revealed };
enum class CellType { unknown, mine, number };

struct Cell {
    int x_position;
    int y_position;
    SolverStatus solver_status = SolverStatus::none;
    CellStatus cell_status = CellStatus::hidden;
    CellType type = CellType::unknown;
    int value = -1; // 0 to 8
    int remaining_mines = -1;
    sf::Sprite sprite;

    Cell(int x, int y, sf::Texture& texture);
    void draw(bool highlighted);
	bool is_included_in(vector<Cell*>& list);
private:
    void highlight_if_necessary(bool highlighted);
    void remove_highlighting();
    void draw_hidden(bool highlighted);
    void draw_flagged(bool highlighted);
    void draw_revealed_default_debug_broken();
    void draw_revealed_mine();
    void draw_revealed_number();
};

#endif //MINESWEEPER_CELL_HPP
