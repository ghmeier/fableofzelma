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
    * Function: Object::Object
    * Description: Object class constructor. Creates a set of linked lists
    * between parent and child.
    *****************************************************************************/
    uint8_t World_JZ::compile(char *fname) {

            char currentline[256];
            //std::ifstream initFile("/scripts/world1.zmf");
            FILE * initFile;
            initFile = fopen("/scripts/world1.zmf","rb");
            char currentWord[256];
            char delim = ' ';
            std::cout <<"**************************\n*******************"<<std::endl;
           while (!feof(initFile)){
                //initFile.getline(currentline,256,' ');
                fscanf(initFile,"%s",currentWord);
                std::cout << currentWord << std::endl;
                if (strstr(".size",currentWord)) {
                        fscanf(initFile,"%d %d",World_JZ::width,World_JZ::height);
                        std::cout << World_JZ::width << " "<< World_JZ::height <<std::endl;
                }
                /*if (strstr(currentline,"totalDimension")!=NULL) {
                    initFile.getline(currentline,256,',');
                    xRooms = atoi(currentline);
                    initFile.getline(currentline,256,',');
                    yRooms = atoi(currentline);

                    if (xRooms<=0 || yRooms<=0) {
                        std::cout << "Incorrect World Dimensions of " << xRooms << "," << yRooms << ". Format dimensions: totalDimension 5 5"<< std::endl;
                    } else {
                        std::cout << "World Dimensions: " << xRooms << "," << yRooms << std::endl;
                    }
                }else if (strstr(currentline,"roomPosition")!=NULL) {
                    //std::cout << "Creating Room " << currentline<< std::endl;
                    int roomX=0,roomY=0;
                    initFile.getline(currentline,256,',');
                    //roomX = atoi(currentline);
                    initFile.getline(currentline,256,',');
                   // roomY = atoi(currentline);
                    initFile.getline(currentline,256,' ');


                    if (strstr(currentline,"roomSize")!=NULL && roomX>=0 && roomX<xRooms && roomY>=0 && roomY<yRooms) {
                        //std::cout << "Position "<<roomX << "," << roomY<< std::endl;
                        int xDim = 0, yDim = 0;
                        initFile.getline(currentline,256,',');
                        //std::cout << currentline << std::endl;
                        xDim = atoi(currentline);
                        initFile.getline(currentline,256,',');
                        //std::cout << currentline << std::endl;
                        yDim = atoi(currentline);
                        int roomLayout[xDim][yDim];
                        for (int i=0;i<xDim;i++) {
                            for (int j=0;j<yDim;j++) {
                                initFile.getline(currentline,256,',');
                                roomLayout[i][j] = atoi(currentline);
                                //std::cout << roomLayout[i][j] << " ";
                            }
                            //std::cout << std::endl;
                        }
                        Room r(xDim,yDim);
                        r.setLayout(*roomLayout);
                        rooms.push_back(r);

                    }
                }
            */
           }
            fclose(initFile);
            return 0;
        }

        int World_JZ::getWidth() {
            return World_JZ::width;
        }

        int World_JZ::getHeight() {
            return World_JZ::height;
        }

        void World_JZ::addRoom(Room_JZ r) {
            //rooms.push_back(r);
        }

} // namespace foz
