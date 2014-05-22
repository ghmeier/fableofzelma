/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * links.cpp - FableofZelma application. This class contains the
 * enumerated types and class declaration for the main Link class.
 *
 *
 * NOTES:
 * 4/12/14 by JAZ::Design created.
 *****************************************************************************/


#include "fableofzelma.hpp"
#define GLOBALHEIGHT 64.0
#define GLOBALWIDTH 58.0

std::string linkNames[NUM_LINK_TYPE][NUM_LINK_SPELLINGS] = {
    {"regular", "link", "normal", "default"}
    };
/* Link costs */
uint16_t linkCosts[NUM_LINK_TYPE] = {1};
/* Link healths */
int16_t linkHealths[NUM_LINK_TYPE] = {1};
/* Link speeds. This variable is link-specific */
uint16_t linkSpeeds[NUM_LINK_TYPE] = {1};


namespace foz {

    /*****************************************************************************
    * Function: Link::update
    * Description: Updates the link sprite based on command type
    *****************************************************************************/
    void Link::update(uint8_t cmd) {
        uint16_t tempSprite;
        switch (cmd) {

            case MOVE_CMD:
                height = GLOBALHEIGHT;
                width = GLOBALWIDTH;
                if (direction == DIRECTION_NORTH) {
                    if (sprite >= LINK_WALKING_NORTH_6) {
                        sprite = LINK_WALKING_NORTH_1;
                    }
                    else if (sprite < LINK_WALKING_NORTH_1) {
                        sprite = LINK_WALKING_NORTH_1;
                    }
                    else {
                        sprite++;
                    }
                }
                if (direction == DIRECTION_SOUTH) {
                    if (sprite >= LINK_WALKING_SOUTH_6) {
                        sprite = LINK_WALKING_SOUTH_1;
                    }
                    else if (sprite < LINK_WALKING_SOUTH_1) {
                        sprite = LINK_WALKING_SOUTH_1;
                    }
                    else {
                        sprite++;
                    }
                }
                if (direction == DIRECTION_WEST) {
                    if (sprite >= LINK_WALKING_WEST_6) {
                        sprite = LINK_WALKING_WEST_1;
                    }
                    else if (sprite < LINK_WALKING_WEST_1) {
                        sprite = LINK_WALKING_WEST_1;
                    }
                    else {
                        sprite++;
                    }

                }
                if (direction == DIRECTION_EAST) {
                    if (sprite >= LINK_WALKING_WEST_6) {
                        sprite = LINK_WALKING_WEST_1;
                    }
                    else if (sprite < LINK_WALKING_WEST_1) {
                        sprite = LINK_WALKING_WEST_1;
                    }
                    else {
                        sprite++;
                    }
                }
                x = x + speed*direction_Modifier[direction][0];
                y = y + speed*direction_Modifier[direction][1];

                break;


            case ATTACK_CMD:

                if (direction == DIRECTION_NORTH) {
                    if (sprite >= LINK_SLASH_NORTH_20){
                        sprite = LINK_SLASH_NORTH_1;


                    }
                    else if (sprite < LINK_SLASH_NORTH_1) {
                        sprite = LINK_SLASH_NORTH_1;

                    }
                    else {
                        sprite++;
                        x =  x + 3.05*(-(link_object_spriteMap[sprite - 1][0] - link_object_spriteMap_centers[sprite - 1][0]) + (link_object_spriteMap[sprite][0] - link_object_spriteMap_centers[sprite][0]));
                        y =  y +(-(link_object_spriteMap[sprite - 1][1] - link_object_spriteMap_centers[sprite - 1][1]) + (link_object_spriteMap[sprite][1] - link_object_spriteMap_centers[sprite][1]));
                    }
                width = 3.05*(link_object_spriteMap[sprite][2] - link_object_spriteMap[sprite][0]);
                height = 3.05*(link_object_spriteMap[sprite][3] - link_object_spriteMap[sprite][1]);

                }
                if (direction == DIRECTION_SOUTH) {
                        tempSprite = sprite;
                    if (sprite >= LINK_SLASH_SOUTH_20){
                        sprite = LINK_SLASH_SOUTH_1;
                        //x =  x + 3.05*(-(link_object_spriteMap[tempSprite][0] - link_object_spriteMap_centers[tempSprite][0]) + (link_object_spriteMap[sprite][0] - link_object_spriteMap_centers[sprite][0]));
                        //y =  y +3.2*(-(link_object_spriteMap[tempSprite][1] - link_object_spriteMap_centers[tempSprite][1]) + (link_object_spriteMap[sprite][1] - link_object_spriteMap_centers[sprite][1]));
                    }
                    else if (sprite < LINK_SLASH_SOUTH_1) {
                        sprite = LINK_SLASH_SOUTH_1;
                        //x =  x + 3.05*(-(link_object_spriteMap[tempSprite][0] - link_object_spriteMap_centers[tempSprite][0]) + (link_object_spriteMap[sprite][0] - link_object_spriteMap_centers[sprite][0]));
                        //y =  y +3.2*(-(link_object_spriteMap[tempSprite][1] - link_object_spriteMap_centers[tempSprite][1]) + (link_object_spriteMap[sprite][1] - link_object_spriteMap_centers[sprite][1]));
                    }
                    else {
                        sprite++;
                        x =  x + 3.05*(-(link_object_spriteMap[sprite - 1][0] - link_object_spriteMap_centers[sprite - 1][0]) + (link_object_spriteMap[sprite][0] - link_object_spriteMap_centers[sprite][0]));
                        y =  y + 3.05*(-(-link_object_spriteMap[sprite - 1][3] + link_object_spriteMap_centers[sprite - 1][1]) + (-link_object_spriteMap[sprite][3] + link_object_spriteMap_centers[sprite][1]));



                    }

                width = 3.05*(link_object_spriteMap[sprite][2] - link_object_spriteMap[sprite][0]);
                height = 3.05*(link_object_spriteMap[sprite][3] - link_object_spriteMap[sprite][1]);

                }
                if (direction == DIRECTION_WEST) {
                    if (sprite >= LINK_SLASH_WEST_20){
                        sprite = LINK_SLASH_WEST_1;
                    }
                    else if (sprite < LINK_SLASH_WEST_1) {
                        sprite = LINK_SLASH_WEST_1;
                    }
                    else {
                        sprite++;
                        x =  x + 3.05*(-(link_object_spriteMap[sprite - 1][0] - link_object_spriteMap_centers[sprite - 1][0]) + (link_object_spriteMap[sprite][0] - link_object_spriteMap_centers[sprite][0]));
                        y =  y +3.05*(-(-link_object_spriteMap[sprite - 1][3] + link_object_spriteMap_centers[sprite - 1][1]) + (-link_object_spriteMap[sprite][3] + link_object_spriteMap_centers[sprite][1]));
                    }
                width = 3.05*(link_object_spriteMap[sprite][2] - link_object_spriteMap[sprite][0]);
                height = 3.05*(link_object_spriteMap[sprite][3] - link_object_spriteMap[sprite][1]);

                }

                if (direction == DIRECTION_EAST) {
                    if (sprite >= LINK_SLASH_WEST_20){
                        sprite = LINK_SLASH_WEST_1;
                    }
                    else if (sprite < LINK_SLASH_WEST_1) {
                        sprite = LINK_SLASH_WEST_1;
                    }
                    else {
                        sprite++;
                        x =  x + 3.05*(-(-link_object_spriteMap[sprite - 1][2] + link_object_spriteMap_centers[sprite - 1][0]) + (-link_object_spriteMap[sprite][2] + link_object_spriteMap_centers[sprite][0]));
                        y =  y +3.05*(-(-link_object_spriteMap[sprite - 1][3] + link_object_spriteMap_centers[sprite - 1][1]) + (-link_object_spriteMap[sprite][3] + link_object_spriteMap_centers[sprite][1]));
                    }
                width = 3.05*(link_object_spriteMap[sprite][2] - link_object_spriteMap[sprite][0]);
                height = 3.05*(link_object_spriteMap[sprite][3] - link_object_spriteMap[sprite][1]);

                    }


                break;

        case LEFT_CMD:
            height = GLOBALHEIGHT;
            width = GLOBALWIDTH;
            if (direction == DIRECTION_NORTH) {
                direction = DIRECTION_WEST;
                sprite = LINK_WALKING_WEST_1;
            }
            else if (direction == DIRECTION_SOUTH) {
                direction = DIRECTION_EAST;
                sprite = LINK_WALKING_WEST_1;
            }
            else if (direction == DIRECTION_WEST) {
                direction = DIRECTION_SOUTH;
                sprite = LINK_WALKING_SOUTH_1;
            }
            else if (direction == DIRECTION_EAST) {
                direction = DIRECTION_NORTH;
                sprite = LINK_WALKING_NORTH_1;
            }
            break;

        case RIGHT_CMD:
            height = GLOBALHEIGHT;
            width = GLOBALWIDTH;
            if (direction == DIRECTION_NORTH) {
                direction = DIRECTION_EAST;
                sprite = LINK_WALKING_WEST_1;
            }
            else if (direction == DIRECTION_SOUTH) {
                direction = DIRECTION_WEST;
                sprite = LINK_WALKING_WEST_1;
            }
            else if (direction == DIRECTION_WEST) {
                direction = DIRECTION_NORTH;
                sprite = LINK_WALKING_NORTH_1;
            }
            else if (direction == DIRECTION_EAST) {
                direction = DIRECTION_SOUTH;
                sprite = LINK_WALKING_SOUTH_1;
            }
            break;
        case WAIT_CMD:
            height = GLOBALHEIGHT;
            width = GLOBALWIDTH;
            if (direction == DIRECTION_EAST) {
                sprite = LINK_WALKING_WEST_1;
            }else if (direction == DIRECTION_WEST) {
                sprite = LINK_WALKING_WEST_1;
            }else if (direction== DIRECTION_NORTH) {
                sprite = LINK_WALKING_NORTH_1;
            }else if (direction == DIRECTION_SOUTH) {
                sprite = LINK_WALKING_SOUTH_1;
            }
            break;
        }
    }





