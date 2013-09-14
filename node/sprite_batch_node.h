#ifndef __sometuke__node_sprite_batch_node__
#define __sometuke__node_sprite_batch_node__

#include "sometuke/texture_atlas.h"
#include "sometuke/node/node.h"
#include "sometuke/shader_cache.h"

namespace sometuke {

class Sprite;

class SpriteBatchNode : public Node {
public:
    SpriteBatchNode()
        : blend_func_src_(GL_ONE),
          blend_func_dst_(GL_ONE_MINUS_SRC_ALPHA) {
        texture_atlas_ = make_shared<TextureAtlas>();
        shader_program_ = ShaderCache::Instance()[kShader_PositionTextureColor];
    }

    virtual ~SpriteBatchNode() {}

    bool InitWithTexture(const shared_ptr<Texture2D>& texture, size_t capacity = 29);
    void AddChild(const shared_ptr<Node>& child);
    void AppendSprite(const shared_ptr<Sprite>& child);

    void Visit();
    void Render();

    shared_ptr<TextureAtlas> texture_atlas() {
        return texture_atlas_;
    }

    shared_ptr<Texture2D> texture() const {
        return texture_atlas_->texture();
    }

    void set_texture(const shared_ptr<Texture2D>& texture) {
        texture_atlas_->set_texture(texture);
        UpdateBlendFunc();
    }

private:
    void UpdateBlendFunc();

    shared_ptr<GLProgram> shader_program_;

    shared_ptr<TextureAtlas> texture_atlas_;
    GLenum blend_func_src_;
    GLenum blend_func_dst_;

    vector<shared_ptr<Sprite> > sprits_;
};

}

#endif /* defined(__sometuke__node_sprite_batch_node__) */
