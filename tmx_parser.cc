#include "sometuke/tmx_parser.h"

#include "sometuke/director.h"

#include "sometuke/ext/rapidxml/rapidxml.hpp"

namespace sometuke {
using namespace rapidxml;

shared_ptr<TmxMapInfo> TmxParser::Parse(const string& file) {
    string bytes = Director::Instance().file_utils().ReadString(file);

    xml_document<> doc;
    doc.parse<0>((char *)(bytes.c_str()));

    auto map_info = make_shared<TmxMapInfo>();
    return map_info;
}
    
}

