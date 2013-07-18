#include "skidarake/node/polygon_sprite.h"

#include "skidarake/logger.h"

namespace skidarake {

static const float EPSILON=0.0000000001f;

vector<vec2gl> SimpleTriangulator::operator()(const vector<vec2gl>& vertices) {
    // allocate and initialize list of vertices in polygon

    size_t n = vertices.size();
    if (n < 3) {
        IIERROR("too small vertices.");
        return vector<vec2gl>();
    }

    vector<vec2gl> result;
    vector<size_t> indices(n);

    bool negative_area = Area(vertices);
    for (size_t i = 0; i < n; ++i) {
        indices[i] = (negative_area ? i : (n - 1 - i));
    }

    size_t count = n * 2;
    for (int m = 0, v = n - 1; n > 2;) {
        if (0 >= count--) {
            IIERROR("probable bad polygon!");
            return vector<vec2gl>();
        }

        /* three consecutive vertices in current polygon, <u,v,w> */
        int u = v;
        if (n <= u) u = 0;
        
        v = u + 1;
        if (n <= v) v = 0;

        int w = v + 1;
        if (n <= w) w = 0;
	
        bool snipped = Snip(vertices, indices, u, v, w, n);
        if (snipped) {
            // true names of the vertices
            size_t a = indices[u];
            size_t b = indices[v];
            size_t c = indices[w];
            
            // output Triangle 
            result.push_back(vertices[a]);
            result.push_back(vertices[b]);
            result.push_back(vertices[c]);
            
            m++;
            
            /* remove v from remaining polygon */
            size_t s, t;
            for (s = v, t = v + 1; t < n; s++, t++) {
                indices[s] = indices[t];
            }
            n--;
            
            /* resest error detection counter */
            count = 2 * n;
        }
    }
    
    return result;
}


float SimpleTriangulator::Area(const vector<vec2gl>& vertices) {
    size_t size = vertices.size();
    float a = 0.0f;
    for (size_t p = size - 1, q = 0; q < size; p = q++) {
        const vec2gl& pv = vertices[p];
        const vec2gl& qv = vertices[q];
        a += pv.x * qv.y - qv.x * pv.y;
    }
    return a * 0.5f;
}

bool SimpleTriangulator::InsideTriangle(const vec2gl& a,
                                        const vec2gl& b,
                                        const vec2gl& c,
                                        const vec2gl& p) {
    float ax = c.x - b.x;
    float ay = c.y - b.y;
    float bx = a.x - c.x;
    float by = a.y - c.y;
    float cx = b.x - a.x;
    float cy = b.y - a.y;

    float apx = p.x - a.x;
    float apy = p.y - a.y;
    float bpx = p.x - b.x;
    float bpy = p.y - b.y;
    float cpx = p.x - c.x;
    float cpy = p.y - c.y;

    float a_cross_bp = ax * bpy - ay * bpx;
    float c_cross_ap = cx * apy - cy * apx;
    float b_cross_cp = bx * cpy - by * cpx;

    return ((a_cross_bp >= 0.0f) && (b_cross_cp >= 0.0f) && (c_cross_ap >= 0.0f));
}

bool SimpleTriangulator::Snip(const vector<vec2gl>& vertices,
                              const vector<size_t>& indices,
                              int u, int v, int w, int n) {
    const vec2gl& a = vertices[indices[u]];
    const vec2gl& b = vertices[indices[v]];
    const vec2gl& c = vertices[indices[w]];
    
    if (EPSILON > (((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x)))) {
        return false;
    }
    
    for (int p = 0; p < n; p++) {
        if ((p == u) || (p == v) || (p == w)) continue;
        bool inside = InsideTriangle(a, b, c, vertices[indices[p]]);
        if (inside) return false;
    }
    
    return true;
}

}
