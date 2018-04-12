# rRootage

for Windows98 or later (OpenGL required)

ver. 0.24

Defeat autocreated huge battleships.
Shootem up game, 'rRootage'.

![screenshot](http://www.asahi-net.or.jp/~cs8k-cyu/windows/rr_1.gif)

### How to install.

[Download: rr0_24.zip](http://abagames.sakura.ne.jp/windows/rr0_24.zip)

[Download: rr0_23.zip](http://abagames.sakura.ne.jp/windows/rr0_23.zip) (for Win9x users)

Unpack rr0_24.zip, and execute 'rr.exe'.

### How to play.

* Movement Arrow key / Joystick
* Laser [Z] / Trigger 1, Trigger 4
* Special [X] / Trigger 2, Trigger 3
* Pause [P]

Select the stage by a keyboard or a joystick.
Press a laser key to start the game.

You can also select the game mode from 4 types.
Press a special key to change the game mode.

#### Normal mode

This is the standard game mode.
Your ship becomes slow while holding the laser key.

* Special -> Bomb

The bomb wipes enemyies bullets.
The number of bombs are displayed at the right-down corner.

#### PSY mode

As your ship grazes a bullet,
the graze meter(displayed at the right-down corner) increases.
When the graze meter becomes full, the ship becomes invincible for a while.

* Special -> Rolling

This movement widen the range that the ship can graze.
While holding this key, the ship becomes slow.
If you want to move faster, tap this key.

#### IKA mode

Your ship has two attributes, white and black.
All bullets also have these attributes,
and your ship can absorb bullets that has the same ttribute.
Absorbed bullets are changed into lasers automatically.

* Special -> Attribution change

Change your ships attribute.

#### GW mode

Your ship can use the reflector.
The reflector reflects bullets around your ship.

* Special -> Reflector

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

* -fullscreen Launch the game in the full-screen.
* -nosound Stop the sound.
* -reverse Reverse the laser key and the bomb key.
* -lowres Use the low resolution mode.

### How to build

1. Install [MSYS2](http://www.msys2.org/).

1. Open mingw32.exe.

1. Install gcc with pacman.

   ```
   $ pacman -S base-devel mingw-w64-i686-toolchain
   ```

1. Install [SDL-devel-1.2.15-mingw32.tar.gz (SDL1.2 Development Libraries for Mingw32)](https://www.libsdl.org/download-1.2.php).

1. Clone or download this repository.

1. Enter the src directory.

1. Make.

   ```
   $ make
   ```

### Comments

If you have any comments, please mail to cs8k-cyu@asahi-net.or.jp

### Webpage

[rRootage webpage](http://www.asahi-net.or.jp/~cs8k-cyu/windows/rr_e.html)

### Acknowledgement

[libBulletML](http://shinh.skr.jp/libbulletml/index_en.html) is used to parse BulletML files.

[Simple DirectMedia Layer](http://www.libsdl.org/) is used for the display handling.

[SDL_mixer](http://www.libsdl.org/projects/SDL_mixer/) and [Ogg Vorbis CODEC](http://www.vorbis.com/) to play BGM/SE.

[Mersenne Twister](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html) to create the random number.

### History

2018 1/15 ver. 0.24

* Update SDL.

2003 8/15 ver. 0.23

* Adjust barrages.
* Update libBulletML.
* Fix memory handling bugs.

2003 4/26 ver. 0.22

* Adjust barrages.

2003 4/ 4 ver. 0.21

* Add new barrages.
* Adjust barrages.

2003 3/21 ver. 0.2

* Add new game modes.
* Timer stops while the battleship changing the shape.
* Adjust bullets.

2003 3/16 ver. 0.1
