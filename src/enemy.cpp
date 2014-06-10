/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * fableofzelma.cpp - The Fable of Zelda application. Takes as input move files
 * and an optional map file and runs a Legend of Zelda-style simulation
 * for up to four teams.
 *
 *
 * NOTES:
 * 1/24/14 by JAZ::Design created.
 *****************************************************************************/


#include "fableofzelma.hpp"
#define GLOBALWIDTH 58.0
#define GLOBALHEIGHT 64.0

namespace foz {

    Enemy::Enemy(uint16_t mytype, float myx, float myy) : Link::Link(mytype, myx, myy){
        cmdIter = 0;
        frameCount = 0;
        cmds_done = false;
        cur_cmdopt = 0;
        cur_cmdframe = 0;
        switch (type) {
            case BSKEL:
                cmdIter;
                health = 20;
                texfile = TEX_ENEMIES;
                sprite = BSKEL_SOUTH_1;
                direction = DIRECTION_SOUTH;
                height = GLOBALHEIGHT;
                width = GLOBALWIDTH;
                break;
            case RSKEL:
                cmdIter;
                health = 20;
                texfile = TEX_ENEMIES;
                sprite = RSKEL_SOUTH_1;
                direction = DIRECTION_SOUTH;
                height = GLOBALHEIGHT;
                width = GLOBALWIDTH;
                break;
            default:
                break;
        }
    };

    void Enemy::update(uint8_t cmd){

        if (health<=0) {
            delete this;
        }
                if ( frameCount>FRAME_RATE) {
                    frameCount = 0;
                }else {
                    frameCount++;
                }
        switch (cmd) {
            case MOVE_CMD:
                if (frameCount%8==3) {
                    this->sprite++;
                    if (this->type == BSKEL) {
                        if (this->sprite>=(this->direction)*6+3)
                            this->sprite = this->direction*6;
                    }else  if (this->type == RSKEL) {
                        if (this->sprite>=(this->direction)*6+ 3 + RSKEL_NORTH_1)
                            this->sprite = this->direction*6 + RSKEL_NORTH_1;
                    }
                }
                this->x = this->x + this->speed*direction_Modifier[direction][0];
                this->y = this->y + this->speed*direction_Modifier[direction][1];

                break;

            case ATTACK_CMD:

                if (frameCount%6 == 3) {
                    this->sprite++;
                    if (this->type == BSKEL) {
                        if (sprite< direction * 6 +3 || sprite >= (direction+1) * 6) {
                            sprite = (direction) * 6 + 3;
                        }
                    }else if (this->type==RSKEL) {
                        if (sprite< direction * 6 +3 + RSKEL_NORTH_1 || sprite >= (direction+1) * 6 + RSKEL_NORTH_1) {
                            sprite = (direction) * 6 + 3 + RSKEL_NORTH_1;
                        }
                    }
                }
                //printf("attacking...%d..\n",sprite);
                break;
            case LEFT_CMD:
                if (direction == DIRECTION_NORTH) {
                    direction = DIRECTION_WEST;
                    sprite = BSKEL_WEST_1;
                }
                else if (direction == DIRECTION_SOUTH) {
                    direction = DIRECTION_EAST;
                    sprite = BSKEL_WEST_1;
                }
                else if (direction == DIRECTION_WEST) {
                    direction = DIRECTION_SOUTH;
                    sprite = BSKEL_SOUTH_1;
                }
                else if (direction == DIRECTION_EAST) {
                    direction = DIRECTION_NORTH;
                    sprite = BSKEL_NORTH_1;
                }

                if (this->type==RSKEL) {
                    sprite += RSKEL_NORTH_1;
                }

                break;
            case RIGHT_CMD:
                if (direction == DIRECTION_NORTH) {
                    direction = DIRECTION_EAST;
                    sprite = BSKEL_WEST_1;
                }
                else if (direction == DIRECTION_SOUTH) {
                    direction = DIRECTION_WEST;
                    sprite = BSKEL_WEST_1;
                }
                else if (direction == DIRECTION_WEST) {
                    direction = DIRECTION_NORTH;
                    sprite = BSKEL_NORTH_1;
                }
                else if (direction == DIRECTION_EAST) {
                    direction = DIRECTION_SOUTH;
                    sprite = BSKEL_SOUTH_1;
                }

                if (this->type==RSKEL) {
                    sprite += RSKEL_NORTH_1;
                }

                break;
            case WAIT_CMD:
                sprite = direction * 6;

                if (this->type==RSKEL) {
                    sprite += RSKEL_NORTH_1;
                }
                break;
        }
    }

    void Enemy::draw(){
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
    }

    Enemy::~Enemy(){

    };

};
