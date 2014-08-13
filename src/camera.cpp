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
        x_pos = 0.0;
        y_pos = 0.0;
        zoom_level = 0.0;

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
//        printf("x_pos: %.1f\n",x_pos);
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

                    zoom_level = 2.0;
                    x_pan_count = 0;
                    y_pan_count = 0;
                    zoom_count = 0;

                    x_left = (1080.0/2.0)*(2*x_pos-width)-420.0;
                    x_right = x_left + 1920.0;
                    y_top = (1080.0/2.0)*(height-2*y_pos);
                    y_bottom = y_top - 1080.0;
                    state = CAMERA_IDLE;
                }
                else {
                    state = CAMERA_IDLE;
                }
                break;

            case CAMERA_PAN_RIGHT:

                if (x_pos >= zoom_level  || zoom_level == 0.0) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                x_left += (1080.0)/CAMERA_PAN_DELTA;
                x_right += (1080.0)/CAMERA_PAN_DELTA;
                x_pan_count--;
                if (x_pan_count == (-CAMERA_PAN_DELTA)) {
                    printf("width: %d, x_pos: %2.1f\n",width,x_pos);
                    state = CAMERA_IDLE;
                    x_pan_count = 0;
                    x_pos += 1.0;
                }
                currentTeam = 255;
                break;

            case CAMERA_PAN_LEFT:
                if (x_pos <= 0 || zoom_level == 0.0) {
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
                    x_pos -= 1.0;
                    printf("modified x/y_pos = [%f, %f]\n", x_pos, y_pos);
                }
                 currentTeam = 255;
                break;

            case CAMERA_PAN_DOWN:
                if (y_pos >= (zoom_level)  || zoom_level == 0.0) {
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
                    printf("modified x/y_pos = [%f, %f]\n", x_pos, y_pos);
                }
                 currentTeam = 255;
                break;

            case CAMERA_PAN_UP:
                if (y_pos <= 0  || zoom_level == 0.0) {
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
                    printf("modified x/y_pos = [%f, %f\n", x_pos, y_pos);
                }
                 currentTeam = 255;
                break;

            case CAMERA_ZOOM_IN:
                if (zoom_level >= width-1) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                this->x_right -= (1500.0)/CAMERA_ZOOM_DELTA;
                this->x_left += (420.0)/CAMERA_ZOOM_DELTA;
                this->y_bottom += (1080.0)/CAMERA_ZOOM_DELTA;
                zoom_count++;

                if (zoom_count >= (CAMERA_ZOOM_DELTA)) {
                    state = CAMERA_IDLE;
                    zoom_level ++;
                    //x_pos -= 1.0;
                    //y_pos -= 1080.0/1920;
                    printf("zoom in x_pos,y_pos = [%2.1f, %2.1f]\n", x_pos, y_pos);
                    zoom_count = 0;
                }
                 currentTeam = 255;
                break;


            case CAMERA_ZOOM_OUT:
                if (zoom_level <=0.0) {
                    state = CAMERA_IDLE;
                    break;
                }

                reposition = true;
                //printf("prev zoom count: %d, x_right: %6.3f, y_bottom: %6.3f, zoom_level: %1.3f\n",zoom_count,x_right,y_bottom,zoom_level);
                if (x_pos > 0.0){
                    x_left -= (1500.0 * (1) )/CAMERA_ZOOM_DELTA;
                    this->x_right += (420.0 * (1) )/CAMERA_ZOOM_DELTA;
                }else {
                    x_right += (1500.0  )/CAMERA_ZOOM_DELTA;
                    this->x_left -= (420.0  )/CAMERA_ZOOM_DELTA;
                }

                if (y_pos > 0.0){
                    y_top += (1080.0* (1) )/CAMERA_ZOOM_DELTA;
                }else {
                    y_bottom -= (1080.0 )/CAMERA_ZOOM_DELTA;
                }
                //printf("next zoom count: %d, x_right: %6.3f, y_bottom: %6.3f\n\n",zoom_count,x_right,y_bottom);
                zoom_count++;

                if (zoom_count >= (CAMERA_ZOOM_DELTA)) {
                    state = CAMERA_IDLE;
                    zoom_level --;

                    printf("zoom out x_pos,y_pos = [%2.1f, %2.1f]\n", x_pos, y_pos);
                    if (x_pos >0.0){
                        x_pos -= 1.0;
                    }
                    if (y_pos >0.0){
                        y_pos -= 1.0;
                    }
                    //y_pos += 1080.0/1920;

                    zoom_count = 0;
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
                //reposition = false;
                //printf("idle zoom count: %d, x_right: %6.3f, y_bottom: %6.3f, zoom_level: %1.3f\n",zoom_count,x_right,y_bottom,zoom_level);
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
