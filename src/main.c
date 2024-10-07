/*
* Project 1BP
* Copyright (C) Zach Wilder 2024
* 
* This file is a part of Project 1BP
*
* Project 1BP is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Project 1BP is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Project 1BP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sys/time.h>
#include <project1bp.h>

long current_ms(void) {
    // Helper function to return the current time in ms
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((tp.tv_sec * 1000) + (tp.tv_usec / 1000));
}

int main(int argc, char **argv) {
    long lag = 0, current = 0, elapsed = 0;
    long prev = current_ms();
    long msperframe = 16; // 16ms = ~60fps, 33ms = ~30fps
    WSL_App *game = wsl_init_sdl(); // Start SDL, load resources

    init_genrand(time(NULL)); // Seed the pnrg

    if(!game) {
        printf("Failed to create WSL_App!\n");
        return 1;
    }

    spawn_player(game, SCREEN_W / 2, SCREEN_H / 2);
    /* Basic game loop, straight outta Game Programming Patterns */
    while(game->running) {
        current = current_ms();
        elapsed = current - prev;
        prev = current;
        lag += elapsed;

        //Handle events
        handle_events(game);

        //Update
        while(lag >= msperframe) {
            lag -= msperframe;
            update(game);
        }

        //Draw
        draw(game);
    }

    wsl_cleanup_sdl(game);
    return 0;
}
