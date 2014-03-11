/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * room.cpp - FableofZelma application. This class contains the
 * main room class for building and managing rooms.
 *
 *
 * NOTES:
 * 3/11/14 by JAZ::Design created.
 *****************************************************************************/


#include "fableofzelma.hpp"


namespace foz {



    /*****************************************************************************
    * Function: Room::compile
    * Description: Compiles the specified room file (checks the current directory
    * as well as the scripts/rooms/ directory for the file) and sets up the
    * individual tiles.
    *****************************************************************************/
    void Room::compile(uint16_t id, bool rev, bool flip) {

        printf("Compiling room %03d, %d, %d\n", id, rev, flip);

        // Initialize the room pointers
        north = NULL;
        south = NULL;
        west = NULL;
        east = NULL;

    }

    /*****************************************************************************
    * Function: ~Room::Room()
    * Description: Simple destructor. Cleans up the room tiles.
    *****************************************************************************/
    Room::~Room() {

        uint16_t i;

        /* Perform a little bit of memory cleanup */
        for (i = 0; i < myTiles.size(); i++) {
            myTiles[i].clear();
        }
        myTiles.clear();

    }





} // namespace foz
