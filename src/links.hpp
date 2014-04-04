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
            Link(uint8_t type, uint16_t myid, uint8_t myteam, uint8_t world_width, uint8_t world_height);
            ~Link(){};
            void update();
            void updateTransition(uint16_t val);
            void draw();
            void move();

            bool active;
            uint16_t action_count;
            uint8_t type;
            uint8_t team;
            uint8_t status;
            int16_t health;
            uint16_t sprite;
            uint32_t depth;
            uint8_t texfile;
            float width, height;
            uint16_t id;
            uint16_t row, col;
            uint8_t room_x, room_y;
            float x, y;
            float speed;
            float dir;
            uint16_t delay;
            uint16_t move_count;
    };

}

