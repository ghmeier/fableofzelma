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



/*Sound effect file enum*/
typedef enum {SFX_BIGITEM, SFX_SMALLITEM, SFX_LINKDIES1, SFX_LINKDIES2, NUM_SFX};
extern std::string sfxFiles[NUM_SFX];

/*Music file enum*/
typedef enum {MUSIC_MAIN, MUSIC_ANNIVERSARY, NUM_MUSIC};
extern std::string musicFiles[NUM_MUSIC];

 /* Texture file enum */
typedef enum {TEX_BASIC_ROOM=0, BLUE_LINK, GREEN_LINK, PURPLE_LINK, RED_LINK, TIER_2_MAP, NUM_TEXTURES} TEXTURE_ENUM;
extern std::string texFiles[NUM_TEXTURES];

/* Sprite enum for LINK*/
typedef enum
{LINK_SLASH_NORTH_1 = 0, LINK_SLASH_NORTH_2, LINK_SLASH_NORTH_3, LINK_SLASH_NORTH_4, LINK_SLASH_NORTH_5,                                                                        //1
LINK_SLASH_NORTH_6, LINK_SLASH_NORTH_7, LINK_SLASH_NORTH_8, LINK_SLASH_NORTH_10, LINK_SLASH_NORTH_11,                                                                           //2
LINK_SLASH_NORTH_12, LINK_SLASH_NORTH_13, LINK_SLASH_NORTH_14, LINK_SLASH_NORTH_15, LINK_SLASH_NORTH_16,                                                                        //3
LINK_SLASH_NORTH_17, LINK_SLASH_NORTH_18, LINK_SLASH_NORTH_19, LINK_SLASH_NORTH_20, LINK_SLASH_NORTH_21,                                                                        //4
LINK_SLASH_WEST_1, LINK_SLASH_WEST_2, LINK_SLASH_WEST_3, LINK_SLASH_WEST_4, LINK_SLASH_WEST_5,                                                                                  //5
LINK_SLASH_WEST_6, LINK_SLASH_WEST_7, LINK_SLASH_WEST_8, LINK_SLASH_WEST_10, LINK_SLASH_WEST_11,                                                                                //6
LINK_SLASH_WEST_12, LINK_SLASH_WEST_13, LINK_SLASH_WEST_14, LINK_SLASH_WEST_15, LINK_SLASH_WEST_16,                                                                             //7
LINK_SLASH_WEST_17, LINK_SLASH_WEST_18, LINK_SLASH_WEST_19, LINK_SLASH_WEST_20, LINK_SLASH_WEST_21,                                                                             //8
LINK_SLASH_SOUTH_1, LINK_SLASH_SOUTH_2, LINK_SLASH_SOUTH_3, LINK_SLASH_SOUTH_4, LINK_SLASH_SOUTH_5,                                                                             //9
LINK_SLASH_SOUTH_6, LINK_SLASH_SOUTH_7, LINK_SLASH_SOUTH_8, LINK_SLASH_SOUTH_10, LINK_SLASH_SOUTH_11,                                                                           //10
LINK_SLASH_SOUTH_12, LINK_SLASH_SOUTH_13, LINK_SLASH_SOUTH_14, LINK_SLASH_SOUTH_15, LINK_SLASH_SOUTH_16,                                                                        //11
LINK_SLASH_SOUTH_17, LINK_SLASH_SOUTH_18, LINK_SLASH_SOUTH_19, LINK_SLASH_SOUTH_20, LINK_SLASH_SOUTH_21,                                                                        //12
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
WALL_BOTTOM_LEFT, WALL_BOTTOM, WALL_BOTTOM_RIGHT, WHOLE_ROOM}                                                                                                                   //3
ROOM_SPRITE_ENUM ;
extern uint16_t room_object_spriteMap[][4];
