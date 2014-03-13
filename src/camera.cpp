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

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-959.5, 959.5, -539.5, 539.5, FRONT_DEPTH, -BACK_DEPTH);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        return;
    }

} // namespace foz
