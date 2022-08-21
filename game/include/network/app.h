#pragma once

#include "engine/system.h"
#include "graphics/graphics.h"

namespace game
{
    
class App : public core::DrawInterface, public core::DrawImGuiInterface, public core::SystemInterface, public core::OnEventInterface
{

};
} // namespace game
