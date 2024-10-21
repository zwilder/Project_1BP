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

Entity* create_tile(int x, int y, SDL_Rect spriterect) {
    // Generic tile entity
    /*
     * Might be worth separating tiles into decor (pretty stuff on the screen)
     * and tiles (stuff that can be interacted with). Easy way to do this is to
     * add another flag (EF_DECOR). This might assist with rendering, I could
     * sort the entity list, EF_DECOR, EF_TILE, EF_ENEMY, EF_PLAYER etc */
    Entity *tile = create_entity(spriterect);
    tile->flags |= EF_ALIVE; // Note, EF_TILE is for tiles that block movement
    tile->pos.x = x;
    tile->pos.y = y;
    tile->color = hex_to_rgb(WHITE);

    return tile;
}

Entity* create_tilev(Vec2i pos, SDL_Rect spriterect) {
    return create_tile(pos.x,pos.y,spriterect);
}

void update_tile(Entity *tile, WSL_App *game) {
    /*
     * This is used for the tiles that have the two frame animation - the gears,
     * blinky boxes, etc. Since the game is held at 30fps, if we wanted to
     * update every 100ms (1/10th of a second) we need to update every 3 frames:
     * check frame % 3 == 0. 
     */
    int i = 0;
    update_entity(tile, game);
    if((tile->frame % 3 == 0 ) && (tile->framecount > 1)) {
        i = (tile->frame + 1) % tile->framecount;
        tile->spriterect = tile->spriteframes[i];
    } 
}

