#ifndef MINESWEEPER_PLAYER_HPP
#define MINESWEEPER_PLAYER_HPP

namespace Player {
    inline int time = 0;
    inline int mines_left;
    inline bool game_ended = false;

    void game_over();
    void stop_timer();
    void won_game();
}

#endif //MINESWEEPER_PLAYER_HPP
