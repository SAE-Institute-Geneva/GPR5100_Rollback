/*! \mainpage GPR5100 - Rollback Game Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the documentation of the GPR5100 - Rollback Game sommative project. It is a simple asteroid-like online multiplayer game using rollback mechanisms for client-side prediction and server authorative validation.
 *
 * \section install_sec Installation
 * To start working on the project, you are required CMake and vcpkg.
 * \subsection windows Windows
 * On Windows, you can used Visual Studio 2022 that supports C++20 pretty well.
 * \section ecs ECS implementation
 * ECS or Entity-Component-System is a pattern of structuring game world objects and their components. In our purpose, it allows to simply replicate the game data as each component is stored by type. 
 * \subsection entity_manager Entity Manager
 * The EntityManager is the main class of our ECS implementation. From the EntityManager, you can get if an Entity exists, if it contains a certain Component and you can destroy it. 
 * 
 * The implementation is pretty straightforward. Using bitwise operation, we store the Component type in a unique EntityMask. This saves a lot of memory and is pretty fast to get the result.
 * 
 * However, you cannot get access to the specific Component of an Entity. For that, you need to get access to its ComponentManager.
 * 
 * Here are the typical use-case of the EntityManager:
 * \code
 * const auto entity = entityManager_.CreateEntity();
 * //...
 * if(entityManager_.HasComponent(entity, static_cast<core::EntityMask>(core::ComponentType::TRANSFORM)))
 * {
 * //...
 * entityManager_.DestroyEntity(entity);
 * \endcode
 * \subsection component_manager Component Manager
 * The ComponentManager is a template class that automatically generates a std::vector of the given component type T. It requires an EntityMManager, such that when adding or removing a component, the ComponentManager can notify the EntityManager of the change.
 * Here are the typical use-case:
 * \code
 * //With Entity entity and ComponentManager spriteManager_
 * spriteManager_.AddComponent(entity);
 * //...
 * auto& sprite = spriteManager_.GetComponent(entity);
 * //... Do things with sprite
 * spriteMManager_.RemoveComponent(entity);
 * \endcode
 * \subsection sprite_manager Sprite Manager
 * The SpriteManager is a ComponentManager that owns the sprites in the game. Sprites are using sprites from SFML to draw on the window.
 * 
 * Sprite draw ordering works with entity ordering. It means that the background shoudl be the first entity to spawn because it will drawn first, therefore behind the next sprites.
 * \subsection physics_manager Physics Manager
 * The PhysicsManager is a class that contains two ComponentManager:
 * - BodyManager owns the Body (or rigid bodies) of the physics engine.
 * - BoxManager owns the Box (or box colliders) of the physics engine.
 * 
 * The Physics Engine is a REALLY simple implementation of basic box triggering. 
 * \subsection transform_manager Transform Manager
 * The TransformManager is a class that contains three ComponentManager:
 * - PositionManager owns the positions in meter of all entities, both used in physics and graphics (converted to pixel with pixelToMeter).
 * - ScaleManager owns the scaling ratios of all entities. This only applies to the graphics part of the game, NOT the physics part.
 * - RotationManager owns the angles of all entites. This only applies to the graphics part of the game, NOT the physics.
 * \section netcode Netcode
 * This project netcode is pretty simple, but should work for any simple game project. 
 * \subsection server_connection Connecting to the server
 * When a client wants to connect to the server, they will be required to connect both in TCP and UDP. 
 * \subsection start_game Starting the game
 * When all players are connected, the server automatically send a START_GAME packet to each player. 
 * \subsection send_input Sending player inputs
 * Each frame, the game sends the current player inputs, as well as the last game::maxInputNmb inputs.
 * \subsection validate_frame Validating the frame
 * When the server finally receives all the player inputs for a specific frame, it will automatically validate the specific frame and will update its lastValidateFrame_ to the new specific frame.
 * 
 * On the client side, when receiving a VALIDATE_FRAME packet, the client will calculate the frame physics status and check that the result is the same as the server one. If it is not the case, there is desynchronisation and the game must end!
 * \section rollback Rollback mechanisms
 * \subsection rollback_how How the rollback works?
 * At any time, each client and the server have the game states of two points time:
 * - Last Validated Frame
 * - Current Frame (except the server)
 */