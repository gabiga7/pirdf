#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "validnext2.c"

// library installation : sudo apt-get install freeglut3
//                        sudo apt-get install freeglut3-dev
// Library verification : ls /usr/include/GL/ -l
// compilation :  gcc main.c -o out -I /usr/include -lglut -lGL -lphthread
// execution : ./out

float x=0;
float y=0;

void initGraph()
{
	glClearColor(0,0,0,1);
	glColor3f(0,0,1);
}

void initPoint()
{
	//glClearColor(0,0,0,1);
	glColor3f(0,1,0);
}

void update()
{
	struct to_give received;
	received=read();
	x=received.resultatO-received.resultatE;
	y=received.resultatS-received.resultatN;
	if (x<=-0.5)
		x=-0.5;
	if (x>=0.5)
		x=0.5;
	if (y<=-0.5)
		y=-0.5;
	if (y>=0.5)
		y=0.5;
	glutPostRedisplay();
	glutTimerFunc(3000,update,0);
}

void drawGraph()
{
	//printf("%f\n",f);

	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5);

	glBegin(GL_LINES);
	glVertex2f(-0.5,0);
	glVertex2f(0.5,0);
	glVertex2f(0,-0.5);
	glVertex2f(0,0.5);
	glEnd();

	glColor3f(0,1,0);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();

	glColor3f(0,0,1);

	glFlush();
}

void drawPoint()
{
	//glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(20);

	glBegin(GL_POINTS);
	glVertex2f(0.4,0.4);
	glEnd();
	glFlush();
}


int main (int argc, char *argv[])
{
	glutInit(&argc,argv);

	glutInitWindowPosition(200,100);
	glutInitWindowSize(800,600);

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutCreateWindow("Radar");
	initGraph();
	glutDisplayFunc(drawGraph);
	//initPoint();
	//glutDisplayFunc(drawPoint);
	update();
	glutMainLoop();
	return 0;
}
