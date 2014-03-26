/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * object.hpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the main object class.
 *
 *
 * NOTES:
 * 2/18/13 by JAZ::Design created.
 *****************************************************************************/

#pragma once
#include "fableofzelma.hpp"

/* Object class. Each link/enemy has multiple objects, each with links to the parent
     * object and (potentially) multiple children. */
class Object {
public:
    float x,y; //location rel to room
    status_type status;
    object_type type;
    int16_t health;
    Room *myRoom;
    Team *myTeam;
    int16_t myTeam;
    //copied from FVU
    Object *parent;
    Object **children;
    uint8_t num_children;
    void setMode(uint8_t mode);
    void update();
    void updateSprite(uint16_t mySprite) {sprite = mySprite;}
    float get_abs_x();
    float get_abs_y();
    void draw();

};
