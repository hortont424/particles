#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/resource.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define ELEMENT_COUNT 2048
#define FRAME_COUNT 200

float * points;
int paint = 0;

void timer(int extra)
{
    glutPostRedisplay();
    glutTimerFunc(17, timer, 0);
}

void display()
{
    float * pts;

    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(2.0, 2.0, 2.0);
    glTranslatef(-0.5, -0.5, -0.5);

    glColor3f(1.0, 0.0, 0.0);
    glPointSize(2.0);

    pts = points + (paint * (ELEMENT_COUNT * 4)) + 1;
    paint = (++paint == FRAME_COUNT ? 0 : paint);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(float) * 4, pts);
    glDrawArrays(GL_POINTS, 0, ELEMENT_COUNT);
    glDisableClientState(GL_VERTEX_ARRAY);

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    int fd;
    long csize;

    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Previewer");

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);

    csize = sizeof(float) * ELEMENT_COUNT * 4 * FRAME_COUNT;
    fd = open("/Users/hortont/Desktop/particles/Simulator/test.out", O_RDWR);
    points = (float*)mmap(NULL, csize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    glutMainLoop();

    return 0;
}
