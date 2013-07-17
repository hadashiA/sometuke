#ifndef __skidarake__
#define __skidarake__

#include "skidarake/memory_pool.h"

#include "skidarake/application.h"
#include "skidarake/director.h"

#include "skidarake/types.h"
#include "skidarake/scheduler.h"
#include "skidarake/event_dispatcher.h"

#include "skidarake/assets_loader.h"
#include "skidarake/texture_2d.h"
#include "skidarake/texture_cache.h"
#include "skidarake/sprite_frame_cache.h"
#include "skidarake/animation_cache.h"
#include "skidarake/shader_cache.h"
#include "skidarake/animation_cache.h"

// actor
#include "skidarake/actor.h"
#include "skidarake/scene.h"

// node
#include "skidarake/node/layer.h"
#include "skidarake/node/sprite.h"
#include "skidarake/node/label_atlas.h"
#include "skidarake/node/polygon_sprite.h"

// process
#include "skidarake/process/process_manager.h"
#include "skidarake/process/animate.h"
#include "skidarake/process/trigger.h"
#include "skidarake/process/move_by.h"
#include "skidarake/process/move_to.h"
#include "skidarake/process/repeat.h"
#include "skidarake/process/sequence.h"
#include "skidarake/process/delay.h"

// structs
#include "skidarake/vector.h"
#include "skidarake/matrix.h"
#include "skidarake/vertices.h"

#include "skidarake/OpenGL_Internal.h"
#include "skidarake/drawing_primitives.h"

#include "skidarake/logger.h"

#endif  /* defined(__skidarake__) */
