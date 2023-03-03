cmake_minimum_required(VERSION 3.17)

project(r-type)
set(CMAKE_CXX_STANDARD 20)

set(SRC_ECS
        ecs/ComponentArray.hpp ecs/ComponentManager.hpp ecs/Coordinator.hpp ecs/EntityManager.hpp ecs/ResourceManager.hpp
        ecs/RuntimeException.hpp ecs/System.hpp ecs/SystemManager.hpp ecs/Types.hpp
        )

set(SRC_ENCAPSULATION
        sfml/Clock.hpp sfml/Color.hpp sfml/ColorManager.hpp sfml/Event.hpp sfml/EventManager.hpp sfml/Font.hpp
        sfml/FontManager.hpp sfml/Image.hpp sfml/ImageManager.hpp sfml/IpAddress.hpp sfml/Joystick.hpp sfml/Keyboard.hpp
        sfml/Mouse.hpp sfml/Mouse.cpp sfml/Music.hpp sfml/MusicManager.hpp sfml/Rect.hpp sfml/RuntimeException.hpp sfml/SoundBuffer.hpp
        sfml/SoundBufferManager.hpp sfml/Sound.hpp sfml/SoundManager.hpp sfml/Sprite.hpp sfml/SpriteManager.hpp sfml/Text.hpp
        sfml/TextManager.hpp sfml/Texture.hpp sfml/TextureManager.hpp sfml/Time.hpp sfml/UDPSocket.hpp sfml/Vector2.hpp sfml/Window.hpp
        )

set (SRC_COMMON
        common/PackageManager.hpp common/RuntimeException.hpp common/ThreadSafeQueue.hpp common/UDPHandler.hpp
        common/packet/PlayerName.hpp common/ReceivedPacket.hpp common/packet/GameStart.hpp common/packet/PlayerInputs.hpp common/packet/EntityPosition.hpp)

set(CMAKE_BUILD_TYPE Debug)
set(SRC_CLIENT
        client/main.cpp
        client/Client.hpp client/PlayerID.hpp client/SceneManager.hpp client/ServerEntityManager.hpp server/component/Speed.hpp server/component/Direction.hpp server/component/Attack.hpp server/component/DestroyEntity.hpp server/component/Health.hpp server/system/DestroyEntityOutWindow.hpp common/system/LinearMove.hpp server/system/Shoot.hpp server/system/KillNoLife.hpp client/system/TintSprite.hpp client/component/Clickable.hpp client/component/Hover.hpp client/component/Hover.hpp client/component/HoverTint.hpp client/component/MovementJoystick.hpp client/component/MusicReference.hpp client/component/Outline.hpp client/component/SoundReference.hpp client/component/Spin.hpp client/component/TextReference.hpp client/system/DrawText.hpp client/system/OutlineText.hpp client/system/PlayMusic.hpp client/system/PlaySound.hpp client/system/SpinSprite.hpp client/system/TintText.hpp client/system/TransformText.hpp client/system/UpdateClick.hpp client/system/UpdateHover.hpp client/system/UpdateHoverTint.hpp client/system/UpdateJoystickMovements.hpp server/system/UpdateEntityPositions.hpp server/component/BackupTransform.hpp common/packet/TransformEntity.hpp common/packet/DestroyEntity.hpp client/PacketManager.hpp common/packet/CreateSpriteReference.hpp)

set(SRC_SERVER
        server/main.cpp
        server/PlayerManager.hpp common/UDPClient.hpp server/Types.hpp server/Server.hpp server/EnemyManager.hpp server/BulletManager.hpp)

#CMake for Windows and Windows Compilers
if (MSVC OR MSYS OR MINGW OR WIN32)
    #Install VCPKG in the repository if not already installed
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09\\vcpkg.exe)
        message(STATUS "VCPKG n'est pas installé, téléchargement en cours...")
        file(DOWNLOAD https://github.com/microsoft/vcpkg/archive/refs/tags/2023.01.09.tar.gz ${CMAKE_BINARY_DIR}\\vcpkg.tar.gz SHOW_PROGRESS)
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf vcpkg.tar.gz WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        execute_process(COMMAND .\\vcpkg-2023.01.09\\bootstrap-vcpkg.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        file(REMOVE ${CMAKE_BINARY_DIR}\\vcpkg.tar.gz)
    endif()
    #Install SFML and all its dependencies
    execute_process(COMMAND ${CMAKE_BINARY_DIR}\\vcpkg-2023.01.09\\vcpkg.exe install sfml --triplet x64-windows
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}\\vcpkg-2023.01.09
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output)
    if(result)
        message(FATAL_ERROR "Echec de l'installation de la SFML: ${output}")
    endif()
    execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09/vcpkg install rapidjson --triplet x64-windows
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output)
    if(result)
    message(FATAL_ERROR "Echec de l'installation de la rapidjson: ${output}")
    endif()
