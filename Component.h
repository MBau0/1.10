#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

enum {
    TRANSFORM_COMPONENT,
    TOTAL_COMPONENTS
};

struct Component {
public:
    Component(Entity* entity) : 
        _entity  ( entity )
    {}

    virtual void update() = 0;

    Entity* _entity;
};

#endif