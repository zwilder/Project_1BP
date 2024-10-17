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

SDL_Color hex_to_rgb(uint32_t hex) {
    SDL_Color color;
    color.r = (hex >> 16) & 0xFF;
    color.g = (hex >> 8) & 0xFF;
    color.b = hex & 0xFF;
    color.a = 255;
    return color;
}

SDL_Color str_to_color(const char *str) {
    if(!str) return hex_to_rgb(WHITE);

    if(strcmp(str, "JET_BLACK") == 0) return hex_to_rgb(JET_BLACK);
    if(strcmp(str, "CHARCOAL") == 0) return hex_to_rgb(CHARCOAL);
    if(strcmp(str, "MED_GRAY") == 0) return hex_to_rgb(MED_GRAY);
    if(strcmp(str, "LT_GRAY") == 0) return hex_to_rgb(LT_GRAY);
    if(strcmp(str, "SILVER") == 0) return hex_to_rgb(SILVER);
    if(strcmp(str, "WHITE") == 0) return hex_to_rgb(WHITE);
    if(strcmp(str, "DARK_BROWN") == 0) return hex_to_rgb(DARK_BROWN);
    if(strcmp(str, "CHESTNUT") == 0) return hex_to_rgb(CHESTNUT);
    if(strcmp(str, "BURNT_ORANGE") == 0) return hex_to_rgb(BURNT_ORANGE);
    if(strcmp(str, "TANGERINE") == 0) return hex_to_rgb(TANGERINE);
    if(strcmp(str, "SPRING_GREEN") == 0) return hex_to_rgb(SPRING_GREEN);
    if(strcmp(str, "LIME_GREEN") == 0) return hex_to_rgb(LIME_GREEN);
    if(strcmp(str, "BRIGHT_MAGENTA") == 0) return hex_to_rgb(BRIGHT_MAGENTA);
    if(strcmp(str, "HOT_PINK") == 0) return hex_to_rgb(HOT_PINK);
    if(strcmp(str, "RUST_RED") == 0) return hex_to_rgb(RUST_RED);
    if(strcmp(str, "TOMATO_RED") == 0) return hex_to_rgb(TOMATO_RED);
    if(strcmp(str, "CORAL") == 0) return hex_to_rgb(CORAL);
    if(strcmp(str, "BRONZE") == 0) return hex_to_rgb(BRONZE);
    if(strcmp(str, "GOLDENROD") == 0) return hex_to_rgb(GOLDENROD);
    if(strcmp(str, "DEEP_SKY_BLUE") == 0) return hex_to_rgb(DEEP_SKY_BLUE);
    if(strcmp(str, "LT_SKY_BLUE") == 0) return hex_to_rgb(LT_SKY_BLUE);
    if(strcmp(str, "AQUAMARINE") == 0) return hex_to_rgb(AQUAMARINE);

    // Default color if no match is found
    return hex_to_rgb(WHITE);
}
