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
    void Camera::init(foz::World *myWorld) {

        state = CAMERA_INIT;
        // Temporary code. Set the 2D view based on the size of the world
        x_left = -1920.0/2.0*myWorld->width;
        x_right = 1920.0/2.0*myWorld->width;
        y_bottom = -1080.0/2.0*myWorld->height;
        y_top = 1080.0/2.0*myWorld->height;

        currentTeam = 255;
        // We need to keep track of what room we are looking at currently, independent of the zoom level
        // This x_pos/y_pos might not be an integer for even-sized worlds.
        width = myWorld->width;
        height = myWorld->height;
        x_pos = (width-1)/2.0;
        y_pos = (height-1)/2.0;
        zoom_level = 1.0*width;

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
    void Camera::update(bool reposition) {

        foz::Link *myLink;
        foz::Team *myTeam;
        uint8_t team_id = 0;

        if ((state == CAMERA_TEAM_1) || (state == CAMERA_TEAM_2) ||
            (state == CAMERA_TEAM_3) || (state == CAMERA_TEAM_4)) {
            team_id = state - (uint8_t)CAMERA_TEAM_1;
            currentTeam = team_id;
        }

        switch(state) {

            case CAMERA_INIT:
                x_left = -1920.0/2.0*width;
                x_right = 1920.0/2.0*width;
                y_bottom = -1080.0/2.0*height;
                y_top = 1080.0/2.0*height;
                state = CAMERA_IDLE;
                currentTeam = 255;
                break;

            case CAMERA_TEAM_1:
            case CAMERA_TEAM_2:
            case CAMERA_TEAM_3:
            case CAMERA_TEAM_4:
                // If the team is already done, there is nothing the camera can do
                myTeam = &myGame->myTeams[team_id];
                if (myTeam->cmds_done != true) {
                    reposition = true;
                    myLink = &myGame->myTeams[myTeam->id].myLinks[myTeam->cur_link];
                    x_pos = (float)myLink->room_x;
                    y_pos = (float)myLink->room_y;

                    zoom_level = 1.0;
                    x_pan_count = 0;
                    y_pan_count = 0;
                    zoom_count = 0;

                    x_left = (1080.0/2.0)*(2*x_pos-width)-420.0;
                    x_right = x_left + 1920.0;
                    y_top = (1080.0/2.0)*(height-2*y_pos);
                    y_bottom = y_top - 1080.0;
                }
                else {
                    state = CAMERA_IDLE;
                }
                break;

            case CAMERA_PAN_RIGHT:
                /*if (x_pos <= (-width)) {
                    state = CAMERA_IDLE;
                    break;
                }*/

                reposition = true;
                x_left += (1080.0)/CAMERA_PAN_DELTA;
                x_right += (1080.0)/CAMERA_PAN_DELTA;
                x_pan_count--;
                if (x_pan_count == (-CAMERA_PAN_DELTA)) {
                    state = CAMERA_IDLE;
                    x_pan_count = 0;
                    x_pos -= 1.0;
                }
                currentTeam = 255;
                break;

            case CAMERA_PAN_LEFT:
                /*if (x_pos >= (width)) {
                    state = CAMERA_IDLE;
                    break;
                }*/

                reposition = true;
                x_left -= (1080.0)/CAMERA_PAN_DELTA;
                x_right -= (1080.0)/CAMERA_PAN_DELTA;
                x_pan_count++;
                if (x_pan_count == (CAMERA_PAN_DELTA)) {
                    state = CAMERA_IDLE;
                    x_pan_count = 0;
                    x_pos += 1.0;
                    printf("modified x/y_pos = [%f, %f]\n", x_pos, y_pos);
                }
                 currentTeam = 255;
                break;

            case CAMERA_PAN_DOWN:
                /*if (y_pos >= (height)) {
                    state = CAMERA_IDLE;
                    break;
                }*/

                reposition = true;
                y_top -= (1080.0)/CAMERA_PAN_DELTA;
                y_bottom -= (1080.0)/CAMERA_PAN_DELTA;
                y_pan_count--;
                if (y_pan_count == (-CAMERA_PAN_DELTA)) {
                    state = CAMERA_IDLE;
                    y_pan_count = 0;
                    y_pos += 1.0;
                    printf("modified x/y_pos = [%f, %f]\n", x_pos, y_pos);
                }
                 currentTeam = 255;
                break;

            case CAMERA_PAN_UP:
                /*if (y_pos <= (- height)) {
                    state = CAMERA_IDLE;
                    break;
                }*/

                reposition = true;
                y_top += (1080.0)/CAMERA_PAN_DELTA;
                y_bottom += (1080.0)/CAMERA_PAN_DELTA;
                y_pan_count++;
                if (y_pan_count == (CAMERA_PAN_DELTA)) {
                    state = CAMERA_IDLE;
                    y_pan_count = 0;
                    y_pos -= 1.0;
                    printf("modified x/y_pos = [%f, %f\n", x_pos, y_pos);
                }
                 currentTeam = 255;
                break;

            case CAMERA_ZOOM_IN:
                if (zoom_level <= 1.0) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                //printf("prev zoom count: %d, x_right: %f, y_bottom:%f, zoom_level: %f\n",zoom_count,x_right,y_bottom,zoom_level);
                this->x_right -= zoom_count *(1920.0)/CAMERA_ZOOM_DELTA;
                this->y_bottom += zoom_count * (1080.0)/CAMERA_ZOOM_DELTA;
                zoom_count++;
                //printf("next zoom count: %d, x_right: %f, y_bottom:%f\n",zoom_count,x_right,y_bottom);
                if (zoom_count == (CAMERA_ZOOM_DELTA)) {
                    //state = CAMERA_IDLE;
                    zoom_count --;
                    zoom_level = 2.0;
                    x_pos -= 1.0;
                    y_pos -= 1080.0/1920;
                    printf("modified x/y_pos = [%f, %f\n", x_pos, y_pos);
                }
                 currentTeam = 255;
                break;


            case CAMERA_ZOOM_OUT:
                if (zoom_level >= width) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                x_right += zoom_count *(1920.0)/CAMERA_ZOOM_DELTA;
                y_bottom -= zoom_count *(1080.0)/CAMERA_ZOOM_DELTA;
                zoom_count--;

                if (zoom_count == (-CAMERA_ZOOM_DELTA)) {
                    //state = CAMERA_IDLE;
                    zoom_count ++;
                    zoom_level = 3.0;
                    x_pos += 1.0;
                    y_pos += 1080.0/1920;
                    printf("modified x/y_pos = [%f, %f\n", x_pos, y_pos);
                }
                 currentTeam = 255;
                break;
            case CAMERA_NEXT_LINK:
                myTeam = &myGame->myTeams[currentTeam];
                myTeam->cur_link++;
                if (currentTeam<4 && myTeam->cur_link >= myTeam->myLinks.size()){
                    myTeam->cur_link = 0;
                }
                if (myTeam->cmds_done != true) {
                    reposition = true;
                    myLink = &myGame->myTeams[myTeam->id].myLinks[myTeam->cur_link];
                    x_pos = (float)myLink->room_x;
                    y_pos = (float)myLink->room_y;

                    zoom_level = 1.0;
                    x_pan_count = 0;
                    y_pan_count = 0;
                    zoom_count = 0;

                    x_left = (1080.0/2.0)*(2*x_pos-width)-420.0;
                    x_right = x_left + 1920.0;
                    y_top = (1080.0/2.0)*(height-2*y_pos);
                    y_bottom = y_top - 1080.0;
                    if (currentTeam ==0 ) {
                        state = CAMERA_TEAM_1;
                    }else if (currentTeam ==1 ) {
                        state = CAMERA_TEAM_2;
                    }else if (currentTeam ==2 ) {
                        state = CAMERA_TEAM_3;
                    }else if (currentTeam ==3 ) {
                        state = CAMERA_TEAM_4;
                    }
                }
                else {
                    state = CAMERA_IDLE;
                }
                break;
            case CAMERA_PREV_LINK:
                myTeam = &myGame->myTeams[currentTeam];
                myTeam->cur_link++;
                if (currentTeam < 4 && myTeam->cur_link >= myTeam->myLinks.size()){
                    myTeam->cur_link = 0;
                }
                if (myTeam->cmds_done != true) {
                    reposition = true;
                    myLink = &myGame->myTeams[myTeam->id].myLinks[myTeam->cur_link];
                    x_pos = (float)myLink->room_x;
                    y_pos = (float)myLink->room_y;

                    zoom_level = 1.0;
                    x_pan_count = 0;
                    y_pan_count = 0;
                    zoom_count = 0;

                    x_left = (1080.0/2.0)*(2*x_pos-width)-420.0;
                    x_right = x_left + 1920.0;
                    y_top = (1080.0/2.0)*(height-2*y_pos);
                    y_bottom = y_top - 1080.0;
                    if (currentTeam ==0 ) {
                        state = CAMERA_TEAM_1;
                    }else if (currentTeam ==1 ) {
                        state = CAMERA_TEAM_2;
                    }else if (currentTeam ==2 ) {
                        state = CAMERA_TEAM_3;
                    }else if (currentTeam ==3 ) {
                        state = CAMERA_TEAM_4;
                    }
                }
                else {
                    state = CAMERA_IDLE;
                }
                break;
            case CAMERA_IDLE:
                reposition = false;
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