endif()

#CMake for Linux and Apple
if (UNIX)
    #Install VCPKG in the repository if not already installed
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09/vcpkg)
        message(STATUS "VCPKG n'est pas installé, téléchargement en cours...")
        file(DOWNLOAD https://github.com/microsoft/vcpkg/archive/refs/tags/2023.01.09.tar.gz ${CMAKE_BINARY_DIR}/vcpkg.tar.gz SHOW_PROGRESS)
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf vcpkg.tar.gz WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        execute_process(COMMAND ./vcpkg-2023.01.09/bootstrap-vcpkg.sh WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        file(REMOVE ${CMAKE_BINARY_DIR}/vcpkg.tar.gz)
    endif()
    #Install SFML and all its dependencies
    message(STATUS "Installation de la SFML...")
    execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09/vcpkg install sfml
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output)
    if(result)
        message(FATAL_ERROR "Echec de l'installation de la SFML: ${output}")
    endif()
    message(STATUS "Installation de la RapidJSON...")
    execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09/vcpkg install rapidjson
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output)
    if(result)
        message(FATAL_ERROR "Echec de l'installation de rapidjson: ${output}")
    endif()
    message(STATUS "Installation de la FreeType...")
    execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09/vcpkg install freetype
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output)
    if(result)
        message(FATAL_ERROR "Echec de l'installation de freetype: ${output}")
    endif()
endif()

add_executable (client ${SRC_CLIENT} ${SRC_ECS} ${SRC_ENCAPSULATION} ${SRC_COMMON})
add_executable (server ${SRC_SERVER} ${SRC_ECS} ${SRC_ENCAPSULATION} ${SRC_COMMON})
set(VCPKG_ROOT_DIR ${CMAKE_BINARY_DIR}/vcpkg-2023.01.09)

if (MSVC OR MSYS OR MINGW OR WIN32)
    set(VCPKG_TARGET_TRIPLET x64-windows)
    set(SFML_DIR ${VCPKG_ROOT_DIR}\\packages\\sfml_x64-windows\\share\\sfml)
    set(VCPKG_LIBRARY_DIR ${VCPKG_ROOT_DIR}\\installed\\${VCPKG_TARGET_TRIPLET})
    file(GLOB release_dll "${VCPKG_LIBRARY_DIR}/bin/*.dll")
    file(GLOB debug_dll "${VCPKG_LIBRARY_DIR}/debug/bin/*.dll")
    file(COPY ${release_dll} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${debug_dll} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(GLOB assets_file "${VCPKG_LIBRARY_DIR}/debug/bin/*.dll")
    file(COPY ./assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/..
        PATTERN "*")
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT_DIR}\\installed\\${VCPKG_TARGET_TRIPLET})
endif()

if(UNIX)
    if (NOT APPLE)
        set(VCPKG_TARGET_TRIPLET x64-linux)
        set(SFML_DIR ${VCPKG_ROOT_DIR}/packages/sfml_x64-linux/share/sfml)
    endif()
    if(APPLE)
        set(VCPKG_TARGET_TRIPLET arm64-osx)
        set(SFML_DIR ${VCPKG_ROOT_DIR}/packages/sfml_arm64-osx/share/sfml)
    endif()
    set(VCPKG_LIBRARY_DIR ${VCPKG_ROOT_DIR}/installed/${VCPKG_TARGET_TRIPLET}/lib)
    set(CMAKE_PREFIX_PATH ${VCPKG_ROOT_DIR}/installed/${VCPKG_TARGET_TRIPLET})
endif()

find_package(SFML 2.5 COMPONENTS audio graphics network system window REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(freetype CONFIG REQUIRED)

set(SFML_LIBS
    sfml-audio
    sfml-graphics
    sfml-network
    sfml-system
    sfml-window
    freetype
    )
target_link_libraries(client ${SFML_LIBS})
target_link_libraries(server ${SFML_LIBS} rapidjson)