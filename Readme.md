# R-TYPE

This project is a remake of the famous game R-Type, made by the french studio IREM in 1987. <br>
We need to add a multiplayer mode to the game. <br>
To do so we will use the [SFML](https://www.sfml-dev.org/) library for the graphical part and also the network part. <br>
We will also use the [ECS](https://en.wikipedia.org/wiki/Entity_component_system) architecture to make the game more modular.

## Contributors

- [**Mathieu Brujan**](https://github.com/Tomeiru)
- [**Ludovic Sutter**](https://github.com/ludovic-str)
- [**Maxime Hoffbeck**](https://github.com/MaximeHff)
- [**Alexandre Frantz**](https://github.com/AlexandreFRANTZ)
- [**Timothé Coniel**](https://github.com/titi0267)

## How to clone the project with submodules

```bash
git clone --recurse-submodules git@github.com:Tomeiru/R-Type.git
```

## How to build the project

### Linux

```bash
mkdir build
cd build
cmake ..
make
```

### Windows

For windows, you need to install Visual Studio and the C++ package.

You have to open a new CMake project and select the folder where you cloned the project.

Then you can build the project.

## How to run the project

```bash
./server <port>
./client <ip> <server port> <client port>
```

## Commands

| Command | Description |
| ------- | ----------- |
| `z`     | Move up     |
| `s`     | Move down   |
| `q`     | Move left   |
| `d`     | Move right  |
| `space` | Shoot       |

## Developers documentation

You can find the documentation of the main repository [here](https://tomeiru.github.io/R-Type/).

You can find the documentation of the sfml encapsulation repository [here](https://github.com/Tomeiru/sfml-encapsulation).

You can find the documentation of the ECS [here](https://github.com/Tomeiru/entity-component-system)
