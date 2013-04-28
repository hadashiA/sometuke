#ifndef __kawaii__node_label_atlas__
#define __kawaii__node_label_atlas__

namespace kawaii {
using namespace std;

class LabelInterface {
public:
    void set_text(const string& text) = 0;
    const string& text() const = 0;
};

}

#endif /* defined(__Hitasura__label_interface__) */
