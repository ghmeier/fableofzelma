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
                    case sf::Keyboard::Num1:
                        myCamera.state = CAMERA_TEAM_1;
                        break;
                    case sf::Keyboard::Num2:
                        myCamera.state = CAMERA_TEAM_2;
                        break;
                    case sf::Keyboard::Num3:
                        myCamera.state = CAMERA_TEAM_3;
                        break;
                    case sf::Keyboard::Num4:
                        myCamera.state = CAMERA_TEAM_4;
                        break;
                    case sf::Keyboard::Z:
                        myCamera.state = CAMERA_PREV_LINK;
                        break;
                   case sf::Keyboard::X:
                        myCamera.state = CAMERA_NEXT_LINK;
                        break;
                    case sf::Keyboard::Left:
                        if (myCamera.state == CAMERA_IDLE || myCamera.state == CAMERA_TEAM_1 || myCamera.state == CAMERA_TEAM_2 || myCamera.state == CAMERA_TEAM_3 || myCamera.state == CAMERA_TEAM_4)
                            myCamera.state = CAMERA_PAN_LEFT;
                        break;
                    case sf::Keyboard::Right:
                        if (myCamera.state == CAMERA_IDLE || myCamera.state == CAMERA_TEAM_1 || myCamera.state == CAMERA_TEAM_2 || myCamera.state == CAMERA_TEAM_3 || myCamera.state == CAMERA_TEAM_4)
                            myCamera.state = CAMERA_PAN_RIGHT;
                        break;
                    case sf::Keyboard::Up:
                        if (myCamera.state == CAMERA_IDLE || myCamera.state == CAMERA_TEAM_1 || myCamera.state == CAMERA_TEAM_2 || myCamera.state == CAMERA_TEAM_3 || myCamera.state == CAMERA_TEAM_4)
                            myCamera.state = CAMERA_PAN_UP;
                        break;
                    case sf::Keyboard::Down:
                        if (myCamera.state == CAMERA_IDLE || myCamera.state == CAMERA_TEAM_1 || myCamera.state == CAMERA_TEAM_2 || myCamera.state == CAMERA_TEAM_3 || myCamera.state == CAMERA_TEAM_4)
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
                        if (myStatus.mode == GAME_MID){
                            myStatus.mode = GAME_MID_NODRAW;
                        }else if (myStatus.mode == GAME_MID_NODRAW){
                            myStatus.mode = GAME_MID;
                        }
                        break;
                    case sf::Keyboard::R:
                        myStatus.mode = GAME_RESET;
                        break;
                    case sf::Keyboard::P:
                        if (myStatus.mode == GAME_PAUSE){
                            myStatus.mode = GAME_MID;
                        }else{
                            myStatus.mode = GAME_PAUSE;
                        }
                        break;
                    case sf::Keyboard::S:
                    case sf::Keyboard::Return:
                        if (myStatus.mode == GAME_START){
                            myStatus.mode = GAME_MID;
                        }else if (myStatus.mode == GAME_PAUSE){
                            myStatus.mode = GAME_MID;
                        }else if (myStatus.mode == GAME_MID || myStatus.mode == GAME_MID_NODRAW){
                            myStatus.mode = GAME_PAUSE;
                        }
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
        myTextures[TEX_RUPEE].spriteMap = rupee_spriteMap;
        myTextures[TEX_ENEMIES].spriteMap = enemy_spriteMap;
        myTextures[TEX_NAVI].spriteMap = navi_object_spriteMap;
        if (myConfig.debug_level > 3)
            printf("done.\n");

    }


    /*****************************************************************************
    * Function: Game::initSFML()
    * Description: Initializes the SFML environment for handling graphics and
    * audio.
    *****************************************************************************/
    void Game::initSFML() {

        if (myConfig.debug_level > 3) {
            printf("Loading Sound buffers...\n");
        }

        for (uint16_t i=0; i<NUM_SFX;i++) {
            if (!mySoundBuffers[i].loadFromFile(sfxFiles[i])) {
                raise_error(ERR_AUDIO,sfxFiles[i].c_str());
            }
        }

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
        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        myCamera.init(&myWorld);

        NUMBER_WIDTH = 70.0/3.0 * myCamera.width;
        NUMBER_HEIGHT = 30.0 * myCamera.width;
        LETTER_WIDTH = 20.0 * myCamera.width;
        LETTER_HEIGHT = 88/3.0 * myCamera.width;

    }

    /*****************************************************************************
    * Function: Game::drawScoreboard()
    * Description: Draws the Game Scoreboards for the four teams
    *****************************************************************************/
    void Game::drawScoreboard(){
        float texCoords[6];
        float baseX, baseY;

        // Reset Camera
        Camera * prevCamera = new Camera();
        prevCamera->currentTeam = myCamera.currentTeam;
        prevCamera->height = myCamera.height;
        prevCamera->state  = myCamera.state;
        prevCamera->width = myCamera.width;
        prevCamera->x_left = myCamera.x_left;
        prevCamera->x_pan_count = myCamera.x_pan_count;
        prevCamera->x_pos = myCamera.x_pos;
        prevCamera->x_right = myCamera.x_right;
        prevCamera->y_bottom = myCamera.y_bottom;
        prevCamera->y_pan_count = myCamera.y_pan_count;
        prevCamera->y_pos = myCamera.y_pos;
        prevCamera->y_top = myCamera.y_top;
        prevCamera->zoom_count = myCamera.zoom_count;
        prevCamera->zoom_level = myCamera.zoom_level;
        //CAMERA_ENUM prev_camera_state = myCamera.state;
        myCamera.state = CAMERA_INIT;
        myCamera.update(true);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-1920.0/2.0*(myWorld.width), 1080.0/2.0*(myWorld.height), 0.0);


        // Hard-Coded Scores and Names
        myStatus.scores[0] = myTeams[0].score;
        myStatus.scores[1] = myTeams[1].score;
        myStatus.scores[2] = myTeams[2].score;
        myStatus.scores[3] = myTeams[3].score;

        /* Draw the team names */
        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_FONTS].texHandle);
        char name[9];
        for (uint8_t i = 0; i < 4; i++) {

            switch(i) {
                case 0:
                    baseX = 3*LETTER_WIDTH;
                    baseY = -LETTER_HEIGHT;
                    break;
                case 1:
                    baseX = 3*LETTER_WIDTH;
                    baseY = -1080.0*(myWorld.height)/1.5-LETTER_HEIGHT;
                    break;
                case 2:
                    baseX = 1920.0*myWorld.width - 11*LETTER_WIDTH;
                    baseY = -LETTER_HEIGHT;
                    break;
                case 3:
                default:
                    baseX = 1920.0*myWorld.width - 11*LETTER_WIDTH;
                    baseY = -1080.0*(myWorld.height)/1.5-LETTER_HEIGHT;
                    break;
            }

            snprintf(name, 9, "%-8s", myTeams[i].name);
            std::stringstream s;
            s << name;
            uint16_t font_num;
            drawWord(s.str(),baseX,baseY);
        }

        /* Draw the scores */
        for (uint8_t i = 0; i < 4; i++) {

            switch(i) {
                case 0:
                    baseX = 3*LETTER_WIDTH;
                    baseY = -2.5*LETTER_HEIGHT;
                    break;
                case 1:
                    baseX = 3*LETTER_WIDTH;
                    baseY = -1080.0*(myWorld.height)/1.5-2.5*LETTER_HEIGHT;
                    break;
                case 2:
                    baseX = 1920.0*myWorld.width - 11*LETTER_WIDTH;
                    baseY = -2.5*LETTER_HEIGHT;
                    break;
                case 3:
                default:
                    baseX = 1920.0*myWorld.width - 11*LETTER_WIDTH;
                    baseY = -1080.0*(myWorld.height)/1.5-2.5*LETTER_HEIGHT;
                    break;
            }

            // Gives Rupee Symbol Animation
            static int RUPEE_CYCLE = 0;
            static int RUPEE_CYCLE_SLOW = 0;

            RUPEE_CYCLE_SLOW++;
            if((RUPEE_CYCLE_SLOW % 60) == 0) {
                RUPEE_CYCLE++;
                RUPEE_CYCLE %= 3;
            }
            RUPEE_CYCLE_SLOW %= 6000;

            /* Draw Rupee Symbol */
            glBindTexture(GL_TEXTURE_2D, myTextures[TEX_RUPEE].texHandle);
            getTexCoords(TEX_RUPEE, 3*i + RUPEE_CYCLE, texCoords);

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

            std::stringstream digitStream;
            digitStream << "x" << myStatus.scores[i];
            glBindTexture(GL_TEXTURE_2D, myTextures[TEX_FONTS].texHandle);
            drawWord(digitStream.str(),baseX+NUMBER_WIDTH,baseY);
        }

        /* Draw the Links remaining */
        char links[4];
        for (uint8_t i = 0; i < 4; i++) {

            switch(i) {
                case 0:
                    baseX = 3*LETTER_WIDTH;
                    baseY = -4.0*LETTER_HEIGHT;
                    break;
                case 1:
                    baseX = 3*LETTER_WIDTH;
                    baseY = -1080.0*(myWorld.height)/1.5-4.0*LETTER_HEIGHT;
                    break;
                case 2:
                    baseX = 1920.0*myWorld.width - 11*LETTER_WIDTH;
                    baseY = -4.0*LETTER_HEIGHT;
                    break;
                case 3:
                default:
                    baseX = 1920.0*myWorld.width - 11*LETTER_WIDTH;
                    baseY = -1080.0*(myWorld.height)/1.5-4.0*LETTER_HEIGHT;
                    break;
            }

            /* Draw Link Symbol */
            glBindTexture(GL_TEXTURE_2D, myTextures[myGame->myTeams[i].myLinks[myTeams[i].cur_link].texfile].texHandle);
            getTexCoords(TEX_BLUE_LINK, LINK_WALKING_SOUTH_1, texCoords);

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


            glBindTexture(GL_TEXTURE_2D, myTextures[TEX_FONTS].texHandle);
            std::stringstream linkStream;
            linkStream << "x" << myGame->myTeams[i].link_left;
            drawWord(linkStream.str(),baseX+LETTER_WIDTH + 10,baseY);
        }

        /* Draw the Time Remaining */
        char time_word[5] = "Time";
        baseX = 1920.0*myWorld.width - 10*LETTER_WIDTH;
        baseY = -1080.0*myCamera.height/3.0 + NUMBER_HEIGHT;
        drawWord("Time",baseX,baseY);

        int32_t time_ms;
        if (myStatus.time_ms >= 6000000) {
            time_ms = 5999999;
        }
        else {
            time_ms = myStatus.time_ms;
        }

        uint32_t fullmins = time_ms / 600000;
        uint32_t mins = (time_ms / 60000) % 10;
        uint32_t fullsecs = (time_ms % 60000) / 10000;
        uint32_t secs = ((time_ms % 60000) / 1000) % 10;
        uint32_t tenths = (time_ms % 1000) / 100;

        baseX = 1920.0*myWorld.width - 11*LETTER_WIDTH;
        baseY  = -1080.0*myCamera.height/3.0 - 1.5*NUMBER_HEIGHT;

        baseX += NUMBER_WIDTH;
        if ((fullmins > 0) || (mins > 0)) {
            std::stringstream timeStream;
            timeStream << mins << ":" << fullsecs << secs;
            drawWord(timeStream.str(),baseX,baseY);
        }
        else {
            // Otherwise, draw the sec.tenths scoreboard mode
            std::stringstream secStream;
            if (fullsecs > 0) {
                secStream << fullsecs;
            }
            secStream << secs << "." << tenths;
            drawWord(secStream.str(),baseX,baseY);
        }

        drawMinimap(myCamera.zoom_level,myCamera.x_pos ,myCamera.y_pos);

        glBindTexture(GL_TEXTURE_2D, myTextures[TEX_FONTS].texHandle);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);

        getTexCoords(TEX_FONTS,GRADIENT,texCoords);
        glTexCoord2d(texCoords[0], texCoords[1]);
        glVertex3f(0, 10, FONT_DEPTH+2);
        glTexCoord2d(texCoords[2], texCoords[1]);
        glVertex3f(-myCamera.x_left/2.5,10, FONT_DEPTH+2);
        glTexCoord2d(texCoords[2], texCoords[3]);
        glVertex3f(-myCamera.x_left/2.5,-1080*myCamera.height-10, FONT_DEPTH+2);
        glTexCoord2d(texCoords[0], texCoords[3]);
        glVertex3f(0,-1080*myCamera.height-10, FONT_DEPTH+2);

        getTexCoords(TEX_FONTS,GRADIENT,texCoords);
        glTexCoord2d(texCoords[2], texCoords[3]);
        glVertex3f(1920.0*myCamera.width-myCamera.x_right/2.5, 10, FONT_DEPTH+2);
        glTexCoord2d(texCoords[0], texCoords[3]);
        glVertex3f(1920.0*myCamera.width +10,10, FONT_DEPTH+2);
        glTexCoord2d(texCoords[0], texCoords[1]);
        glVertex3f(1920*myCamera.width+10,-1080*myCamera.height-10, FONT_DEPTH+2);
        glTexCoord2d(texCoords[2], texCoords[1]);
        glVertex3f(1920.0*myCamera.width-myCamera.x_right/2.5,-1080*myCamera.height-10, FONT_DEPTH+2);

        glEnd();

        if (myStatus.mode == GAME_PAUSE){
            std::string pause = "Press enter or 'p' to resume";
            drawWord(pause,-myCamera.x_left - 1080.0*myCamera.width/4.0,-1080.0*myCamera.height/2 +LETTER_HEIGHT);
        }else if (myStatus.mode == GAME_START){
            std::string startString  = " Press enter to begin game. ";
            drawWord(startString,-myCamera.x_left - 1080.0*myCamera.width/4.0,-1080.0*myCamera.height/2 + LETTER_HEIGHT);
        }

        // Reset Camera
        myCamera.currentTeam = prevCamera->currentTeam;
        myCamera.height = prevCamera->height;
        myCamera.state  = prevCamera->state;
        myCamera.width = prevCamera->width;
        myCamera.x_left = prevCamera->x_left;
        myCamera.x_pan_count = prevCamera->x_pan_count;
        myCamera.x_pos = prevCamera->x_pos;
        myCamera.x_right = prevCamera->x_right;
        myCamera.y_bottom = prevCamera->y_bottom;
        myCamera.y_pan_count = prevCamera->y_pan_count;
        myCamera.y_pos = prevCamera->y_pos;
        myCamera.y_top = prevCamera->y_top;
        myCamera.zoom_count = prevCamera->zoom_count;
        myCamera.zoom_level = prevCamera->zoom_level;
        delete prevCamera;
        myCamera.update(true);

    }


    void Game::drawWord(std::string word, float baseX, float baseY ){
            float texCoords[6];
            uint16_t font_numx;
            const char* time_word = word.c_str();
            for (uint8_t j = 0; j < word.length(); j++) {
            font_numx = NUM_FONTS;
            if (time_word[j] >= 'A' && time_word[j] <= 'Z') {
                font_numx = time_word[j] - 'A' + LETTER_A;
            }
            if (time_word[j] >= 'a' && time_word[j] <= 'z') {
                font_numx = time_word[j] - 'a' + LETTER_a;
            }
            if (time_word[j] == ':') {
                font_numx = LETTER_colon;
            }
            if (time_word[j] == '.') {
                font_numx = LETTER_dot;
            }
            if (time_word[j] == '-') {
                    getTexCoords(TEX_FONTS, LETTER_NEG, texCoords);
            }
            if ((time_word[j] >= '0') && (time_word[j] <= '9')) {
                font_numx = time_word[j] - '0' + LETTER_0;
            }

            if (font_numx != NUM_FONTS) {
                glBegin(GL_QUADS);
                    getTexCoords(TEX_FONTS, font_numx, texCoords);
                    glTexCoord2d(texCoords[0], texCoords[1]);
                    glVertex3f(baseX + j*LETTER_WIDTH, baseY - LETTER_HEIGHT, FONT_DEPTH);
                    glTexCoord2d(texCoords[2], texCoords[1]);
                    glVertex3f(baseX+(j+1)*LETTER_WIDTH, baseY - LETTER_HEIGHT, FONT_DEPTH);
                    glTexCoord2d(texCoords[2], texCoords[3]);
                    glVertex3f(baseX+(j+1)*LETTER_WIDTH, baseY, FONT_DEPTH);
                    glTexCoord2d(texCoords[0], texCoords[3]);
                    glVertex3f(baseX + j*LETTER_WIDTH, baseY, FONT_DEPTH);
                glEnd();
            }
        }
        delete time_word;
    }

    void Game::drawMinimap(float zoomLevel,float x, float y){
        glBindTexture(GL_TEXTURE_2D, myGame->myTextures[TEX_BASIC_ROOM].texHandle);
        float texCoords[6];
        float baseX =  0.0;
        float baseY = 0.0;
        float gridSize = (-myCamera.x_left/2.5-200.0)/myWorld.width;

        baseY = -myWorld.height * 1080.0 / 2 + gridSize * myWorld.height;
        baseX = 100.0;
        for (int i=1; i <= myGame->myWorld.width; i++) {
                for (int j=1; j<=myGame->myWorld.height; j++){
                    glBegin(GL_QUADS);

                    getTexCoords(TEX_BASIC_ROOM,SMALL_ROOM,texCoords);
                    glTexCoord2d(texCoords[0], texCoords[1]);
                    glVertex3f(baseX + (i-1)*gridSize, baseY - (j+1)*gridSize, FONT_DEPTH);
                    glTexCoord2d(texCoords[2], texCoords[1]);
                    glVertex3f(baseX + (i-1)*gridSize, baseY - j*gridSize, FONT_DEPTH);
                    glTexCoord2d(texCoords[2], texCoords[3]);
                    glVertex3f(baseX + i*gridSize, baseY - j*gridSize, FONT_DEPTH);
                    glTexCoord2d(texCoords[0], texCoords[3]);
                    glVertex3f(baseX + i*gridSize, baseY - (j+1)*gridSize, FONT_DEPTH);

                    glEnd();
                    baseY-= 10;
                }
                baseX+=10;
                baseY += 10*myWorld.height;
        }

        baseY = -myWorld.height * 1080.0 / 2  + 10.0 + gridSize * myWorld.height - gridSize * y - 10.0*y;
        baseX = 90.0 + gridSize *(x-1) + 10.0*x;
        glBegin(GL_QUADS);

        //getTexCoords(TEX_BASIC_ROOM,FLOOR_TILE,texCoords);
        getTexCoords(TEX_BASIC_ROOM,GOLD_BLOCK,texCoords);
        glTexCoord2d(texCoords[0], texCoords[1]);
        glVertex3f(baseX + (myWorld.width+1.0- zoomLevel)*gridSize + 10.0*(myWorld.height+1.0-zoomLevel), baseY - (myWorld.height+1.0-zoomLevel)*gridSize - 10.0*(myWorld.height+1.0-zoomLevel), FONT_DEPTH+1);
        glTexCoord2d(texCoords[2], texCoords[1]);
        glVertex3f(baseX + (myWorld.width+1.0- zoomLevel)*gridSize +  10.0*(myWorld.height+1.0-zoomLevel), baseY - 1*gridSize, FONT_DEPTH+1);
        glTexCoord2d(texCoords[2], texCoords[3]);
        glVertex3f(baseX + 1.0*gridSize , baseY - 1.0*gridSize, FONT_DEPTH+1);
        glTexCoord2d(texCoords[0], texCoords[3]);
        glVertex3f(baseX + 1.0*gridSize , baseY - (myWorld.height+1.0-zoomLevel)*gridSize - 10.0*(myWorld.height+1.0-zoomLevel), FONT_DEPTH+1);

        glEnd();

    }

} // namespace foz
