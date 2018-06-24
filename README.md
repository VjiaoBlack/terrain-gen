
![alt text](https://raw.githubusercontent.com/VjiaoBlack/terrain-gen/master/sample_athena.gif)

athena
=====

athena is a civilization builder game in pre-alpha stage.

Currently it is mostly still a terrain generator and visualizer.

I use other bits of code to help with some tricky math stuff - I cite them in comments where I use them.

Compilation
-----------

Type "make" to compile.
To run, type "./athena".

Controls:
--------
| Key | Function |
| --- | -------- |
| q | quit |
| b | blur heightmap |
| u | decreases heightmap by 1 |
| i | increases heightmap by 1 |
| n | set normal map |
| j | set wind map (needs normal map) |
| m | run moisture engine (needs wind map and normal map) |
| r | rain! |
| w | run water |
| e | if water is running, allow erosion. |
| arrows | moves around terrain |
| shift | increases heightmap movement / shifting speed |
| s | attempt to save colormap and heightmap |

Documentation
-------------
Requires:
`cmake`, `sdl2`, `sdl2_image`, `sdl2_ttf`, `glm`

Classes and their functions
- `TrData` - handles importing the data for different entity types and formulas. Currently 
doesn't handle maps and stuff (I don't have much Map or Game IO right now), but it might later.
- `TrECS` - handles the main Entity Component System pattern for game entities. I don't really 
use the "System" part of the ECS design pattern right now, but I probably will later.
- `TrGUI` - handles the GUI system. Currently does not use ECS, but it probably will later.
- `TrMap` - handles rendering and simulating the map data, such as rain, shadow mapping, ray 
tracing, day/night cycle, erosion, and more.
- `TrRenderLoop` - handles the updating and rendering, depending on what "state" of the game 
you're in - different TrRenderLoop's handles the main menu, the main game, and transitions.
- `TrUtils` - holds a bunch of generic helper functions.

External includes
- `fast-cpp-csv-parser` - currently used in parsing the data files. I think I might make my own 
later with c++'s regex system.
- `fft` - runs a fast fourier transform based ocean wave simulation. I use it for bump-mapping 
water.

Refer to this document for some details about how this works:
https://docs.google.com/document/d/1xySCdTcphTbBVFzzvw51X8ISexEsv2MM4zNanQW6Hbc/edit?usp=sharing

