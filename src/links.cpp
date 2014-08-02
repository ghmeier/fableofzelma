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
#define MAX_HEALTH 50
#define LINK_START_X -25.0
#define LINK_START_Y -83.0
#define GLOBALSPEED 0.983333

std::string linkNames[NUM_LINK_TYPE][NUM_LINK_SPELLINGS] = {
    {"regular", "link", "normal", "default"},
    {"archer","arrow","sniper","ranged"},
    {"heavy","hammer","warhammer","big"},
    {"bomb","demolition","explosion","demo"},
    {"navi","scout","fast","light"}
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
        //link shouldn't update unless it is active
        if (!active) {
            return;
        }

        //don't worry about Link if it's out of health
        if (health<=0) {
            this->active = false;
        }

        switch (cmd) {

            case MOVE_CMD:
                height = GLOBALHEIGHT;
                width = GLOBALWIDTH;

                if (cur_cmdframe%10==0) {
                    sprite++;
                    if (type == NAVI_LINK){
                        if (sprite >= team * 6 + 5){
                            sprite = team*6;
                        }
                    }else {
                        if (direction == DIRECTION_NORTH) {
                            if (sprite > LINK_WALKING_NORTH_6 || sprite < LINK_WALKING_NORTH_1 || cur_cmdframe==0) {
                                sprite = LINK_WALKING_NORTH_1;
                            }
                        }
                        if (direction == DIRECTION_SOUTH) {
                            if (sprite > LINK_WALKING_SOUTH_6 || sprite < LINK_WALKING_SOUTH_1 || cur_cmdframe==0) {
                                sprite = LINK_WALKING_SOUTH_1;
                            }
                        }
                        if (direction == DIRECTION_WEST || direction == DIRECTION_EAST) {
                            if (sprite > LINK_WALKING_WEST_6 || sprite < LINK_WALKING_WEST_1 || cur_cmdframe==0) {
                                sprite = LINK_WALKING_WEST_1;
                            }
                        }
                    }
                }
                x = x + speed*direction_Modifier[direction][0];
                y = y + speed*direction_Modifier[direction][1];

                break;

            case ATTACK_CMD:
                height = GLOBALHEIGHT+45;
                width = GLOBALWIDTH+45;
                if (cur_cmdframe <= 0 ){
                    if (direction==DIRECTION_NORTH) {
                        x-=22;
                       // y-=25;
                    }else if (direction == DIRECTION_EAST) {
                        y-=22;
                        //x+=45;
                    }else if (direction == DIRECTION_WEST){
                        y-=22;
                        x-=45;
                    }else if (direction == DIRECTION_SOUTH){
                        y-=45;
                        x-=22;
                    }else {
                        x-=20;
                        y-=20;
                    }
                }else if (cur_cmdframe == 59){
                    if ((direction==DIRECTION_NORTH)) {
                        x+=22;
                       // y+=25;
                    }else if (direction == DIRECTION_EAST) {
                        y+=22;
                       // x-=45;
                    }else if (direction == DIRECTION_WEST){
                        y+=22;
                        x+=45;
                    }else if (direction == DIRECTION_SOUTH){
                        y+=45;
                        x+=22;
                    }else {
                        x+=20;
                        y+=20;
                    }
                }

                if (cur_cmdframe%8 == 0){
                    sprite++;

                    if (direction == DIRECTION_NORTH) {
                        if (sprite >= LINK_SLASH_NORTH_13 || sprite <LINK_SLASH_NORTH_6 || cur_cmdframe==0){
                            sprite = LINK_SLASH_NORTH_6;
                        }
                   }else if (direction == DIRECTION_SOUTH) {
                        if (sprite >= LINK_SLASH_SOUTH_13 || sprite < LINK_SLASH_SOUTH_6 || cur_cmdframe==0){
                            sprite = LINK_SLASH_SOUTH_6;
                        }
                   }else if (direction == DIRECTION_WEST) {
                        if (sprite >= LINK_SLASH_WEST_13 || sprite < LINK_SLASH_WEST_6 || cur_cmdframe==0){
                            sprite = LINK_SLASH_WEST_6;
                        }
                   }else if (direction == DIRECTION_EAST) {
                        if (sprite >= LINK_SLASH_WEST_13 || sprite < LINK_SLASH_WEST_6 || cur_cmdframe==0){
                            sprite = LINK_SLASH_WEST_6;
                        }
                   }

               }
               break;

        case LEFT_CMD:
            height = GLOBALHEIGHT;
            width = GLOBALWIDTH;
            if (direction == DIRECTION_NORTH) {
                direction = DIRECTION_WEST;
                 if (type != NAVI_LINK)
                sprite = LINK_WALKING_WEST_1;
            }
            else if (direction == DIRECTION_SOUTH) {
                direction = DIRECTION_EAST;
                 if (type != NAVI_LINK)
                sprite = LINK_WALKING_WEST_1;
            }
            else if (direction == DIRECTION_WEST) {
                direction = DIRECTION_SOUTH;
                 if (type != NAVI_LINK)
                sprite = LINK_WALKING_SOUTH_1;
            }
            else if (direction == DIRECTION_EAST) {
                direction = DIRECTION_NORTH;
                 if (type != NAVI_LINK)
                sprite = LINK_WALKING_NORTH_1;
            }

            break;

        case RIGHT_CMD:
            height = GLOBALHEIGHT;
            width = GLOBALWIDTH;
            if (direction == DIRECTION_NORTH) {
                direction = DIRECTION_EAST;
                if (type!=NAVI_LINK)
                sprite = LINK_WALKING_WEST_1;
            }
            else if (direction == DIRECTION_SOUTH) {
                direction = DIRECTION_WEST;
                if (type!=NAVI_LINK)
                sprite = LINK_WALKING_WEST_1;
            }
            else if (direction == DIRECTION_WEST) {
                direction = DIRECTION_NORTH;
                if (type!=NAVI_LINK)
                sprite = LINK_WALKING_NORTH_1;
            }
            else if (direction == DIRECTION_EAST) {
                direction = DIRECTION_SOUTH;
                if (type!=NAVI_LINK)
                sprite = LINK_WALKING_SOUTH_1;
            }
            break;
        case SHOOT_CMD:
            if (cur_cmdframe%10==0){
                sprite++;

                if(direction==DIRECTION_NORTH){
                    if (sprite > LINK_ARROW_NORTH_6 || sprite < LINK_ARROW_NORTH_1 || cur_cmdframe==0){
                        sprite = LINK_ARROW_NORTH_1;
                    }
                }else if (direction == DIRECTION_EAST || direction == DIRECTION_WEST){
                    if (sprite > LINK_ARROW_WEST_6 || sprite < LINK_ARROW_WEST_1|| cur_cmdframe==0) {
                        sprite = LINK_ARROW_WEST_1;
                    }
                }else if (direction == DIRECTION_SOUTH) {
                    if (sprite > LINK_ARROW_SOUTH_6 || sprite < LINK_ARROW_SOUTH_1 || cur_cmdframe==0) {
                        sprite = LINK_ARROW_SOUTH_1;
                    }
                }
            }
            break;

        case THROW_CMD:
            if (cur_cmdframe%10==0){
                sprite++;

                if(direction==DIRECTION_NORTH){
                    if (sprite > LINK_THROW_NORTH_6 || sprite < LINK_THROW_NORTH_1 || cur_cmdframe==0){
                        sprite = LINK_THROW_NORTH_1;
                    }
                }else if (direction == DIRECTION_EAST || direction == DIRECTION_WEST){
                    if (sprite > LINK_THROW_WEST_6 || sprite < LINK_THROW_WEST_1 || cur_cmdframe==0) {
                        sprite = LINK_THROW_WEST_1;
                    }
                }else if (direction == DIRECTION_SOUTH) {
                    if (sprite > LINK_THROW_SOUTH_6 || sprite < LINK_THROW_SOUTH_1 || cur_cmdframe==0) {
                        sprite = LINK_THROW_SOUTH_1;
                    }
                }
            }
            break;
        case WARHAMMER_CMD:
            if (cur_cmdframe<=0){
                myGame->playSound(SFX_HAMMER_SWING,100,true);
                if (direction == DIRECTION_NORTH  || direction == DIRECTION_SOUTH){
                    height = GLOBALHEIGHT +40;
                    width = GLOBALWIDTH + 10;
                    x -= 5;
                    y-= 10;
                }else {
                    height =  GLOBALHEIGHT + 20;
                    width = GLOBALWIDTH + 40;
                    if (direction == DIRECTION_WEST){
                    x-=25;
                    }
                    y+=5;
                }
            }
            if (cur_cmdframe == 50){
                myGame->playSound(SFX_HAMMER_HIT,100,true);
            }
            if (cur_cmdframe>=59){
                if (direction == DIRECTION_NORTH  || direction == DIRECTION_SOUTH){
                    height = GLOBALHEIGHT;
                    width = GLOBALWIDTH;
                    x += 5;
                    y += 10;
                }else {
                    height =  GLOBALHEIGHT;
                    width = GLOBALWIDTH;
                    if (direction == DIRECTION_WEST){
                    x+=25;
                    }
                    y-=5;
                }
            }
            if (cur_cmdframe%10==0){
                sprite++;

                if(direction==DIRECTION_NORTH){
                    if (sprite > LINK_WARHAMMER_NORTH_6 || sprite < LINK_WARHAMMER_NORTH_1 || cur_cmdframe==0){
                        sprite = LINK_WARHAMMER_NORTH_1;
                    }
                }else if (direction == DIRECTION_EAST || direction == DIRECTION_WEST){
                    if (sprite > LINK_WARHAMMER_WEST_6 || sprite < LINK_WARHAMMER_WEST_1 || cur_cmdframe==0) {
                        sprite = LINK_WARHAMMER_WEST_1;
                    }
                }else if (direction == DIRECTION_SOUTH) {
                    if (sprite > LINK_WARHAMMER_SOUTH_6 || sprite < LINK_WARHAMMER_SOUTH_1 || cur_cmdframe==0) {
                        sprite = LINK_WARHAMMER_SOUTH_1;
                    }
                }
            }
            break;
        case WAIT_CMD:
            height = GLOBALHEIGHT;
            width = GLOBALWIDTH;
            if (type == NAVI_LINK){
                sprite = team*6;
            }else {
            if (direction == DIRECTION_EAST) {
                sprite = LINK_WALKING_WEST_1;
            }else if (direction == DIRECTION_WEST) {
                sprite = LINK_WALKING_WEST_1;
            }else if (direction== DIRECTION_NORTH) {
                sprite = LINK_WALKING_NORTH_1;
            }else if (direction == DIRECTION_SOUTH) {
                sprite = LINK_WALKING_SOUTH_1;
            }
            }
            break;
        case DEATH_CMD:
            //delete this;
            break;
        }
    }

    /*****************************************************************************
    * Function: Link::doDamage(int16_t amount)
    * Description: remove amount from link's health
    *****************************************************************************/
    void Link::doDamage(int16_t amount) {
        this->health -= amount;
        myGame->playSound(SFX_LINKHURT_1,100,true);
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
        health = MAX_HEALTH;
        height = GLOBALHEIGHT;
        width = GLOBALWIDTH;
        if (type == NAVI_LINK){
            speed = GLOBALSPEED*2;
        }else {
            speed = GLOBALSPEED;
        }
        damage = 30;
        active = true;
        cur_cmd = 0;
        cur_cmdframe = 0;

        switch(team) {
            case 0:
            default:
                if (type == NAVI_LINK){
                    texfile = TEX_NAVI;
                    sprite = GREEN_NAVI_1;
                }else{
                    texfile = TEX_GREEN_LINK;
                    sprite = LINK_WALKING_SOUTH_1;
                }
                room_x = 0;
                room_y = 0;
                x = LINK_START_X;
                y = LINK_START_Y;

                direction = DIRECTION_SOUTH;
                depth = FRONT_DEPTH;
                break;
            case 1:
                if (type == NAVI_LINK){
                    texfile = TEX_NAVI;
                    sprite = PURPLE_NAVI_1;
                }else{
                    texfile = TEX_PURPLE_LINK;
                    sprite = LINK_WALKING_SOUTH_1;
                }
                room_x = 0;
                room_y = world_height-1;
                x = LINK_START_X;
                y = LINK_START_Y;

                direction = DIRECTION_SOUTH;
                depth = FRONT_DEPTH;
                break;
            case 2:
                if (type == NAVI_LINK){
                    texfile = TEX_NAVI;
                    sprite = BLUE_NAVI_1;
                }else{
                    texfile = TEX_BLUE_LINK;
                    sprite = LINK_WALKING_SOUTH_1;
                }
                room_x = world_width-1;
                room_y = 0;
                x = LINK_START_X;
                y = LINK_START_Y;

                direction = DIRECTION_SOUTH;
                depth = FRONT_DEPTH;

                break;
            case 3:
                if (type == NAVI_LINK){
                    texfile = TEX_NAVI;
                    sprite = RED_NAVI_1;
                }else{
                    texfile = TEX_RED_LINK;
                    sprite = LINK_WALKING_SOUTH_1;
                }
                room_x = world_width-1;
                room_y = world_height-1;
                x = LINK_START_X;
                y = LINK_START_Y;

                direction = DIRECTION_SOUTH;
                depth = FRONT_DEPTH;
                break;
        }


    }

    /*****************************************************************************
    * Function Link:Link
    * Description: The Enemy constructor. Constructs things on the enemy team.
    ******************************************************************************/
    Link::Link(uint16_t mytype, float myx, float myy) {
        type = mytype-100;
        id = 1;
        height = GLOBALHEIGHT;
        width = GLOBALWIDTH;
        depth = FRONT_DEPTH;
        speed = GLOBALSPEED;
        x = myx;
        y = myy;
        room_x = 0;
        room_y = 0;
        team = 255;
        texfile = TEX_ENEMIES;
        damage = enemyDamage[type];
    }

    /*****************************************************************************
    * Function: Link::draw
    * Description: Link drawing function.
    *****************************************************************************/
    void Link::draw(){
        if (active) {
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
        }
        return;

    }

}
