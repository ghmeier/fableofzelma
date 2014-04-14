#ifndef OBJECT_H
#define OBJECT_H
#include "fableofzelma.hpp"

namespace foz {
    class Object{
        public:
            Object(uint8_t myType, uint8_t myX, uint8_t myY);
            ~Object();
            void update(uint8_t cmd);
            void updateTransition(uint16_t val);
            void draw();
            void move();

            bool active;
            uint8_t type;
            uint8_t direction;
            uint8_t status;
            uint16_t sprite;
            uint32_t depth;
            uint8_t texfile;
            float width, height;
            uint16_t id;
            uint16_t row, col;
            uint8_t room_x, room_y;
            float x, y;

    };
};

#endif // OBJECT_H
