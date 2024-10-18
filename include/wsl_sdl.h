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

#ifndef WSL_SDL_H
#define WSL_SDL_H

typedef struct Entity Entity;

typedef struct {
    SDL_Texture *tex;
    SDL_Renderer *renderer; // Maybe unnecessary
    int w;
    int h;
} WSL_Texture;

typedef struct {
    SDL_Window *window; // The SDL Window
    SDL_Surface *screen_surface;
    SDL_Renderer *renderer; // The SDL Renderer
    TTF_Font *font; // SDL Font

    Vec2i windowdim; // Window dimensions
    WSL_Texture *spritesheet; // Spritesheet with all the sprites
    WSL_Texture *bg; // Background texture, will be an array eventually?

    WSL_Texture *bufferA; // Double buffer action
    WSL_Texture *bufferB;
    WSL_Texture *cur_buffer; // The current buffer being rendered on the screen
    WSL_Texture *next_buffer; // The current buffer being rendered on the screen

    WSL_Texture *text; // Display text (Needs better name)
    bool keyboard[MAX_KEYBOARD_KEYS]; // Keypress "flags" for all keys

    bool running; // Simple "is this running?" flag
    bool fullscreen; 
    float dt; // Delta time - how much time has elapsed since last update
    bool scanlines; // Draw "scanlines" flag
    bool hitbox; // Draw hitbox flag
    int state; // Current game state

    Entity *entities; // Linked list of entities
} WSL_App;

/*****
 * WSL_App - wsl_sdl_app.c
 *****/
WSL_App* wsl_init_sdl(void);
void wsl_cleanup_sdl(WSL_App *app);
bool wsl_load_media(WSL_App *app);
bool wsl_create_scr_buffers(WSL_App *app);
void wsl_add_entity(WSL_App *app, Entity *entity);
Entity* wsl_remove_entity(WSL_App *app, Entity *entity);
void wsl_destroy_entity(WSL_App *app, Entity *entity);
void wsl_set_fullscreen(WSL_App *app);

/*****
 * WSL_Texture - wsl_sdl_texture.c
 *****/
WSL_Texture* create_wsl_texture(SDL_Renderer *renderer);
void destroy_wsl_texture(WSL_Texture *t);
bool wsl_texture_load(WSL_Texture *t, char *path);
WSL_Texture* wsl_create_blank_texture(SDL_Renderer *r, int w, int h);

#endif //WSL_SDL_H
