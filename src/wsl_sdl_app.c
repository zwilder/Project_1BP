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

WSL_App* wsl_init_sdl(void) {
    bool success = true;
    int imgflags = IMG_INIT_PNG;
    int i = 0;

    WSL_App *app = malloc(sizeof(WSL_App));
    app->window = NULL;
    app->screen_surface = NULL;
    app->renderer = NULL;
    app->font = NULL;
    app->spritesheet = NULL;
    app->bg = NULL;

    app->bufferA = NULL;
    app->bufferB = NULL;
    app->cur_buffer = NULL;
    app->next_buffer = NULL;
    app->text = NULL;

    app->entities = NULL;

    app->windowdim.x = 768;
    app->windowdim.y = 720;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    // Create the window
    if(success) {
        app->window = SDL_CreateWindow("Project 1BP",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                app->windowdim.x, app->windowdim.y, SDL_WINDOW_SHOWN);
        if(!app->window) {
            printf("Unable to create window. SDL Error: %s\n",
                    SDL_GetError());
            success = false;
        }
    }

    // Create the renderer
    if(success) {
        app->renderer = SDL_CreateRenderer(app->window, -1,
                SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
        if(!app->renderer) {
            printf("Renderer could not be created. SDL Error %s\n",
                    SDL_GetError());
            success = false;
        }
    }

    // Initialize SDL_Image
    if(success) {
        SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        if(!(IMG_Init(imgflags) & imgflags)) {
            printf("SDL Image could not initialize. SDL image error: %s\n",
                    IMG_GetError());
            success = false;
        }
    }

    // Initialize SDL_ttf
    if(success) {
        if(TTF_Init() == -1) {
            //TTF_Init returns 0 on success, -1 on failure
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                    TTF_GetError());
            success = false;
        }
    }

    // Initialize SDL_Mixer
    if(success) {
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
            //Mix_OpenAudio returns 0 on success, -1 on failure
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                    Mix_GetError());
            success = false;
        }
    }

    // Get the window surface
    if(success) {
        app->screen_surface = SDL_GetWindowSurface(app->window);
    }

    // Load Media
    if(!wsl_load_media(app)) {
        success = false;
    }

    // Start screen buffer texture
    if(!wsl_create_scr_buffers(app)) {
        success = false;
    }

    // If everything went well, return app, otherwise cleanup and return NULL
    if(!success) {
        wsl_cleanup_sdl(app);
        app = NULL;
    } else {
        app->running = true;
        app->scanlines = true;
        app->fullscreen = false;
        app->hitbox = false;
        // Set keyboard flags to false
        for(i = 0; i < MAX_KEYBOARD_KEYS; i++) {
            app->keyboard[i] = false;
        }
    }
    
    return app;
}

void wsl_cleanup_sdl(WSL_App *app) {
    if(!app) return;
    Entity *entity = NULL;

    // Cleanup SDL
    destroy_wsl_texture(app->spritesheet);
    destroy_wsl_texture(app->bg);
    destroy_wsl_texture(app->text);
    destroy_wsl_texture(app->bufferA);
    destroy_wsl_texture(app->bufferB);

    SDL_FreeSurface(app->screen_surface);
    app->screen_surface = NULL;
    SDL_DestroyRenderer(app->renderer);
    app->renderer = NULL;
    SDL_DestroyWindow(app->window);
    app->window = NULL;
    TTF_CloseFont(app->font);
    app->font = NULL;

    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();

    // Cleanup entity list
    while(app->entities) {
        entity = app->entities;
        app->entities = app->entities->next;
        destroy_entity(entity);
    }

    free(app);
}

