#include "cell.hpp"

// create new cell
Cell::Cell(int x, int y, sf::Texture& texture){
    x_position = x;
    y_position = y;
    sprite.setPosition((float)x * CELL_SIZE, (float)y * CELL_SIZE);
    float scale = CELL_SIZE / (float)CELL_TEXTURE_SIZE;
    sprite.setScale(scale, scale);
    sprite.setTexture(texture);
}

// draw highlighted cell if mouse points to the cell
void Cell::highlight_if_necessary(bool highlighted) {
    constexpr int NO_CHANGE = 0xFF;
    constexpr int HIGHLIGHT = 0x55;
    if (highlighted) {
        sprite.setColor(sf::Color(HIGHLIGHT, HIGHLIGHT, NO_CHANGE, HIGHLIGHT));
    } else {
        sprite.setColor(sf::Color(NO_CHANGE, NO_CHANGE, NO_CHANGE, NO_CHANGE));
    }
}

// remove highlighting if mouse doesn't point on the cell
void Cell::remove_highlighting() {
    constexpr int NO_CHANGE = 0xFF;
    sprite.setColor(sf::Color(NO_CHANGE, NO_CHANGE, NO_CHANGE));
}

// draw the cell based on its status and type
void Cell::draw(bool highlighted) {
    switch (cell_status) {
        case CellStatus::hidden:
            draw_hidden(highlighted);
            return;
        case CellStatus::flagged:
            draw_flagged(highlighted);
			return;
        case CellStatus::revealed:
            switch (type) {
                case CellType::mine:
                    draw_revealed_mine();
                    break;
                case CellType::number:
                    draw_revealed_number();
                    break;
                default:
                    draw_revealed_default_debug_broken();
                    break;
            }
            break;
    }
}

// draw texture for hidden cell
void Cell::draw_hidden(bool highlighted) {
    sprite.setTextureRect(sf::IntRect(0, 0, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    highlight_if_necessary(highlighted);
}

// draw texture for flagged cell
void Cell::draw_flagged(bool highlighted) {
    sprite.setTextureRect(sf::IntRect(CELL_TEXTURE_SIZE, 0, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    highlight_if_necessary(highlighted);
}

// draw texture broken states
// shouldn't happen
void Cell::draw_revealed_default_debug_broken() {
    // can't happen, do default
    sprite.setTextureRect(sf::IntRect(CELL_TEXTURE_SIZE / 2, CELL_TEXTURE_SIZE / 2, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    remove_highlighting();
}

// draw texture for revealed number cell
void Cell::draw_revealed_number() {
    switch (value) {
        case 0:
            sprite.setTextureRect(sf::IntRect(CELL_TEXTURE_SIZE * 3, 0, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            break;
        case 1: case 2: case 3: case 4:
            sprite.setTextureRect(sf::IntRect(CELL_TEXTURE_SIZE * (value - 1), // 1 is first texture in the second row
                                              CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            break;
        case 5: case 6: case 7: case 8:
            sprite.setTextureRect(sf::IntRect(CELL_TEXTURE_SIZE * (value - 5), // 5 is first texture in the third row
                                              CELL_TEXTURE_SIZE * 2, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            break;
        default: // -1 (unset)
            Cell::draw_revealed_default_debug_broken();
            break;
    }
    remove_highlighting();
}

// draw texture for revealed mine cell
void Cell::draw_revealed_mine() {
    sprite.setTextureRect(sf::IntRect(CELL_TEXTURE_SIZE * 2, 0, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    remove_highlighting();
}

// return bool whether the cell is included in a list of cells
bool Cell::is_included_in(vector<Cell*>& list) {
	for (auto&& list_cell : list) {
		if (x_position == list_cell->x_position && y_position == list_cell->y_position) {
			return true;
		}
	}
	return false;
}