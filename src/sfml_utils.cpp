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
//        glClearDepth(BACK_DEPTH);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
//        glOrtho(-1032.5, 1032.5, -581.0, 581.0, FRONT_DEPTH, -BACK_DEPTH);
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_TEXTURE_2D);

        glAlphaFunc(GL_GREATER, 0.425);
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    }

    /*****************************************************************************
    * Function: Game::testDraw
    * Description: Testing the drawing
    *****************************************************************************/

    void Game::testDraw() {

        float texCoords[6];

        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_BASIC_ROOM].texHandle);
        getTexCoords(TEX_BASIC_ROOM, WHOLE_ROOM, texCoords);
        glBegin(GL_QUADS);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(-1, -1, 0);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(1, -1, 0);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(1, 1, 0);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(-1, 1, 0);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_BLUE_LINK].texHandle);
        getTexCoords(TEX_BLUE_LINK, LINK_SLASH_NORTH_4, texCoords);
        glBegin(GL_QUADS);
            glTexCoord2d(texCoords[0], texCoords[1]);
            glVertex3f(0, -.2, 0);
            glTexCoord2d(texCoords[2], texCoords[1]);
            glVertex3f(.1, -.2, 0);
            glTexCoord2d(texCoords[2], texCoords[3]);
            glVertex3f(.1, 0, 0);
            glTexCoord2d(texCoords[0], texCoords[3]);
            glVertex3f(0, 0, 0);
        glEnd();

    }


} // namespace foz
