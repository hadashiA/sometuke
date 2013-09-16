#include "sometuke/tmx_parser.h"

#include "sometuke/director.h"
#include "sometuke/logger.h"

#include "sometuke/ext/rapidxml/rapidxml.hpp"

namespace sometuke {
using namespace rapidxml;

shared_ptr<TmxMapInfo> TmxParser::Parse(const string& file) {
    auto map_info = make_shared<TmxMapInfo>();

    string bytes = Director::Instance().file_utils().ReadString(file);

    xml_document<> doc;
    doc.parse<0>((char *)(bytes.c_str()));

    xml_node<> *mapnode = doc.first_node("map");
    string version = mapnode->first_attribute("version")->value();
    if (version != "1.0") {
        S2ERROR("TmxParser unsupported TMX version: %s", version.c_str());
        return map_info;
    }

    int width  = atoi(mapnode->first_attribute("width")->value());
    int height = atoi(mapnode->first_attribute("height")->value());
    map_info->map_size = ivec2(width, height);

    float tile_width  = atof(mapnode->first_attribute("tilewidth")->value());
    float tile_height = atof(mapnode->first_attribute("tileheight")->value());
    map_info->tile_size = vec2(tile_width, tile_height);

    xml_node<> *tilesetnode = mapnode->first_node("tileset");
    while (tilesetnode) {
        auto tileset_info = make_shared<TmxTilesetInfo>();
        tileset_info->first_gid = atoi(tilesetnode->first_attribute("firstgid")->value());

        map_info->tilesets.push_back(tileset_info);
    }
    

    return map_info;
}
    
}

