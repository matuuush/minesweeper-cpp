#ifndef MINESWEEPER_SETTINGS_HPP
#define MINESWEEPER_SETTINGS_HPP

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "field.hpp"

using namespace std;

enum class OptionType { width, height, mines };

struct Option {
	OptionType type;
	int x_pos, y_pos, input, min, max;
	string text;
	bool selected = false;
	Option(OptionType ot, int x, int y, string t, int i, int mi, int ma) : type(ot), x_pos(x), y_pos(y), text(std::move(t)), input(i), min(mi), max(ma) {}
};

namespace Settings {
	const string SETTINGS_WINDOW_NAME = "Settings";
	const int settings_window_width = 500, settings_window_height = 300;
	const sf::Color text_color = sf::Color::White;
	inline unordered_map<OptionType, Option> options;
	inline long current_input_value;
	inline OptionType selected;
	const int text_size = 24;
	inline sf::Font font;

	void set_selected_option();
	void check_confirm_button(sf::RenderWindow& main_window, sf::RenderWindow& settings_window, sf::Vector2i& mouse_position, Field& field);
	void modify_input_value(int value);
	void handle_events(sf::RenderWindow& window);
	void open_window(Field& field, sf::RenderWindow& main_window);
	void init_options();
	void handle_mouse_press(sf::RenderWindow& main_window, sf::RenderWindow& settings_window, Field& field);
	static void draw_one_option(OptionType type, sf::RenderWindow& settings_window);
	void draw_options(sf::RenderWindow& settings_window);
	void check_option(Option& option, sf::Vector2i& mouse_position);
}

#endif //MINESWEEPER_SETTINGS_HPP