    /*****************************************************************************
    * Function: Link::Link
    * Description: Class constructor. Uses an enum type to set link-specific
    * parameters
    *****************************************************************************/
    Link::Link(uint8_t mytype, uint16_t myid, uint8_t myteam, uint8_t world_width, uint8_t world_height) {

        type = mytype;
        id = myid;
        team = myteam;

        height = GLOBALHEIGHT;
        width = GLOBALWIDTH;
        speed = 2.95;

        switch(team) {
            case 0:
            default:
                texfile = TEX_GREEN_LINK;
                room_x = 0;
                room_y = 0;
                x = -25.0;
                y = -83.0;
                sprite = LINK_WALKING_SOUTH_1;
                direction = DIRECTION_SOUTH;
                depth = FRONT_DEPTH;
                break;
            case 1:
                texfile = TEX_PURPLE_LINK;
                room_x = 0;
                room_y = world_height-1;
                x = -25;
                y = 83.0;
                sprite = LINK_WALKING_NORTH_1;
                direction = DIRECTION_NORTH;
                depth = FRONT_DEPTH;
                break;
            case 2:
                texfile = TEX_BLUE_LINK;
                room_x = world_width-1;
                room_y = 0;
                x = -25.0;
                y = 83.0;
                sprite = LINK_WALKING_SOUTH_1;
                direction = DIRECTION_SOUTH;
                depth = FRONT_DEPTH;

                break;
            case 3:
                texfile = TEX_RED_LINK;
                room_x = world_width-1;
                room_y = world_height-1;
                x = -25.0;
                y = 83.0;
                sprite = LINK_WALKING_NORTH_1;
                direction = DIRECTION_NORTH;
                depth = FRONT_DEPTH;
                break;
        }


    }

    /*****************************************************************************
    * Function: Link::draw
    * Description: Link drawing function.
    *****************************************************************************/
    void Link::draw(){

        float texCoords[6];

        getTexCoords(texfile, sprite, texCoords);
        glBindTexture(GL_TEXTURE_2D, myGame->myTextures[texfile].texHandle);

        glBegin(GL_QUADS);

            if (direction == DIRECTION_EAST) {
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(x, y, depth);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(x + width, y, depth);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(x + width, y + height, depth);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(x, y + height, depth);
            }

            else {
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(x, y, depth);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(x + width, y, depth);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(x + width, y + height, depth);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(x, y + height, depth);
            }

        glEnd();

        return;

    }

}
