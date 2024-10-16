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

#include <project1bp.h>

void handle_events(WSL_App *game) {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        switch(e.type) {
            case SDL_QUIT:
                game->running = false;
                break;
            case SDL_KEYDOWN:
                handle_keydown(&e.key, game);
                break;
            case SDL_KEYUP:
                handle_keyup(&e.key, game);
                break;
            default:
                break;
        }
    }
}

void handle_keydown(SDL_KeyboardEvent *event, WSL_App *game) {
    if((event->repeat == 0) && (event->keysym.scancode < MAX_KEYBOARD_KEYS)) {
        game->keyboard[event->keysym.scancode] = true;
    }
    switch(event->keysym.sym) {
        case SDLK_F1:
            wsl_set_fullscreen(game);
            break;
        case SDLK_F2:
            game->scanlines = !game->scanlines;
            break;
        case SDLK_q:
        case SDLK_ESCAPE:
            game->running = false;
            break;
        default: break;
    }
}

void handle_keyup(SDL_KeyboardEvent *event, WSL_App *game) {
    if((event->repeat == 0) && (event->keysym.scancode < MAX_KEYBOARD_KEYS)) {
        game->keyboard[event->keysym.scancode] = false;
    }
}
