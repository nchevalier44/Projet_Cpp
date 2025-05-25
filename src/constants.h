#ifndef PROJET_CPP_CONSTANTS_H
#define PROJET_CPP_CONSTANTS_H

//Definition of all constants we use in the project

//Main menu images
#define PATH_MAIN_MENU_BACKGROUND "../assets/images/menu/Main_bg.png"

//Start menu
#define PATH_MAIN_MENU_BUTTON_BACKGROUND "../assets/images/menu/button_background.png"
#define PATH_MAIN_MENU_BUTTON_ANIMATION "../assets/images/menu/button_animation.gif"
#define PATH_MAIN_MENU_COMBOBOX_BACKGROUND "../assets/images/menu/dropdown_item.png"
#define PATH_MAIN_MENU_COMBOBOX_ARROW "../assets/images/menu/dropdown_arrow.png"
#define PATH_MAIN_MENU_SETTINGS_BACKGROUND "../assets/images/menu/settings_background.png"
#define PATH_MAIN_MENU_SLIDER_BAR "../assets/images/menu/slider_bar.png"
#define PATH_MAIN_MENU_SLIDER_HANDLE "../assets/images/menu/slider_handle.png"


/////////////MUSIC & SOUND EFFECT/////////////////
//Main menu music
#define PATH_MAIN_MENU_MUSIC "../assets/musics/menu_music.mp3"
#define PATH_MAIN_MENU_BUTTON_SOUND "../assets/sounds_effects/button.wav"

//Game music
#define PATH_GAME_MUSIC "../assets/musics/game_music.mp3"
#define PATH_GAME_MUSIC2 "../assets/musics/game_music2.mp3"


//Font
#define PATH_JERSEY10_FONT "../assets/fonts/Jersey10-Regular.ttf"

//Bat
#define PATH_BAT_ATTACK "../assets/images/bat/Bat-Attack1.png"
#define PATH_BAT_MOVE "../assets/images/bat/Bat-IdleFly.png"
#define PATH_BAT_DEATH "../assets/images/bat/Bat-Die.png"
#define NB_FRAME_BAT_MOVE 9
#define ANIM_SPEED_BAT_MOVE 70
#define NB_FRAME_BAT_DEATH 12
#define ANIM_SPEED_BAT_DEATH 100
#define NB_FRAME_BAT_ATTACK 8
#define ANIM_SPEED_BAT_ATTACK 100

#define PATH_BAT_DEATH_SOUND "../assets/sounds_effects/batDie.wav"
#define PATH_BAT_HIT_SOUND "../assets/sounds_effects/hitBat.wav"

#define BAT_HP 5


//Goblin
#define PATH_GOBLIN_ATTACK "../assets/images/goblin/enemy1_attack.png"
#define PATH_GOBLIN_MOVE "../assets/images/goblin/enemy1_moving.png"
#define PATH_GOBLIN_IDLE "../assets/images/goblin/enemy1_idle.png"
#define PATH_GOBLIN_PROJECTILE_MOVE "../assets/images/goblin/enemy1_missile.gif"
#define PATH_GOBLIN_PROJECTILE_BLOW "../assets/images/goblin/enemy1_missile_blow.gif"
#define NB_FRAME_GOBLIN_MOVE 4
#define ANIM_SPEED_GOBLIN_MOVE 100
#define NB_FRAME_GOBLIN_IDLE 6
#define ANIM_SPEED_GOBLIN_IDLE 325
#define NB_FRAME_GOBLIN_ATTACK 6
#define ANIM_SPEED_GOBLIN_ATTACK 100

#define PATH_GOBLIN_DEATH_SOUND "../assets/sounds_effects/minionDie.wav"
#define PATH_GOBLIN_HIT_SOUND "../assets/sounds_effects/hitGoblin.wav"
#define PATH_GOBLIN_PROJECTILE_MOVE_SOUND "../assets/sounds_effects/minionMissile.wav"

#define GOBLIN_HP 10


