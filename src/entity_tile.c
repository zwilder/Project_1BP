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
