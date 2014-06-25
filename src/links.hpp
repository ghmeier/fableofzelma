/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * links.hpp - FableofZelma application. This class contains the
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

namespace foz {

    class Link {
        public:
            Link(uint8_t type, uint16_t myid, uint8_t myteam, uint8_t world_width, uint8_t world_height);
            Link(uint16_t type, float x, float y);
            ~Link(){};
            void update(uint8_t cmd);
            void draw();
            void doDamage(int16_t amount);

            bool active;            //draws the link when true, and ignores when false
            bool can_move;          //used to determine when link is colliding with anything
            uint8_t type;           //determines which type of link it is, CURRENTLY - only 0
            uint8_t direction;      //links directional facing, corresponds to the ENUM
            uint8_t team;           //reference to links team (0-3) corresponds with loaded teams
            uint8_t status;         //only saving this in the event that link gets status effects
            int16_t health;         //link's integer health value
            int16_t damage;         //the amount link takes down enemy's health when attacking
            uint16_t sprite;        //current sprite being drawn to the screen, see link_object ENUM
            uint32_t depth;         //used for layering
            uint8_t texfile;        //which texture link is using, red, green, blue, purple
            float width, height;    //in pixels. determines links size both visually and in collisions
            uint16_t id;            //link's identifier on the team (not sure if this works yet)
            uint8_t room_x, room_y; //these are used to place link in a room
            float x, y;             //link's coordinate position within the room
            float speed;            //amount link can move per frame, currently moves him ~59 pixels per move call
            uint8_t frameCount;     //modulates the way sprites change on link to slow them down to normal speed
            uint16_t numKeys;       //number of keys to open chests that link has
    };

}