//Crystal Knight
#define PATH_CK_IDLE "../assets/images/crystalKnight/boss_idle.png"
#define NB_FRAME_CK_IDLE 4
#define ANIM_SPEED_CK_IDLE 100
#define PATH_CK_MOVE "../assets/images/crystalKnight/boss_teleport.png"
#define NB_FRAME_CK_MOVE 16
#define ANIM_SPEED_CK_MOVE 100
#define PATH_CK_ATTACK_LEFT "../assets/images/crystalKnight/boss_attackLeft.png"
#define PATH_CK_ATTACK_RIGHT "../assets/images/crystalKnight/boss_attackRight.png"
#define NB_FRAME_CK_ATTACK_LR 6
#define ANIM_SPEED_CK_ATTACK_LR 150
#define PATH_CK_THUNDER "../assets/images/crystalKnight/boss_thunder.png"
#define NB_FRAME_CK_THUNDER 10
#define ANIM_SPEED_CK_THUNDER 150
#define PATH_CK_DEATH "../assets/images/crystalKnight/boss_die.png"
#define NB_FRAME_CK_DEATH 10
#define ANIM_SPEED_CK_DEATH 150

#define PATH_MISSILE_SPELL_GROW_ANIMATION "../assets/images/characters/Missile_spellGrow.gif"

//Character
#define PATH_PLAYER_FRONT_IDLE "../assets/images/characters/Front_idle.png"
#define PATH_PLAYER_FRONT_WALK "../assets/images/characters/Front_walk.png"
#define PATH_PLAYER_LEFT_IDLE "../assets/images/characters/Left_idle.png"
#define PATH_PLAYER_LEFT_WALK "../assets/images/characters/Left_walk.png"
#define PATH_PLAYER_RIGHT_IDLE "../assets/images/characters/Right_idle.png"
#define PATH_PLAYER_RIGHT_WALK "../assets/images/characters/Right_walk.png"
#define PATH_PLAYER_BACK_IDLE "../assets/images/characters/Back_idle.png"
#define PATH_PLAYER_BACK_WALK "../assets/images/characters/Back_walk.png"
#define PATH_PLAYER_DEATH "../assets/images/characters/die.png"
#define ANIM_SPEED_PLAYER_IDLE 100
#define NB_FRAME_PLAYER_DIE 8

#define PATH_PLAYER_DEATH_SOUND "../assets/sounds_effects/playerDie.wav"
#define PATH_PLAYER_HIT_SOUND "../assets/sounds_effects/hitPlayer.wav"
#define PATH_PLAYER_PROJECTILE_MOVE_SOUND "../assets/sounds_effects/playerMissile.wav"



//HUD
#define PATH_HP_ANIMATION "../assets/images/characters/Fire_head.gif"
#define PATH_HP_DEAD_ANIMATION "../assets/images/characters/Fireless_head.gif"
#define PATH_HP_DEAD "../assets/images/characters/Fire_head.png"

//Other
#define DEFAULT_WINDOW_HEIGHT 720
#define LIST_WINDOW_HEIGHT {1440, 1080, 720, 480}




//PLayer animation and direction
enum Direction {None, Up, Down, Left, Right};




//Projectile

#define PATH_PLAYER_PROJECTILE "../assets/images/characters/Missile_spell.gif"
#define PATH_PLAYER_PROJECTILE_GROW "../assets/images/characters/Missile_spellGrow.gif"
#define PATH_PLAYER_PROJECTILE_FADE "../assets/images/characters/Missile_spellFade.gif"

//Slash attack
#define PATH_PLAYER_SLASH1 "../assets/images/characters/Slash1.gif"
#define PATH_PLAYER_SLASH2 "../assets/images/characters/Slash2.gif"
#define PATH_PLAYER_SLASH3 "../assets/images/characters/Slash3.gif"

#define PATH_PLAYER_SLASH_SOUND "../assets/sounds_effects/firstSlashAttack.wav"
#define PATH_PLAYER_LAST_SLASH_SOUND "../assets/sounds_effects/thirdSlashAttack.wav"

//Slash
#define PATH_PLAYER_SHIELD "../assets/images/characters/Shield.gif"
//HUD

#define PATH_SPELL_BOX "../assets/images/characters/SpellBox.png"
#define PATH_SPELL_BOX_SELECTED "../assets/images/characters/SpellBoxSelected.png"
#define PATH_MISSILE_SPELL_ICONE "../assets/images/characters/MissileSpell_icone.png"
#define PATH_MISSILE_SLASH_ICONE "../assets/images/characters/SlashSpell_icone.png"
#define PATH_MISSILE_SHIELD_ICONE "../assets/images/characters/Shield_icone.png"
#endif //PROJET_CPP_CONSTANTS_H
