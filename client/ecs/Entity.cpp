#include "Entity.hpp"

Entity::Entity() : id(nextId++)
{
}
uint64_t Entity::getId() const
{
    return id;
}
