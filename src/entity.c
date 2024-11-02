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

int count_entities(Entity *entity) {
    if(!entity) return 0;
    return(1 + count_entities(entity->next));
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

bool tile_solid(int x, int y, WSL_App *game) {
    // Find entity at x,y - check to see if it has EF_TILE
    // Assumes entities are aligned to map coordintes
    Entity *e = game->entities;
    while(e) {
        if(((e->pos.x / TILE_SIZE) == x) && ((e->pos.y / TILE_SIZE) == y)) {
            if(check_flag(e->flags, EF_TILE)) {
                return true;
            }
        }
        e = e->next;
    }

    return false;
}

bool tile_platform(int x, int y, WSL_App *game) {
    // Find entity at x,y - check to see if it has EF_PLATFORM_
    // Assumes entities are aligned to map coordintes
    Entity *e = game->entities;
    while(e) {
        if(((e->pos.x / TILE_SIZE) == x) && ((e->pos.y / TILE_SIZE) == y)) {
            if(check_flag(e->flags, EF_PLATFORM)) {
                return true;
            }
        }
        e = e->next;
    }

    return false;
}

bool is_on_ground(Entity *entity, WSL_App *game) {
    // Check to see if the entity is on a solid tile (or the bottom of the
    // screen temporarily)
    /*
    int y = entity->pos.y + TILE_SIZE; // Bottom of entity
    if (y >= SCREEN_H) {
        return true;
    }
    int my = y / TILE_SIZE; // Map coordinates of bottom of entity
    int mx = entity->pos.x / TILE_SIZE; // Map coords
    if(tile_solid(mx,my, game)) {
        return true;
    }
    return false;
    */

    int bottom_y = entity->pos.y + TILE_SIZE;
    int mx_left = entity->pos.x / TILE_SIZE;
    int mx_right = (entity->pos.x + TILE_SIZE - 1) / TILE_SIZE;
    int tile_below_y = bottom_y / TILE_SIZE;
    if(tile_solid(mx_left, tile_below_y, game) ||
            tile_solid(mx_right, tile_below_y, game)) {
        return true;
    }
    return false;
}

void resolve_movement(Entity *entity, SDL_Rect *hitbox, WSL_App *game) {
    SDL_Rect otherbox; 
    int etop,ebtm,elt,ert;
    Entity *other = game->entities;
    /*
    //Check for x collisions
    hitbox->x += entity->dpos.x;
    other = game->entities;
    while(other) {
        if(other != entity) {
            otherbox = get_hitbox(other);
            if(check_flag(other->flags, EF_TILE)) {
                if(xy_in_rect(hitbox->x + hitbox->w, hitbox->y, otherbox)) {
                    // Entity right is colliding with other
                    hitbox->x = otherbox.x - hitbox->w - 1; // Slightly off
                    entity->dpos.x = 0;
                } else if (xy_in_rect(hitbox->x, hitbox->y, otherbox)) {
                   // Entity left is colliding with other
                   hitbox->x = otherbox.x + otherbox.w + 1;
                   entity->dpos.x = 0;
                }
            }
        }
        other = other->next;
    }
    // Check for y collisions
    // First check to see if the entity is on the ground (tile below entity)
    // if so, and dpos.y < 0 (moving up/jumping) add dpos.y to hitbox->y. If not,
    // add dpos.y to hitbox->y.
    if(is_on_ground(entity,game)) {
        if(entity->dpos.y < 0) hitbox->y += entity->dpos.y;
    }else{
        hitbox->y += entity->dpos.y;
        entity->flags &= ~EF_ONGROUND;
    }
    //hitbox->y += entity->dpos.y;
    other = game->entities;
    while(other) {
        if(other != entity) {
            otherbox = get_hitbox(other);
            if(check_flag(other->flags, EF_TILE)) {
                if(xy_in_rect(hitbox->x,hitbox->y + hitbox->h, otherbox)) {
                    //Entity bottom is colliding with other
                    hitbox->y = otherbox.y - hitbox->h;
                    entity->flags |= EF_ONGROUND;
                    entity->dpos.y = 0;
                } else if (xy_in_rect(hitbox->x, hitbox->y, otherbox)) {
                    //Entity top is colliding with other
                    hitbox->y = otherbox.y + otherbox.h; // Slightly off
                    entity->dpos.y = 0; // Stop movement
                }
            }
        }
        other = other->next;
    }
    */
    while(other) {
        if(other != entity) {
            otherbox = get_hitbox(other);
            etop = hitbox->y; // For clarity, entity top/bottom/left/right
            ebtm = hitbox->y + hitbox->h;
            elt = hitbox->x;
            ert = hitbox->x + hitbox->w;
            if(check_flag(other->flags, EF_TILE)) {
                // Check tile collisions
                // TODO: Each entity should have it's own way of handling tile
                // collisions, an "on_collide" function or something. 
                // x-axis
                if((entity->dpos.x > 0) && 
                        (xy_in_rect(ert, etop, otherbox) || 
                         xy_in_rect(ert, ebtm - 2, otherbox))) {
                    //Moving right and colliding with top OR bottom edge -
                    //buffer
                    if(entity->dpos.y > 0 || entity->dpos.y < 0) {
                        // Nothing
                    } else {
                        hitbox->x = otherbox.x - hitbox->w - 2;
                    }
                    entity->dpos.x = 0;
                }
                if ((entity->dpos.x < 0) &&
                        (xy_in_rect(elt, etop, otherbox) ||
                         xy_in_rect(ert, ebtm - 2, otherbox))) {
                    //Moving left and colliding with top OR bottom edge - buffer
                    if(entity->dpos.y > 0 || entity->dpos.y < 0) {
                        // Nothing
                    } else {
                        hitbox->x = otherbox.x + otherbox.w + 2;
                    }
                    entity->dpos.x = 0;
                }

                // y-axis
                if((entity->dpos.y >= 0) &&
                        (xy_in_rect(elt, ebtm, otherbox) ||
                         xy_in_rect(ert, ebtm, otherbox))) {
                    //Moving down and colliding with bottom left OR right edge
                    hitbox->y = otherbox.y - hitbox->h;
                    entity->flags |= EF_ONGROUND;
                    entity->dpos.y = 0;
                } 
                if ((entity->dpos.y < 0) &&
                        (xy_in_rect(elt, etop, otherbox) ||
                         xy_in_rect(ert, etop, otherbox))) {
                    //Moving up and colliding with top left OR right edge
                    hitbox->y = otherbox.y + otherbox.h;
                    entity->dpos.y = 0;
                }
            }
            if(check_flag(other->flags, EF_PLATFORM)) {
                // Check platform collisions
                // Only care about the platform when falling down onto it.
                // Platforms we only care about the top of the sprite, so we
                // need to adjust the otherbox. Also, adjust the width so it's
                // slightly smaller?
                otherbox.h = TILE_SIZE / 2;
                otherbox.w -= 2;
                otherbox.x += 1;
                if((entity->dpos.y >= 0) &&
                        (xy_in_rect(elt, ebtm, otherbox) ||
                         xy_in_rect(ert, ebtm, otherbox))) {
                    //Moving down and colliding with bottom left OR right edge
                    hitbox->y = otherbox.y - hitbox->h;
                    entity->flags |= EF_ONGROUND;
                    entity->dpos.y = 0;
                } 
            }
        }
        other = other->next;
    }
}

void handle_physics(Entity *entity, WSL_App *game) {
    /*
     * Third attempt at sorting this function out. Still needs work for the
     * corners of tiles to be handled properly BUT its a darn good start.
     */
    float gravity = 1.5f; 
    float friction = 0.5f;
    bool sx = !(entity->dpos.x < 0); // Sign of X, true positive/false negative
    bool new_sx = false;
    SDL_Rect hitbox = get_hitbox(entity);
    SDL_Rect otherbox;
    Entity *other = game->entities;
    int etop, ebtm, elt, ert; // Entity top/bottom/left/right
    int x,y, targetX,targetY;

    /* Add gravity/friction */
    if(!check_flag(entity->flags, EF_ONGROUND)) {
        entity->dpos.y += gravity;
    }
    
    if(check_flag(entity->flags, EF_ONGROUND)) {
        if(entity->dpos.x < 0) {
            //Moving left, add friction right
            entity->dpos.x += friction;
        } else {
            entity->dpos.x -= friction;
        }
        // Friction moves entity dpos.x towards 0 but doesnt throw them in
        // reverse (TODO: It does if the ground is slippery... like ice?)
        new_sx = !(entity->dpos.x < 0);
        if(sx != new_sx) {
            entity->dpos.x = 0;
        }
    }
    
    /* Resolve movement */
    hitbox.x += entity->dpos.x;
    if(is_on_ground(entity,game)) {
        if(entity->dpos.y < 0) hitbox.y += entity->dpos.y; //Moving up
    }else{
        hitbox.y += entity->dpos.y; //Not on ground
        entity->flags &= ~EF_ONGROUND;
    }
    resolve_movement(entity, &hitbox, game);

    // Check if hitbox is oob (temporarily block movement off screen)
    /*
    if(!in_bounds(hitbox.x + hitbox.w, hitbox.y)) {
        //right oob
        hitbox.x = SCREEN_W - TILE_SIZE - 1;
    } else if(!in_bounds(hitbox.x, hitbox.y)) {
        //left oob
        hitbox.x = 1;
    }
    if(!in_bounds(hitbox.x, hitbox.y + hitbox.h)) {
        //bottom oob
        hitbox.y = SCREEN_H - TILE_SIZE - 1;
        entity->flags |= EF_ONGROUND;
    } else if(!in_bounds(hitbox.x, hitbox.y)) {
        //top oob
        hitbox.y = 0;
    }
    */

    // Fun color just to visualize collisions 
    /*
    other = game->entities;
    while(other) {
        if(other != entity) {
            otherbox = get_hitbox(other);
            other->color = hex_to_rgb(CHARCOAL);
            if(check_collision_rect(otherbox, hitbox)) {
                other->color = hex_to_rgb(SPRING_GREEN);
            }
        }
        other = other->next;
    }
    */

    //Finalize position - move entity to resolved hitbox
    entity->pos.x = hitbox.x;
    entity->pos.y = hitbox.y;
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

SDL_Rect get_sprite_rect(int index) {
    SDL_Rect result = {0,0,TILE_SIZE,TILE_SIZE};
    Vec2i xy = get_sprite_coords(index);
    result.x = xy.x;
    result.y = xy.y;
    return result;
}

Vec2i get_map_coords(int x, int y) {
    Vec2i result = {0,0};
    result.x = x * TILE_SIZE;
    result.y = y * TILE_SIZE;
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
    bool x = ((bx >= a.x) && (bx < a.x + a.w));
    bool y = ((by >= a.y) && (by < a.y + a.h));
    return (x && y);
}
