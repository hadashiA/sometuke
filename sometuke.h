#ifndef __sometuke__
#define __sometuke__

#include "sometuke/memory_pool.h"

#include "sometuke/director.h"

#include "sometuke/types.h"

#include "sometuke/texture_2d.h"
#include "sometuke/texture_cache.h"
#include "sometuke/sprite_frame_cache.h"
#include "sometuke/animation_cache.h"
#include "sometuke/shader_cache.h"
#include "sometuke/animation_cache.h"

// actor
#include "sometuke/actor.h"
#include "sometuke/scene.h"

// node
#include "sometuke/node/layer.h"
#include "sometuke/node/sprite.h"
#include "sometuke/node/label_atlas.h"
#include "sometuke/node/polygon_sprite.h"

// process
#include "sometuke/process/process_manager.h"
#include "sometuke/process/animate.h"
#include "sometuke/process/trigger.h"
#include "sometuke/process/move_by.h"
#include "sometuke/process/move_to.h"
#include "sometuke/process/repeat.h"
#include "sometuke/process/sequence.h"
#include "sometuke/process/delay.h"

// structs
#include "sometuke/vector.h"
#include "sometuke/matrix.h"
#include "sometuke/vertices.h"

#include "sometuke/OpenGL_Internal.h"
#include "sometuke/drawing_primitives.h"

#include "sometuke/logger.h"

#endif  /* defined(__sometuke__) */
