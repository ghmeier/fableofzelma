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
        x_left = -1920.0/2.0*myWorld.width;
        x_right = 1920.0/2.0*myWorld.width;
        y_bottom = -1080.0/2.0*myWorld.height;
        y_top = 1080.0/2.0*myWorld.height;

        // We need to keep track of what room we are looking at currently, independent of the zoom level
        // This x_pos/y_pos might not be an integer for even-sized worlds.
        width = myWorld.width;
        height = myWorld.height;
        x_pos = (width-1)/2.0;
        y_pos = (height-1)/2.0;

        x_pan_count = 0;
        y_pan_count = 0;
        zoom_count = 0;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(x_left, x_right, y_bottom, y_top, FRONT_DEPTH, -BACK_DEPTH);

        return;
    }


    /*****************************************************************************
    * Function: Camera::update
    * Description: Updates the camera position based on state.
    *****************************************************************************/
    void Camera::update() {

        bool reposition = false;


        switch(state) {

            case CAMERA_INIT:
                state = CAMERA_IDLE;
                break;
            case CAMERA_PAN_LEFT:
                if (x_pos < 1.0) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                x_left += (1080.0)/CAMERA_PAN_DELTA;
                x_right += (1080.0)/CAMERA_PAN_DELTA;
                x_pan_count--;
                if (x_pan_count == (-CAMERA_PAN_DELTA)) {
                    state = CAMERA_IDLE;
                    x_pan_count = 0;
                    x_pos -= 1.0;
                }
                break;
            case CAMERA_PAN_RIGHT:
                if (x_pos > (width-2.0)) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                x_left -= (1080.0)/CAMERA_PAN_DELTA;
                x_right -= (1080.0)/CAMERA_PAN_DELTA;
                x_pan_count++;
                if (x_pan_count == (CAMERA_PAN_DELTA)) {
                    state = CAMERA_IDLE;
                    x_pan_count = 0;
                    x_pos += 1.0;
                }
                break;
            case CAMERA_PAN_UP:
                if (y_pos > (height-2.0)) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                y_top -= (1080.0)/CAMERA_PAN_DELTA;
                y_bottom -= (1080.0)/CAMERA_PAN_DELTA;
                y_pan_count--;
                if (y_pan_count == (-CAMERA_PAN_DELTA)) {
                    state = CAMERA_IDLE;
                    y_pan_count = 0;
                    y_pos += 1.0;
                }
                break;
            case CAMERA_PAN_DOWN:
                if (y_pos < (1.0)) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                y_top += (1080.0)/CAMERA_PAN_DELTA;
                y_bottom += (1080.0)/CAMERA_PAN_DELTA;
                y_pan_count++;
                if (y_pan_count == (CAMERA_PAN_DELTA)) {
                    state = CAMERA_IDLE;
                    y_pan_count = 0;
                    y_pos -= 1.0;
                }
                break;

            default:
                break;
        }

        if (reposition == true) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(x_left, x_right, y_bottom, y_top, FRONT_DEPTH, -BACK_DEPTH);
        }

    }



} // namespace foz
