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

static vector<unsigned char> base64_decode(const string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    vector<unsigned char> ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
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

        for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
    }

    return ret;
}

shared_ptr<TmxMapInfo> TmxParser::Parse(const string& file) {
    string dirname = Director::Instance().file_utils().Dirname(file);

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

    string orientation_str = mapnode->first_attribute("orientation")->value();
    if (orientation_str == "orthogonal") {
        map_info->orientation = TmxOrientation::ORTHO;
    } else if (orientation_str == "isometric") {
        map_info->orientation = TmxOrientation::ISO;
    } else if (orientation_str == "hexagonal") {
        map_info->orientation = TmxOrientation::HEX;
    } else {
        S2ERROR("TmxFormat Unsupported orientation: %s", orientation_str.c_str());
    }

    int width  = atoi(mapnode->first_attribute("width")->value());
    int height = atoi(mapnode->first_attribute("height")->value());
    map_info->map_size = ivec2(width, height);

    float tile_width  = atof(mapnode->first_attribute("tilewidth")->value());
    float tile_height = atof(mapnode->first_attribute("tileheight")->value());
    map_info->tile_size = ivec2(tile_width, tile_height);

    // tileset
    xml_node<> *tilesetnode = mapnode->first_node("tileset");
    while (tilesetnode) {
        auto external_tileset_filename = tilesetnode->first_attribute("source");
        if (external_tileset_filename) {
            S2ERROR("TmxParser external tileset filename not supported.");
            return map_info;
        } else {
            TmxTilesetInfo tileset_info;
            tileset_info.name        = tilesetnode->first_attribute("name")->value();
            tileset_info.first_gid   = atoi(tilesetnode->first_attribute("first_gid")->value());
            tileset_info.spacing     = atoi(tilesetnode->first_attribute("spacing")->value());
            tileset_info.margin      = atoi(tilesetnode->first_attribute("margin")->value());
            tileset_info.tile_size.x = atoi(tilesetnode->first_attribute("tilewidth")->value());
            tileset_info.tile_size.y = atoi(tilesetnode->first_attribute("tileheight")->value());

            xml_node<> *imagenode = tilesetnode->first_node("image");
            string image_filename = imagenode->first_attribute("source")->value();
            string image_basename = Director::Instance().file_utils().Basename(image_filename);
            tileset_info.image_source = dirname + "/" + image_basename;

            map_info->tilesets.push_back(tileset_info);
        }
        
        tilesetnode = tilesetnode->next_sibling("tileset");
    }

    xml_node<> *layernode = mapnode->first_node("layer");
    while (layernode) {
        TmxLayerInfo layer_info;
        layer_info.name = layernode->first_attribute("name")->value();
        layer_info.num_tiles.x  = atoi(layernode->first_attribute("width")->value());
        layer_info.num_tiles.y = atoi(layernode->first_attribute("height")->value());

        auto visible_attr = layernode->first_attribute("visible");
        if (visible_attr) {
            layer_info.visible = (string(visible_attr->value()) != "0");
        }

        auto opacity_attr = layernode->first_attribute("opacity");
        if (opacity_attr) {
            layer_info.opacity = atoi(opacity_attr->value());
        }

        layer_info.pos.x = atoi(layernode->first_attribute("x")->value());
        layer_info.pos.y = atoi(layernode->first_attribute("y")->value());

        xml_node<> *datanode = layernode->first_node("data");
        if (datanode) {
            string encoding    = datanode->first_attribute("encoding")->value();
            string compression = datanode->first_attribute("compression")->value();

            TmxFormat format;
            if (encoding == "base64") {
                format = TmxFormat::BASE64;
            } else {
                S2ERROR("TmxParser only basr64 and/or gzip/zlib maps are supported");
                return map_info;
            }

            TmxCompression c = TmxCompression::NONE;
            if (compression == "gzip") {
                c = TmxCompression::GZIP;
            } else if (compression == "zlib") {
                c = TmxCompression::ZLIB;
            }

            layer_info.gids = ParseLayerData(datanode->value(), format, c,
                                             layer_info.num_tiles.x * layer_info.num_tiles.y);
        }

        map_info->layers.push_back(layer_info);
        layernode = layernode->next_sibling("layer");
    }

    return map_info;
}
    
vector<unsigned int> TmxParser::ParseLayerData(const string data,
                                               TmxFormat format, TmxCompression compression,
                                               size_t num_tiles) {
    vector<unsigned int> gids;

    size_t size = num_tiles * sizeof(uint32_t);

    if (format == TmxFormat::BASE64) {
        vector<unsigned char> buffer = base64_decode(data);
        
    }
    return gids;
}

}

