Asset:
- Entities in the game will be rendered using various Textures and Animations which we will be calling Assets (along with Fonts)
- Assets are loaded once at the beginning of the program and stored in the Assets class, which is stored by the GameEngine class
- All Assets are defined in assets.txt, with the syntax defined below

Note:
- All entity positions denote the center of their rectangular sprite. It also denotes the center of bounding box, if it has one. This is set via sprite.setOrigin() in the Animation class constructor.

Player:
- The player entity in the game is represented by Megaman, which has several different Animations: Stand, Run and Air. You must determine which state the player is currently in and assign the correct Animation.
- The player moves with the following controls:
  - Left: Q
  - Right: D
  - Jump: Z
  - Shoot: Space
- The player can move left, move right, or shoot at any time during the game. This means the player can move left/right while in the air.
- The player can only jump if it is currently standing on a tile.
- If the jump key is held, the player should not continuously jump, but instead it should only jump once per button press. If the player lets go f the jump key mid-jump, it should start falling back down immediately.
- If the player moves left/right, the player's sprite will face in that direction until the other direction has been pressed.
- Bullets shot by the player travel in the direction the player is facing.
- The player collides with 'Tile' entities in the level (see level syntax) and cannot move through them. The player land on a Tile entity and stand in place if it falls on it from above.
- The player does not collide with 'Dec' (decoration) entities in the level.
- If the player falls below the bottom of the screen, he respawns at the start.
- The player should have a Gravity component which constantly accelerates it downward on the screen until it collides with a tile.
- The player has a maximum speed specified in the Level file (see below) which it should not exceed in either x or y direction.
- The player will be given  a CBoundingBox of a size specified in the level file.
- The player's sprite and bounding box are centered on the player's position.

Animations:
- See below for Animation asset specification
- Animations are implemented by storing multiple frames inside a texture.
- The Animation class handles frame advancement based on animation speed.
- You need to implement Animation::update() to properly progress animations.
- You need to implement Animation::hasEnded() which returns true if an animation has finished its last frame, false otherwise.
- Animations can be repeating (loop forever) or non-repeating (play once).
- Any entity with a non-repeating animation should be destroyed once its Animation's hasEnded() return true (has finished one cycle).

Decoration Entities:
- Decoration entities ('Dec' in a level file) are simply drawn to the screen, and do not interact with any other entities in the game in any way.
- Decorations can be given any Animation in the game, but intuitively they should be reserved for things like clouds, bushes, etc.

Tiles:
- Tiles are Entities that define the level geometry and interact with players.
- Tiles can be given any Animation that is defined in the Assets file.
- Tiles will be given a CBoundingBox equal to the size of the animation tile->getComponent<CAnimation>().animation.getSize()
- The current animation displayed for a tile can be retrieved with: tile->getComponent<CAnimation>().animation.getName()
- Tiles have different behavior depending on which Animation they are given.

  Brick Tiles:
  - Brick tiles are given the 'Brick' Animation.
  - When a brick tile collides with a bullet, or is hit by a player from below:
    - Its animation should change to 'Explosion' (non-repeat).
    - Non-repeating animation entities are destroyed when hasEnded() is true.
    - Its CBoundingBox component should be removed.

  Question Tiles:
  - Question tiles are given the 'Question' Animation when created.
  - When a Question tile is hit by a player from below, 2 things happen:
    - Its Animation changes to the darker 'Question2' animation
    - A temporary lifespan entity with the 'Coin' animation should appear for 30 frames, 64 pixels above the location of the Question entity.

Drawing:
- Entity rendering has been implemented for you, no need to change that system.

Misc:
- The 'P' key should pause the game.
- Pressing the 'T' key toggles drawing textures.
- Pressing the 'C' key toggles drawing bounding boxes of entities.
- Pressing the 'G' key toggles drawing of the grid. This should be very helpful for debugging.
- The 'ESC' key should go back to the Main Menu, or quit if on the Main Menu.
