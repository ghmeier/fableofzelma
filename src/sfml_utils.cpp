/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/

/*****************************************************************************
 * sfml_utils.cpp - graphics and audio functionality using the SFML-2.1
 * library with OpenGL.
 *
 *
 * NOTES:
 * 1/24/14 by JAZ::Design created.
 *****************************************************************************/

#include "fableofzelma.hpp"

namespace foz {


    /*****************************************************************************
    * Function: Game::processEvents
    * Description: Processes keyboard and other SFML events.
    *****************************************************************************/
    void Game::processEvents() {

        sf::Event event;
        while (myWindow.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
/*                    case sf::Keyboard::Num1:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
                            myStatus.scores[0]--;
                        else
                            myStatus.scores[0]++;
                        break;
                    case sf::Keyboard::Num2:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
                            myStatus.scores[1]--;
                        else
                            myStatus.scores[1]++;
                        break;
                    case sf::Keyboard::Num3:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
                            myStatus.scores[2]--;
                        else
                            myStatus.scores[2]++;
                        break;
                    case sf::Keyboard::Num4:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
                            myStatus.scores[3]--;
                        else
                            myStatus.scores[3]++;
                        break;*/
                    case sf::Keyboard::Left:
                        if (myCamera.state == CAMERA_IDLE)
                            myCamera.state = CAMERA_PAN_LEFT;
                        break;
                    case sf::Keyboard::Right:
                        if (myCamera.state == CAMERA_IDLE)
                            myCamera.state = CAMERA_PAN_RIGHT;
                        break;
                    case sf::Keyboard::Up:
                        if (myCamera.state == CAMERA_IDLE)
                            myCamera.state = CAMERA_PAN_UP;
                        break;
                    case sf::Keyboard::Down:
                        if (myCamera.state == CAMERA_IDLE)
                            myCamera.state = CAMERA_PAN_DOWN;
                        break;
                    case sf::Keyboard::Equal:
                    case sf::Keyboard::Add:
                        if (myCamera.state == CAMERA_IDLE)
                            myCamera.state = CAMERA_ZOOM_IN;
                        break;
                    case sf::Keyboard::Dash:
                    case sf::Keyboard::Subtract:
                        if (myCamera.state == CAMERA_IDLE)
                            myCamera.state = CAMERA_ZOOM_OUT;
                        break;
                    case sf::Keyboard::Escape:
                    case sf::Keyboard::Q:
                        myWindow.close();
                        break;
                    case sf::Keyboard::E:
                        //myStatus.mode = GAME_END;
                        break;
                    case sf::Keyboard::R:
                        reset();
                        break;
                    case sf::Keyboard::S:
                    case sf::Keyboard::Return:
                        //if (myStatus.mode == DEMO_MID)
                        //    myStatus.mode = DEMO_END;
                        break;
                    default:
                        break;
                }

            }
        }

    }

    /*****************************************************************************
    * Function: Game::loadResources
    * Description: Loads the various texture maps and audio files used in the
    * application.
    *****************************************************************************/
    void Game::loadResources() {

        /* Load the sound buffers. These can safely fit in memory */
        if (myConfig.debug_level > 3)
            printf("Loading sound buffers...");

        /* Load the various textures from the appropriate files */
        sf::Image pixels;
        GLuint myTextureHandles[NUM_TEXTURES];
        glGenTextures(NUM_TEXTURES, myTextureHandles);
        if (myConfig.debug_level > 3)
            printf("Loading texture data...");


        for (int16_t i = 0; i < NUM_TEXTURES; i++) {
            if (!pixels.loadFromFile(texFiles[i]))
                raise_error(ERR_NOFILE1, texFiles[i].c_str());
            myTextures[i].scale = 1.0;
            myTextures[i].width = pixels.getSize().x;
            myTextures[i].height = pixels.getSize().y;
            myTextures[i].texHandle = myTextureHandles[i];

            glBindTexture(GL_TEXTURE_2D, myTextures[i].texHandle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myTextures[i].width,
            myTextures[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.getPixelsPtr());
        }

        /* Bind the individual spriteMaps */
        myTextures[TEX_BASIC_ROOM].spriteMap = room_object_spriteMap;
        myTextures[TEX_BLUE_LINK].spriteMap = link_object_spriteMap;
        myTextures[TEX_GREEN_LINK].spriteMap = link_object_spriteMap;
        myTextures[TEX_PURPLE_LINK].spriteMap = link_object_spriteMap;
        myTextures[TEX_RED_LINK].spriteMap = link_object_spriteMap;
        myTextures[TEX_FONTS].spriteMap = font_spriteMap;

        if (myConfig.debug_level > 3)
            printf("done.\n");

    }


    /*****************************************************************************
    * Function: Game::initSFML()
    * Description: Initializes the SFML environment for handling graphics and
    * audio.
    *****************************************************************************/
    void Game::initSFML() {

        // Attempt to create the user-specified window
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
        bool foundMode = false;

        if (myConfig.debug_level > 10) {
            printf("\nAvailable video modes:\n");
        }
        for (std::size_t i = 0; i < modes.size(); i++) {
            sf::VideoMode mode = modes[i];
            if (myConfig.debug_level > 10) {
                printf("\tMode #%u: [%u,%u] - %ubpp\n", i, mode.width, mode.height, mode.bitsPerPixel);
            }
            if ((mode.width == myConfig.screen_width) && (mode.height == myConfig.screen_height)
                && (mode.bitsPerPixel == myConfig.screen_depth))
                foundMode = true;
        }

        if (foundMode == false) {
            if (myConfig.debug_level > 0) {
                printf("Warning: specified [%u, %u] resolution not available\n",
                       myConfig.screen_width, myConfig.screen_height);
                printf("Substituting with [%u, %u]\n", modes[0].width, modes[0].height);
                myConfig.screen_width = modes[0].width;
                myConfig.screen_height = modes[0].height;
                myConfig.screen_depth = modes[0].bitsPerPixel;
            }
        }

        myWindow.create(sf::VideoMode(myConfig.screen_width, myConfig.screen_height,
                        myConfig.screen_depth), WINDOW_TITLE, sf::Style::Fullscreen);

        mySettings = myWindow.getSettings();
        if (myConfig.debug_level > 3) {
            printf("\n%s Window configuration: \n", WINDOW_TITLE);
            printf("\tDepth bits: %u\n", mySettings.depthBits);
            printf("\tStencil bits: %u\n", mySettings.stencilBits);
            printf("\tAnti-Aliasing level: %u\n", mySettings.antialiasingLevel);
            printf("\tOpenGL v%u.%u\n\n", mySettings.majorVersion, mySettings.minorVersion);
        }

        myWindow.setVerticalSyncEnabled(true);
        //myWindow.setFramerateLimit(FRAME_RATE);


        /* Configure OpenGL default state */
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClearDepth(BACK_DEPTH);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_TEXTURE_2D);

        glAlphaFunc(GL_GREATER, 64.0/255.0);
        glEnable(GL_ALPHA_TEST);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        myCamera.init(myWorld);

    }

    /*****************************************************************************
    * Function: Game::drawScoreboard()
    * Description: Draws the Game Scoreboards for the four teams
    *****************************************************************************/
