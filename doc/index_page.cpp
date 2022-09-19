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
 * \subsection ubuntu Ubuntu
 * Tested on Ubuntu 20.04 with gcc-10 and g++-10 and Ubuntu 22.04. To install the required packages for vcpkg:
 * \code
 * sudo apt install autoconf libtool libsfml-dev libxrandr-dev 
 * \endcode
 * In command-line, you can setup cmake and make with:
 * \code
 * cmake -DCMAKE_TOOLCHAIN_FILE=<VCPKG_HOME>/scripts/buildsystems/vcpkg.cmake .. //-DCMAKE_BUILD_TYPE= Debug/RelWithDebInfo/Release
 * make -j 4
 * \endcode
 * \section ecs ECS implementation
 * ECS or Entity-Component-System is a pattern of structuring game world objects and their components. In our purpose, it allows to simply replicate the game data as each component is stored by type. 
 * \subsection entity_manager Entity Manager
 * The <a href="classcore_1_1_entity_manager.html">EntityManager</a> is the main class of our ECS implementation. From the EntityManager, you can get if an Entity exists, if it contains a certain Component and you can destroy it. 
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
 * The <a href="classcore_1_1_component_manager.html">ComponentManager</a> is a template class that automatically generates a std::vector of the given component type T. It requires an EntityManager, such that when adding or removing a component, the ComponentManager can notify the EntityManager of the change.
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
 * The <a href="classcore_1_1_sprite_manager.html">SpriteManager</a> is a ComponentManager that owns the sprites in the game. Sprites are using sprites from SFML to draw on the window.
 * 
 * Sprite draw ordering works with entity ordering. It means that the background shoudl be the first entity to spawn because it will drawn first, therefore behind the next sprites.
 * 
 * Sprite are centered on the position of the PositionManager by default.
 * \subsection physics_manager Physics Manager
 * The <a href="classgame_1_1_physics_manager.html">PhysicsManager</a> is a class that contains two ComponentManager:
 * - <a href="classgame_1_1_body_manager.html">BodyManager</a> owns the <a href="structgame_1_1_body.html">Body</a> (or rigid bodies) of the physics engine.
 * - <a href="classgame_1_1_box_manager.html">BoxManager</a> owns the <a href="structgame_1_1_box.html">Box</a> (or box colliders) of the physics engine.
 * 
 * The Physics Engine is a REALLY simple implementation of basic box triggering. 
 * \subsection transform_manager Transform Manager
 * The <a href="classcore_1_1_transform_manager.html">TransformManager</a> is a class that contains three ComponentManager:
 * - <a href="classcore_1_1_position_manager.html">PositionManager</a> owns the positions in meter of all entities, both used in physics and graphics (converted to pixel with pixelToMeter).
 * - <a href="classcore_1_1_scale_manager.html">ScaleManager</a> owns the scaling ratios of all entities. This only applies to the graphics part of the game, NOT the physics part.
 * - <a href="classcore_1_1_rotation_manager.html">RotationManager</a> owns the angles of all entites. This only applies to the graphics part of the game, NOT the physics.
 * \section netcode Netcode
 * This project netcode is pretty simple, but should work for any simple game project. 
 * \subsection server_connection Connecting to the server
 * When a client wants to connect to the server to be able to join a game, they will have to connect both in TCP and UDP. This is the step-by-step:
 * 1. The Client connects to the TCP IP address and port and sends a JOIN packet
 * 2. The Server answers with a JOIN_ACK packet containing the UDP port.
 * 3. The Client sends a JOIN packet on the UDP channel.
 * 4. The Server answers with a JOIN_ACK on the UDP channel. The Client is then a valid connected client. 
 * \subsection start_game Starting the game
 * When all players are connected, the server automatically send a <a href="structgame_1_1_start_game_packet.html">StartGamePacket</a> to each player through the TCP channel. Each client will then wait about <a href="game__globals_8h.html">game::startDelay</a> milliseconds before starting their game session.
 * \subsection send_input Sending player inputs
 * Each frame, the game sends the current player inputs (<a href="structgame_1_1_player_input_packet.html">PlayerInputPacket</a>), as well as the last <a href="game__globals_8h.html">game::maxInputNmb</a> inputs in an UDP packet.
 * \subsection validate_frame Validating the frame
 * When the server finally receives all the player inputs for a specific frame, it will automatically validate the specific frame and will update its lastValidateFrame_ to the new specific frame. It will then sends a <a href="structgame_1_1_validate_frame_packet.html">ValidateFramePacket</a> to all clients.
 * 
 * On the client side, when receiving a ValidateFramePacket, the client will calculate the frame physics status and check that the result is the same as the server one. If it is not the case, there is desynchronisation and the game must end!
 * \subsection ping Ping
 * It is always important to know the current round trip time between a client and a server. The ping system is pretty simple. The client sends a PING Packet to the server containing the current time and the server sends the same Packet back. When the client gets the Packet back, it can calculate the time it took for the Packet to do the round trip (RTT).
 * 
 * We then use TCP Retransmission Timer to calculate srtt and rttvar to get an idea of the average and variability of the packet.
 * \section rollback Rollback mechanisms
 * \subsection rollback_how How the rollback works?
 * At any time, each client have the game world state of two points in time:
 * - Last Validated Frame
 * - Current Frame (except the server)
 * The server only stores the last validated frame's physics state.
 * \subsection current_frame Client Current Frame
 * To allow real time illusion, the client controls its player character in real time without waiting the validation of the server. For other clients, the rollback maanger will simply repeat the last received inputs.
 * 
 * After receiving other clients inputs, the rollback manager will run all the Fixed Update between the last validated frame and the current frame before running the new current frame.
 * \subsection physics_checksum Validating a Frame
 * When validating a frame, the server calculates the new physics state and will then generate a checksum (a 16-bit number) per player of the player character positions, rotations and velocities (linear and angular). This number is sent in the <a href="structgame_1_1_validate_frame_packet.html">ValidateStatePacket</a> with the validated frame index.
 * 
 * The clients will then validate the frame by calculating the physics state up to the server validated frame and will then compare the checksums values. If the values differ, it is the end of the game, because the physics state of the client is in desync.
 * \subsection destroy_entity Destroy Entities
 * 
 * \section sqlite SQLite
 * To debug efficiently the missbehavior of the netcode, the framework is providing a SQLite database allowing to review the last session.
 * \subsection input_dbg Input debugging
 * The SQLite stores the input of all players per frame on each client and servers.
 * \subsection physics_state Physics State debugging
 */