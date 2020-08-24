---------------~MiniginEngine~---------------------
This custom engine is built upon the foundations of 
the given Minigin engine.
The engine uses SDL to render textures, which is its 
main capacity.

public GitHub repo: https://github.com/kdedeurw/MiniginBubbleBobble

---------------------Main--------------------------
It uses a Unity-like design, where objects are 
represented by empty game objects carrying components.
There are a few base components such as an FPSComponent, 
Texture2DComponent, SpriteComponent and a PhysicsComponent.
There also exists a TextureObject class, which acts as a 
GameObject, but without the ability to carry components.
It can only draw text(s) based upon fonts in real-time.

---------------------MiniMath-----------------------
MiniMath contains Vector classes; Vector2, Vector3 
and Vector4.
It also contains global static functions such as 
Clamp and overloaded operators for Vectors and RGBA.

---------------------Transform----------------------
A Transform is a class, not a component, owned by 
all GameObjects and TextObjects.
It contains the position, rotation and scaling of set
SceneObject.

---------------------GameObject---------------------
A GameObject is an empty object that carries a 
reference to a transform component.
A GameObject can be parented/childed by another 
GameObject, the transform should become local and
the final position should be given by the public 
function; 'GetFinalPos'.
It carries a vector of components and updates and
renders them accordingly.
A GameObject is created by a memoryallocator and 
its existence is managed by a scene.

---------------------Scenes--------------------------
The engine uses a scene manager to hold scenes.
A scene consists out of a vector of GameObjects and/or
TextObjects. A scene is responsible for both updating 
and rendering its objects.
Every scene manages their Objects' memories.

---------------------Rendering-----------------------
The engine uses a renderer, which has one main function
called RenderTexture, which has been overloaded a lot 
to improve compatibility with most objects and components.

---------------------Resources-----------------------
There are a couple of base resources available that are
accessible everywhere. These are Texture2D, Font and
RigidBody.
A resource manager also exists as a singleton, it is 
used to either load in textures and/or fonts or 
retrieve already existing pointers to textures.
This makes it fairly easy and quick to swap textures.

---------------------Input--------------------------
It also has full support for gamepads and keyboard and mouse.
There exist 2 singletons named; 'KeyboardAndMouse' and 
'ControllerListener'.
The controllerlistener uses the XInput API and the 
KeyboardAndMouse uses the SDL wrapper for keyboard and 
mouse input.

---------------------Memory--------------------------
The entirety of the engine's memory is being handled
by custom memory allocators which are easy to use.
There exists a singleton wrapper named; 
GlobalMemoryAllocators.
All GameObjects, Components, Scenes and projects 
should use these. The sizes of the allocated memory 
is easily changeable.

---------------------Globals------------------------
There is a header named; 'Globals', which includes
most singletons.
List of all Globals:
- GlobalInput
- GlobalMemoryPools
- GameState (see below)
- SceneManager
- Renderer
- ResourceManager

---------------------GameState----------------------
The GameState is a singleton that carries a timer,
DeltaTime, WindowWidth and WindowHeight, the current
Scene and most Globals.

---------------------MiniginGame--------------------
MiniginGame is a base class to inherit from in a 
project. It has 3 overloadable functions:
- LoadGame: initializes all resources and game info
- Update: additional update after base update
- Render: additional render after base render
Use this as a start to begin your project since it
will handle everything described above.

-------------~3rdParty libraries~-------------------

---------------------Box2D--------------------------
The engine is slightly supported by the Box2D framework
and uses its most basic collision calculation.
This is expanded upon with the RigidBody and 
RigidBodyComponent.
