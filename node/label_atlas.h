#ifndef __skidarak__node_label_atlas__
#define __skidarak__node_label_atlas__

#include "skidarak/node/atlas_node.h"

#include <string>
#include <functional>
#include <unordered_map>

namespace skidarak {
using namespace std;

class LabelAtlas : public AtlasNode, public LabelInterface {
public:
    typedef unordered_map<string, Vector2<size_t> > CharMap;

    bool InitWithText(const string& text,
                      const string& texture_path,
                      size_t width, size_t height,
                      char map_start_char);

    virtual void Render();
    virtual void UpdateAtlasValues();

    // LabelInterface

    virtual void set_text(const string& text);
    virtual const string& text() const {
        return text_;
    }

private:
    char map_start_char_;
    string text_;
};

}

#endif /* defined(__skidarak__node_label_atlas__) */
