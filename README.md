## Azul 3D
Game Engine Research Project at DePaul University.

### Main Contents:
1. Standalone 3D Game Engine Core
2. Interactive World Editor with Gizmos and GUI
3. In-Engine Terrain Editor with GPU-based editing brushes

#### More Features
1. Savable scenes and terrain assets for iterative design
2. Skinned animations accelerated on the GPU
3. Sprites, fonts, and text-string game objects
4. Dynamic lighting system driven by light components
5. Paintable foliage objects with optimized rendering for high density

#### Note
Developed & functional on Windows with Visual Studio. Large assets are stored with git LFS. This repo was developed on a Perfore server so the commit history here is only intermittent.

## How to read

### Engine Code
Source files are flat on disc under [`Engine/src`](https://github.com/robbiegrier/Azul3D/tree/main/Engine/src).

Open `Engine.sln` which organizes the source files nicely with VS filters.

#### Interesting Sections
1. `Engine/src/Editor` contains files related to the interactive world editor with widgets, scenes, and edit history.
2. `Engine/src/Terrain` contains files related to the in-engine terrain editor and rendering system. This includes the GPU brush architecture, foliage system, and the quad tree data structure.
3. `Engine/src/Animation` contains files related to skinned animations, powered by compute shaders.

#### Shaders
HLSL Shaders are under [`Engine/Shaders/Original`](https://github.com/robbiegrier/Azul3D/tree/main/Engine/Shaders/Original).
1. `Engine/Shaders/Original/Graphics` contains core graphics shaders
2. `Engine/Shaders/Original/Compute/Terrain Brushes` contains GPU-based terrain editing brushes for sculpting and texture painting
3. `Engine/Shaders/Original/Compute/Animation` contains compute shaders for processing skinned animations

---

### Other Components

#### Asset Converter
`Converter.sln` converts raw assets for meshes, textures, and animations into a runtime format.

#### In-house Libraries
1. `Math`: Optimized 3D Math Library
2. `PCSTree`: Tree data structure for game object heirarchy
3. `Manager`: Asset management and infrastructure library
4. `File`: File IO wrapper

#### Save Data Directories
1. `Models/`: contains raw assets
2. `Scenes/`: contains JSON scene save files
3. `Terrains/`: contains saved terrain assets created in the engine

#### Included dependencies:
1. [`IMGUI`](https://github.com/ocornut/imgui): stored under `GUI/` as a VS project
2. `RapidXML`: under `RapidXML`
3. `Protobuf`: wrapped with the `ProtoBuf/` project
4. [`DirectXTex`](https://github.com/microsoft/DirectXTex): used in `Engine.sln`
5. [`JSON`](https://github.com/nlohmann/json): used in `Engine.sln`
6. [`TinyGLTF`](https://github.com/syoyo/tinygltf): used in `Converter.sln`

#### Provided Resources
1. `Framework/`: Grading framework
2. `Time/`: Abstract time format library

## How to run
1. Open and run `Converter.sln` to populate `Data/` directory with runtime models
2. Open and run `Engine.sln`
