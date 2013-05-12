#include "kawaii/node/atlas_node.h"

namespace kawaii {

bool AtlasNode::InitWithTileFile(const string& path,
                                 size_t tile_width, size_t tile_height,
                                 size_t items_to_render) {
    item_width_  = tile_width;
    item_height_ = tile_height;

    opacity_ = 255;
    color_ = color_unmodified_ = Color3B(255, 255, 255);

    blend_func_src_ = GL_ONE;
    blend_func_dst_ = GL_ONE_MINUS_SRC_ALPHA;

    shared_ptr<TextureAtlas> atlas = make_shared<TextureAtlas>();
    if (atlas->InitWithFile(path, items_to_render)) {
        return false;
    }
    texture_atlas_ = atlas;

    CalculateMaxItems();
    UpdateBlendFunc();
    UpdateOpacityModifyRGB();

    shader_program_ = ShaderCache::Instance().get(kShader_PositionTexture_uColor);
    uniform_color_ = glGetUniformLocation(shader_program_->id(), "u_Color");
    
    return true;
}

void AtlasNode::Render() {
    shader_program_->Use();
    shader_program_->SetUniformsForBuiltins();
    glEnable(GL_BLEND);
    glBlendFunc(blend_func_src_, blend_func_dst_);
    
    GLfloat colors[4] = {
        color_.r / 255.0f, color_.g / 255.0f, color_.b / 255.0f, opacity_ / 255.0f
    };
    glUniform4fv(uniform_color_, 1, colors);

    texture_atlas_->RenderQuads(quads_to_draw_, 0);
}

}
