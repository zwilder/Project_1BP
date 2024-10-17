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

#ifndef COLOR_H
#define COLOR_H

/*****
 * Palette: Chroma Noir - VEXED
 * https://lospec.com/palette-list/chroma-noir
 *****/
#define JET_BLACK 0x0d0d0d
#define CHARCOAL 0x383838
#define MED_GRAY 0x4f4f4f
#define LT_GRAY 0x828282
#define SILVER 0xb5b5b5
#define WHITE 0xd9d9d9
#define DARK_BROWN 0x4c2712
#define CHESTNUT 0x60361d
#define BURNT_ORANGE 0xa86437
#define TANGERINE 0xe67a30
#define SPRING_GREEN 0x4ae364
#define LIME_GREEN 0x99e550
#define BRIGHT_MAGENTA 0xd151ee
#define HOT_PINK 0xf873e4
#define RUST_RED 0x9c3a2b
#define TOMATO_RED 0xe64e35
#define CORAL 0xf25a5a
#define BRONZE 0xad8830
#define GOLDENROD 0xf7c756
#define DEEP_SKY_BLUE 0x306082
#define LT_SKY_BLUE 0x639bff
#define AQUAMARINE 0x4dcced

SDL_Color hex_to_rgb(uint32_t hex);
SDL_Color str_to_color(const char *str);

#endif //COLOR_H
