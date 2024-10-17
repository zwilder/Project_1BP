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

XMLNode* find_tag(XMLNode *root, const char *tag) {
    // Depth first search
    XMLNode *result = NULL;
    // If root is NULL, obviously return NULL
    if(root == NULL) {
        return result;
    }

    // If root is the tag we are looking for, return root
    if(strcmp(root->tag, tag) == 0) {
        return root;
    }

    // Check children
    result = find_tag(root->children, tag);
    if(result != NULL) {
        return result;
    }

    // Check siblings
    result = find_tag(root->next, tag);
    return result;
}

XMLNode* find_tag_keyvalue(XMLNode *root, const char *tag, 
        const char *key, const char *value) {
    // Find a tag with an attribute key/value pair
    XMLNode *result = NULL;
    XMLAttribute *attr = NULL;

    // If root is NULL, return NULL
    if(root == NULL) return NULL;

    // If root is the tag with the right attribute key/value
    if(strcmp(root->tag, tag) == 0) {
        attr = find_attribute(root->attributes, key);
        if(attr) {
            if(strcmp(attr->value, value) == 0) {
                return root;
            }
        }
    }

    // Check children
    result = find_tag_keyvalue(root->children, tag, key, value);
    if(result != NULL) {
        return result;
    }

    // Check siblings
    result = find_tag_keyvalue(root->next, tag, key, value);
    return result;
}

XMLAttribute* find_attribute(XMLAttribute* root, const char *key) {
    XMLAttribute *result = NULL;
    if(root == NULL) {
        return result;
    }
    if(strcmp(root->key, key) == 0) {
        return root;
    }
    result = find_attribute(root->next, key);
    return result;
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
            if(attr) color[i] =attr->value;
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
            if(attr) objects[i] = attr->value;
            attr = find_attribute(data->attributes, "name");
            if(attr) obj_names[i] = attr->value;
            data = data->next;
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
                for(i = 0; i < num_colors; i++) {
                    if(xy_in_rect((x * TILE_SIZE), (y * TILE_SIZE), color_rects[i])) {
                        if(color[i]) {
                            tile->color = str_to_color(color[i]);
                        }
                    }
                }
                for(i = 0; i < num_objects; i++) {
                    if(xy_in_rect(x * TILE_SIZE, y * TILE_SIZE, obj_rects[i])) {
                        if(objects[i]) {
                            if(strcmp(objects[i], "tile") == 0) tile->flags |= EF_TILE;
                            if(strcmp(objects[i], "platform") == 0) tile->flags |= EF_PLATFORM;
                            if(strcmp(objects[i], "start") == 0)  {
                                result.x = tile->pos.x;
                                result.y = tile->pos.y;
                            }
                        }
                    }

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
