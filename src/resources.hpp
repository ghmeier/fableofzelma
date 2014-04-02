/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * fableofzelma.hpp - The Fable of Zelda application. Takes as input move files
 * and an optional map file and runs a Legend of Zelda-style simulation
 * for up to four teams.
 *
 *
 * NOTES:
 * 1/24/14 by JAZ::Design created.
 *****************************************************************************/

#pragma once

/* Predicate types and spellings */
#define NUM_PRED_SPELLINGS 2
typedef enum {ALWAYS_PRED=0, NEVER_PRED, READY_PRED, ALIVE_PRED, DEAD_PRED, DAMAGE_PRED, EAT_PRED, HIT_PRED, SCORE_PRED, WINNING_PRED, LOSING_PRED, EMPTY_PRED, PASSED_PRED, NUM_PRED_TYPES} PRED_TYPE;
extern std::string predNames[NUM_PRED_TYPES][NUM_PRED_SPELLINGS];

/* Sound effect file enum */
typedef enum {SFX_BIGITEM=0, SFX_SMALLITEM, SFX_LINKDIES1, SFX_LINKDIES2, NUM_SFX} SFX_ENUM;
extern std::string sfxFiles[NUM_SFX];

/* Music file enum */
typedef enum {MUSIC_MAIN=0, MUSIC_ANNIVERSARY, NUM_MUSIC} MUSIC_ENUM;
extern std::string musicFiles[NUM_MUSIC];

 /* Texture file enum */
typedef enum {TEX_BASIC_ROOM=0, TEX_BLUE_LINK, TEX_GREEN_LINK, TEX_PURPLE_LINK, TEX_RED_LINK, NUM_TEXTURES} TEXTURE_ENUM;
extern std::string texFiles[NUM_TEXTURES];

/* Depth enum */
typedef enum {FRONT_DEPTH=0, FONT_DEPTH=60000, OVERLAY_DEPTH=60001, CORNER_DEPTH, WALL_DEPTH, FLOOR_DEPTH, BACK_DEPTH} DEPTH_ENUM;

/* Camera state enum */
typedef enum {CAMERA_INIT=0, CAMERA_PAN_LEFT, CAMERA_PAN_RIGHT, CAMERA_PAN_UP, CAMERA_PAN_DOWN, CAMERA_ZOOM_IN, CAMERA_ZOOM_OUT, CAMERA_IDLE} CAMERA_ENUM;

/* Sprite enum for LINK objects */
typedef enum
{LINK_SLASH_NORTH_1 = 0, LINK_SLASH_NORTH_2, LINK_SLASH_NORTH_3, LINK_SLASH_NORTH_4, LINK_SLASH_NORTH_5,                                                                        //1
LINK_SLASH_NORTH_6, LINK_SLASH_NORTH_7, LINK_SLASH_NORTH_8, LINK_SLASH_NORTH_9, LINK_SLASH_NORTH_10,                                                                            //2
LINK_SLASH_NORTH_11, LINK_SLASH_NORTH_12, LINK_SLASH_NORTH_13, LINK_SLASH_NORTH_14, LINK_SLASH_NORTH_15,                                                                         //3
LINK_SLASH_NORTH_16, LINK_SLASH_NORTH_17, LINK_SLASH_NORTH_18, LINK_SLASH_NORTH_19, LINK_SLASH_NORTH_20,                                                                       //4
LINK_SLASH_WEST_1, LINK_SLASH_WEST_2, LINK_SLASH_WEST_3, LINK_SLASH_WEST_4, LINK_SLASH_WEST_5,                                                                                  //5
LINK_SLASH_WEST_6, LINK_SLASH_WEST_7, LINK_SLASH_WEST_8, LINK_SLASH_WEST_9, LINK_SLASH_WEST_10,                                                                                 //6
LINK_SLASH_WEST_11, LINK_SLASH_WEST_12, LINK_SLASH_WEST_13, LINK_SLASH_WEST_14, LINK_SLASH_WEST_15,                                                                           //7
LINK_SLASH_WEST_16, LINK_SLASH_WEST_17, LINK_SLASH_WEST_18, LINK_SLASH_WEST_19, LINK_SLASH_WEST_20,                                                                            //8
LINK_SLASH_SOUTH_1, LINK_SLASH_SOUTH_2, LINK_SLASH_SOUTH_3, LINK_SLASH_SOUTH_4, LINK_SLASH_SOUTH_5,                                                                             //9
LINK_SLASH_SOUTH_6, LINK_SLASH_SOUTH_7, LINK_SLASH_SOUTH_8, LINK_SLASH_SOUTH_9,   LINK_SLASH_SOUTH_10,                                                                            //10
LINK_SLASH_SOUTH_11, LINK_SLASH_SOUTH_12, LINK_SLASH_SOUTH_13, LINK_SLASH_SOUTH_14, LINK_SLASH_SOUTH_15,                                                                         //11
LINK_SLASH_SOUTH_16, LINK_SLASH_SOUTH_17, LINK_SLASH_SOUTH_18, LINK_SLASH_SOUTH_19, LINK_SLASH_SOUTH_20,                                                                       //12
LINK_WALKING_NORTH_1, LINK_WALKING_NORTH_2, LINK_WALKING_NORTH_3, LINK_WALKING_NORTH_4, LINK_WALKING_NORTH_5,                                                                   //13
LINK_WALKING_NORTH_6,                                                                                                                                                           //14
LINK_WALKING_WEST_1, LINK_WALKING_WEST_2, LINK_WALKING_WEST_3, LINK_WALKING_WEST_4, LINK_WALKING_WEST_5,                                                                        //15
LINK_WALKING_WEST_6,                                                                                                                                                            //16
LINK_WALKING_SOUTH_1, LINK_WALKING_SOUTH_2, LINK_WALKING_SOUTH_3, LINK_WALKING_SOUTH_4, LINK_WALKING_SOUTH_5,                                                                   //17
LINK_WALKING_SOUTH_6,                                                                                                                                                           //18
LINK_DYING_1, LINK_DYING_2, LINK_DYING_3, LINK_DYING_4, LINK_DYING_5,                                                                                                           //19
LINK_DYING_6}                                                                                                                                                                   //20
LINK_SPRITE_ENUM;
extern uint16_t link_object_spriteMap[][4];

/*Sprite enum for ROOM*/
typedef enum
{WALL_TOP_LEFT, WALL_TOP_CENTER, WALL_TOP_RIGHT, DOOR_LEFT, DOOR_TOP,                                                                                                           //1
DOOR_RIGHT, WALL_LEFT, FLOOR_TILE, WALL_RIGHT, DOOR_BOTTOM,                                                                                                                     //2
WALL_BOTTOM_LEFT, WALL_BOTTOM, WALL_BOTTOM_RIGHT, CHEST, LETTER_A, LETTER_B}                                                                                                                   //3
ROOM_SPRITE_ENUM ;
extern uint16_t room_object_spriteMap[][4];
