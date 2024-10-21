# Project 1-Bit Platformer

![Screenshot](https://zwilder.github.io/assets/Various/1BP_WIP_241020.gif)

SDL2 Platformer project inspired by the wonderful [1-Bit
Platformer Art Assets](https://kenney.nl/assets/1-bit-platformer-pack) designed by
[Kenney](https://kenney.nl).

After "finishing" the SDL2
[Spaceshooter](https://github.com/zwilder/spaceShooter/), I wanted to tackle a
more complex project. Keeping the end goals simple, the "finished" game should
have levels, scores, collectibles, environmental obstacles, multiple enemies.

Right now the project will load and interpret the .tmx files saved by
[Tiled](https://www.mapeditor.org/). You can draw the tilemap, define
objects/colors with rectangles, and the game will load them from the command
line with `./1BP mapfile.tmx`. There are a couple (basic)
[examples](https://github.com/zwilder/Project_1BP/blob/master/assets/MapB.tmx)
in `assets/`.

The main character moves with basic physics - accelerates to a top speed, jumps
and falls with gravity. The collision system is still a work in progress, and is
a bit rough around the edges. The player can't walk through solid tiles, and can
jump up through platforms to stand on them. 


WIP Videos:
- [10/17/2024](https://zwilder.github.io/assets/Various/1BP_WIP_241017.mp4)

Goals:
- Improve SDL2 functions written for Spaceshooter/work on SDL2 toolbox
- Render the screen to a texture, and then render the texture at different
  window resolutions (including fullscreen). Game resolution should be 256x240
with 16x16 sprites (16 tiles w x 15 tiles high). This 256x240 was chosen because
it is the same as the NES - ideally this will give the project that sort of
vibe. Additionally rendering the entire scene to a texture might allow some cool
full screen effects! - In progress, _should_ be good to go... will wait until I
have more action on the screen to see if the rendering pipeline is decent.
- Basic platformer physics - run, jump, climb, fight, collide
- Scrolling maps - horizontal AND vertical
- Controller functionality! Platformers "feel" better with a controller

Might include:
- Advanced platformer physics - hang, wall jumps, push/pull blocks
- Puzzles! "Block" puzzles, "control room" puzzles, lock/key puzzles
- Scripted events
- Load Tiled .tmx files for levels
- Alternatively, make a "level editor" program including the scripted events
  that can output XML files - like Tiled, except hand rolled and tailored to
this game
- Ambitious goal: Procedurally generate levels using chunks 
- Compressed asset file - all levels, sprites, sounds etc loaded from a single
  "1BP_assets.dat" file
