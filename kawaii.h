#ifndef __kawaii__
#define __kawaii__

#include "kawaii/memory_pool.h"

#include "kawaii/application.h"
#include "kawaii/director.h"

// actor
#include "kawaii/types.h"
#include "kawaii/actor.h"


// process
#include "kawaii/process/process_scheduler.h"
#include "kawaii/event_dispatcher.h"

// resource
#include "kawaii/assets.h"
#include "kawaii/texture_2d.h"
#include "kawaii/texture_cache.h"
#include "kawaii/sprite_frame_cache.h"
#include "kawaii/shader_cache.h"
#include "kawaii/animation_cache.h"

// node
#include "kawaii/scene.h"
#include "kawaii/sprite.h"

// process
#include "kawaii/process/animate.h"

// structs
#include "kawaii/vector.h"
#include "kawaii/matrix.h"
#include "kawaii/vertices.h"

#include "kawaii/logger.h"

#include "kawaii/OpenGL_Internal.h"

namespace kawaii {
using namespace std;
}

#endif  /* defined(__kawaii__) */
