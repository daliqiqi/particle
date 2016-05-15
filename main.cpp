
#include "mainwindow.h"
#include <QApplication>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "Particle.h"

#define WIDTH 900
#define HEIGHT 900

ParticleSystem *firework;
const double sensitivity = 0.002f;
double walk_speed = 0.05;
double cam_pos[3] = {0, 0, 4};
double cam_view[3] = {0, 0, -1};
double old_x, old_y;
//前三个参数表示了观察点的位置，中间三个参数表示了观察目标的位置，最后三个参数代表从(0,0,0)到(x,y,z)的直线，它表示了观察者认为的“上”方向。
void camLook()
{
    gluLookAt(cam_pos[0],cam_pos[1],cam_pos[2],
    cam_pos[0] + cam_view[0],cam_pos[1] + cam_view[1],cam_pos[2] + cam_view[2],
    0.0, 1.0, 0.0
    );
}

void myIdle()
{
    if(GetAsyncKeyState('W')){
            cam_pos[0] += cam_view[0] * walk_speed;
            cam_pos[1] += cam_view[1] * walk_speed;
            cam_pos[2] += cam_view[2] * walk_speed;
    }
    if(GetAsyncKeyState('S')){
            cam_pos[0] -= cam_view[0] * walk_speed;
            cam_pos[1] -= cam_view[1] * walk_speed;
            cam_pos[2] -= cam_view[2] * walk_speed;
    }
    if(GetAsyncKeyState('A')){
            cam_pos[0] += cam_view[2] * walk_speed;
            cam_pos[2] -= cam_view[0] * walk_speed;
    }
    if(GetAsyncKeyState('D')){
            cam_pos[0] -= cam_view[2] * walk_speed;
            cam_pos[2] += cam_view[0] * walk_speed;
    }
    if(GetAsyncKeyState(VK_SPACE)){
            cam_pos[1] += walk_speed;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    old_x = x;
    old_y = y;
}

void normalize(double *v) {
    double magnitude = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= magnitude;
    v[1] /= magnitude;
    v[2] /= magnitude;
}

void rotate_view(double *view, double angle, double x, double y, double z) {
    double new_x;
    double new_y;
    double new_z;

    double c = cos(angle);
    double s = sin(angle);

    new_x  = (x*x*(1-c) + c)	* view[0];
    new_x += (x*y*(1-c) - z*s)	* view[1];
    new_x += (x*z*(1-c) + y*s)	* view[2];

    new_y  = (y*x*(1-c) + z*s)	* view[0];
    new_y += (y*y*(1-c) + c)	* view[1];
    new_y += (y*z*(1-c) - x*s)	* view[2];

    new_z  = (x*z*(1-c) - y*s)	* view[0];
    new_z += (y*z*(1-c) + x*s)	* view[1];
    new_z += (z*z*(1-c) + c)	* view[2];

    view[0] = new_x;
    view[1] = new_y;
    view[2] = new_z;

    normalize(view);
}

void motion(int x, int y) {
        double rot_x, rot_y;
        double rot_axis[3];

        //x -= half_width;
        //y -= half_height;

        rot_x = -(double)(x - old_x) * sensitivity;
        rot_y = -(double)(y - old_y) * sensitivity;

        old_x = x;
        old_y = y;
            rotate_view(cam_view, rot_x, 0.0f, 1.0f, 0.0f);

            rot_axis[0] = -cam_view[2];
            rot_axis[1] = 0.0f;
            rot_axis[2] = cam_view[0];

            normalize(rot_axis);

            rotate_view(cam_view, rot_y, rot_axis[0], rot_axis[1], rot_axis[2]);
}

GLvoid reshape(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
    if (height==0)										// Prevent A Divide By Zero By
    {
        height=1;										// Making Height Equal One
    }

    glViewport(0,0,width,height);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    gluPerspective(45.0f,(GLdouble)width/(GLdouble)height,0.1f,200.0f);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();									// Reset The Modelview Matrix
}

void Init()
{
    firework = new ParticleSystem();
    firework->initEmitter();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);	// ENABLE DEPTH TESTING
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camLook();
    glPushMatrix();
    TransAndRotate(firework->emitter->pos, firework->emitter->direction);

    firework->renderEmitter(1);
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

w.show();





    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow("演示");

    Init();
    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);

    glutMainLoop();




    return a.exec();
}
