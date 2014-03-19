/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * camera.cpp - FableofZelma application. This class contains the
 * main camera class for viewing the World structure.
 *
 *
 * NOTES:
 * 3/11/14 by JAZ::Design created.
 *****************************************************************************/


#include "fableofzelma.hpp"


namespace foz {


    /*****************************************************************************
    * Function: Camera::init
    * Description: Camera class initializer. Uses World configuration information
    * in order to decide on a starting position.
    *****************************************************************************/
    void Camera::init(foz::World myWorld) {

        state = CAMERA_INIT;

        // Temporary code. Set the 2D view based on the size of the world
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
//        glOrtho(-1.0*myWorld.width, 1.0*myWorld.width, -1.0*myWorld.height, 1.0*myWorld.height, FRONT_DEPTH, -BACK_DEPTH);
        glOrtho(-1920/2.0, 1920/2.0, -1080/2.0, 1080/2.0, FRONT_DEPTH, -BACK_DEPTH);


        return;
    }

} // namespace foz
