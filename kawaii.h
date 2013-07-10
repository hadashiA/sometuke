#ifndef __skidarak__
#define __skidarak__

#include "skidarak/memory_pool.h"

#include "skidarak/application.h"
#include "skidarak/director.h"

#include "skidarak/types.h"
#include "skidarak/scheduler.h"
#include "skidarak/event_dispatcher.h"

#include "skidarak/assets_loader.h"
#include "skidarak/texture_2d.h"
#include "skidarak/texture_cache.h"
#include "skidarak/sprite_frame_cache.h"
#include "skidarak/animation_cache.h"
#include "skidarak/shader_cache.h"
#include "skidarak/animation_cache.h"

// actor
#include "skidarak/actor.h"
#include "skidarak/scene.h"

// node
#include "skidarak/node/layer.h"
#include "skidarak/node/sprite.h"
#include "skidarak/node/label_atlas.h"

// process
#include "skidarak/process/process_manager.h"
#include "skidarak/process/animate.h"
#include "skidarak/process/trigger.h"
#include "skidarak/process/move_by.h"
#include "skidarak/process/move_to.h"
#include "skidarak/process/repeat.h"
#include "skidarak/process/sequence.h"
#include "skidarak/process/delay.h"

// structs
#include "skidarak/vector.h"
#include "skidarak/matrix.h"
#include "skidarak/vertices.h"

#include "skidarak/OpenGL_Internal.h"
#include "skidarak/drawing_primitives.h"

#include "skidarak/logger.h"

namespace skidarak {
}

#endif  /* defined(__skidarak__) */
