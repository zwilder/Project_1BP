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

Entity *create_entity(SDL_Rect spriterect) {
    // Create a generic entity
    Entity *entity = malloc(sizeof(Entity));
    entity->next = NULL;
    entity->prev = NULL;

    entity->pos.x = 0;
    entity->pos.y = 0;
    entity->dpos.x = 0;
    entity->dpos.y = 0;

    entity->frame = 0;
    entity->flags = EF_NONE;
    entity->spriterect = spriterect;
    entity->color.r = 255;
    entity->color.g = 255;
    entity->color.b = 255;
    entity->color.a = 255;

    entity->update = &update_entity;
    entity->render = &render_entity;
    entity->deathfunc = NULL;
    return entity;
}

void destroy_entity(Entity *entity) {
    /* 
     * Frees memory allocated for entity and it's components. Should **NOT** be
     * called directly in most cases, since entities are a linked list actual
     * destruction of the Entity is done by the list manager WSL_App
     */
    if(!entity) return;

    free(entity);
}

void update_entity(Entity *entity, WSL_App *game) {
    // Generic entity update function
    // update() is called once per second (Gameloop is locked at 60fps)
    entity->frame += 1;
    if(entity->frame > 120) {
        entity->frame = 0;
    }
    if(!((entity->flags & EF_ALIVE) == EF_ALIVE)) {
        //Entity is dead, call it's death function
    }
}

void render_entity(Entity *entity, WSL_App *game) {
    // Generic entity render function
    SDL_Rect renderquad;
    renderquad.x = entity->pos.x;
    renderquad.y = entity->pos.y;
    renderquad.w = entity->spriterect.w;
    renderquad.h = entity->spriterect.h;
    SDL_SetTextureColorMod(game->spritesheet->tex, entity->color.r, entity->color.g,
            entity->color.b);
    SDL_SetTextureAlphaMod(game->spritesheet->tex, entity->color.a);
    SDL_RenderCopy(game->renderer, game->spritesheet->tex, 
            &entity->spriterect, &renderquad);
}

Vec2i get_sprite_coords(int index) {
    // Helper function to find x,y coordinates of sprites given an index
    // This assumes the sprites are numbered left to right, top to bottom
    uint8_t spritesize = 16;
    uint8_t rows = 20;
    uint8_t cols = 20;
    Vec2i result = {(index % rows) * spritesize, 
        (int)(index / cols) * spritesize};

    return result;
}

SDL_Rect get_hitbox(Entity *e) {
    /*
     * Rectangle around the sprite, the "Axis Aligned Bounding Box"....
     * I should call this "get_aabb" but that literally saves typing two
     * characters and I know that I'll type abbb or aab or something
     * accidentally and be annoyed.
     */
    SDL_Rect result = e->spriterect;
    result.x = e->pos.x;
    result.y = e->pos.y;
    return result;
}

bool check_collision_rect(SDL_Rect a, SDL_Rect b) {
    /* 
     * From rect.h/c in the toolbox, check if rectangle "a" intersects rectangle
     * "b".
     */
    int ax1,ay1,ax2,ay2;
    int bx1,by1,bx2,by2;
    bool result = false;

    ax1 = a.x;
    ay1 = a.y;
    ax2 = ax1 + a.w;
    ay2 = ay1 + a.h;
    bx1 = b.x;
    by1 = b.y;
    bx2 = bx1 + b.w;
    by2 = by1 + b.h;
    
    if((ax1 <= bx2) && (ax2 >= bx1) && (ay1 <= by2) && (ay2 >= by1)) {
        result = true;
    }

    return result; 
}
