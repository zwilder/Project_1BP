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
    const float fixed_step = 32.333f;
    float accumulated = 0.0f;
    uint32_t current = SDL_GetTicks();
    uint32_t prev = current;
    float dt = 0.0f;
    uint8_t frame = 0;
    Vec2i startpos = {0,0};

    WSL_App *game = NULL;
    game = wsl_init_sdl();
    if(!game) {
        printf("Failed to create WSL_App!\n");
        return 1;
    }

    init_genrand(time(NULL));


    //Temporarily draw some tiles here
    if(argc > 1) {
        startpos = load_tilemap(game, argv[1]);
    } else {
        startpos = load_tilemap(game, "assets/MapA.tmx");
    }

    //Temporarily spawn the player here
    spawn_player(game, startpos.x, startpos.y);

    // Fixed time step game loop
    while(game->running) {
        frame += 1;
        current = SDL_GetTicks();
        dt = current - prev;
        prev = current;

        accumulated += dt;
        game->dt = dt / 100.0;

        while(accumulated >= fixed_step) {
            handle_events(game);
            update(game);
            accumulated -= fixed_step;
        }

        if(frame % 2 == 0) {
            draw(game);
        }
    }

    wsl_cleanup_sdl(game);
    return 0;
}

/*
int main_old(int argc, char **argv) {
    long lag = 0, current = 0, elapsed = 0;
    long prev = current_ms();
    long msperframe = 33; // 16ms = ~60fps, 33ms = ~30fps
    uint8_t frame = 0;
    WSL_App *game = wsl_init_sdl(); // Start SDL, load resources

    init_genrand(time(NULL)); // Seed the pnrg

    if(!game) {
        printf("Failed to create WSL_App!\n");
        return 1;
    }

    spawn_player(game, SCREEN_W / 2, SCREEN_H / 2);
    // Basic variable time step game loop, straight outta Game Programming Patterns
    while(game->running) {
        current = current_ms();
        elapsed = current - prev;
        prev = current;
        lag += elapsed;

        game->dt = elapsed; // update delta time

        //Handle events
        handle_events(game);

        //Update
        while(lag >= msperframe) {
            lag -= msperframe;
            update(game);
        }

        //Draw every other frame
        if(frame % 2 == 0) {
            draw(game);
        }

        frame += 1;
    }
    wsl_cleanup_sdl(game);
    return 0;
}
*/
