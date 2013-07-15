#ifndef __skidarake__node_filled_polygon__
#define __skidarake__node_filled_polygon__

#include "skidarake/vertices.h"
#include "skidarake/node/node.h"

#include <OpenGLES/ES2/gl.h>

#include <functional>
#include <vector>
#include <memory>

using namespace std;

namespace skidarake {

class Texture2D;

struct SimpleTriangulator : public unary_function<const vector<vec2gl>&, vector<vec2gl> > {
public:
    vector<vec2gl> operator()(const vector<vec2gl>& vertices);
    
private:
    float Area(const vector<vec2gl>& vertices);
    bool InsideTriangle(const vec2gl& a,
                        const vec2gl& b,
                        const vec2gl& c,
                        const vec2gl& p);
    bool Snip(const vector<vec2gl>& vertices,
              const vector<size_t>& indices,
              int u, int v, int w, int n);
};

template<class Triangulator = SimpleTriangulator>
class FilledPolygon : Node {
public:
    virtual ~FilledPolygon() {}

private:
    shared_ptr<Texture2D> texture_;
    GLenum blend_func_src_;
    GLenum blend_func_dst_;

    Triangulator triangulator_;

    vector<vec2gl> area_triangle_points_;
    vector<vec2gl> texture_coodinates_;
};

}

#endif /* defined(__skidarake__node_filled_polygon__) */
