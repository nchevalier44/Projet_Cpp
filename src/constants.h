#ifndef PROJET_CPP_CONSTANTS_H
#define PROJET_CPP_CONSTANTS_H

//Definition of all constants we use in the project

//Main menu images
#define PATH_MAIN_MENU_BACKGROUND "../assets/images/menu/background.png"

//Start menu
#define PATH_MAIN_MENU_BUTTON_BACKGROUND "../assets/images/menu/button_background.png"
#define PATH_MAIN_MENU_BUTTON_ANIMATION "../assets/images/menu/button_animation.gif"
#define PATH_MAIN_MENU_COMBOBOX_BACKGROUND "../assets/images/menu/dropdown_item.png"
#define PATH_MAIN_MENU_COMBOBOX_ARROW "../assets/images/menu/dropdown_arrow.png"
#define PATH_MAIN_MENU_SETTINGS_BACKGROUND "../assets/images/menu/settings_background.png"
#define PATH_MAIN_MENU_SLIDER_BAR "../assets/images/menu/slider_bar.png"
#define PATH_MAIN_MENU_SLIDER_HANDLE "../assets/images/menu/slider_handle.png"

//Main menu music
#define PATH_MAIN_MENU_MUSIC "../assets/musics/start_menu_theme.wav"
#define PATH_MAIN_MENU_BUTTON_SOUND "../assets/sounds_effects/button.wav"

//Font
#define PATH_JERSEY10_FONT "../assets/fonts/Jersey10-Regular.ttf"

//Bat
#define PATH_BAT_ATTACK "../assets/images/bat/Bat-Attack1.png"
#define PATH_BAT_MOVE "../assets/images/bat/Bat-IdleFly.png"
#define PATH_BAT_DEATH "../assets/images/bat/Bat-Die.png"
#define NB_FRAME_BAT_MOVE 9
#define ANIM_SPEED_BAT_MOVE 90


#define PATH_MISSILE_SPELL_GROW_ANIMATION "../assets/images/characters/Missile_spellGrow.gif"

//Character
#define PATH_PLAYER_FOOTSTEP_SOUND "../assets/sounds_effects/footsteps.wav"

#define PATH_PLAYER_FRONT_IDLE "../assets/images/characters/Front_idle.png"
#define PATH_PLAYER_FRONT_WALK "../assets/images/characters/Front_walk.png"
#define PATH_PLAYER_LEFT_IDLE "../assets/images/characters/Left_idle.png"
#define PATH_PLAYER_LEFT_WALK "../assets/images/characters/Left_walk.png"
#define PATH_PLAYER_RIGHT_IDLE "../assets/images/characters/Right_idle.png"
#define PATH_PLAYER_RIGHT_WALK "../assets/images/characters/Right_walk.png"
#define PATH_PLAYER_BACK_IDLE "../assets/images/characters/Back_idle.png"
#define PATH_PLAYER_BACK_WALK "../assets/images/characters/Back_walk.png"

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

//HUD

#define PATH_SPELL_BOX "../assets/images/characters/SpellBox.png"
#define PATH_SPELL_BOX_SELECTED "../assets/images/characters/SpellBoxSelected.png"
#define PATH_MISSILE_SPELL_ICONE "../assets/images/characters/MissileSpell_icone.png"
#define PATH_MISSILE_SLASH_ICONE "../assets/images/characters/SlashSpell_icone.png"
#define PATH_MISSILE_SHIELD_ICONE "../assets/images/characters/Shield_icone.png"
#endif //PROJET_CPP_CONSTANTS_H
