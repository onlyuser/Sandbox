#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "GL/freeglut.h"

static GLfloat spin = 0.0;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glRotatef(spin, 0.0, 0.0, 1.0);
    glRectf(-25.0, -25.0, 25.0, 25.0);
    glPopMatrix();

    glFlush();
    glXSwapBuffers(glutXDisplay(), glutXWindow());
}

void spinDisplay(void)
{
    spin = spin + 2.0;
    if (spin > 360.0)
        spin = spin - 360.0;
    display();
}

void startIdleFunc(AUX_EVENTREC *event)
{
    glutIdleFunc(spinDisplay);
}

void stopIdleFunc(AUX_EVENTREC *event)
{
    glutIdleFunc(0);
}

void myinit(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glShadeModel(GL_FLAT);
}

void myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho (-50.0, 50.0, -50.0*(GLfloat)h/(GLfloat)w,
            50.0*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
    else
        glOrtho (-50.0*(GLfloat)w/(GLfloat)h,
            50.0*(GLfloat)w/(GLfloat)h, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}


int main(int argc, char** argv)
{
    glutInitDisplayMode(AUX_DOUBLE | AUX_RGBA);
    glutInitPosition(0, 0, 500, 500);
    glutInitWindow(argv[0]);
    myinit();
    glutReshapeFunc(myReshape);
    glutIdleFunc(spinDisplay);
    glutMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, startIdleFunc);
    glutMouseFunc(AUX_MIDDLEBUTTON, AUX_MOUSEDOWN, stopIdleFunc);
    glutMainLoop(display);
}
