#include "technic.hpp"

#include <filesystem>
#include "main.hpp"
#include "player.hpp"
#include "settings.hpp"

// Serves as a helper function for loading the textures of cells
void Technic::load_textures(sf::Texture& texture) {

	// cout << "path: " << filesystem::current_path() << endl;

	bool on_windows = filesystem::current_path().string().ends_with("Debug");

    if (!texture.loadFromFile(on_windows ? windows_textures_path : linux_textures_path)) {
        cout << "texture loading error" << endl;
        // todo error...
        throw;
    }
    if (!font.loadFromFile(on_windows ? windows_font_path : linux_font_path)) {
        cout << "font loading error" << endl;
        // todo error...
        throw;
    }
	if (!Settings::font.loadFromFile(on_windows ? windows_font_path : linux_font_path)) {
		cout << "font loading error in settings window" << endl;
		// todo error...
		throw;
	}
}

// Serves as a helper function for handling the closing of the window with the button X
void Technic::handle_events(sf::RenderWindow& window) {

    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

// Serves as a function for reloading the game and its initial variable values
void Technic::end_game(sf::RenderWindow& window) {

	window.close();
	Player::game_ended = false;
	before_first_left_mouse_press = true;
	show_current_time = false;
	Player::time = 0;
	main();
}

// Handler for dealing with pressing the left mouse button
void Technic::handle_left_mouse_button_press(Field& field, sf::Vector2i& mouse_position, sf::RenderWindow& window) {

    if (Player::game_ended) {
		end_game(window);
    }
    if (!before_first_left_mouse_press) {
        field.reveal_cell(mouse_position);
    } else {
        field.start_generation(mouse_position);
        field.reveal_cell(mouse_position);
		before_first_left_mouse_press = false;
		clock.restart();
        show_current_time = true;
    }
    while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {}
}

// Function for finding out whether the lower bar of the window was pressed
bool settings_button_pressed(sf::Vector2i& mouse_position, Field& field) {

	return mouse_position.y / CELL_SIZE == field.height;
}

// Handler for dealing with pressing the mouse buttons
void Technic::handle_mouse_press(Field& field, sf::Vector2i& mouse_position, sf::RenderWindow& window) {

	if (!window.hasFocus()) {
		return;
	}
	bool left_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (!field.mouse_is_on_the_field(mouse_position)) {
		if (left_pressed && settings_button_pressed(mouse_position, field)) {
			Settings::open_window(field, window);
		}
		return;
	}
    if (left_pressed) {
        handle_left_mouse_button_press(field, mouse_position, window);
        return;
    }
    bool right_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
    if (right_pressed) {
        if (before_first_left_mouse_press || Player::game_ended) {
            // before field generation
            return;
        }
        field.flag_cell(mouse_position);
        while (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {}
        return;
    }
}

// Handles rendering of the counters of time and mines
void Technic::draw_player_info(sf::RenderWindow& window) {

    sf::Text mines, time;

	time.setFont(font);
	int current_time = (show_current_time) ? (int)clock.getElapsedTime().asSeconds() : Player::time;
	time.setString(to_string(current_time));
	time.setCharacterSize(text_size);
	time.setFillColor(text_color);
	time.setPosition(0, info_text_y);
	window.draw(time);

	mines.setFont(font);
	mines.setString(to_string(Player::mines_left));
	mines.setCharacterSize(text_size);
	mines.setFillColor(text_color);
	mines.setPosition(info_text_x, info_text_y);
	window.draw(mines);
}

// Handles rendering of the settings button
void Technic::draw_buttons(sf::RenderWindow& window) {

	sf::Text settings;

	settings.setFont(font);
	settings.setString("SETTINGS");
	settings.setCharacterSize(text_size);
	settings.setFillColor(text_color);
	settings.setPosition(info_text_x * (3.0/7), info_text_y);
	window.draw(settings);
}