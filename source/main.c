#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>
#include <pthread.h>

struct to_give received; //data received from read
#include "read.c"

// library installation : sudo apt-get install freeglut3
//                        sudo apt-get install freeglut3-dev
// Library verification : ls /usr/include/GL/ -l
// compilation :  gcc main.c -o out -I /usr/include -lglut -lGL -lphthread
// execution : ./out

float x=0; //position of detection
float y=0; //position of detection

float r=0; //rayon of the wave

int collision = 0; //if wave deetects somthing

int channel=7; //channel scanned

time_t t; //time displayed


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

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * (float)(ii) / (float)(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}

void updateRead()
{
	pthread_t th;
	pthread_create(&th,NULL,&read,NULL);
            pthread_detach (th);
    	x=received.resultatO-received.resultatE;
    	y=received.resultatS-received.resultatN;
    //x=0.5;
    //y=0.5;
    channel=7;
    if (x<=-0.75)
        x=-0.75;
    if (x>=0.75)
        x=0.75;
    if (y<=-0.75)
        y=-0.75;
    if (y>=0.75)
        y=0.75;
    glutPostRedisplay();
    glutTimerFunc(3000,updateRead,0);
}

void updateTime()
{
    //	struct to_give received;
    //	received=read();
    //	x=received.resultatO-received.resultatE;
    //	y=received.resultatS-received.resultatN;
    time(&t);

    glutPostRedisplay();
    glutTimerFunc(1000,updateTime,0);
}

void *play_sound()
{
    system("paplay source/sonar.wav");
    pthread_exit(NULL);
}

void updateWave()
{
    //	struct to_give received;
    //	received=read();
    //	x=received.resultatO-received.resultatE;
    //	y=received.resultatS-received.resultatN;

    if (r>=2.5)
        r=0;
    else
        r+=0.01;

    //float val=sqrtf(x*x+y*y);
    //printf("%f\n",val);
    if ((sqrtf(x*x+y*y)-0.05<=r && r<= sqrtf(x*x+y*y)+0.05) && (x!=0 || y!=0))
    {
        collision=1;
        if (sqrtf(x*x+y*y)-0.005<=r && r<= sqrtf(x*x+y*y)+0.005)
        {
            pthread_t thread_sound;
            pthread_create (&thread_sound,NULL,&play_sound,NULL);
            pthread_detach (thread_sound);
        }
    }
    else
        collision=0;
    glutPostRedisplay();
    glutTimerFunc(10,updateWave,0);
}

void drawGraph()
{
    //printf("%f\n",f);

    glClear(GL_COLOR_BUFFER_BIT);

    //graph
    glLineWidth(2);

    glBegin(GL_LINES);
    glVertex2f(-0.75,0);
    glVertex2f(0.75,0);
    glVertex2f(0,-0.75);
    glVertex2f(0,0.75);
    glEnd();
    //graph end

    //circle
    glLineWidth(1);
    drawCircle(0,0,0.25,150);
    drawCircle(0,0,0.45,150);
    drawCircle(0,0,0.65,150);
    //circle end 

    //wave
    if (collision==0 || (x==0&&y==0))
    {
        glColor3f(1,0,0);
        glLineWidth(1);
    }
    else
    {
        glColor3f(1,0,1);
        glLineWidth(5);
    }
    drawCircle(0,0,r,150);
    //wave end 

    //detection
    if (x==0 && y==0)
        glColor3f(0,1,0);
    else
        glColor3f(1,0,0);

    glPointSize(15);
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(x,y);
    glEnd();
    //detection end

    //channel
    glColor3f(0,1,0);
    char *chan=malloc(sizeof(char)*20);
    char *channum=malloc(sizeof(char)*5);
    strcpy(chan,"CHANNEL : ");
    sprintf(channum,"%d",channel);
    strcat(chan,channum);
    int c = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, chan);
    glRasterPos2f(-0.98, 0.95);
    int len = strlen(chan);
    for (size_t i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,chan[i]);
    }
    //channel end

    //time
    glColor3f(0,1,0);
    int w = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, ctime(&t));
    glRasterPos2f(-1, -1);
    len = strlen(ctime(&t));
    for (size_t i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ctime(&t)[i]);
    }
    //time end

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
    //glutInitWindowSize(800,600);
    glutInitWindowSize(1080,1080);

    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Radar");
    initGraph();
    glutDisplayFunc(drawGraph);
    //initPoint();
    //glutDisplayFunc(drawPoint);
    updateWave();
    updateRead();
    updateTime();
    glutMainLoop();
    return 0;
}
