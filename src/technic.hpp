#ifndef MINESWEEPER_TECHNIC_HPP
#define MINESWEEPER_TECHNIC_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "field.hpp"

using namespace std;

enum class GameMode { normal, solver };

namespace Technic {
    const string linux_textures_path = "../src/all_textures.png";
    const string windows_textures_path = "../" + linux_textures_path;
    const string linux_font_path = "../src/digital-7.ttf";
    const string windows_font_path = "../" + linux_font_path;
    const int text_size = 24;
    const sf::Color text_color = sf::Color::White;
    inline sf::Font font;
    inline float info_text_x, info_text_y;
    inline bool before_first_left_mouse_press = true;
    inline bool show_current_time = false;
	inline GameMode game_mode = GameMode::normal;
    inline sf::Clock clock;

	void end_game(sf::RenderWindow& window);
    void load_textures(sf::Texture& texture);
    void handle_events(sf::RenderWindow& window);
    void handle_left_mouse_button_press(Field& field, sf::Vector2i& mouse_position, sf::RenderWindow& window);
    void handle_mouse_press(Field& field, sf::Vector2i& mouse_position, sf::RenderWindow& window);
    void draw_player_info(sf::RenderWindow& window);
	void draw_buttons(sf::RenderWindow& window);
}

#endif //MINESWEEPER_TECHNIC_HPP
