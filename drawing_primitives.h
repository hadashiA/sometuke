#ifndef __kawaii__drawing_primitives__
#define __kawaii__drawing_primitives__

#include "vector.h"
#include "color.h"

namespace kawaii {

void DrawColor(const Color4B& color);
void DrawColor(const Color4F& color);

void DrawLine(const vec3& origin, const vec3& destination);
void DrawRect(const Rect& rect);
void DrawPoly(const vec2 *poli, unsigned int num_points, bool close_polygon);

}

#endif /* defined(__kawaii__drawing_primitives__) */






