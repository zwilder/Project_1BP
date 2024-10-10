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
    Vec2i xy;
    Entity *player = create_entity(spriterect);
    player->flags |= EF_ALIVE | EF_ONGROUND;

    player->update = &update_player;
    player->color = hex_to_rgb(SPRING_GREEN);

    //Hardcoded animation stuff should be replaced by a proper component later
    player->spriteframes = malloc(3 *sizeof(SDL_Rect));
    player->spriteframes[0] = spriterect;
    player->spriteframes[1] = spriterect;
    player->spriteframes[2] = spriterect;

    xy = get_sprite_coords(241);
    player->spriteframes[0].x = xy.x;
    player->spriteframes[0].y = xy.y;

    xy = get_sprite_coords(242);
    player->spriteframes[1].x = xy.x;
    player->spriteframes[1].y = xy.y;

    xy = get_sprite_coords(243);
    player->spriteframes[2].x = xy.x;
    player->spriteframes[2].y = xy.y;

    player->framecount = 2;
    return player;
}

void entity_set_sprite(Entity *e, int id) {
    Vec2i xy = get_sprite_coords(id);
    e->spriterect.x = xy.x;
    e->spriterect.y = xy.y;
}
void update_player(Entity *player, WSL_App *game) {
    // Save some typing below with logic checking
    bool btn_up = game->keyboard[SDL_SCANCODE_UP];
    bool btn_lt = game->keyboard[SDL_SCANCODE_LEFT];
    bool btn_rt = game->keyboard[SDL_SCANCODE_RIGHT];
    bool btn_sp = game->keyboard[SDL_SCANCODE_SPACE];

    float speed = 8.0f; // Temporarily here, should be stored somewhere
    float acceleration = 1.0f;
    float jumpheight = -12.0f;

    player->frame += 1;
    //Update state
    switch(player->state) {
        case ST_IDLE:
            entity_set_sprite(player,241);
            if(!check_flag(player->flags, EF_ONGROUND)){
                player->prev_state = ST_IDLE;
                player->state = ST_JUMP;
            } else if(btn_sp) {
                player->prev_state = ST_IDLE;
                player->state = ST_JUMP;
                player->dpos.y = jumpheight;
            } else if (btn_lt != btn_rt) {
                player->prev_state = ST_IDLE;
                player->state = ST_WALK;
            }
            break;
        case ST_WALK:
            if(player->frame / 4 > player->framecount) {
                player->frame = 0;
            }
            player->spriterect = player->spriteframes[player->frame / 4];
            // Accelerate to max speed
            if(!check_flag(player->flags, EF_ONGROUND)){
                player->prev_state = ST_IDLE;
                player->state = ST_JUMP;
            } else if(btn_lt) {
                player->flip = SDL_FLIP_HORIZONTAL;
                player->dpos.x -= acceleration;
            } else if (btn_rt) {
                player->flip = SDL_FLIP_NONE;
                player->dpos.x += acceleration;
            } else {
                player->prev_state = ST_WALK;
                player->state = ST_IDLE;
            }

            if(player->dpos.x < (-1 * speed)) {
                player->dpos.x = -1 * speed;
            }

            if(player->dpos.x > speed) {
                player->dpos.x = speed;
            }

            if(btn_sp) {
                player->prev_state = ST_WALK;
                player->state = ST_JUMP;
                player->dpos.y = jumpheight;
            }

            break;
        case ST_JUMP:
            if(player->dpos.x > 0) {
                entity_set_sprite(player, 244);
            } else if(player->dpos.x < 0) {
                entity_set_sprite(player, 244);
            } else {
                entity_set_sprite(player,245);
            }
            if(check_flag(player->flags, EF_ONGROUND)) {
                player->prev_state = ST_JUMP;
                player->state = ST_IDLE;
            }
            break;
        default:
            break;
    }
    
    //Update physics
    handle_physics(player,game);
    //Update animations

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
