/*
* Toolbox
* Copyright (C) Zach Wilder 2022-2024
* 
* This file is a part of Toolbox
*
* Toolbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Toolbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Toolbox.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef VEC2F_H
#define VEC2F_H

#include <math.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
} Vec2f;

/*****
 * Vec2f
 *****/
Vec2f make_vec2f(float x, float y);
Vec2f add_vec2f(Vec2f a, Vec2f b);
Vec2f subtract_vec2f(Vec2f a, Vec2f b);
bool eq_vec2f(Vec2f a, Vec2f b);
float lerp_f(float a, float b, float t);
Vec2f lerp_vec2f(Vec2f a, Vec2f b, float t);
Vec2f get_vec2f_bezier(Vec2f start, Vec2f mid, Vec2f end, float t);
Vec2f get_vec2f_bezier_opt(Vec2f start, Vec2f mid, Vec2f end, float t);
Vec2f get_vec2f_cubic_bezier(Vec2f start, Vec2f ctA, 
        Vec2f ctB, Vec2f end, float t);
Vec2f get_vec2f_cubic_bezier_opt(Vec2f start, Vec2f ctA, 
        Vec2f ctB, Vec2f end, float t);

#endif //VEC2F_H
