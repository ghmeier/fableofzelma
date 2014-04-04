/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * plants.hpp - FableofZelma application. This class contains the
 * enumerated types and class declaration for the main link class.
 *
 *
 * NOTES:
 * 4/04/14 by JAZ::Design created.
 *****************************************************************************/


#pragma once

#include "fableofzelma.hpp"


/* Main enum for the link types. We specify NUM_SPELLINGS to some reasonable amount to minimize
 * the number of compile errors */
#define NUM_LINK_SPELLINGS 4
typedef enum {REGULAR_LINK=0, NUM_LINK_TYPE} LINK_TYPE;
extern std::string linkNames[NUM_LINK_TYPE][NUM_LINK_SPELLINGS];

/* Data arrays for the main link types */
extern uint16_t linkCosts[NUM_LINK_TYPE];
extern int16_t linkHealths[NUM_LINK_TYPE];
extern uint16_t linkSpeeds[NUM_LINK_TYPE];


namespace foz {

    class Link {
        public:
            Link(uint8_t type, uint16_t myid);
            ~Link(){};
            void update();
            void place(uint8_t team, uint16_t row, uint16_t col);
            void updateTransition(uint16_t val);
            void draw(uint16_t index);
            uint8_t getStatus() {return status;}
            uint16_t getID() {return id;}
            uint8_t getType() {return type;}
            uint16_t getRow() {return row;}
            int16_t getHealth() {return health;}
            void setStatus(uint8_t val) {status = val;}
            void setHealth(int16_t myhealth) {health = myhealth;}
            void setRow(uint16_t myrow) {row = myrow;}
            void setCol(uint16_t mycol) {col = mycol;}
            uint16_t getCol() {return col;}
            uint8_t getTeam() {return team;}
            float getGameX() {return game_x;}
            float getGameY() {return game_y;}
            float getDir() {return dir;}
            void move();
            uint16_t action_count;

        private:
            uint8_t type;
            uint8_t status;
            int16_t health;
            uint16_t id;
            uint16_t row, col;
            float speed;
            float game_x, game_y;
            float dir;
            uint8_t team;
            uint16_t delay;
            uint16_t move_count;
    };

}

