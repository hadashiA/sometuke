#include "sometuke/tmx_parser.h"

#include "sometuke/director.h"
#include "sometuke/logger.h"

#include "sometuke/ext/rapidxml/rapidxml.hpp"

namespace sometuke {
using namespace rapidxml;

static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


static bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

static string base64_decode(const string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

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

