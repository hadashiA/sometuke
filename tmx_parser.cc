#include "sometuke/tmx_parser.h"

#include "sometuke/director.h"
#include "sometuke/logger.h"

#include "sometuke/ext/rapidxml/rapidxml.hpp"
#include "sometuke/zlib_utils.h"

namespace sometuke {
using namespace rapidxml;

static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


static bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

static unsigned char *base64_decode(const string& encoded_string, size_t *decoded_length) {
    size_t input_length = encoded_string.size();
    if (input_length % 4 != 0) {
        return nullptr;
    }

    *decoded_length = input_length / 4 * 3;
    unsigned char *decoded_data = static_cast<unsigned char *>(malloc(*decoded_length));

    int i = 0;
    int j = 0;
    int in_ = 0;
    size_t decoded_idx = 0;
    unsigned char char_array_4[4], char_array_3[3];

    while (input_length-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                decoded_data[decoded_idx++] = char_array_3[i];
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

        for (j = 0; (j < i - 1); j++)
            decoded_data[decoded_idx++] = char_array_3[j];
    }

    return decoded_data;
}

shared_ptr<TmxMapInfo> TmxParser::Parse(const string& file) {
    string dirname = Director::Instance().file_utils().Dirname(file);

    auto map_info = make_shared<TmxMapInfo>();

    string bytes = Director::Instance().file_utils().ReadString(file);
    if (bytes.empty()) {
        S2ERROR("TmxParser: read faild %s", file.c_str());
        return map_info;
    }

    xml_document<> doc;
    doc.parse<0>((char *)(bytes.c_str()));

    xml_node<> *map_node = doc.first_node("map");
    map_info.version = atof(map_node->first_attribute("version")->value());
    if (version != 1.0) {
        S2ERROR("TmxParser unsupported TMX version: %s", version.c_str());
        return map_info;
    }

    string orientation_str = map_node->first_attribute("orientation")->value();
    if (orientation_str == "orthogonal") {
        map_info->orientation = TmxOrientation::ORTHO;
    } else if (orientation_str == "isometric") {
        map_info->orientation = TmxOrientation::ISO;
    } else if (orientation_str == "hexagonal") {
        map_info->orientation = TmxOrientation::HEX;
    } else {
        S2ERROR("TmxFormat Unsupported orientation: %s", orientation_str.c_str());
    }

    map_info.size_in_tiles.x = atoi(map_node->first_attribute("width")->value());
    map_info.size_in_tiles.y = atoi(map_node->first_attribute("height")->value());

    map_info.tile_size.x = atof(map_node->first_attribute("tilewidth")->value());
    map_info.tile_size.y = atof(map_node->first_attribute("tileheight")->value());

    if (auto backgroundcolor_attr = map_node->first_attribute("backgroundcolor")) {
        map_info.background_color = Color3B(backgroundcolor_attr->value());
    }

    // tileset
    xml_node<> *tilesetnode = map_node->first_node("tileset");
    while (tilesetnode) {
        auto external_tileset_filename = tilesetnode->first_attribute("source");
        if (external_tileset_filename) {
            S2ERROR("TmxParser external tileset filename not supported.");
            return map_info;
        } else {
            TmxTilesetInfo tileset_info;
            tileset_info.name        = tilesetnode->first_attribute("name")->value();
            tileset_info.first_gid   = atoi(tilesetnode->first_attribute("firstgid")->value());
            tileset_info.tile_size.x = atoi(tilesetnode->first_attribute("tilewidth")->value());
            tileset_info.tile_size.y = atoi(tilesetnode->first_attribute("tileheight")->value());

            if (auto spacing = tilesetnode->first_attribute("spacing")) {
                tileset_info.spacing = atoi(spacing->value());
            }
            if (auto margin = tilesetnode->first_attribute("margin")) {
                tileset_info.margin = atoi(margin->value());
            }

            xml_node<> *imagenode = tilesetnode->first_node("image");
            string image_filename = imagenode->first_attribute("source")->value();
            string image_basename = Director::Instance().file_utils().Basename(image_filename);
            tileset_info.image_source = dirname + "/" + image_basename;

            map_info->tilesets.push_back(tileset_info);
        }
        
        tilesetnode = tilesetnode->next_sibling("tileset");
    }

    xml_node<> *layernode = map_node->first_node("layer");
    while (layernode) {
        TmxLayerInfo layer_info;
        layer_info.name = layernode->first_attribute("name")->value();
        layer_info.size_in_tiles.x = atoi(layernode->first_attribute("width")->value());
        layer_info.size_in_tiles.y = atoi(layernode->first_attribute("height")->value());

        if (auto visible_attr = layernode->first_attribute("visible")) {
            layer_info.visible = (atoi(visible_attr->value()) == 1);
        }
        
        if (auto opacity_attr = layernode->first_attribute("opacity")) {
            layer_info.opacity = (atoi(opacity_attr->value()) == 1);
        }

        if (auto x_attr = layernode->first_attribute("x")) {
            layer_info.offset.x = atoi(x_attr->value());
        }

        if (auto y_attr = layernode->first_attribute("y")) {
            layer_info.offset.y = atoi(y_attr->value());
        }

        xml_node<> *data_node = layernode->first_node("data");
        if (data_node) {
            string encoding_str    = data_node->first_attribute("encoding")->value();
            string compression_str = data_node->first_attribute("compression")->value();

            TmxEncoding encoding;
            if (encoding_str == "base64") {
                encoding = TmxEncoding::BASE64;
            } else {
                S2ERROR("TmxParser only basr64 and/or gzip/zlib maps are supported");
                return map_info;
            }

            TmxCompression compression = TmxCompression::NONE;
            if (compression_str == "gzip") {
                compression = TmxCompression::GZIP;
            } else if (compression_str == "zlib") {
                compression = TmxCompression::ZLIB;
            }

            string data_str = data_node->value();
            data_str.erase(0, data_str.find_first_not_of(" \n\r\t"));
            data_str.erase(data_str.find_last_not_of(" \n\r\t") + 1);
            ParseLayerData(layer_info, data_str,
                           encoding,
                           compression);
        }

        map_info->layers.push_back(layer_info);
        layernode = layernode->next_sibling("layer");
    }

    xml_node<> *objectgroup_node = map_node->first_node("objectgroup");
    while (objectgroup_node) {
        TmxObjectGroup object_group;
        object_group.name = objectgroup_node->first_attribute("name")->value();
        object_group.size_in_tiles.x = atoi(objectgroup_node->first_attribute("width")->value());
        object_group.size_in_tiles.y = atoi(objectgroup_node->first_attribute("height")->value());

        if (auto x_attr = objectgroup_node->first_attribute("x")) {
            object_group.offset_in_tiles.x = atoi(x_attr->value());
        }

        if (auto y_attr = objectgroup_node->first_attribute("y")) {
            object_group.offset_in_tiles.y = atoi(y_attr->value());
        }

        if (auto opacity_attr = objectgroup_node->first_attribute("opacity")) {
            object_group.opacity = atof(opacity_attr->value());
        }

        if (auto visible_attr = objectgroup_node->first_attribute("visible")) {
            object_group.visible = (atoi(visible_attr->value()) == 1 ? true : false);
        }

        xml_node<> *object_node = objectgroup_node->first_node("object");
        while (object_node) {
            XmxObject object;
            object.name = object_node->first_attribute("name")->value();
            object.type = object_node->first_attribute("type")->value();
            object.offset_in_tiles.x = atoi(object_node->first_attribute("x")->value());
            object.offset_in_tiles.y = atoi(object_node->first_attribute("y")->value());
            object.size_in_pixels.x = atoi(object_node->first_attribute("width")->value());
            object.size_in_pixels.y = atoi(object_node->first_attribute("height")->value());

            if (auto rotation_attr = object_node->first_attribute("rotation")) {
                object.rotation = (atoi(rotation_attr->value()) == 1);
            }
            if (auto gid_attr = object_node->first_attribute("gid")) {
                object.gid = atoi(gid_attr.value());
            }
            if (auto visible_attr = object_node->first_attribute("visible")) {
                object.visible = (atoi(visible_attr->value()) == 1);
            }

            object_group.objects.push_back(object);
            object_node = object_node->next_sibling("object");
        }

        objectgroup_node = objectgroup_node->next_sibling("objectgroup");
    }

    return map_info;
}
    
bool TmxParser::ParseLayerData(TmxLayerInfo &layer_info,
                               const string data,
                               TmxEncoding encoding,
                               TmxCompression compression) {
    if (encoding == TmxEncoding::BASE64) {
        tmx_gid *gids_begin = nullptr;
        tmx_gid *gids_end   = nullptr;

        size_t decoded_length;
        unsigned char *decoded_data = base64_decode(data, &decoded_length);
        if (decoded_data == nullptr) {
            return false;
        }

        if (compression == TmxCompression::ZLIB ||
            compression == TmxCompression::GZIP) {
            size_t size_hint =
                layer_info.size_in_tiles.x *
                layer_info.size_in_tiles.y *
                sizeof(tmx_gid);
            size_t inflated_length = 0;
            unsigned char *inflated_data = inflate_memory_with_hint(decoded_data,
                                                                    decoded_length,
                                                                    &inflated_length,
                                                                    size_hint);
            if (inflated_data == nullptr) {
                S2ERROR("TmxParser: inflate data error.");
                return false;
            }

            free(decoded_data);
            decoded_data = nullptr;

            gids_begin = reinterpret_cast<tmx_gid *>(inflated_data);
            gids_end   = reinterpret_cast<tmx_gid *>(inflated_data + inflated_length);
            
        } else {
            gids_begin = reinterpret_cast<tmx_gid *>(decoded_data);
            gids_end   = reinterpret_cast<tmx_gid *>(decoded_data + decoded_length);
        }

        layer_info.gids = vector<tmx_gid>(gids_begin, gids_end);
        return true;
    } 
    
    return false;
}

}

