#include "game/input_manager.h"
#include "SFML/Window/Keyboard.hpp"

namespace game
{
    
PlayerInput GetPlayerInput(int index)
{
    switch(index)
    {
    case 0:
    {
        PlayerInput clientInput1;
        clientInput1 = PlayerInput{ clientInput1 | (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ?
                PlayerInput::LEFT : PlayerInput::NONE) };
        clientInput1 = PlayerInput{ clientInput1 | (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ?
                PlayerInput::RIGHT : PlayerInput::NONE) };
        clientInput1 = PlayerInput{ clientInput1 | (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ?
                PlayerInput::UP : PlayerInput::NONE) };
        clientInput1 = PlayerInput{ clientInput1 | (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ?
                PlayerInput::DOWN : PlayerInput::NONE) };
        clientInput1 = PlayerInput{clientInput1 | (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) ?
                PlayerInput::SHOOT : PlayerInput::NONE) };

        return clientInput1;
    }
    case 1:
    {
        PlayerInput clientInput2;
        clientInput2 = PlayerInput{ clientInput2 | (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ?
                PlayerInput::LEFT : PlayerInput::NONE) };
        clientInput2 = PlayerInput{ clientInput2 | (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ?
                PlayerInput::RIGHT : PlayerInput::NONE) };
        clientInput2 = PlayerInput{ clientInput2 | (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ?
                PlayerInput::UP : PlayerInput::NONE) };
        clientInput2 = PlayerInput{ clientInput2 | (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ?
                PlayerInput::DOWN : PlayerInput::NONE) };
        clientInput2 = PlayerInput{ clientInput2 | (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ?
                PlayerInput::SHOOT : PlayerInput::NONE) };
        return clientInput2;
    }
    default:
        break;
    }
    return {};
   
}
} // namespace game
