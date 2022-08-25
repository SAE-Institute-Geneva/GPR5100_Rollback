/*! \mainpage GPR5100 - Rollback Game Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the documentation of the GPR5100 - Rollback Game sommative project. It is a simple asteroid-like online multiplayer game using rollback mechanisms for client-side prediction and server authorative validation.
 *
 * \section install_sec Installation
 * To start working on the project, you are required CMake and vcpkg.
 * \section ecs ECS implementation
 * ECS or Entity-Component-System is a pattern of structuring game world objects and their components. In our purpose, it allows to simply replicate the game data as each component is stored by type. 
 * \subsection entity_manager Entity Manager
 * The EntityManager is the main class of our ECS implementation. From the EntityManager, you can get if an Entity exists, if it contains a certain Component and you can destroy it. 
 * 
 * The implementation is pretty straightforward. Using bitwise operation, we store the Component type in a unique EntityMask. This saves a lot of memory and is pretty fast to get the result.
 * 
 * However, you cannot get access to the specific Component of an Entity. For that, you need to get access to its ComponentManager.
 * \subsection component_manager Component Manager
 * The ComponentManager is a template class that automatically generates a std::vector of the given component type T. It requires an EntityMManager, such that when adding or removing a component, the ComponentManager can notify the EntityManager of the change.
 * \subsection sprite_manager Sprite Manager
 * The SpriteManager is a ComponentManager that owns the sprites in the game. Sprites are using sprites from SFML to draw on the window.
 * 
 * Sprite draw ordering works with entity ordering. It means that the background shoudl be the first entity to spawn because it will drawn first, therefore behind the next sprites.
 * \subsection physics_manager Physics Manager
 * The PhysicsManager is a class that contains two ComponentManager:
 * - BodyManager
 * - BoxManager
 * \subsection transform_manager Transform Manager
 * The TransformManager is a class that contains three ComponentManager:
 * - PositionManager
 * - ScaleManager
 * - RotationManager
 * \section netcode Netcode
 * This project netcode is pretty simple, but should work for any simple game project.
 * \subsection server_connection Connecting to the server
 * \subsection start_game Starting the game
 * \subsection send_input Sending player inputs
 * \subsection validate_frame Validating the frame
 * \section rollback Rollback mechanisms
 * 
 */