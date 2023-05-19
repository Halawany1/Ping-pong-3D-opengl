#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include <windows.h>
#include <iostream>
#include <math.h>
#include <stdio.h> // Header File For Standard Input / Output
#include <stdarg.h> // Header File For Variable Argument Routines
#include <string.h> // Header File For String Management
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/stb_image.h>

using namespace std;

// Values
float initialPlayerSpeed = 0.2;
float initialBallSpeed = 0.2;
float maxPoints = 7;
float ballAccel = 1.07; // Ball Acceleration
float playerAccel = 1.03; // Player Acceleration

// Structures
/*struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char* data;
};

typedef struct Image Image;

// Variables
 // 1 - crate, 2 - floor*/
GLuint texture_id[3];

unsigned int texture;
int width, height, nrChannels;
unsigned char* data = NULL;
GLfloat xOne[2] = { 5.0f, 5.5f }, xTwo[2] = { 14.5f, 15.0f };
GLfloat yOne[2] = { 4.0f, 6.0f }, yTwo[2] = { 4.0f, 6.0f };
GLfloat zHeight[2] = { 1.0f, 3.0f };
GLfloat fieldX[2] = { 0.0f, 20.0f };
GLfloat fieldYOne[2] = { 0.0f, 0.5f }, fieldYTwo[2] = { 9.5f, 10.0f };
GLfloat fieldZ[2] = { 1.0f, 2.0f };
GLfloat ball[3] = { 10.0f, 5.0f, 1.0f };
GLfloat ballRadius = 0.3;
float tamPong = 1.5;
int keystates[4] = { 0, 0, 0, 0 }; // { UP, DOWN, W, S };
int directionX = 0;
int gameRunning = 0;
int npoints[2] = { 0, 0 };
float ballVerticalSpeed = 0;
float playerSpeed;
float ballSpeed;
float cam[9] = { 10.0, -3, 9, 10.0, 5.0, 0.0, 0.0, 0.0, 1.0 };
bool mouesePressed = false;
int xOrigin = -1;
float deltaAngle = 0.0f;
float angle = 0.0f;

