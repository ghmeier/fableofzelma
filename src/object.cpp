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

    Object::Object(uint8_t myType, uint8_t myX, uint8_t myY)
    {
        type = myType;
        sprite = myType;
        width = GLOBALWIDTH;
        height = GLOBALHEIGHT;
        texfile = TEX_BASIC_ROOM;
        x = myX;
        y = myY;
    }

    void Object::draw(){

        float texCoords[6];

        getTexCoords(texfile, sprite, texCoords);
        printf("I can get here \n");
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

        return;

    }

    Object::~Object()
    {
        //dtor
    }

}
