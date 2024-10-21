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

/*
 * Take a parsed XML file (from an XML_Node), and create game entities
 */

void assign_colors(Entity *e, int x, int y, int num, 
        SDL_Rect *rects, char **colors) {
    /* load_tilemap() was getting chonky, assign_colors and assign_objects were
     * good, easy things to pull out and put into it's own function */
    int i = 0;
    for(i = 0; i < num; i++) {
        if(xy_in_rect((x * TILE_SIZE), (y * TILE_SIZE), rects[i])) {
            if(colors[i]) {
                e->color = str_to_color(colors[i]);
            }
        }
    }
}

void assign_objects(Entity *e, int id, int x, int y, int num,
        SDL_Rect *rects, char **objects, char **names) {
    int i = 0;
    for(i = 0; i < num; i++) {
        if(xy_in_rect(x * TILE_SIZE, y * TILE_SIZE, rects[i])) {
            if(objects[i]) {
                if(strcmp(objects[i], "tile") == 0) e->flags |= EF_TILE;
                if(strcmp(objects[i], "platform") == 0) e->flags |= EF_PLATFORM;
                if(strcmp(objects[i], "start") == 0) {
                    printf("Start read at %d,%d.\n",x,y);
                    e->flags |= EF_START; 
                }
                if(strcmp(objects[i], "anim") == 0 ) {
                    if(names[i]) {
                        e->spriteframes = malloc(sizeof(SDL_Rect) * 2);
                        e->framecount = 2;
                        e->spriteframes[0] = get_sprite_rect(id - 1);
                        e->spriteframes[1] = get_sprite_rect(atoi(names[i]));
                        e->update = &update_tile;
                    }
                }
                if(strcmp(objects[i], "flip") == 0) e->flip = SDL_FLIP_HORIZONTAL;
            }
        }

    }
}

