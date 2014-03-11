/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * world.cpp - FableofZelma application. This class contains the
 * main world class for building and managing worlds (maps).
 *
 *
 * NOTES:
 * 3/11/14 by JAZ::Design created.
 *****************************************************************************/


#include "fableofzelma.hpp"


namespace foz {


    /*****************************************************************************
    * Function: World::compile
    * Description: Compiles the specified world file (checks the current directory
    * as well as the scripts/maps/ directory for the file) and builds the
    * individual rooms.
    *****************************************************************************/

    uint8_t World_JZ::compile(Config myConfig) {

        std::ifstream infile;
        char *fname;

        /* Open and compile the team file */
        if (myConfig.debug_level > 1) {
            printf("Compiling map file %s\n", myConfig.map_fname);
        }


        // Does the file exist in the current directory?
        infile.open(myConfig.map_fname, std::ifstream::in);
        if (!infile) {
            if (myConfig.debug_level > 1) {
                printf("Map file %s not found in root directory, checking %s\n", myConfig.map_fname, MAP_DIR_DEFAULT);
            }

            // If not, just check in the MAP_DIR_DEFAULT directory
            fname = (char *)malloc(strlen(myConfig.map_fname)+strlen(MAP_DIR_DEFAULT)+1);
            strcpy(fname, MAP_DIR_DEFAULT);
            strcat(fname, myConfig.map_fname);
            infile.open(fname, std::ifstream::in);
            if (!infile) {
                raise_error(ERR_NOFILE3, myConfig.map_fname);
            }
        }


        return 0;
    }


} // namespace foz
