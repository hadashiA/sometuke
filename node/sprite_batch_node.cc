#include "sometuke/node/sprite_batch_node.h"

#include "sometuke/node/sprite.h"
#include "sometuke/texture_2d.h"

namespace sometuke {

bool SpriteBatchNode::InitWithTexture(const shared_ptr<Texture2D>& texture, size_t capacity) {
    texture_atlas_->InitWithTexture(texture, capacity);

    UpdateBlendFunc();
    
    return true;
}

void SpriteBatchNode::AddChild(const shared_ptr<Sprite>& child) {
    if (child->texture()->id() != texture_atlas_->texture()->id()) {
        S2ERROR("not using the same texture id");
        return;
    }

    Node::AddChild(child);

    AppendSprite(child);
}

void SpriteBatchNode::AppendSprite(const shared_ptr<Sprite>& sprite) {
    is_reorder_child_dirty_ = true;

    size_t next_index = sprits_.size() - 1;
    sprite->set_batch_node(static_pointer_cast<SpriteBatchNode>(shared_from_this()),
                          next_index);

    sprits_.push_back(sprite);

    vector<shared_ptr<Node> > children = sprite->children();
    for (vector<shared_ptr<Node> >::iterator i = children.begin();
         i != children.end(); ++i) {
        if (shared_ptr<Sprite> child = dynamic_pointer_cast<Sprite>(*i)) {
            AppendSprite(child);
        }
    }
}


void SpriteBatchNode::UpdateBlendFunc() {
    if (!texture_atlas_->texture()->has_premultiplied_alpha()) {
        blend_func_src_ = GL_SRC_ALPHA;
        blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
    }
}

}