bool wsl_load_media(WSL_App *app) {
    // It would be super cool if this just loaded a single "1BP_Assets.dat"
    bool success = true;
    app->spritesheet = create_wsl_texture(app->renderer);
    app->text = create_wsl_texture(app->renderer);
    app->bg = create_wsl_texture(app->renderer);

    if(!app->bg || !app->text || !app->spritesheet) {
        success = false;
    } else {
        if(!wsl_texture_load(app->spritesheet,
                    "assets/monochrome_tilemap_packed.png")) {
            printf("Unable to load assets/monochrome_tilemap_packed.png!\n");
            success = false;
        }
        /*
        if(!wsl_texture_load(app->bg, "assets/black.png")) {
            printf("Unable to load assets/black.png!\n");
            success = false;
        }
        app->font = TTF_OpenFont("assets/kenvector_future.ttf",FONT_SIZE);
        if(!app->font) {
            printf("Unable to load assets/kenvector_future.ttf!\n");
            success = false;
        }
        */
    }

    return success;
}

bool wsl_create_scr_buffers(WSL_App *app) {
    /* Need to do the following twice, once for bufferA and once for bufferB */
    if(app->bufferA) {
        destroy_wsl_texture(app->bufferA);
        app->bufferA = NULL;
    }
    if(app->bufferB) {
        destroy_wsl_texture(app->bufferB);
        app->bufferB = NULL;
    }
    app->bufferA = wsl_create_blank_texture(app->renderer, SCREEN_W, SCREEN_H);
    app->bufferB = wsl_create_blank_texture(app->renderer, SCREEN_W, SCREEN_H);
    app->cur_buffer = app->bufferA; // buffer is the current rendered buffer
    app->next_buffer = app->bufferB;

    /*
    WSL_Texture *t = malloc(sizeof(WSL_Texture));
    t->tex = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
    if(t->tex == NULL) {
        printf("Unable to create screen buffer texture! SDL_Error: %s\n",
                SDL_GetError());
    } else {
        t->w = SCREEN_W;
        t->h = SCREEN_H;
    }
    app->buffer = t;
    */
    
    return ((app->bufferA != NULL) && (app->bufferB != NULL));
}

void wsl_add_entity(WSL_App *app, Entity *entity){
    Entity *e = NULL;
    if(!app || !entity) return;
    if(!app->entities) {
        // First entity in list!
        app->entities = entity;
        return;
    }
    // Find last entity, set it's next to entity
    e = app->entities;
    while(e->next) {
        e = e->next;
    }
    e->next = entity;

    // Set entity's previous to the last entity
    entity->prev = e;
}

Entity* wsl_remove_entity(WSL_App *app, Entity *entity){
    /* 
     * Find Entity in app->entities, update the previous entity's next and the
     * next entity's previous to each other, and then return the entity that was
     * removed from the list.
     */
    if(!app || !entity) return NULL;
    if(!app->entities) return NULL;
    Entity *e = app->entities;
    Entity *prev = NULL;
    if(e == entity) {
        app->entities = app->entities->next;
        e->next = NULL;
        e->prev = NULL;
    } else {
        while(e != entity && e) {
            prev = e;
            e = e->next;
        }
        if(e) {
            prev->next = e->next;
            if(e->next) {
                e->next->prev = prev;
            }
            e->next = NULL;
            e->prev = NULL;
        }
    }
    return e;
}

void wsl_destroy_entity(WSL_App *app, Entity *entity){
    if(!app) {
        destroy_entity(entity);
        return;
    } 
    // Remove entity from game list
    if(!entity) return;
    if(wsl_remove_entity(app, entity)) {
        // Destroy entity
        destroy_entity(entity);
    }
}

void wsl_set_fullscreen(WSL_App *app) {
    if(app->fullscreen) {
        // Already fullscreen, change back to windowed
        SDL_SetWindowFullscreen(app->window, 0);
        SDL_SetWindowSize(app->window, 768, 720);
        SDL_GetWindowSize(app->window, &(app->windowdim.x), &(app->windowdim.y));
        app->fullscreen = false;
    } else {
        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_GetWindowSize(app->window, &(app->windowdim.x), &(app->windowdim.y));
        app->fullscreen = true;
    }
}
