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

#ifndef PROJECT1BP_H
#define PROJECT1BP_H

/*****
 * System
 *****/
#include <stdio.h>
#include <stdbool.h>

/*****
 * SDL2
 *****/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

/*****
 * Toolbox
 *****/
#include <mt19937.h>
#include <vec2i.h>
#include <vec2f.h>
#include <flags.h>
#include <xml_parse.h>

/*****
 * Project
 *****/
#include <defs.h>
#include <color.h>
#include <wsl_sdl.h>
#include <entity.h>
#include <handle_events.h>
#include <update.h>
#include <draw.h>
#include <load_tilemap.h>

#endif //PROJECT1BP_H
