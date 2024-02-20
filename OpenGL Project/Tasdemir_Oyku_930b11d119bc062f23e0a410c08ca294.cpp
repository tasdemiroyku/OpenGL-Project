/*********
CTIS164 - Template Source Program
----------
STUDENT :   Oyku Tasdemir
SECTION :   164-1
HOMEWORK:   1
----------
PROBLEMS:
    Since I am using Mac and Xcode, there are some differences. I will address them with comments in order to make the code work on your computer.
----------
ADDITIONAL FEATURES:
    When pressed <spacebar> traffic light changes color.
    When pressed <d> or <D> changes to day or night.
            If night, moon shows up.
            If day, sun shows up.
    When pressed <f> or <F> car speed changes.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef __APPLE__               //
#define GL_SILENCE_DEPRECATION // These lines are for Mac and xCode. You can comment them.
#endif                         //

#ifdef __APPLE__
#include <GLUT/glut.h>         // These lines are for Mac and xCode. You can comment them.
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD  10
#define TIMER_ON      1

#define D2R 0.0174532


/* Global Variables */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool activeTimer = true;

//car coordinates
float xC = -340, yC = -45;
float xc0=xC;

//cloud coordinates
float xcl = 300, ycl = 70;
float xcl0 = xcl;

//traffic light color
bool r = false, g = true;

//day or night
bool day=true;

//car speed
bool carspeed=false;



void circle( int x, int y, int r )
{
#define PI 3.1415
   float angle ;
   glBegin( GL_POLYGON ) ;
   for ( int i = 0 ; i < 100 ; i++ )
   {
      angle = 2*PI*i/100;
      glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
   }
   glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
   float angle;

   glBegin(GL_LINE_LOOP);
   for (int i = 0; i < 100; i++)
   {
      angle = 2 * PI*i / 100;
      glVertex2f(x + r*cos(angle), y + r*sin(angle));
   }
   glEnd();
}

void print(int x, int y, const char *string, void *font )
{
   int len, i ;

   glRasterPos2f( x, y );
   len = (int) strlen( string );
   for ( i =0; i<len; i++ )
   {
      glutBitmapCharacter( font, string[i]);
   }
}

void vprint(int x, int y, void *font, const char *string , ...)
{
   va_list ap;
   va_start ( ap, string );
   char str[1024] ;
   vsprintf( str, string, ap ) ; //You might want to change it back to vsprintf_s but xCode doesnt accept it.
   va_end(ap) ;

   int len, i ;
   glRasterPos2f( x, y );
   len = (int) strlen( str );
   for ( i =0; i<len; i++ )
   {
      glutBitmapCharacter( font, str[i]);
   }
}

void vprint2(int x, int y, float size, const char *string, ...) {
   va_list ap;
   va_start(ap, string);
   char str[1024];
   vsprintf(str, string, ap); //You might want to change it back to vsprintf_s but xCode doesnt accept it
   va_end(ap);
   glPushMatrix();
      glTranslatef(x, y, 0);
      glScalef(size, size, 1);

      int len, i;
      len = (int)strlen(str);
      for (i = 0; i<len; i++)
      {
         glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
      }
   glPopMatrix();
}

void drawCloud(){

    glColor3f(1, 1, 1);
    circle(xcl, ycl, 20);
    circle(xcl+23, ycl, 20);
    circle(xcl+15, ycl+10, 18);


}

void drawCar(){

//body
    glColor3ub(255, 153, 204);
    glRectf(xC, yC, xC+90, yC-25);

//windows
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xC+10, yC-5);
    glVertex2f(xC+15, yC+20);
    glVertex2f(xC+55, yC+20);
    glVertex2f(xC+75, yC-5);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(xC+35, yC+20);
    glVertex2f(xC+35, yC);
    glEnd();

//tires
    glColor3f(0, 0, 0);
    circle(xC+20, yC-25, 12);
    circle(xC+70, yC-25, 12);

    glColor3f(1, 1, 1);
    circle(xC+20, yC-25, 3);
    circle(xC+70, yC-25, 3);

//headlights
    glColor3f(1, 0, 0);
    glRectf(xC, yC, xC+7, yC-7);

    glColor3f(0, 0, 0);
    glRectf(xC+80, yC, xC+90, yC-7);

    glColor3f(1, 1, 1);
    glRectf(xC+84, yC, xC+90, yC-3);
    glRectf(xC, yC, xC+4, yC-4);

//door handle
    glColor3f(0, 0, 0);
    glRectf(xC+40, yC-5, xC+47, yC-8);

}

void display_background() {

    int x;

//Brick Design
    glLineWidth(3);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);

        for(x=-320;x<400;x+=80)
    {
            glVertex2f(x, 250);
            glVertex2f(x, -250);

    }
    glVertex2f(-400, 100);
    glVertex2f(400, 100);

    glVertex2f(-400, -100);
    glVertex2f(400, -100);

    glEnd();

//Name Frame
    glColor3f(0, 0, 0);
    glRectf(-270, 240, 270, 215);

    glColor3ub(255, 153, 204);
    vprint(-125, 223, GLUT_BITMAP_9_BY_15, "Road from Oyku Tasdemir's Room");

//Window
    glColor3ub(36, 18, 2);
    glRectf(-300, 200, 300, -200);

    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-300, 200);
    glVertex2f(300, 200);
    glVertex2f(300, -200);
    glVertex2f(-300, -200);
    glEnd();

//sky

    if(day)
        {
            glColor3ub(253, 205, 83);
            glRectf(-260, 160, 260, -160);
        glColor3f(1, 1, 0);
        circle(-170, 100, 30);
        }
    else
        {
            glColor3ub(0, 0, 112);
            glRectf(-260, 160, 260, -160);
        glColor3ub(160, 160, 160);
        circle(100, 90, 20);
        }


