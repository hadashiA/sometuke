#ifndef __sometuke__drawing_primitives__
#define __sometuke__drawing_primitives__

#include "sometuke/vector.h"
#include "sometuke/color.h"
#include "sometuke/vertices.h"

#include <vector>

namespace sometuke {
using namespace std;

void DrawColor(const Color4B& color);
void DrawColor(const Color4F& color);
void DrawPointSize(float point_size);

void DrawLine(const vec3& origin, const vec3& destination);
void DrawRect(const Rect& rect);
void DrawPoly(const vector<vec2gl>& vertices, bool close_polygon);
void DrawSolidPoly(const vector<vec2gl>& vertices, const Color4F& color);

}

#endif /* defined(__sometuke__drawing_primitives__) */






