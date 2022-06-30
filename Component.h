#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

enum {
    TRANSFORM_COMPONENT,
    BUILDING_COMPONENT,
    UNIT_COMPONENT,
    TOTAL_COMPONENTS
};

struct Component {
public:
    Component(Entity* entity) : 
        _entity  ( entity )
    {}

    virtual void update() = 0;

    int get_index() const { return _index; } // required for compactArray

    void set_index(int index) { _index = index; } // required for compactArray

    Entity* _entity;

private:
    int _index = 0; // required for compactArray dont touch
};

#endif