Vec2i load_tilemap(WSL_App *game, const char *xmlfile) {
    /* Take an XML/tmx file (generated from Tiled), create tiles, return the
     * "start" location
     */
    Vec2i result = {0,0};
    Vec2i mapdim = {0,0}; 
    XMLAttribute *attr = NULL;
    XMLNode *layer = NULL, *data = NULL;
    XMLNode *xml = parse_xml(xmlfile);
    Token *lines = NULL;
    Token *ytmp = NULL;
    Token *cols = NULL;
    Token *xtmp = NULL;
    Entity *tile = NULL;
    SDL_Rect *color_rects = NULL;
    SDL_Rect *obj_rects = NULL;
    char **color = NULL;
    char **objects = NULL;
    char **obj_names = NULL;
    int i = 0, x = 0, y = 0, id = 0, num_colors = 0, num_objects = 0;

    // Find the "objectgroup" tag with name=colors
    layer = find_tag_keyvalue(xml, "objectgroup", "name", "colors");
    if(layer) {
        // count the number of objects
        data = layer->children;
        num_colors = count_siblings_XMLNode(layer->children);

        // Allocate memory for array of color_rects
        color_rects = malloc(sizeof(SDL_Rect) * num_colors);
        color = malloc(sizeof(char*) * num_colors);
        for(i = 0; i < num_colors; i++) {
            attr = find_attribute(data->attributes, "x");
            if(attr) color_rects[i].x = atoi(attr->value);
            attr = find_attribute(data->attributes, "y");
            if(attr) color_rects[i].y = atoi(attr->value);
            attr = find_attribute(data->attributes, "width");
            if(attr) color_rects[i].w = atoi(attr->value);
            attr = find_attribute(data->attributes, "height");
            if(attr) color_rects[i].h = atoi(attr->value);
            attr = find_attribute(data->attributes, "type");
            if(attr) {
                color[i] = attr->value;
            } else {
                color[i] = NULL;
            }
            data = data->next;
        }
    }

    // Find the "objectgroup" tag with name=objects
    layer = find_tag_keyvalue(xml, "objectgroup", "name", "objects");
    if(layer) {
        data = layer->children;
        num_objects = count_siblings_XMLNode(layer->children);

        // Allocate memory for array of obj_rects and char *objects
        obj_rects = malloc(sizeof(SDL_Rect) * num_objects);
        objects = malloc(sizeof(char*) * num_objects);
        obj_names = malloc(sizeof(char*) * num_objects);
        for(i = 0; i < num_objects; i++) {
            attr = find_attribute(data->attributes, "x");
            if(attr) obj_rects[i].x = atoi(attr->value);
            attr = find_attribute(data->attributes, "y");
            if(attr) obj_rects[i].y = atoi(attr->value);
            attr = find_attribute(data->attributes, "width");
            if(attr) obj_rects[i].w = atoi(attr->value);
            attr = find_attribute(data->attributes, "height");
            if(attr) obj_rects[i].h = atoi(attr->value);
            attr = find_attribute(data->attributes, "type");
            if(attr) {
                objects[i] = attr->value;
            } else {
                objects[i] = NULL;
            }
            attr = find_attribute(data->attributes, "name");
            if(attr) {
                obj_names[i] = attr->value;
            } else {
                obj_names[i] = NULL;
            }
            data = data->next;
        }
        for(i = 0; i < num_objects; i++) {
            if(objects[i]) {
                printf("Objects[%d] is a %s\n",i, objects[i]);
            }
            if(obj_names[i]) {
                printf("\tName: %s\n", obj_names[i]);
            }
        }
    }

    // Find the "layer" tag
    layer = find_tag(xml, "layer");
    if(!layer) {
        destroy_XMLNode(xml);
        return result;
    }
    // Get the map height/width in tiles from layer->attributes, subtract 1 from each (0 indexed)
    attr = find_attribute(layer->attributes, "width");
    if(attr != NULL) {
        mapdim.x = atoi(attr->value);
    }
    attr = find_attribute(layer->attributes, "height");
    if(attr != NULL) {
        mapdim.y = atoi(attr->value);
    }
    // Find "data" tag in layer->children
    data = find_tag(layer->children, "data");
    if(!data) {
        destroy_XMLNode(xml);
        return result;
    }
    if(!(data->text)) {
        destroy_XMLNode(xml);
        return result;
    }

    // data->text is then split by newlines, then by commas. 
    // each line is a row (y) of tiles. Loop through each row and each comma
    // separated entry to get sprite indexes. Create tiles for each at that x,y.
    lines = create_token_list(data->text, '\n', '\0');
    ytmp = lines;
    y = -1;
    
    while(ytmp) {
        destroy_token_list(cols);
        cols = create_token_list(ytmp->s, ',','\0');
        xtmp = cols;
        x = 0;
        while(xtmp) {
            // Create tile with spriterect at id - 1 (0 indexed)
            id = atoi(xtmp->s);
            if(id != 0) {
                tile = create_tilev(get_map_coords(x,y),get_sprite_rect(id -1));
                // For each created tile, check to see if it's x,y is inside one
                // of the identified color/object rects. If it is, modify the tile's
                // attributes accordingly.
                assign_colors(tile, x, y, num_colors, color_rects, color);
                assign_objects(tile, id, x, y, num_objects, obj_rects, 
                        objects, obj_names);
                if(check_flag(tile->flags, EF_START)) {
                    result = tile->pos;
                    printf("Start assigned to tile. Result is: %d,%d\n",result.x,result.y);
                }
                wsl_add_entity(game,tile);
            }
            xtmp = xtmp->next;
            x += 1;
        }
        ytmp = ytmp->next;
        y += 1;
    }
    

    //Cleanup
    free(color_rects);
    free(obj_rects);
    free(color); // just freeing the array of pointers, the actual strings are owned elsewhere
    free(objects);
    free(obj_names);
    destroy_token_list(lines);
    destroy_token_list(cols);
    destroy_XMLNode(xml);
    printf("Loaded %s, w/h %d/%d\n", xmlfile, mapdim.x,mapdim.y);
    return result;
}
