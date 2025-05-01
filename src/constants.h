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

//Other
#define DEFAULT_WINDOW_HEIGHT 720
#define LIST_WINDOW_HEIGHT {1440, 1080, 720, 480}




//PLayer animation and direction
enum Direction {None, Up, Down, Left, Right};




//Projectile

#define PATH_PLAYER_PROJECTILE "../assets/images/characters/Missile_spell.gif"
#define PATH_PLAYER_PROJECTILE_GROW "../assets/images/characters/Missile_spellGrow.gif"
#define PATH_PLAYER_PROJECTILE_FADE "../assets/images/characters/Missile_spellFade.gif"

//HUD

#define PATH_SPELL_BOX "../assets/images/characters/SpellBox.png"
#define PATH_SPELL_BOX_SELECTED "../assets/images/characters/SpellBoxSelected.png"
#define PATH_MISSILE_SPELL_ICONE "../assets/images/characters/MissileSpell_icone.png"
#endif //PROJET_CPP_CONSTANTS_H
