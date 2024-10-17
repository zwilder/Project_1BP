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

#ifndef XML_READ_H
#define XML_READ_H

XMLNode* find_tag(XMLNode *root, const char *tag);
XMLNode* find_tag_keyvalue(XMLNode *root, const char *tag, 
        const char *key, const char *value);
XMLAttribute* find_attribute(XMLAttribute* root, const char *key);
Vec2i load_tilemap(WSL_App *game, const char *xmlfile);

#endif //XML_READ
