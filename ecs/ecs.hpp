#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

/* ------------------------------------ Components ------------------------------------ */


//General
#include "./Components/General/HealthComponent.hpp"
#include "./Components/General/TimeCooldownComponent.hpp"

//Collisions
#include "./Components/Collisions/CollisionComponent.hpp"

// Transforms
#include "./Components/Transforms/PositionComponent.hpp"

// Physics
#include "./Components/Physics/VelocityComponent.hpp"
#include "./Components/Physics/DestinationComponent.hpp"
#include "./Components/Physics/BoundingBoxComponent.hpp"


// Renders
#include "./Components/Renders/RenderableComponent.hpp"
#include "./Components/Renders/AnimationComponent.hpp"

// Inputs
#include "./Components/Inputs/InputComponent.hpp"

// Entities Manager
#include "./Entities/EntityManager.hpp"

/* ------------------------------------ SYSTEMS ------------------------------------ */

// Inputs Systems
#include "./Systems/Inputs/InputSystem.hpp"

// Transforms Sytems
#include "./Systems/Transforms/MovementSystem.hpp"

// Physics Sytems
#include "./Systems/Physics/CollisionsSystem.hpp"

// Renders Sytems
#include "./Systems/Renders/ParallaxSystem.hpp"
#include "./Systems/Renders/RenderSystem.hpp"
#include "./Systems/Renders/AnimationSystem.hpp"
