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

Entity* create_player(SDL_Rect spriterect) {
    Entity *player = create_entity(spriterect);
    player->flags |= EF_ALIVE;

    player->update = &update_player;
    player->color.r = 0;
    player->color.b = 0;
    return player;
}

void update_player(Entity *player, WSL_App *game) {
    // This does nothing... for now
    update_entity(player, game);
}

void spawn_player(WSL_App *game, float x, float y) {
    // Get the sprite for the player
    SDL_Rect spriterect = {0,0,16,16};
    Vec2i spritecoords = get_sprite_coords(241);
    spriterect.x = spritecoords.x;
    spriterect.y = spritecoords.y;
    // Create the player
    Entity *player = create_player(spriterect);
    // Place the player
    player->pos.x = x;
    player->pos.y = y;
    // Add the player to the entity list
    wsl_add_entity(game, player);
}
