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

void wsl_buffer_to_scr(WSL_App *app);
void swap_buffers(WSL_App *app);

void draw(WSL_App *game) {
    int i = 0;
    Entity *entity = NULL;
    SDL_Rect hitbox;

    SDL_SetRenderDrawColor(game->renderer, 0x0F, 0x10, 0x10, 0xFF);
    SDL_RenderClear(game->renderer);

    //Set first buffer as render target
    SDL_SetRenderTarget(game->renderer, game->cur_buffer->tex);

    SDL_RenderSetIntegerScale(game->renderer, true);
    //Clear the screen 
    SDL_SetRenderDrawColor(game->renderer, 0x10, 0x10, 0x10, 0xFF);
    SDL_RenderClear(game->renderer);

    SDL_Rect fillrect = {0,0, SCREEN_W, SCREEN_H};
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &fillrect);

    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    //Draw stuff on the first buffer
    entity = game->entities;
    while(entity) {
        hitbox = get_hitbox(entity);
        SDL_RenderDrawRect(game->renderer, &hitbox);
        if(entity->render) entity->render(entity, game);
        entity = entity->next;
    }

    //Reset render target
    SDL_SetRenderTarget(game->renderer, NULL);
    
    //Draw the second buffer on the screen
    wsl_buffer_to_scr(game);

    //Swap the first and second buffer
    swap_buffers(game);
    
    //Make some fake scanlines just for fun why not
    //Silly as heck to do this every cycle, it should just be it's own
    //texture and then draw it on top of everything. 
    // TODO: This looks awesome as heck but valgrind complains that
    // game->renderer is "uninitialized".
    if(game->scanlines) {
        SDL_SetRenderDrawColor(game->renderer, 0x11, 0x11, 0x11, 0x64);
        for(i = 0; i < game->windowdim.y; i += 3) {
            SDL_RenderDrawLine(game->renderer, 0, i, game->windowdim.x, i);
        }
    }

    //Present the screen
    SDL_RenderPresent(game->renderer);
}

void swap_buffers(WSL_App *app) {
    WSL_Texture *tmp = app->cur_buffer;
    app->cur_buffer = app->next_buffer;
    app->next_buffer = tmp;
}

void wsl_buffer_to_scr(WSL_App *app) {
    //Render the screen "buffer" texture to the screen
    //This needs to:
    //- Figure out how big to "stretch" app->buffer
    int x,y;
    int scalefactor;
    SDL_Rect renderquad;
    x = app->windowdim.x / SCREEN_W;
    y = app->windowdim.y / SCREEN_H;
    scalefactor = (x > y) ? y : x;
    //- Put app->buffer centered on the screen
    x = (app->windowdim.x) / 2; // Center of window
    x -= (SCREEN_W * scalefactor) / 2; // Minus half the screen width
    y = (app->windowdim.y) / 2;
    y -= (SCREEN_H * scalefactor) / 2;
    renderquad.x = x;
    renderquad.y = y;
    renderquad.w = app->next_buffer->w * scalefactor;
    renderquad.h = app->next_buffer->h * scalefactor;
    // Put app->buffer->tex on app->buffer->renderer, the whole thing taking up
    // renderquad space
    SDL_RenderCopy(app->renderer, app->next_buffer->tex, NULL, &renderquad);
}

