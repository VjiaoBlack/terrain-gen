
![alt text](https://raw.githubusercontent.com/VjiaoBlack/terrain-gen/master/sample_athena.gif)

athena
=====

athena is a civilization builder game in pre-alpha stage.

Currently it is mostly still a terrain generator.

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
Refer to this document for some details about how this works:
https://docs.google.com/document/d/1xySCdTcphTbBVFzzvw51X8ISexEsv2MM4zNanQW6Hbc/edit?usp=sharing
