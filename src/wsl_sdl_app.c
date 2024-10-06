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

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    // Create the window
    if(success) {
        app->window = SDL_CreateWindow("Project 1BP",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                1024, 768, SDL_WINDOW_SHOWN);
        if(!app->window) {
            printf("Unable to create window. SDL Error: %s\n",
                    SDL_GetError());
            success = false;
        }
    }

    // Create the renderer
    if(success) {
        app->renderer = SDL_CreateRenderer(app->window, -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

    if(!wsl_load_media(app)) {
        success = false;
    }

    // If everything went well, return app, otherwise cleanup and return NULL
    if(!success) {
        wsl_cleanup_sdl(app);
        app = NULL;
    } else {
        app->running = true;
        // Set keyboard flags to false
        for(i = 0; i < MAX_KEYBOARD_KEYS; i++) {
            app->keyboard[i] = false;
        }
    }
    
    return app;
}

void wsl_cleanup_sdl(WSL_App *app) {
    if(!app) return;

    // Cleanup SDL
    destroy_wsl_texture(app->spritesheet);
    destroy_wsl_texture(app->bg);
    destroy_wsl_texture(app->text);
    SDL_DestroyRenderer(app->renderer);
    app->renderer = NULL;
    SDL_DestroyWindow(app->window);
    app->window = NULL;
    IMG_Quit();
    TTF_CloseFont(app->font);
    TTF_Quit();
    //Mix_Quit();
    SDL_Quit();

    free(app);
}

bool wsl_load_media(WSL_App *app) {
    bool success = true;
    //int i;
    app->spritesheet = create_wsl_texture(app->renderer);
    if(!wsl_texture_load(app->spritesheet,
                "assets/monochrome_tilemap_packed.png")) {
        printf("Unable to load assets/monochrome_tilemap_packed.png!\n");
        success = false;
    }
    /*
    app->bg = create_wsl_texture(app->renderer);
    if(!wsl_texture_load(app->bg, "assets/black.png")) {
        printf("Unable to load assets/black.png!\n");
        success = false;
    }
    app->font = TTF_OpenFont("assets/kenvector_future.ttf",FONT_SIZE);
    if(!app->font) {
        printf("Unable to load assets/kenvector_future.ttf!\n");
        success = false;
    }
    app->text = create_wsl_texture(app->renderer);
    */

    return success;
}

