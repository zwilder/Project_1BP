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
    entity->state = ST_IDLE;
    entity->spriterect = spriterect;
    entity->color.r = 255;
    entity->color.g = 255;
    entity->color.b = 255;
    entity->color.a = 255;

    entity->spriteframes = NULL;
    entity->framecount = 1;
    entity->flip = SDL_FLIP_NONE;

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

    if(entity->spriteframes) {
        free(entity->spriteframes);
    }

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
    int rx = (int)(ceilf(entity->pos.x));
    int ry = (int)(ceilf(entity->pos.y));
    
    renderquad.x = rx;
    renderquad.y = ry;
    renderquad.w = entity->spriterect.w;
    renderquad.h = entity->spriterect.h;
    
    SDL_SetTextureColorMod(game->spritesheet->tex, entity->color.r, entity->color.g,
            entity->color.b);
    SDL_SetTextureAlphaMod(game->spritesheet->tex, entity->color.a);
    
    SDL_RenderCopyEx(game->renderer, game->spritesheet->tex,
            &entity->spriterect, &renderquad,
            0, NULL, entity->flip);
    /*
    SDL_RenderCopy(game->renderer, game->spritesheet->tex, 
            &entity->spriterect, &renderquad);
            */
}

bool is_on_ground(Entity *entity, WSL_App *game) {
    // Check to see if the entity is on a solid tile (or the bottom of the
    // screen temporarily)
    return((entity->pos.y + TILE_SIZE) >= SCREEN_H);

}

void handle_physics(Entity *entity, WSL_App *game) {
    float gravity = 1.5f; 
    float friction = 0.5f;
    int new_x = (int)entity->pos.x;
    int new_y = (int)entity->pos.y;

    // Check to see if entity is on ground, update EF_ONGROUND
    if(is_on_ground(entity, game)) {
        entity->flags |= EF_ONGROUND;
    } else {
        entity->flags &= ~EF_ONGROUND;
    }

    // Add gravity
    entity->dpos.y += gravity;

    // Add friction TODO: friction should be a property of the ground/tile that
    // the entity is on. (set this to 0 and the entity slides around on ice!)
    if(check_flag(entity->flags, EF_ONGROUND)) {
        if(entity->dpos.x < 0) {
            //Moving left, add friction right
            entity->dpos.x += friction;
        } else {
            entity->dpos.x -= friction;
        }
    }

    // Check movement
    new_x += (int)(entity->dpos.x + 0.5f);
    new_y += (int)(entity->dpos.y + 0.5f);
    if(!in_bounds(new_x, entity->pos.y)) {
        // moving horizontally out of bounds TODO: This assumes entities can't
        // move "off screen"
        new_x = (new_x < 0) ? (0) : (SCREEN_W - TILE_SIZE);
    }
    if(!in_bounds(entity->pos.x, new_y)) {
        // Moving vertically out of bounds
        new_y = (new_y < 0) ? (0) : (SCREEN_H - TILE_SIZE);
    }

    // Check collisions
    
    // Resolve movement
    entity->pos.x = (int)new_x;
    entity->pos.y = (int)new_y;
}

bool in_bounds(float x, float y) {
    /*
     * This function is the result of me weighing the options of either typing:
     * "out_of_bounds(x,y)" or "in_bounds(x,y)" it makes more sense to me to
     * check if something is in_bounds(x,y), or !in_bounds(x,y) than to say
     * something is in bounds by asking !out_of_bounds(x,y).
     */
    bool xoob = (x < 0) || ((x + TILE_SIZE) > SCREEN_W);
    bool yoob = (y < 0) || ((y + TILE_SIZE) > SCREEN_H);
    return !(xoob || yoob);
}

void resolve_movement(Entity *entity, WSL_App *game) {

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
     *
     * Since some entities are smaller than the sprite size of 16x16 I may make
     * this something stored in the entity, and then just use this to get the
     * default hitbox - would allow for entities that change shape or something
     * too.
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

bool xy_in_rect(float bx, float by, SDL_Rect a) {
    bool x = ((bx > a.x) && (bx < a.x + a.w));
    bool y = ((by > a.y) && (by < a.y + a.h));
    return (x && y);
}
