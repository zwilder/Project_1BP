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

WSL_Texture* create_wsl_texture(SDL_Renderer *renderer) {
    WSL_Texture *t = malloc(sizeof(WSL_Texture));
    t->tex = NULL;
    t->w = 0;
    t->h = 0;
    t->renderer = renderer;
    return t;
}

void destroy_wsl_texture(WSL_Texture *t) {
    if(!t) return;
    if(t->tex) {
        SDL_DestroyTexture(t->tex);
    }
    free(t);
    t = NULL;
}

bool wsl_texture_load(WSL_Texture *t, char *path) {
    SDL_Surface *loaded = NULL;
    
    if(!t) return false;
    if(t->tex) {
        SDL_DestroyTexture(t->tex);
        t->tex = NULL;
    }
    
    loaded = IMG_Load(path);
    if(!loaded) {
        printf("Unable to load image %s! SDL_image Error: %s\n",
                path, IMG_GetError());
        return false;
    }

    // 0: nearest, 1: linear, 2: best (Same as linear according to SDL2Wiki)
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
    {
        printf("Warning: Linear texture filtering not enabled!");
    }

    // SDL_SetColorKey sets the "transparent" color of the texture
    SDL_SetColorKey(loaded, SDL_TRUE, SDL_MapRGB(loaded->format, 0, 0, 0));
    t->tex = SDL_CreateTextureFromSurface(t->renderer, loaded);
    if(!t->tex) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path,
                SDL_GetError());
        return false;
    }
    t->w = loaded->w;
    t->h = loaded->h;

    SDL_FreeSurface(loaded);
    return true;
}

WSL_Texture* wsl_create_blank_texture(SDL_Renderer *r, int w, int h) {
    WSL_Texture *t = malloc(sizeof(WSL_Texture));
    t->tex = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, w, h);
    if(t->tex == NULL) {
        printf("Unable to create blank texture! SDL_Error: %s\n",
                SDL_GetError());
    } else {
        t->w = SCREEN_W;
        t->h = SCREEN_H;
    }

    return t;
}
