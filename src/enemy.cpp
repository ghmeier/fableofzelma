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
                health = 20;
                texfile = TEX_ENEMIES;
                sprite = BSKEL_SOUTH_1;
                direction = DIRECTION_SOUTH;
                height = GLOBALHEIGHT;
                width = GLOBALWIDTH;
                break;
            case RSKEL:
                health = 20;
                texfile = TEX_ENEMIES;
                sprite = RSKEL_SOUTH_1;
                direction = DIRECTION_SOUTH;
                height = GLOBALHEIGHT;
                width = GLOBALWIDTH;
                break;
           case GEYEGORE:
                health = 100;
                texfile = TEX_ENEMIES;
                sprite = GEYEGORE_SOUTH_1;
                direction = DIRECTION_SOUTH;
                height = GLOBALHEIGHT;
                width = GLOBALWIDTH;
                break;
            default:
                break;
        }
    };

    /**********************************************
    * Function Enemy::update(uint8_t cmd)
    * Description: based on the given command, handles the results for each enemy.
    ***********************************************/
    void Enemy::update(uint8_t cmd){
        if (health<=0 && active) {
            switch (type) {
                case BSKEL:
                case RSKEL:
                    myGame->playSound(SFX_ENEMYKILL,100,true);
                    break;
                case GEYEGORE:
                    myGame->playSound(SFX_ENEMYKILL,100,true);
                    break;
                default:
                    break;
            }
            this->active = false;
        }

        if (!this->active)
            return;

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
                    }else if (this->type == GEYEGORE) {
                         if (this->sprite>=(this->direction+1)*3 + GEYEGORE_NORTH_1)
                            this->sprite = this->direction*3 + GEYEGORE_NORTH_1;
                    }
                }
                this->x = this->x + this->speed*direction_Modifier[direction][0];
                this->y = this->y + this->speed*direction_Modifier[direction][1];

                break;

            case SHOOT_CMD:
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
                    }else if (this->type==GEYEGORE) {
                        if (sprite< direction * 3  + GEYEGORE_NORTH_ATTACK_1 || sprite >= (direction+1) * 3 + GEYEGORE_NORTH_ATTACK_1) {
                            sprite = (direction) * 3  + GEYEGORE_NORTH_ATTACK_1;
                        }
                    }
                }

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
                }else if (this->type==GEYEGORE) {
                    sprite = direction*3 + GEYEGORE_NORTH_1;
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
                }else if (this->type==GEYEGORE) {
                    sprite = direction*3 + GEYEGORE_NORTH_1;
                }

                break;
            case WAIT_CMD:
                sprite = direction * 6;

                if (this->type==RSKEL) {
                    sprite += RSKEL_NORTH_1;
                }else if (this->type==GEYEGORE) {
                    sprite = direction*3 + GEYEGORE_NORTH_1;
                }
                break;
        }

    }

    /*****************************************
    * Function: Enemy::draw()
    * Description: handles drawing the enemy to the screen in GL
    ******************************************/
    void Enemy::draw(){

        if (this->active) {
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
    }

    /*****************************************
    * Function: Enemy::doDamage(int16_t amount)
    * Description: removes amount from enemy's health and plays corresponding sound
    ******************************************/
    void Enemy::doDamage(int16_t amount) {
        this->health -= amount;
        switch (type) {
            case RSKEL:
            case BSKEL:
                myGame->playSound(SFX_ENEMYHIT,100,true);
                break;
            case GEYEGORE:
                myGame->playSound(SFX_ENEMYHIT,100,true);
                break;
            default:
                break;
        }
    }

    Enemy::~Enemy(){

    };

};
