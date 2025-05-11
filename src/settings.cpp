#include "settings.hpp"
#include "technic.hpp"

// return value of number key pressed, or -1 if no number key pressed
static int number_key_pressed() {
	int value;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0)) {
		value = 0;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)) {
		value = 1;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
		value = 2;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3)) {
		value = 3;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
		value = 4;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)) {
		value = 5;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
		value = 6;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)) {
		value = 7;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
		value = 8;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)) {
		value = 9;
	} else {
		value = -1;
	}
	return value;
}

// standard function for translating digits to decadic number
void Settings::modify_input_value(int value) {
	current_input_value *= 10;
	current_input_value += value;
}

// handling events for the settings window
void Settings::handle_events(sf::RenderWindow& window) {
	sf::Event event{};
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::KeyPressed) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
				current_input_value = 0;
			}
			int value = number_key_pressed();
			if (value != -1) {
				Settings::modify_input_value(value);
			}
		}
	}
}

// select option
void Settings::set_selected_option() {
	auto& selected_option = options.at(selected);
	if (current_input_value < selected_option.min) {
		selected_option.input = selected_option.min;
	} else if (current_input_value > selected_option.max) {
		selected_option.input = selected_option.max;
	} else {
		selected_option.input = current_input_value;
	}
}

// check which option is selected based on the mouse position
void Settings::check_option(Option& option, sf::Vector2i& mouse_position) {
	if (mouse_position.y >= option.y_pos - 25 && mouse_position.y < option.y_pos + 25) {
		if (selected != option.type) {
			set_selected_option();
			current_input_value = 0;
		}
		for (auto&& option_pair : options) {
			option_pair.second.selected = false;
		}
		selected = option.type;
		option.selected = true;
	}
}

// handle clicking on the Confirm button
void Settings::check_confirm_button(sf::RenderWindow& main_window, sf::RenderWindow& settings_window, sf::Vector2i& mouse_position, Field& field) {
	if (mouse_position.y >= settings_window_width * (3.0/7) - 25 && mouse_position.y < settings_window_width * (3.0/7) + 25) {
		set_selected_option();
		settings_window.close();
		Technic::end_game(main_window);
	}
}

// handle left mouse button press
void Settings::handle_mouse_press(sf::RenderWindow& main_window, sf::RenderWindow& settings_window, Field& field) {
	if (!settings_window.hasFocus()) {
		return;
	}
	bool left_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (left_pressed) {
		sf::Vector2i mouse_position = sf::Mouse::getPosition(settings_window);
		for (auto&& option_pair : options) {
			check_option(option_pair.second, mouse_position);
		}
		check_confirm_button(main_window, settings_window, mouse_position, field);
	}

}

// used for drawing one option on window
static void Settings::draw_one_option(OptionType type, sf::RenderWindow& settings_window) {
	Option& option = options.at(type);
	sf::Text option_text, option_input;

	option_text.setFont(font);
	option_text.setString(option.text);
	option_text.setCharacterSize(text_size);
	option_text.setFillColor(text_color);
	option_text.setPosition(option.x_pos,option.y_pos);
	settings_window.draw(option_text);

	option_input.setFont(font);
	option_input.setString(to_string(option.selected ? Settings::current_input_value : option.input));
	option_input.setCharacterSize(text_size);
	option_input.setFillColor(option.selected ? sf::Color::Red : text_color);
	option_input.setPosition(option.x_pos * 2,option.y_pos);
	settings_window.draw(option_input);
}

// used for drawing options and Confirm button on window
void Settings::draw_options(sf::RenderWindow& settings_window) {
	sf::Text confirm_text;

	for (auto&& option_pair : options) {
		draw_one_option(option_pair.first, settings_window);
	}

	confirm_text.setFont(font);
	confirm_text.setString("CONFIRM");
	confirm_text.setCharacterSize(text_size);
	confirm_text.setFillColor(text_color);
	confirm_text.setPosition(settings_window_width * (3.0/7), 200);
	settings_window.draw(confirm_text);
}

// initialise values of options and memorise them in the map
void Settings::init_options() {
	Option width_opt(OptionType::width, settings_window_width * (2.0/7), 50, "WIDTH", 10, 10, 30);
	Option height_opt(OptionType::height, settings_window_width * (2.0/7), 100, "HEIGHT", 10, 10, 20);
	Option mines_opt(OptionType::mines, settings_window_width * (2.0/7), 150, "MINES", 10, 10, 120);
	options.emplace(OptionType::width, std::move(width_opt));
	options.emplace(OptionType::height, std::move(height_opt));
	options.emplace(OptionType::mines, std::move(mines_opt));
}

// handle opening of the settings window
void Settings::open_window(Field& field, sf::RenderWindow& main_window) {
	sf::RenderWindow settings_window(sf::VideoMode(settings_window_width, settings_window_height), SETTINGS_WINDOW_NAME);
	settings_window.setKeyRepeatEnabled(false);
	init_options();
	while (settings_window.isOpen()) {
		handle_events(settings_window);
		settings_window.requestFocus();
		handle_mouse_press(main_window, settings_window, field);

		settings_window.clear(); // beginning
		draw_options(settings_window);
		settings_window.display(); // end
	}
}
