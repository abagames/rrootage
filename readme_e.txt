rRootage  readme_e.txt
for Windows98/2000/XP(OpenGL required)
ver. 0.23
(C) Kenta Cho

Defeat autocreated huge battleships.
Shootem up game, 'rRootage'.


- How to install.

Unpack rr0_23.zip, and execute 'rr.exe'.
(If the game is too heavy for your PC, please try 'rr_lowres.bat'.
 This batch file launches the rRootage in the low resolution mode.)


- How to play.

 - Movement  Arrow key / Joystick
 - Laser     [Z]       / Trigger 1, Trigger 4
 - Special   [X]       / Trigger 2, Trigger 3
 - Pause     [P]

Select the stage by a keyboard or a joystick.
Press a laser key to start the game.

You can also select the game mode from 4 types.
Press a special key to change the game mode.

. Normal mode
 This is the standard game mode.
 Your ship becomes slow while holding the laser key.
 - Special -> Bomb
 The bomb wipes enemyies bullets.
 The number of bombs are displayed at the right-down corner.

. PSY mode
 As your ship grazes a bullet,
 the graze meter(displayed at the right-down corner) increases.
 When the graze meter becomes full, the ship becomes invincible for a while.
 - Special -> Rolling
 This movement widen the range that the ship can graze.
 While holding this key, the ship becomes slow.
 If you want to move faster, tap this key.

. IKA mode
 Your ship has two attributes, white and black.
 All bullets also have these attributes, 
 and your ship can absorb bullets that has the same attribute.
 Absorbed bullets are changed into lasers automatically.
 - Special -> Attribution change
 Change your ships attribute.

. GW mode
 Your ship can use the reflector.
 The reflector reflects bullets around your ship.
 - Special -> Reflector
 To use the reflector,
 you have to hold this key until 
 the reflector meter(displayed at the right-down corner) becomes empty.
 You can use the reflector only if the reflector meter displays 'OK'.

Control your ship and avoid the barrage.
Use the laser to destroy the battleship of the enemy.
You can cause more damage if you fire the laser close to the enemy.

When all ships are destroyed, the game is over.
The ship extends 200,000 and every 500,000 points.

These options are available:
 -lowres        Use the low resolution mode.
 -nosound       Stop the sound.
 -window        Launch the game in the window, not use the full-screen.
 -reverse       Reverse the laser key and the bomb key.


- Comments

If you have any comments, please mail to cs8k-cyu@asahi-net.or.jp


- Webpage

rRootage webpage:
http://www.asahi-net.or.jp/~cs8k-cyu/windows/rr_e.html


- Acknowledgement

libBulletML is used to parse BulletML files.
 libBulletML
 http://user.ecc.u-tokyo.ac.jp/~s31552/wp/libbulletml/
 
Simple DirectMedia Layer is used for the display handling. 
 Simple DirectMedia Layer
 http://www.libsdl.org/

SDL_mixer and Ogg Vorbis CODEC to play BGM/SE. 
 SDL_mixer 1.2
 http://www.libsdl.org/projects/SDL_mixer/
 Vorbis.com
 http://www.vorbis.com/

Mersenne Twister to create the random number.
 http://www.math.keio.ac.jp/matumoto/emt.html


- History

2003  8/15  ver. 0.23
            Adjust barrages.
            Update libBulletML.
            Fix memory handling bugs.
2003  4/26  ver. 0.22
            Adjust barrages.
2003  4/ 4  ver. 0.21
            Add new barrages.
            Adjust barrages.
2003  3/21  ver. 0.2
            Add new game modes.
            Timer stops while the battleship changing the shape.
            Adjust bullets.
2003  3/16  ver. 0.1


-- License

License
-------

Copyright 2003 Kenta Cho. All rights reserved. 

Redistribution and use in source and binary forms, 
with or without modification, are permitted provided that 
the following conditions are met: 

 1. Redistributions of source code must retain the above copyright notice, 
    this list of conditions and the following disclaimer. 

 2. Redistributions in binary form must reproduce the above copyright notice, 
    this list of conditions and the following disclaimer in the documentation 
    and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
