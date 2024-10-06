#Project 1-Bit Platformer

SDL2 Platformer project inspired by the wonderful [1-Bit
Platformer](https://kenney.nl/assets/1-bit-platformer-pack) designed by
[Kenney](https://kenney.nl).

After "finishing" the SDL2
[Spaceshooter](https://github.com/zwilder/spaceShooter/), I wanted to tackle a
more complex project. Keeping the end goals simple, the "finished" game should
have levels, scores, collectibles, environmental obstacles, multiple enemies.

Goals:
- Improve SDL2 functions written for Spaceshooter/work on SDL2 toolbox
- Render the screen to a texture, and then render the texture at different
  window resolutions (including fullscreen). Game resolution should be 256x240
with 16x16 sprites (16 tiles w x 15 tiles high). This 256x240 was chosen because
it is the same as the NES - ideally this will give the project that sort of
vibe. Additionally rendering the entire scene to a texture might allow some cool
full screen effects!
- Basic platformer physics - run, jump, climb, fight, collide

Might include:
- Load Tiled .tmx files for levels
- Ambitious goal: Procedurally generate levels using chunks 
- Compressed asset file - all levels, sprites, sounds etc loaded from a single
  "1BP_assets.dat" file