void Game::drawScoreboard(){

    #define NUMBER_WIDTH 70
    #define NUMBER_HEIGHT 90
    #define LETTER_WIDTH 80
    #define LETTER_HEIGHT 88

        float texCoords[6];
        float baseX = -1480.0, baseY = 100.0;

        // For setting up the Scoreboard Camera
        float x_left = -1920.0/2.0*myWorld.width;
        float x_right = 1920.0/2.0*myWorld.width;
        float y_bottom = -1080.0/2.0*myWorld.height;
        float y_top = 1080.0/2.0*myWorld.height;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(x_left, x_right, y_bottom, y_top, FRONT_DEPTH, -BACK_DEPTH);


        // Hard-Coded Scores and Names
        myStatus.scores[0] = 4001;
        myStatus.scores[1] = -234;
        myStatus.scores[2] = 7587;
        myStatus.scores[3] = 8864;

        myTeams[0].name = "Team 1";
        myTeams[1].name = "Team 2";
        myTeams[2].name = "Team 3";
        myTeams[3].name = "Team 4";

        /* Draw the team names */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_FONTS].texHandle);
        char name[9];
        glBegin(GL_QUADS);
            for (uint8_t i = 0; i < 4; i++) {
                snprintf(name, 9, "%-8s", myTeams[i].name);
                uint16_t font_num;
                for (uint8_t j = 0; j < 8; j++) {
                    font_num = NUM_FONTS;
                    if (name[j] >= 'A' && name[j] <= 'Z') {
                        font_num = name[j] - 'A' + LETTER_A;
                    }
                    if (name[j] >= 'a' && name[j] <= 'z') {
                        font_num = name[j] - 'a' + LETTER_a;
                    }
                    if (name[j] >= '0' && name[j] <= '9') {
                    font_num = name[j] - '0' + LETTER_0;
                    }

                    if (font_num != NUM_FONTS) {
                        getTexCoords(TEX_FONTS, font_num, texCoords);
                        glTexCoord2d(texCoords[0], texCoords[3]);
                        glVertex3f(baseX + j*LETTER_WIDTH, baseY + LETTER_HEIGHT, FONT_DEPTH);
                        glTexCoord2d(texCoords[2], texCoords[3]);
                        glVertex3f(baseX+(j+1)*LETTER_WIDTH, baseY + LETTER_HEIGHT, FONT_DEPTH);
                        glTexCoord2d(texCoords[2], texCoords[1]);
                        glVertex3f(baseX+(j+1)*LETTER_WIDTH, baseY, FONT_DEPTH);
                        glTexCoord2d(texCoords[0], texCoords[1]);
                        glVertex3f(baseX + j*LETTER_WIDTH, baseY, FONT_DEPTH);
                    }
                }
            switch(i){
            case 0:
                baseX = 2900;
                baseY = 100;
                break;
            case 1:
                baseY = -2200;
                baseX = -1480;
                break;
            case 2:
                baseY = -2200;
                baseX = 2900;
                break;

                }
            }

    /* Draw the scores */
     baseX = -1480.0;
     baseY = 0.0;
        char digits[4];
            for (uint8_t i = 0; i < 4; i++) {

                sprintf(digits,"%3d",myStatus.scores[i]);

                        /* Draw Rupee Symbol
                        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_RUPEES].texHandle);
                        getTexCoords(TEX_RUPEES, i, texCoords);

                    glBegin(GL_QUADS);
                        glTexCoord2d(texCoords[0], texCoords[1]);
                        glVertex3f(baseX, baseY - NUMBER_HEIGHT, FONT_DEPTH);
                        glTexCoord2d(texCoords[2], texCoords[1]);
                        glVertex3f(baseX+NUMBER_WIDTH, baseY - NUMBER_HEIGHT, FONT_DEPTH);
                        glTexCoord2d(texCoords[2], texCoords[3]);
                        glVertex3f(baseX+NUMBER_WIDTH, baseY, FONT_DEPTH);
                        glTexCoord2d(texCoords[0], texCoords[3]);
                        glVertex3f(baseX, baseY, FONT_DEPTH);
                    glEnd();
                    */

                for (uint8_t j = 0; j < 4; j++) {
                glBindTexture(GL_TEXTURE_2D, myTextures[TEX_FONTS].texHandle);
                    if (digits[j] == '-') {
                        getTexCoords(TEX_FONTS, LETTER_NEG, texCoords);
                    }
                    else if (digits[j] != ' ') {
                        getTexCoords(TEX_FONTS, digits[j] - '0' + LETTER_0, texCoords);
                    }
                    if (digits[j] != ' ') {

                    glBegin(GL_QUADS);
                        glTexCoord2d(texCoords[0], texCoords[1]);
                        glVertex3f(baseX+NUMBER_WIDTH*(j+1), baseY - NUMBER_HEIGHT, FONT_DEPTH);
                        glTexCoord2d(texCoords[2], texCoords[1]);
                        glVertex3f(baseX+NUMBER_WIDTH*(j+2), baseY - NUMBER_HEIGHT, FONT_DEPTH);
                        glTexCoord2d(texCoords[2], texCoords[3]);
                        glVertex3f(baseX+NUMBER_WIDTH*(j+2), baseY, FONT_DEPTH);
                        glTexCoord2d(texCoords[0], texCoords[3]);
                        glVertex3f(baseX+NUMBER_WIDTH*(j+1), baseY, FONT_DEPTH);
                    glEnd();
                    }
                }



                switch(i){
            case 0:
                baseX = 2900;
                break;
            case 1:
                baseY = -2300;
                baseX = -1480;
                break;
            case 2:
                baseX = 2900;
                break;
                }

                }
    // Reset Camera
    myCamera.update(true);

}

} // namespace foz