//Headers Functions
unsigned int getint(FILE* fp);
unsigned int getshort(FILE* fp);
//GLvoid LoadTexture(GLvoid);
//int LoadBMP(char* filename, Image* image);
void drawFunc();
void load(int imgnum);
void check(unsigned char* data);
void keyboardArrows(int key, int x, int y);
void keyboardArrowsUp(int key, int x, int y);
void keyboardLetters(unsigned char key, int x, int y);
void keyboardLettersUp(unsigned char key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
void init();
void display();
void reshape(int w, int h);
void resetBoard(int mode);
void getRandomDirection();
void loop();
void changeDirectionY(int side);
void drawText(char* text, float x, float y, float z);

//Functions
int main(int argc, char** argv)
{
  
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pong-Game | BFCAI Project");
    init();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutIdleFunc(loop);
    glutSpecialFunc(keyboardArrows);
    glutSpecialUpFunc(keyboardArrowsUp);
    glutKeyboardFunc(keyboardLetters);
    glutKeyboardUpFunc(keyboardLettersUp);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    resetBoard(1);

    glutMainLoop();
    return 0;
}



void init() {

    //LoadTexture(); // Load The Texture(s) ( NEW )
    glEnable(GL_TEXTURE_2D); // Enable Texture Mapping ( NEW )

    // This Will Clear The Background Color To Black
    glClearColor(0.752941f, 0.752941f, 0.752941f, 0.0f);

    glClearDepth(1.0); // Enables Clearing Of The Depth Buffer

    glDepthFunc(GL_LESS); // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

    // LIGHTING
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 10.0, -1.0, 3.0, 1.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    float amb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

    // float diff[] = { 1.0f, 1.0f, 1.0f, 10.0f };
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

    //END LIGHTING

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Reset The Projection Matrix

    gluPerspective(45.0f, (GLfloat)1000 / (GLfloat)600, 0.1f, 100.0f);

    /*glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);*/

   //Keyboard inits
    glutIgnoreKeyRepeat(1);
}

void drawFunc()
{
    glClearColor(0, 0.3, 0, 0.2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (!gameRunning) {
        if (npoints[0] == maxPoints) {
            glColor3f(1.0, 0.0, 0.0);
            char p1_win[] = "Player 1 wins!";
            drawText(p1_win, 8.9, 10, 4.8);
            resetBoard(1);
        }
        else if (npoints[1] == maxPoints) {
            glColor3f(0.0, 0.0, 1.0);
            char p2_win[] = "Player 2 wins!";
            drawText(p2_win, 8.9, 10, 4.8);
            resetBoard(1);
        }
        else {
            char clk[] = "Click any key!";
            drawText(clk, 8.9, 10, 4.8);
        }
    }
    glColor3f(1.0, 1.0, 1.0);
    char src[] = "SCORE";
    char da4[] = "-";
    drawText(src, 9.4, 10, 4);
    drawText(da4, 9.9, 10, 3);
    char score1[2], score2[2];
    sprintf(score1, "%d", npoints[0]);
    sprintf(score2, "%d", npoints[1]);
    drawText(score1, 9.38, 10, 3);
    drawText(score2, 10.5, 10, 3);

    // FIELD LIMITS - NEAR


    glBegin(GL_QUADS);
    // Front Face = + close to the x axis
    glColor3f(.1, .05, .05);
    glVertex3f(fieldX[0], fieldYOne[0], fieldZ[0]); // Bottom Left Of The Texture and Quad
    glVertex3f(fieldX[1], fieldYOne[0], fieldZ[0]); // Bottom Right Of The Texture and Quad
    glVertex3f(fieldX[1], fieldYOne[0], fieldZ[1]); // Top Right Of The Texture and Quad
    glVertex3f(fieldX[0], fieldYOne[0], fieldZ[1]); // Top Left Of The Texture and Quad

    // Top Face
    glColor4f(.07, .7, .07,0.7);
    glVertex3f(fieldX[0], fieldYOne[1], fieldZ[1]); // Top Left Of The Texture and Quad
    glVertex3f(fieldX[0], fieldYOne[0], fieldZ[1]); // Bottom Left Of The Texture and Quad
    glVertex3f(fieldX[1], fieldYOne[0], fieldZ[1]); // Bottom Right Of The Texture and Quad
    glVertex3f(fieldX[1], fieldYOne[1], fieldZ[1]); // Top Right Of The Texture and Quad

    glColor3f(.1, .05, .05);
    // Front Face = + proxima do eixo x
    glVertex3f(fieldX[0], fieldYTwo[0], fieldZ[0]); // Bottom Left Of The Texture and Quad
    glVertex3f(fieldX[1], fieldYTwo[0], fieldZ[0]); // Bottom Right Of The Texture and Quad
    glVertex3f(fieldX[1], fieldYTwo[0], fieldZ[1]); // Top Right Of The Texture and Quad
    glVertex3f(fieldX[0], fieldYTwo[0], fieldZ[1]); // Top Left Of The Texture and Quad

    // Top Face
    glColor4f(.07, .7, .07, 0.7);
    glVertex3f(fieldX[0], fieldYTwo[1], fieldZ[1]); // Top Left Of The Texture and Quad
    glVertex3f(fieldX[0], fieldYTwo[0], fieldZ[1]); // Bottom Left Of The Texture and Quad
    glVertex3f(fieldX[1], fieldYTwo[0], fieldZ[1]); // Bottom Right Of The Texture and Quad
    glVertex3f(fieldX[1], fieldYTwo[1], fieldZ[1]); // Top Right Of The Texture and Quad
  // glEnd();
    glEnd();

   

    // FIELD LIMITS - FAR
    load(1);
    glBegin(GL_QUADS);
  

    // FIELD LIMITS - FLOOR
    //glBindTexture(GL_TEXTURE_2D, texture_id[1]);
    glColor4f(1, 0.85882352941176, 0.98, 1.0f);
    glBegin(GL_QUADS);

    glTexCoord2f(1.0f, 0.0f);

    // glTexCoord2f(0.0f, 0.0f);
    glVertex3f(fieldX[0], fieldYOne[0], fieldZ[0]); // Bottom Left Of The Texture and Quad
    // glTexCoord2f(0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(fieldX[1], fieldYOne[1], fieldZ[0]); // Bottom Right Of The Texture and Quad
    // glTexCoord2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);

    glVertex3f(fieldX[1], fieldYTwo[1], fieldZ[0]); // Top Right Of The Texture and Quad
    // glTexCoord2f(1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);

    glVertex3f(fieldX[0], fieldYTwo[0], fieldZ[0]); // Top Left Of The Texture and Quad
    glEnd();

    // glBindTexture(GL_TEXTURE_2D, texture_id[0]);

     // PLAYER 1
    load(2);
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.184314, 0.184314);
    // Front Face = + proxima do eixo x
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xOne[0], yOne[0], zHeight[0]); // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xOne[1], yOne[0], zHeight[0]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xOne[1], yOne[0], zHeight[1]); // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xOne[0], yOne[0], zHeight[1]); // Top Left Of The Texture and Quad

    // Back Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xOne[0], yOne[1], zHeight[0]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xOne[1], yOne[1], zHeight[0]); // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xOne[1], yOne[1], zHeight[1]); // Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xOne[0], yOne[1], zHeight[1]); // Bottom Left Of The Texture and Quad

    // Top Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xOne[0], yOne[1], zHeight[1]); // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xOne[0], yOne[0], zHeight[1]); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xOne[1], yOne[0], zHeight[1]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xOne[1], yOne[1], zHeight[1]); // Top Right Of The Texture and Quad

    // Bottom Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xOne[1], yOne[1], zHeight[0]); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xOne[0], yOne[1], zHeight[0]); // Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xOne[0], yOne[0], zHeight[0]); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xOne[1], yOne[0], zHeight[0]); // Bottom Right Of The Texture and Quad

    // Right face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xOne[1], yOne[1], zHeight[0]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xOne[1], yOne[1], zHeight[1]); // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xOne[1], yOne[0], zHeight[1]); // Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xOne[1], yOne[0], zHeight[0]); // Bottom Left Of The Texture and Quad

    // Left Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xOne[0], yOne[1], zHeight[0]); // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xOne[0], yOne[0], zHeight[0]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xOne[0], yOne[0], zHeight[1]); // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xOne[0], yOne[1], zHeight[1]); // Top Left Of The Texture and Quad
    glEnd();

    // PLAYER 2
    load(2);
    glBegin(GL_QUADS);
    glColor3f(0.196078, 0.3, 1.0);
    // Front Face = + proxima do eixo x
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xTwo[0], yTwo[0], zHeight[0]); // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(xTwo[1], yTwo[0], zHeight[0]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(xTwo[1], yTwo[0], zHeight[1]); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(xTwo[0], yTwo[0], zHeight[1]); // Top Left Of The Texture and Quad

    // Back Face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xTwo[0], yTwo[1], zHeight[0]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xTwo[1], yTwo[1], zHeight[0]); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xTwo[1], yTwo[1], zHeight[1]); // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xTwo[0], yTwo[1], zHeight[1]); // Bottom Left Of The Texture and Quad

    // Top Face
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(xTwo[0], yTwo[1], zHeight[1]); // Top Left Of The Texture and Quad
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(xTwo[0], yTwo[0], zHeight[1]); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.5f);
    glVertex3f(xTwo[1], yTwo[0], zHeight[1]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xTwo[1], yTwo[1], zHeight[1]); // Top Right Of The Texture and Quad

    // Bottom Face
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xTwo[1], yTwo[1], zHeight[0]); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xTwo[0], yTwo[1], zHeight[0]); // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xTwo[0], yTwo[0], zHeight[0]); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xTwo[1], yTwo[0], zHeight[0]); // Bottom Right Of The Texture and Quad

    // Right face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xTwo[1], yTwo[1], zHeight[0]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xTwo[1], yTwo[1], zHeight[1]); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xTwo[1], yTwo[0], zHeight[1]); // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xTwo[1], yTwo[0], zHeight[0]); // Bottom Left Of The Texture and Quad

    // Left Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xTwo[0], yTwo[1], zHeight[0]); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xTwo[0], yTwo[0], zHeight[0]); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xTwo[0], yTwo[0], zHeight[1]); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xTwo[0], yTwo[1], zHeight[1]); // Top Left Of The Texture and Quad
    glEnd();

    // CYLINDER
    glPushMatrix();
    glTranslatef(ball[0], ball[1], ball[2]);
    // glColor3f(0.35, 0.35, 0.35);
    glColor3f(0, 0, 0);
    GLUquadricObj* quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, 0.3f, ballRadius, 0.5f, 32, 32);
    // gluSphere(quadratic, ballRadius, 32, 32);
    glPopMatrix();

    // TOP DISK OF CYLINDER
    glPushMatrix();
    glTranslatef(ball[0], ball[1], ball[2] + 0.5f);
    glColor3f(1, 1, 1);
    GLUquadricObj* quadratic2;
    quadratic2 = gluNewQuadric();
    gluDisk(quadratic, 0, ballRadius, 32, 32);
    glPopMatrix();
}

