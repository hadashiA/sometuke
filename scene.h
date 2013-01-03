#ifndef __kawaii__scene__
#define __kawaii__scene__

namespace kawaii {

class Scene {
public:
    virtual ~Scene() {}

    virtual void OnEnter() = 0;
    virtual void Update(const float delta_time) = 0;
    virtual void Render() = 0;
};

}

#endif /* defined(__kawaii__scene__) */



