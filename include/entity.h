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

#ifndef ENTITY_H
#define ENTITY_H

enum { 
    EF_NONE         = 1 << 0,
    EF_ALIVE        = 1 << 1,
    EF_PLAYER       = 1 << 2,
    EF_ENEMY        = 1 << 3
};

struct Entity {
    Entity *next; //Entity is a double linked list
    Entity *prev; 

    Vec2f pos; //Entity position x,y
    Vec2f dpos; // Entity delta position dx,dy
    int frame; // Animation frame timer
    int flags; // Entity flags
    SDL_Rect spriterect; // Rect of the entity's sprite
    SDL_Color color;

    void (*update)(Entity*, WSL_App*); // Entity update function
    void (*render)(Entity*, WSL_App*); // Entity render function
    void (*deathfunc)(Entity*, WSL_App*); // Function called on destruction
};

/*****
 * Entity - entity.c
 *****/
Entity *create_entity(SDL_Rect spriterect);
void destroy_entity(Entity *entity);

void update_entity(Entity *entity, WSL_App *game);
void render_entity(Entity *entity, WSL_App *game);

Vec2i get_sprite_coords(int index);
SDL_Rect get_hitbox(Entity *e);
bool check_collision_rect(SDL_Rect a, SDL_Rect b);

/*****
 * Player - entity_player.c
 *****/
Entity *create_player(SDL_Rect spriterect);
void spawn_player(WSL_App *game, float x, float y);
void update_player(Entity *player, WSL_App *game);

#endif //ENTITY_H
