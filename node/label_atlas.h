#ifndef __kawaii__node_label_atlas__
#define __kawaii__node_label_atlas__

#include "kawaii/node/atlas_node.h"

#include <string>

namespace kawaii {
using namespace std;

class LabelAtlas : public AtlasNode, public LabelInterface {
public:
    bool InitWithText(const string& text,
              const string& char_map_file,
              size_t width, size_t height, size_t start_char);

    virtual void Render();
    virtual void UpdateAtlasValues();

    // LabelInterface

    virtual void set_text(const string& text);
    virtual const string& text() const {
        return text_;
    }

private:
    size_t map_start_char_;
    string text_;
};

}

#endif /* defined(__kawaii__node_label_atlas__) */
