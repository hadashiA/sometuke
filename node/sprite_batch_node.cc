#include "sometuke/node/sprite_batch_node.h"

#include "sometuke/node/sprite.h"
#include "sometuke/texture_2d.h"
#include "sometuke/matrix_stack.h"
#include "sometuke/texture_atlas.h"

namespace sometuke {

bool SpriteBatchNode::InitWithTexture(const shared_ptr<Texture2D>& texture, size_t capacity) {
    texture_atlas_->InitWithTexture(texture, capacity);

    UpdateBlendFunc();
    
    return true;
}

void SpriteBatchNode::AddChild(const shared_ptr<Node>& child) {
    shared_ptr<Sprite> sprite = dynamic_pointer_cast<Sprite>(child);

    if (!sprite) {
        S2ERROR("SpriteBatchNode only supports Sprite as children");
        return;
    }

    if (sprite->texture()->id() != texture_atlas_->texture()->id()) {
        S2ERROR("not using the same texture id");
        return;
    }

    Node::AddChild(sprite);

    AppendSprite(sprite);
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

void SpriteBatchNode::Visit() {
    assert(!parent_.expired());

    if (visible_) {
        return;
    }

    mat4& model_view = MatrixStack<GLModelView>::Instance().Push();
    model_view = LocalTransform() * model_view;
    Render();
    MatrixStack<GLModelView>::Instance().Pop();
}

void SpriteBatchNode::Render() {
    shader_program_->Use();
    shader_program_->SetUniformsForBuiltins();

    for (vector<shared_ptr<Node> >::iterator i = children_.begin(); i != children_.end(); ++i) {
        shared_ptr<Sprite> child = static_pointer_cast<Sprite>(*i);
        child->UpdateTextureAtlas();
    }

    // set blending
    if (blend_func_src_ == GL_ONE && blend_func_dst_ == GL_ZERO) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        glBlendFunc(blend_func_src_, blend_func_dst_);
    }

    texture_atlas_->RenderQuads();
}

void SpriteBatchNode::UpdateBlendFunc() {
    if (!texture_atlas_->texture()->has_premultiplied_alpha()) {
        blend_func_src_ = GL_SRC_ALPHA;
        blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;
    }
}

}
