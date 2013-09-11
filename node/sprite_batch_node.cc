#include "sometuke/node/sprite_batch_node.h"

namespace sometuke {

bool SpriteBatchNode::InitWithTexture(const shared_ptr<Texture2D>& texture, size_t capacity) {
    texture_atlas_->InitWithTexture(texture, capacity);

    UpdateBlendFunc();
}

void SpriteBatchNode::AddChild(const shared_ptr<Sprite>& child) {
    if (child->texture()->id() != texture_atlas_->texture()) {
        S2ERROR("not using the same texture id");
        return;
    }

    Node::AddChild(child);

    child->set_batch_node(static_pointer_cast<SpriteBatchNode>(shared_from_this()));
    child->set_dirty(true);
}


void SpriteBatchNode::UpdateBlendFunc() {
    if (! texture_atlas_->texture()->has_premultiplied_alpha) {
        blend_func_src_ = GL_SRC_ALPHA;
        blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
    }
}

}
