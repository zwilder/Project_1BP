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

void draw(WSL_App *game) {
    int i = 0;
    Entity *entity = NULL;

    //Clear the screen (RED temporarily just so I can "see" the buffer edges)
    SDL_SetRenderDrawColor(game->renderer, 0x10, 0x10, 0x10, 0xFF);
    SDL_RenderClear(game->renderer);

    //Set buffer as render target
    SDL_SetRenderTarget(game->renderer, game->buffer->tex);

    
    //SDL_Rect fillrect = {SCREEN_W / 4, SCREEN_H /4, SCREEN_W / 2, SCREEN_H / 2};
    //SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //SDL_RenderFillRect(game->renderer, &fillrect);

    //Draw stuff on the buffer
    entity = game->entities;
    while(entity) {
        if(entity->render) entity->render(entity, game);
        entity = entity->next;
    }

    //Reset render target
    SDL_SetRenderTarget(game->renderer, NULL);
    
    //Draw the buffer on the screen
    wsl_buffer_to_scr(game);
    
    if(game->scanlines) {
        //Make some fake scanlines just for fun why not
        //Silly as heck to do this every cycle, it should just be it's own
        //texture and then draw it on top of everything. 
        SDL_SetRenderDrawColor(game->renderer, 0x11, 0x11, 0x11, 0x64);
        for(i = 0; i < game->windowdim.y; i += 3) {
            SDL_RenderDrawLine(game->renderer, 0, i, game->windowdim.x, i);
        }
    }

    //Present the screen
    SDL_RenderPresent(game->renderer);
}

