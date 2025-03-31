#ifndef LAYER_H
#define LAYER_H

class ParallaxLayer : public std::enable_shared_from_this<ParallaxLayer> {
public:
    virtual void update(const float delta_time) = 0;

    virtual void draw() const =0;

    virtual ~ParallaxLayer() = default;
};

#endif //LAYER_H