//road
    glColor3ub(50, 50, 50);
    glRectf(-260, -40, 260, -160);

    //lane stripes
    glColor3f(1, 1, 1);
    glRectf(-230, -90, -140, -100);
    glRectf(-50, -90, 50, -100);
    glRectf(140, -90, 230, -100);

    //traffic lights
    glColor3f(0, 0, 0);
    glRectf(220, 60, 250, 0);
    glRectf(230, 0, 240, -40);


    glColor3f(0, g, 0);
    circle(235, 12, 6);

    glColor3f(r, 0, 0);
    circle(235, 48, 6);

//instructions
    glColor3ub(253, 205, 83);
    vprint(-250, -175, GLUT_BITMAP_8_BY_13, "d / D : day/night       spacebar : start/stop - red/green light");
    vprint(-250, -190, GLUT_BITMAP_8_BY_13, "f / F : car fast/slow");
}

void displayFront(){
    //middle window panel
        glColor3ub(36, 18, 2);
        glRectf(-20, 160, 20, -160);

        glLineWidth(4);
        glColor3f(0, 0, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-260, 160);
        glVertex2f(-20, 160);
        glVertex2f(-20, -160);
        glVertex2f(-260, -160);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2f(20, 160);
        glVertex2f(260, 160);
        glVertex2f(260, -160);
        glVertex2f(20, -160);
        glEnd();

        //Handle
        glColor3ub(0, 0, 0);
        circle(-10, 0, 5);
        circle(10, 0, 5);

        glLineWidth(4);
        glBegin(GL_LINES);

        glVertex2f(10, 0);
        glVertex2f(10, -30);

        glVertex2f(-10, 0);
        glVertex2f(-10, -30);

        glEnd();

    glColor3d(0.3, 0, 0);
    glRectf(-400, 99, -262, -98);
    glRectf(262, 99, 400, -98);

    glColor3ub(36, 18, 2);
    glRectf(-300, 99, -262, -99);
    glRectf(262, 99, 300, -99);

    glColor3ub(0, 0, 0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-320, 100);
    glVertex2f(-320, -100);

    glVertex2f(320, 100);
    glVertex2f(320, -100);

    glVertex2f(-300, 100);
    glVertex2f(-300, -100);

    glVertex2f(300, 100);
    glVertex2f(300, -100);
    glEnd();
}

void display() {

   glClearColor(0.3, 0, 0, 0);
   glClear(GL_COLOR_BUFFER_BIT);

    display_background();

    drawCar();
    drawCloud();

    if(xC>300)
        xC = xc0;

    if(xcl<-300)
        xcl = xcl0;

    displayFront();

  glutSwapBuffers();
}



//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y )
{
    //exit when ESC is pressed.
    if (key == 27)
       exit(0);

    if (key == ' ')
   {     activeTimer = !activeTimer;
       r=!r;
       g=!g;
   }

    if(key == 100 || key == 68)
        day=!day;

    if(key == 102 || key == 70)
        carspeed=!carspeed;


   // to refresh the window it calls display() function
   glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
      exit(0);

   glutPostRedisplay() ;
}

void onSpecialKeyDown( int key, int x, int y )
{
   switch (key) {
   case GLUT_KEY_UP:
      up = true;
      break;
   case GLUT_KEY_DOWN:
      down = true;
      break;
   case GLUT_KEY_LEFT:
      left = true;
      break;
   case GLUT_KEY_RIGHT:
      right = true;
      break;
   }

   glutPostRedisplay() ;
}

void onSpecialKeyUp( int key, int x, int y )
{
   switch (key) {
   case GLUT_KEY_UP:
      up = false;
      break;
   case GLUT_KEY_DOWN:
      down = false;
      break;
   case GLUT_KEY_LEFT:
      left = false;
      break;
   case GLUT_KEY_RIGHT:
      right = false;
      break;
   }

   glutPostRedisplay() ;
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick( int button, int stat, int x, int y )
{


   glutPostRedisplay() ;
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize( int w, int h )
{
   winWidth = w;
   winHeight = h;
   glViewport( 0, 0, w, h ) ;
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
   glMatrixMode( GL_MODELVIEW);
   glLoadIdentity();
   display();
}

void onMoveDown( int x, int y ) {




   glutPostRedisplay() ;
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove( int x, int y ) {




   glutPostRedisplay() ;
}

#if TIMER_ON == 1
void onTimer( int v ) {

   glutTimerFunc( TIMER_PERIOD, onTimer, 0 );

    if (activeTimer)
        {
        xcl-=0.4;
        if(carspeed)
            xC+=1.5;
        else
            xC++;
       }

   glutPostRedisplay();

}
#endif

void Init() {

   // Smoothing shapes
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

int main( int argc, char *argv[] ) { //doesnt work unless main is int on mac.
   glutInit(&argc, argv );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutInitWindowPosition(300, 200);
   glutCreateWindow("CTIS164 - Homework1 - TasdemirOyku");

   glutDisplayFunc(display);
   glutReshapeFunc(onResize);

   //
   // keyboard registration
   //
   glutKeyboardFunc(onKeyDown);
   glutSpecialFunc(onSpecialKeyDown);

   glutKeyboardUpFunc(onKeyUp);
   glutSpecialUpFunc(onSpecialKeyUp);

   //
   // mouse registration
   //
   glutMouseFunc(onClick);
   glutMotionFunc(onMoveDown);
   glutPassiveMotionFunc(onMove);

   #if  TIMER_ON == 1
   // timer event
   glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
   #endif

   Init();

   glutMainLoop();
}