void keyboardArrows(int key, int x, int y)
{
    gameRunning = 1;
    switch (key) {
    case GLUT_KEY_UP: {
        //printf("Pos: %f \nLimit %f\n",yOne[1] , fieldYTwo[0]);
        keystates[0] = 1;
        break;
    }
    case GLUT_KEY_DOWN: {
        //printf("Pos: %f \nLimit %f\n",yOne[0] , fieldYOne[1]);
        keystates[1] = 1;
        break;
    }
    default:
        return;
    }
}

void keyboardArrowsUp(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP: {
        //printf("Pos: %f \nLimit %f\n",yOne[1] , fieldYTwo[0]);
        keystates[0] = 0;
        break;
    }
    case GLUT_KEY_DOWN: {
        //printf("Pos: %f \nLimit %f\n",yOne[0] , fieldYOne[1]);
        keystates[1] = 0;
        break;
    }
    default:
        return;
    }
}

void keyboardLetters(unsigned char key, int x, int y)
{
    gameRunning = 1;
    switch (key) {
    case 'w': {
        keystates[2] = 1;
        break;
    }
    case 's': {
        keystates[3] = 1;
        break;
    }
    case 'r': {
        resetBoard(1);
        break;
    }
    default:
        return;
    }
}

void keyboardLettersUp(unsigned char key, int x, int y)
{
    switch (key) {
    case 'w': {
        keystates[2] = 0;
        break;
    }
    case 's': {
        keystates[3] = 0;
        break;
    }
    default:
        return;
    }
}

