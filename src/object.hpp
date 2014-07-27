/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * links.hpp - FableofZelma application. This class contains the
 * enumerated types and class declaration for the main object class.
 *
 *
 * NOTES:
 * 4/04/14 by JAZ::Design created.
 *****************************************************************************/


#pragma once

#include "fableofzelma.hpp"

namespace foz {

    class Object{
        public:
            Object(uint8_t myType, float myX, float myY, int16_t mySubject);
            Object(uint8_t myType, float myX, float myY);
            void init(uint8_t myType, float myX, float myY);
            ~Object();
            void update(uint8_t cmd);
            void draw();
            void setDirection(uint8_t dir);

            bool active;
            bool can_move;
            uint8_t type;
            uint8_t direction;
            uint8_t status;
            uint16_t sprite;
            uint32_t depth;
            uint8_t texfile;
            float width, height;
            uint8_t room_x, room_y;
            float x, y;
            uint8_t frameCount;
            uint16_t subject;

    };
};
