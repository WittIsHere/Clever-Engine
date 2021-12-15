# Clever Engine

[Clever Engine](https://github.com/WittIsHere/Clever-Engine) is a 3D Game Engine developed in C & C++ made by Carles López, Aram Galarza and Albert Espinosa, students of [CITM](https://www.citm.upc.edu/) in Barcelona during our [Game Design and Development Bachelor's Degree](https://www.citm.upc.edu/cat/estudis/grau-videojocs-bcn/).

## NEW Features
All that we have added for this new assignment

* Scene serialization: The scene is saved and loaded in a JSON file
* Custom File Format: FBX assets are now saved on the Library folder with our custom file format.
* Bounding Boxes displayed.
* Game Objects now have a random UUID on creation.
* Inspector window shows data of the components of the Game Object successfully
* When selecting a Game Object (from the hierarchy) you can Transform, Rotate and Scale it.
* Added a Folder window.
* You can now Delete Game Objects
* Our Custom File Format Meshes are now loaded on a resource mesh. This resource is then added to a component mesh. It is a first approach to the whole resources problem, but it works well. 

**ISSUES**
* We do not load textures anymore, because it is incompatible with the whole resources change and we couldn't finish a resouce texture.
* Mouse picking and Frustum not finished.


## OLD Features

**Console:**
Logs real-time information about the engine's processes
* Autoscroll
* Clear console and last log buttons

**Configuration:**
Shows the properties of the engine and its rendering
* Application, Hardware and Input information
* Window and Rendering options
* Camera movement 

**Main Tab:**
Menus of all the functionalities of the engine
* File
* GameObject creation
* Windows
* Help
* Hierarchy window displaying GO names

### Functionalities

* **Drag & Drop:** meshes and textures can be dropped to the engine.
* **Game Object:** structure with Component Pattern implemented.
* **ImGUI interface:** fully functional user interface created using ImGui.
* **Camera:** unity-like camera to explore the scene and view the geometry.
* **Basic Shapes Lib:** implemented a library with many basic shapes on the menu "Create".

### Extra Features

* Docking

## Usage

### Camera Controls

* **Right Click**: change camera orientation.
* **WASD**: move camera while right clicking in different directions.
* **Q/E**: move upwards and downwards.
* **Mouse wheel**: Zoom in and out.


## Authors

### Albert Espinosa

- Github: [albertec1](https://github.com/albertec1)
- LinkedIn: [Albert Espinosa](https://www.linkedin.com/in/albert-espinosa-castillo-29860a1a2/)
	
### Carles López
 - Github: [carlesli](https://github.com/carlesli)
### Aram Galarza

- Github: [WittIsHere](https://github.com/WittIsHere)


## License

Licensed under the MIT LICENCE