void mouseButton(int button, int state, int x, int y)
{

    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {

        // when the button is released
        if (state == GLUT_UP) {
            // angle += deltaAngle;
            // mouesePressed = false;
            xOrigin = -1;
        }
        else { // state = GLUT_DOWN
            // mouesePressed = true;
            xOrigin = x;
        }
    }
}
void mouseMove(int x, int y) {
    // this will only be true when the left button is down
    if (xOrigin >= 0) {
        // update deltaAngle
        deltaAngle = (x - xOrigin) * 0.001f;

        // update camera's direction
        cam[0] = sin(angle + deltaAngle) * 10 + 10;
    }
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity(); /* clear the matrix */
    /* viewing transformation  */

    gluLookAt(cam[0], cam[1], cam[2], cam[3], cam[4], cam[5], cam[6], cam[7], cam[8]);

    glScalef(1, 1, 1); /* modeling transformation */
    drawFunc();
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 70.0);
    glMatrixMode(GL_MODELVIEW);
}

void getRandomDirection()
{
    int random = rand() % 100;
    if (random < 50)
        directionX = -1;
    else
        directionX = 1;
}

void resetBoard(int mode)
{
    gameRunning = 0;
    if (mode != 0) // Restart game
    {
        npoints[0] = 0;
        npoints[1] = 0;
    }
    ball[0] = 10.0f;
    ball[1] = 5.0f;
    getRandomDirection();
    yOne[0] = 4.0f;
    yOne[1] = 6.0f;
    yTwo[0] = 4.0f;
    yTwo[1] = 6.0f;
    ballSpeed = initialBallSpeed;
    playerSpeed = initialPlayerSpeed;
    ballVerticalSpeed = 0;
}

