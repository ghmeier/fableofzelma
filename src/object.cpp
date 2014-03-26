/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * object.hpp - FableOfZelma application. This class contains the
 * enumerated types and class declaration for the main object class.
 *
 *
 * NOTES:
 * 2/18/13 by JAZ::Design created.
 *****************************************************************************/

#pragma once
#include "fableofzelma.hpp"
#include "links.hpp"

namespace foz {

 /*****************************************************************************
    * Function: Object::draw
    * Description: Object drawing function. Recursively draws each object.
    *****************************************************************************/
void Object::draw(){

float texCoords[6];

getTexCoords(texfile, sprite, texCoords);
glBindTexture(GL_TEXTURE_2D, myGame->myTextures[texfile].texHandle);

            glBegin(GL_QUADS);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(x, y + height, depth);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(x + width, y + height, depth);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(x + width, y, depth);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(x, y, depth);
            glEnd();

}

}



