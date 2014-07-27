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

    Object::Object(uint8_t myType, float myX, float myY, int16_t mySubject){
        if (mySubject<0) {
            active = false;
            subject = -mySubject;
        }else{
            active = true;
            subject = mySubject;
        }
        init(myType,myX,myY);
    }

    Object::Object(uint8_t myType, float myX, float myY){
        subject = 0;
        active = true;
        init(myType,myX,myY);
    }

    void Object::init(uint8_t myType, float myX, float myY){
        type = myType;
        width = GLOBALWIDTH;
        height = GLOBALHEIGHT;
        x = myX;
        y = myY;
        depth = 3;
        texfile = TEX_BASIC_ROOM;
        sprite = type;
        frameCount = 0;

        if ((type >=ARROW_NORTH && type <= ARROW_WEST)|| (type >=FIREBALL_NORTH && type<=FIREBALL_WEST) || (type >BOMB_1 && type <= BOMB_9)){
            can_move = true;
        }else {
            can_move = false;
        }


        if((type>12 && type<RUPEE_GREEN_1) || type >KEY ) {
            sprite = type;
            if (type == VOID_BLOCK || type==BUTTON || type == BOMB_1) {
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

    void Object::draw(){
        if (!active) {
            //delete this;
            return;
        }

        if ( (type >= ARROW_NORTH && type <= ARROW_WEST) || (type >= FIREBALL_NORTH && type <= FIREBALL_WEST) || (type>= BOMB_1 && type<=BOMB_9)) {
                    double speed = 3.0;
                    if (sprite >= BOMB_9 && frameCount%10==0){
                        this->active = false;
                    }
                    if (type>=BOMB_1 && type <= BOMB_9) {
                        frameCount++;
                        if (sprite < BOMB_2){
                            //status = TRANSPARENT;
                            if (frameCount%120 == 0){
                                sprite++;
                            }
                            if (direction_Modifier[direction][1] == 0){
                                y += .2 * (-frameCount/10 + 6);
                            }
                            speed = 1.5;
                        }else if (sprite < BOMB_3){
                            if (frameCount%40 ==0) {
                                sprite ++;
                            }
                            if (speed >0) {
                                speed =1.5 - frameCount/100 ;
                            }else {
                                speed=0;
                            }
                        }else {
                             //status = SOLID;
                            if (frameCount%10 == 0) {
                                height += 15;
                                width += 15;
                                x -= 7.5;
                                y -= 7.5;
                                sprite++;
                            }
                            if (sprite>BOMB_9){
                                this->active = false;
                            }
                            speed = 0;
                        }

                    }

                    if (this->y <= -328.0) {
                        if (this->direction == DIRECTION_SOUTH) {
                                if (this->sprite >= BOMB_1 && this->sprite <= BOMB_9){
                                   speed = 0;
                                } else {
                                    this->active = false;
                                }

                                if (type>=ARROW_NORTH && type <=ARROW_WEST) {
                                    myGame->playSound(SFX_LINKARROW,100,true);
                                }else if (type>=ARROW_NORTH && type <=ARROW_WEST){
                                    myGame->playSound(SFX_FIREBALL,100,true);
                                }
                        }
                    }
                    if (this->y >= 266.0) {
                        if (this->direction == DIRECTION_NORTH) {
                                if (this->sprite >= BOMB_1 && this->sprite <= BOMB_9){
                                   speed = 0;
                                } else {
                                    this->active = false;
                                }
                                if (type>=ARROW_NORTH && type <=ARROW_WEST) {
                                    myGame->playSound(SFX_LINKARROW,100,true);
                                }else if (type>=ARROW_NORTH && type <=ARROW_WEST){
                                    myGame->playSound(SFX_FIREBALL,100,true);
                                }
                        }
                    }
                    if (this->x >= 270.0) {
                        if (this->direction == DIRECTION_EAST) {
                                if (this->sprite >= BOMB_1 && this->sprite <= BOMB_9){
                                   speed = 0;
                                } else {
                                    this->active = false;
                                }
                                if (type>=ARROW_NORTH && type <=ARROW_WEST) {
                                    myGame->playSound(SFX_LINKARROW,100,true);
                                }else if (type>=ARROW_NORTH && type <=ARROW_WEST){
                                    myGame->playSound(SFX_FIREBALL,100,true);
                                }
                        }
                    }
                    if (this->x <= -321.0) {
                        if (this->direction == DIRECTION_WEST) {
                                if (this->sprite >= BOMB_1 && this->sprite <= BOMB_9){
                                   speed = 0;
                                } else {
                                    this->active = false;
                                }
                                if (type>=ARROW_NORTH && type <=ARROW_WEST) {
                                    myGame->playSound(SFX_LINKARROW,100,true);
                                }else if (type>=ARROW_NORTH && type <=ARROW_WEST){
                                    myGame->playSound(SFX_FIREBALL,100,true);
                                }
                        }
                    }
                if (can_move){
                    x += speed*direction_Modifier[direction][0];
                    y += speed*direction_Modifier[direction][1];
                }
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
