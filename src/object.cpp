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
#define RUPEE_START 21
#define RUPEE_END 24

namespace foz {

    Object::Object(uint8_t myType, float myX, float myY)
    {
        type = myType;
        width = GLOBALWIDTH;
        height = GLOBALHEIGHT;
        active = true;
        x = myX;
        y = myY;
        depth = 3;

        if (type>12 && type<RUPEE_START) {
            texfile = TEX_BASIC_ROOM;
            sprite = type;
            status = SOLID;
        }else if (type>=RUPEE_START &&  type<=RUPEE_END) {
            texfile = TEX_RUPEE;
            sprite = (type-RUPEE_START)*3;
            status = TRANSPARENT;
        }else {
            texfile = TEX_BASIC_ROOM;
            sprite = type;
            status = SOLID;
        }
    }

    void Object::setRoomXY(uint8_t roomx, uint8_t roomy) {
        room_x = roomx;
        room_y = roomy;
    }

    void Object::draw(){
        if (!active) {
            return;
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
        if (texfile == TEX_RUPEE) {
            sprite++;
            if (sprite>=(type-RUPEE_START+1)*3) {
                sprite = (type-RUPEE_START)*3;
            }
        }
        return;

    }

    Object::~Object()
    {
        //dtor
    }

}
