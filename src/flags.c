/*
* Goblin Caves
* Copyright (C) Zach Wilder 2022-2023
* 
* This file is a part of Goblin Caves
*
* Goblin Caves is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Goblin Caves is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Goblin Caves.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <flags.h>

/*
 * These are all just short hand because sometimes I forget which binary
 * operator I want, and it's honestly easier to read "engage_flag"
 */
bool check_flag(int mask, int flag) {
    /* Is flag on? */
    /* Example:
     * a = 1011
     * b = 0001
     * a & b = 0001
     * (a & b) == b is 0001 == 0001, which is true. 
     */ 
    return ((mask & flag) == flag);
}
void toggle_flag(int *mask, int flag) {
    /* Toggles flag on/off */
    /* Example:
     * a = 1011
     * b = 0010
     * a ^= b is 1001, turning the flag off.
     *
     * a = 1001
     * a ^= b is 1011, turning the flag on.
     * Neat!
     */
    //return (mask ^= flag);
    *mask ^= flag;
}
void remove_flag(int *mask, int flag) {
    /* Turns off flag */
    /* Example:
     * a = 1011
     * b = 0010
     * ~b is 1101
     * a &= ~b is 1011 &= 1101 which is 1001.
     * Turning the flag off. If the flag is already off:
     * a = 1001
     * b = 0010, ~b = 1101
     * a &= ~b is 1001 &= 1101, which is 1001. Flag stays off.
     */
    //return (mask &= ~flag);
    *mask &= ~flag;
}
void engage_flag(int *mask, int flag) {
    /* Turns on flag */
    /* Example:
     * a = 1001
     * b = 0010
     * a |= b is 1011, turning the flag on. If flag is already on:
     * a = 1011
     * b = 0010
     * a |= b is 1011, keeping the flag on.
     */
    //return (mask |= flag);
    *mask |= flag;
}

