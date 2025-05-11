#include "player.hpp"

#include <SFML/Graphics.hpp>
#include "technic.hpp"

// do necessary work to end the game
void Player::game_over() {
    // todo let player check the field before resetting the game
    while(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {}
    game_ended = true;
    stop_timer();
}
void Player::stop_timer() {
    time = (int)Technic::clock.getElapsedTime().asSeconds();
    Technic::show_current_time = false;
}
void Player::won_game() {
    Player::game_over();
}