void loop()
{
    if (!gameRunning)
        return;

    if (keystates[0]) {
        yTwo[1] += playerSpeed;
        yTwo[0] += playerSpeed;
        if (yTwo[1] > fieldYTwo[0]) {
            yTwo[1] -= playerSpeed;
            yTwo[0] -= playerSpeed;
        }
    }

    if (keystates[1]) {
        yTwo[1] -= playerSpeed;
        yTwo[0] -= playerSpeed;
        if (yTwo[0] < fieldYOne[1]) {
            yTwo[1] += playerSpeed;
            yTwo[0] += playerSpeed;
        }
    }

    if (keystates[2]) {
        yOne[1] += playerSpeed;
        yOne[0] += playerSpeed;
        if (yOne[1] > fieldYTwo[0]) {
            yOne[1] -= playerSpeed;
            yOne[0] -= playerSpeed;
        }
    }

    if (keystates[3]) {
        yOne[1] -= playerSpeed;
        yOne[0] -= playerSpeed;
        if (yOne[0] < fieldYOne[1]) {
            yOne[1] += playerSpeed;
            yOne[0] += playerSpeed;
        }
    }

    // Direction + Colisions X + Change direction Y
    switch (directionX) {
    case -1: {
        ball[0] -= ballSpeed;
        if (ball[0] - ballRadius <= xOne[1]) {
            if (ball[1] > yOne[0] && ball[1] < yOne[1]) {
                directionX = 1;
                ballSpeed *= ballAccel;
                playerSpeed *= playerAccel;
                changeDirectionY(-1);
            }
            else if (ball[0] - ballRadius <= xOne[0] - 1.0f) {
                resetBoard(0);
                npoints[1]++;
            }
        }
        break;
    }
    case 1: {
        ball[0] += ballSpeed;
        if (ball[0] + ballRadius >= xTwo[0]) {
            if (ball[1] > yTwo[0] && ball[1] < yTwo[1]) {
                directionX = -1;
                ballSpeed *= ballAccel;
                playerSpeed *= playerAccel;
                changeDirectionY(1);
            }
            else if (ball[0] + ballRadius >= xTwo[1] + 1.0f) {
                resetBoard(0);
                npoints[0]++;
            }
        }
    }
    }
    //End direction X + colisions X + direction Y
    //Colisions Y
    if (ball[1] + ballRadius >= fieldYTwo[0]) {
        ballVerticalSpeed *= -1;
    }
    if (ball[1] - ballRadius <= fieldYOne[1]) {
        ballVerticalSpeed *= -1;
    }

    //End Colisions Y
    ball[1] += ballVerticalSpeed;
    glutPostRedisplay();
}

void changeDirectionY(int side)
{
    //side = -1, side = 1 
    switch (side) {
    case -1: {
        if (keystates[2] == keystates[3])
            return;
        if (keystates[2])
            ballVerticalSpeed += 0.002;
        if (keystates[3])
            ballVerticalSpeed -= 0.002;
    }
    case 1: {
        if (keystates[0] == keystates[1])
            return;
        if (keystates[0])
            ballVerticalSpeed += 0.002;
        if (keystates[1])
            ballVerticalSpeed -= 0.002;
        break;
    }
    }
}

void drawText(char* text, float x, float y, float z)
{
    glRasterPos3f(x, y, z);
    int i;
    for (i = 0; i < (int)strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
    }
}

void load(int imgnum) {
    unsigned char* data = NULL;
    if (imgnum == 1) {

        data = stbi_load("g2.png", &width, &height, &nrChannels, 0);
        check(data);
    }
    else if (imgnum == 2) {

        data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
        check(data);
    }
}

void check(unsigned char* data) {
    if (data)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}