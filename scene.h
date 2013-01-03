#ifndef __kawaii__scene__
#define __kawaii__scene__

namespace kawaii {

class Scene {
public:
    virtual ~Scene() {}

    virtual void OnEnter() {};
    virtual void Update(const float delta_time) {};
    virtual void Render() {};
};

}

#endif /* defined(__kawaii__scene__) */



