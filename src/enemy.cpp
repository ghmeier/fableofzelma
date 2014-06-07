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
        switch (type) {
            case BSKEL:
                cmdIter;
                health = 20;
                sprite = BSKEL_SOUTH_1;
                direction = DIRECTION_SOUTH;
                height = GLOBALHEIGHT;
                width = GLOBALWIDTH;
                break;
            default:
                break;
        }
    };

    void Enemy::update(uint8_t cmd){
        uint16_t tempSprite = 0;

        if (health<=0) {
            delete this;
        }

        switch (cmd) {

            case MOVE_CMD:
                sprite++;
                if (frameCount%8==3) {
                    if (type == BSKEL) {
                        if (sprite>=(direction+1)*3)
                            sprite = direction*3;
                    }

                    if ( frameCount>FRAME_RATE) {
                        frameCount = 0;
                    }else {
                        frameCount++;
                    }
                    x = x + speed*direction_Modifier[direction][0];
                    y = y + speed*direction_Modifier[direction][1];
                }
                break;
            case ATTACK_CMD:

                printf("ATTACKING\n");
                break;
            case LEFT_CMD:
                direction--;
                if (direction<0) {
                    direction = 3;
                }
                sprite = direction*3;
                break;
            case RIGHT_CMD:
                direction++;
                if (direction>3) {
                    direction = 0;
                }
                sprite = direction * 3;
                break;
            case WAIT_CMD:
                sprite = direction * 3;
                break;

        }
    }

    Enemy::~Enemy(){

    };

};
