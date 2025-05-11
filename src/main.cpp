#include "main.hpp"

#include "technic.hpp"
#include "field.hpp"
#include "settings.hpp"

int main() {
	Settings::init_options();
	// get options
	int width = Settings::options.at(OptionType::width).input;
	int height = Settings::options.at(OptionType::height).input;
	int mines = Settings::options.at(OptionType::mines).input;
	// fix number of mines is the original value is too big
	if (mines > width * height * 0.2) {
		mines = width * height * 0.2;
	}
	// initialise field
    Field field(width, height, mines);
	// init window
    sf::RenderWindow window(sf::VideoMode(field.width * CELL_SIZE, (field.height + 1) * CELL_SIZE), "Minesweeper");
    Technic::info_text_x = (float)(field.width - 1) * CELL_SIZE;
    Technic::info_text_y = (float)field.height * CELL_SIZE;
	// so that user can click anywhere on the field anytime he wants
    while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {}

    while (window.isOpen()) {
		// handle all events
        Technic::handle_events(window);
		window.requestFocus();
        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
        Technic::handle_mouse_press(field, mouse_position, window);

        window.clear(); // beginning of drawing
        field.draw(window, mouse_position);
        Technic::draw_player_info(window);
		Technic::draw_buttons(window);
        window.display(); // end of drawing
    }
    return 0;
}