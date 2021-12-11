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
int channel=7; //channel scanned
int isautomode=0; //auto mode

#include "read.c"

#define RESOLUTION 1080

// library installation : sudo apt-get install freeglut3
//                        sudo apt-get install freeglut3-dev
// Library verification : ls /usr/include/GL/ -l
// compilation :  gcc main.c -o out -I /usr/include -lglut -lGL -lphthread
// execution : ./out

float x=0; //position of detection
float y=0; //position of detection

float r=0; //rayon of the wave

int collision = 0; //if wave deetects somthing


float my=0; //mouse position
float mx=0;; //mouse position

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
    //channel=7;
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

void mouse(int button, int state, int mousex, int mousey)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        mx = mousex;
        my = mousey;
        printf("%f %f\n",mx,my);
        glutPostRedisplay();
    }
    if (mx>0 && mx <((75.f/1080)*RESOLUTION) && my > ((125.f/1080)*RESOLUTION) && my <((175.f/1080)*RESOLUTION) && channel <16)
    {
        channel++;
        printf("increase channel\n");
    }
    else if (mx>0 && mx <((75.f/1080)*RESOLUTION) && my > ((50.f/1080)*RESOLUTION) && my <((100.f/1080)*RESOLUTION) && channel >1)
    {
        channel--;
        printf("decrease channel\n");
    }
    else if (mx>0 && mx <((75.f/1080)*RESOLUTION) && my > ((200.f/1080)*RESOLUTION) && my <((275.f/1080)*RESOLUTION) && isautomode==0)
    {
        isautomode=1;
        printf("auto on\n");
    }
    else if (mx>0 && mx <((75.f/1080)*RESOLUTION) && my > ((200.f/1080)*RESOLUTION) && my <((275.f/1080)*RESOLUTION) && isautomode==1)
    {
        isautomode=0;
        printf("auto off\n");
    }
    else if (mx>0.85*RESOLUTION && my > ((0.85)*RESOLUTION))
    {
        exit(0);
    }
    mx = 0;
    my = 0;
    printf("%f %f\n",mx,my);
}

void autoMode()
{
    //printf("isautomode=%d\n",isautomode);
    if (isautomode==1)
        glColor3f(0,1,0);
    else
        glColor3f(1,0,0);
    glLineWidth(3);
    drawCircle(-0.94,0.55,0.05,150);
    drawCircle(-0.94,0.55,0.025,150);
    drawCircle(-0.94,0.55,0.005,150);
    glEnd();


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


    //plus&minus
    if (isautomode==0)
        glColor3f(0,1,0);
    else
        glColor3f(1,0,0);
    glLineWidth(3);

    glBegin(GL_LINES);
    glVertex2f(-0.98,0.85);
    glVertex2f(-0.90,0.85);


    glVertex2f(-0.98,0.70);
    glVertex2f(-0.90,0.70);
    glVertex2f(-0.94,0.66);
    glVertex2f(-0.94,0.74);

    glEnd();
    //plus&minus end

    //auto mode
autoMode();
    //auto mode end

    //arrow
        glColor3f(0,1,0);
    glLineWidth(5);

    glBegin(GL_LINES);
    glVertex2f(0.90,0.80);
    glVertex2f(0.90,0.95);

    glVertex2f(0.90,0.95);
    glVertex2f(0.85,0.90);

    glVertex2f(0.90,0.95);
    glVertex2f(0.95,0.90);

    glEnd();
    glColor3f(0,0,1);
    //arrow end

//quit cross
        glColor3f(1,0,0);
    glLineWidth(5);

    glBegin(GL_LINES);
    glVertex2f(0.90,-0.80);
    glVertex2f(0.80,-0.90);
    glVertex2f(0.80,-0.800);
    glVertex2f(0.90,-0.90);

    glEnd();
    glColor3f(0,0,1);
    //quit cross end



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
    char *chan=malloc(sizeof(char)*50);
    char *channum=malloc(sizeof(char)*5);
    strcpy(chan,"PMR446 - CHANNEL : ");
    sprintf(channum,"%d",channel);
    strcat(chan,channum);
    //int c = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, chan);
    glRasterPos2f(-0.98, 0.90);
    int len = strlen(chan);
    for (int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,chan[i]);
    }
    free(chan);
    free(channum);
    //channel end

    //time
    glColor3f(0,1,0);
    //int w = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, ctime(&t));
    glRasterPos2f(-1, -1);
    len = strlen(ctime(&t));
    for (int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ctime(&t)[i]);
    }
    //printf("%f %f\n",mx,my);
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

    glutInitWindowPosition(0,0);
    //glutInitWindowSize(800,600);
    glutInitWindowSize(RESOLUTION,RESOLUTION);

    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Radar");
    initGraph();
    //glutFullScreen();
    glutDisplayFunc(drawGraph);
    //initPoint();
    //glutDisplayFunc(drawPoint);
    updateWave();
    updateRead();
    updateTime();
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
