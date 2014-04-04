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
 * 4/04/14 by JAZ::Design created.
 *****************************************************************************/

#include "fableofzelma.hpp"

namespace foz {

 /*****************************************************************************
    * Function: Object::draw
    * Description: Object drawing function. Recursively draws each object.
    *****************************************************************************/
void Object::draw(){

float texCoords[6];

// Sets matrix to draw object in its current room
glTranslatef(1080*Xroom,1080*(-Yroom),0);

getTexCoords(texfile, sprite, texCoords);
glBindTexture(GL_TEXTURE_2D, myGame->myTextures[texfile].texHandle);

            glBegin(GL_QUADS);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(x, y , depth);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(x + width, y, depth);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(x + width, y + height, depth);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(x, y + height, depth);
            glEnd();

// Resests Matrix to previous state
glTranslatef(1080*(-Xroom),1080*Yroom,0);

}
void Object::SlashNorthAnim(){

    static uint8_t i;
    static uint8_t j;

    this->height = 3*(link_object_spriteMap[i][3] - link_object_spriteMap[i][1]);
    this->width = 3*(link_object_spriteMap[i][2] - link_object_spriteMap[i][0]);
    this->sprite = i;
    this->x =  3*(-link_object_spriteMap_centers[i][0] + link_object_spriteMap[i][0]) ;
    this->y =  (-link_object_spriteMap_centers[i][1] + link_object_spriteMap[i][1]);
    this->draw();
    ++j;
    if (j%5 == 0){
        ++i;
    }
    if (i > 19){
        i = 0;
    }
}
/*void Object::SlashWestAnim(){
    static uint8_t i;

    this->sprite = i;
    this->draw();
    ++i;
    if (i > 39){
        i = 20;
    }
}
void Object::SlashSouthAnim(){
    static uint8_t i;

    this->sprite = i;
    this->draw();
    ++i;
    if (i > 20){
        i = 0;
    }
}
void Object::WalkNorthAnim(){
    static uint8_t i;

    this->sprite = i;
    this->draw();
    ++i;
    if (i > 20){
        i = 0;
    }
}
void Object::WalkWestAnim(){
    static uint8_t i;

    this->sprite = i;
    this->draw();
    ++i;
    if (i > 20){
        i = 0;
    }
}
void Object::DieAnim(){
    static uint8_t i;

    this->sprite = i;
    this->draw();
    ++i;
    if (i > 20){
        i = 0;
    }
}*/

}



