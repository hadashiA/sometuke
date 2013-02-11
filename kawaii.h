#ifndef __kawaii__
#define __kawaii__

#include "application.h"
#include "director.h"

// process
#include "process_scheduler.h"
#include "event_dispatcher.h"

// resource
#include "assets.h"
#include "texture_2d.h"
#include "texture_cache.h"
#include "sprite_frame_cache.h"
#include "shader_cache.h"
#include "animation_cache.h"

// node
#include "scene.h"
#include "sprite.h"

// structs
#include "vector.h"
#include "matrix.h"
#include "vertices.h"

#include "logger.h"

#include "OpenGL_Internal.h"

namespace kawaii {
using namespace std;
}

#endif  /* defined(__kawaii__) */
