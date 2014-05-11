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

#define GLOBALHEIGHT 58.0
#define GLOBALWIDTH 58.0

namespace foz {

    Object::Object(uint8_t myType, float myX, float myY)
    {
        type = myType;
        width = GLOBALWIDTH;
        height = GLOBALHEIGHT;

        x = myX;
        y = myY;
        depth = 3;

        if (type>12 && type<20) {
            texfile = TEX_BASIC_ROOM;
            sprite = type;
            status = SOLID;
        }else if (type>19 &&  type<24) {
            texfile = TEX_RUPEE;
            sprite = (type-20)*3;
            status = TRANSPARENT;
        }
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
            if (sprite>=(type-19)*3) {
                sprite = (type-20)*3;
            }
        }
        return;

    }

    Object::~Object()
    {
        //dtor
    }

}
