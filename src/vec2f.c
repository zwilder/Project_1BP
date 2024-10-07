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

#include <vec2f.h>

Vec2f make_vec2f(float x, float y) {
    Vec2f result = {x,y};
    return result;
}

Vec2f add_vec2f(Vec2f a, Vec2f b) {
    return (make_vec2f(a.x + b.x, a.y + b.y));
}

Vec2f subtract_vec2f(Vec2f a, Vec2f b) {
    return (make_vec2f(a.x - b.x, a.y - b.y));
}

bool eq_vec2f(Vec2f a, Vec2f b) {
    return((a.x == b.x) && (a.y == b.y));
}

int man_dist_2f(Vec2f a, Vec2f b) {
    /* Returns the manhattan distance between two Vec2f */
    return (fabs(a.x - b.x) + fabs(a.y - b.y));
}

float lerp_f(float a, float b, float t) {
    /* Floating linear interpolation point, floating lerp */
    return(a + ((b-a) * t));
}

Vec2f lerp_vec2f(Vec2f a, Vec2f b, float t) {
    /* Floating linear interpolation Vec2f */
    return(make_vec2f(lerp_f(a.x,b.x,t),lerp_f(a.y,b.y,t)));
}

Vec2f get_vec2f_bezier(Vec2f start, Vec2f mid, Vec2f end, float t) {
    /* Lerp the lerps to get a lerp */
    return(lerp_vec2f(lerp_vec2f(start,mid,t),lerp_vec2f(mid,end,t),t));
}

Vec2f get_vec2f_bezier_opt(Vec2f start, Vec2f mid, Vec2f end, float t) {
    /* Optimized version of above, credit for this goes to
     * https://www.summbit.com/blog/bezier-curve-guide/ */
    const float remainder = 1 - t;
    const float stPtMult = remainder * remainder;
    const float midPtMult = remainder * t * 2;
    const float endPtMult = t * t;
    return make_vec2f(
            (start.x * stPtMult) + (mid.x * midPtMult) + (end.x * endPtMult),
            (start.y * stPtMult) + (mid.y * midPtMult) + (end.y * endPtMult));
}

Vec2f get_vec2f_cubic_bezier(Vec2f start, Vec2f ctA, Vec2f ctB, Vec2f end, float t) {
    return lerp_vec2f(
            get_vec2f_bezier(start, ctA, ctB, t),
            get_vec2f_bezier(ctA, ctB, end, t),
            t);
}

Vec2f get_vec2f_cubic_bezier_opt(Vec2f start, Vec2f ctA, Vec2f ctB, Vec2f end, float t) {
    /* Optimized version of above, credit for this goes to
     * https://www.summbit.com/blog/bezier-curve-guide/ 
     */
    const float remainder = 1 - t;
    const float tsq = t * t;
    const float remaindersq = remainder * remainder;
    const float stPtMult = remaindersq * remainder;
    const float ctAMult = remaindersq * t * 3;
    const float ctBMult = tsq * remainder * 3;
    const float endPtMult = tsq * t;
    return make_vec2f(
            (start.x * stPtMult) + (ctA.x * ctAMult) + (ctB.x * ctBMult) + (end.x * endPtMult),
            (start.y * stPtMult) + (ctA.y * ctAMult) + (ctB.y * ctBMult) + (end.y * endPtMult));
}
