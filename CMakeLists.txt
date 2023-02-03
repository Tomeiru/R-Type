cmake_minimum_required(VERSION 3.17)

project(r-type)

set(SRC_ECS
        ecs/ComponentArray.hpp ecs/ComponentManager.hpp ecs/Coordinator.hpp ecs/EntityManager.hpp ecs/ResourceManager.hpp
        ecs/RuntimeException.hpp ecs/System.hpp ecs/SystemManager.hpp ecs/Types.hpp
        )

set (SRC_ENCAPSULATION
        sfml/Clock.hpp sfml/Color.hpp sfml/ColorManager.hpp sfml/Event.hpp sfml/EventManager.hpp sfml/Font.hpp
        sfml/FontManager.hpp sfml/Image.hpp sfml/ImageManager.hpp sfml/IpAddress.hpp sfml/Joystick.hpp sfml/Keyboard.hpp
        sfml/Mouse.hpp sfml/Music.hpp sfml/MusicManager.hpp sfml/Rect.hpp sfml/RuntimeException.hpp sfml/SoundBuffer.hpp
        sfml/SoundBufferManager.hpp sfml/Sound.hpp sfml/SoundManager.hpp sfml/Sprite.hpp sfml/SpriteManager.hpp sfml/Text.hpp
        sfml/TextManager.hpp sfml/Texture.hpp sfml/TextureManager.hpp sfml/Time.hpp sfml/UDPSocket.hpp sfml/Vector2.hpp sfml/Window.hpp
        )

set (SRC_COMMON
        common/PackageManager.hpp common/RuntimeException.hpp common/ThreadSafeQueue.hpp common/UDPHandler.hpp
        common/packet/PlayerName.hpp common/ReceivedPacket.hpp common/packet/GameStart.hpp)

set(SRC_CLIENT
        client/main.cpp
        )

set(SRC_SERVER
        server/main.cpp
        server/PlayerManager.hpp common/UDPClient.hpp server/Types.hpp)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09/vcpkg)
    message(STATUS "VCPKG n'est pas installé, téléchargement en cours...")
    file(DOWNLOAD https://github.com/microsoft/vcpkg/archive/refs/tags/2023.01.09.tar.gz ${CMAKE_BINARY_DIR}/vcpkg.tar.gz SHOW_PROGRESS)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf vcpkg.tar.gz WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ./vcpkg-2023.01.09/bootstrap-vcpkg.sh WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    file(REMOVE ${CMAKE_BINARY_DIR}/vcpkg.tar.gz)
endif()
execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09/vcpkg install sfml
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output)
if(result)
    message(FATAL_ERROR "Echec de l'installation de la SFML: ${output}")
endif()
add_executable (client ${SRC_CLIENT} ${SRC_ECS} ${SRC_ENCAPSULATION} ${SRC_COMMON})
add_executable (server ${SRC_SERVER} ${SRC_ECS} ${SRC_ENCAPSULATION} ${SRC_COMMON})
set(VCPKG_ROOT_DIR ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09)

if(UNIX AND NOT APPLE)
    set(VCPKG_TARGET_TRIPLET x64-linux)
    set(SFML_DIR ${VCPKG_ROOT_DIR}/packages/sfml_x64-linux/share/sfml)
endif()
if(MSVC OR MSYS OR MINGW OR WIN32)
    set(VCPKG_TARGET_TRIPLET x64-windows)
    set(SFML_DIR ${VCPKG_ROOT_DIR}/packages/sfml_x64-windows/share/sfml)
endif()
if(APPLE)
    set(VCPKG_TARGET_TRIPLET arm64-osx)
    set(SFML_DIR ${VCPKG_ROOT_DIR}/packages/sfml_arm64-osx/share/sfml)
endif()

set(VCPKG_LIBRARY_DIR ${VCPKG_ROOT_DIR}/installed/${VCPKG_TARGET_TRIPLET}/lib)
set(CMAKE_PREFIX_PATH ${VCPKG_ROOT_DIR}/installed/${VCPKG_TARGET_TRIPLET})
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
set(SFML_LIBS
        sfml-audio
        sfml-graphics
        sfml-network
        sfml-system
        sfml-window
        )
target_link_libraries(client ${SFML_LIBS})
target_link_libraries(server ${SFML_LIBS})