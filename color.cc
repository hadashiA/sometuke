#include "sometuke/color.h"

namespace sometuke {

// Color4B::Color4B(const Color4F &color4f)
//     : r((GLubyte)(color4f.r * 255.0f)),
//       g((GLubyte)(color4f.g * 255.0f)),
//       b((GLubyte)(color4f.b * 255.0f)),
//       a((GLubyte)(color4f.a * 255.0f)) {
// }

const Color3B Color3B::WHITE(255,255,255);
const Color3B Color3B::YELLOW(255,255,0);
const Color3B Color3B::GREEN(0,255,0);
const Color3B Color3B::BLUE(0,0,255);
const Color3B Color3B::RED(255,0,0);
const Color3B Color3B::MAGENTA(255,0,255);
const Color3B Color3B::BLACK(0,0,0);
const Color3B Color3B::ORANGE(255,127,0);
const Color3B Color3B::GRAY(166,166,166);

const Color4B Color4B::WHITE(255,255,255,255);
const Color4B Color4B::YELLOW(255,255,0,255);
const Color4B Color4B::GREEN(0,255,0,255);
const Color4B Color4B::BLUE(0,0,255,255);
const Color4B Color4B::RED(255,0,0,255);
const Color4B Color4B::MAGENTA(255,0,255,255);
const Color4B Color4B::BLACK(0,0,0,255);
const Color4B Color4B::ORANGE(255,127,0,255);
const Color4B Color4B::GRAY(166,166,166,255);

const Color4F Color4F::WHITE(1,1,1,1);
const Color4F Color4F::YELLOW(1,1,0,1);
const Color4F Color4F::GREEN(0,1,0,1);
const Color4F Color4F::BLUE(0,0,1,1);
const Color4F Color4F::RED(1,0,0,1);
const Color4F Color4F::MAGENTA(1,0,1,1);
const Color4F Color4F::BLACK(0,0,0,1);
const Color4F Color4F::ORANGE(1,0.5,0,1);
const Color4F Color4F::GRAY(0.65,0.65,0.65,1);
    
}
