#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

/* ------------------------------------ Components ------------------------------------ */

// Transforms
#include "./Components/Transforms/PositionComponent.hpp"

// Physics
#include "./Components/Physics/VelocityComponent.hpp"
#include "./Components/Physics/DestinationComponent.hpp"

// Renders
#include "./Components/Renders/RenderableComponent.hpp"

// Inputs
#include "./Components/Inputs/InputComponent.hpp"

// Entities Manager
#include "./Entities/EntityManager.hpp"

/* ------------------------------------ SYSTEMS ------------------------------------ */

// Inputs Systems
#include "./Systems/Inputs/InputSystem.hpp"

// Transforms Sytems
#include "./Systems/Transforms/MovementSystem.hpp"

// Renders Sytems
#include "./Systems/Renders/ParallaxSystem.hpp"
#include "./Systems/Renders/RenderSystem.hpp"