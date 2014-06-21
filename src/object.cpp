/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * object.cpp - FableofZelma application. This class contains the
 * enumerated types and class declaration for the main Object class.
 *
 *
 * NOTES:
 * 4/12/14 by JAZ::Design created.
 *****************************************************************************/

#include "fableofzelma.hpp"
#include "resources.hpp"

#define GLOBALHEIGHT 59.0
#define GLOBALWIDTH 59.0
#define RUPEE_START 22
#define RUPEE_END 25

namespace foz {

    void Object::setDirection(uint8_t dir) {
        direction =dir;
    }

    Object::Object(uint8_t myType, float myX, float myY)
    {
        type = myType;
        width = GLOBALWIDTH;
        height = GLOBALHEIGHT;
        active = true;
        x = myX;
        y = myY;
        depth = 3;
        texfile = TEX_BASIC_ROOM;
        sprite = type;
        active = true;
        if((type>12 && type<RUPEE_GREEN_1) || type >KEY) {

            sprite = type;
            if (type == VOID_BLOCK) {
                status = TRANSPARENT;
            }else {
                status = SOLID;
            }
        }else if (type>=RUPEE_GREEN_1 &&  type<=RUPEE_RED_3) {
            status = TRANSPARENT;
            sprite += rand() % 3;
        }else if (type==KEY) {
            status = TRANSPARENT;
        }else {
            sprite = type;
            status = SOLID;
        }
    }

    void Object::setRoomXY(uint8_t roomx, uint8_t roomy) {
        room_x = roomx;
        room_y = roomy;
    }

    void Object::draw(){
        if (!active) {
            //delete this;
            return;
        }

        if (type == ARROW_EAST || type == ARROW_WEST || type == ARROW_NORTH || type == ARROW_SOUTH) {
                    if (this->y <= -328.0) {
                        if (this->direction == DIRECTION_SOUTH) {
                                this->active = false;
                                myGame->playSound(SFX_LINKARROW,100,true);
                        }
                    }
                    if (this->y >= 266.0) {
                        if (this->direction == DIRECTION_NORTH) {
                             this->active = false;
                             myGame->playSound(SFX_LINKARROW,100,true);
                        }
                    }
                    if (this->x >= 270.0) {
                        if (this->direction == DIRECTION_EAST) {
                                this->active = false;
                                myGame->playSound(SFX_LINKARROW,100,true);
                        }
                    }
                    if (this->x <= -321.0) {
                        if (this->direction == DIRECTION_WEST) {
                                this->active = false;
                                myGame->playSound(SFX_LINKARROW,100,true);
                        }
                    }

                x += 3.0*direction_Modifier[direction][0];
                y += 3.0*direction_Modifier[direction][1];
        }

        float texCoords[6];

        getTexCoords(texfile, sprite, texCoords);
        glBindTexture(GL_TEXTURE_2D, myGame->myTextures[texfile].texHandle);

        glBegin(GL_QUADS);

                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(x, y, depth);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(x + width, y, depth);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(x + width, y + height, depth);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(x, y + height, depth);

        glEnd();
        if (type >= RUPEE_GREEN_1 && type <=RUPEE_RED_3) {
            if (frameCount%18 == 3) {
                sprite++;
                if (sprite>=type+3) {
                    sprite = type;
                }
            }
            if (frameCount>FRAME_RATE) {
                frameCount = 0;
            }else {
                frameCount++;
            }
        }
        return;

    }

    Object::~Object()
    {

    }

}
