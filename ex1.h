////////////////////////////////////////////////////////////////
// School of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons 
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Skeleton code for COMP37111 coursework, 2013-14
//
// Authors: Arturs Bekasovs and Toby Howard
//
/////////////////////////////////////////////////////////////////

#ifndef ex1_h
#define ex1_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "bitmap.c"


#ifdef MACOS
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

// a struct to define the particles
typedef struct
{
  int active; // if the particle is active
  float life; // life time
  float r, g, b;  // the r, g b colour for the particles
  float x_ini, y_ini, z_ini;  // the initial coordinates
  float x, y, z; // the coordinate
  float xV, yV, zV; // the moving coordinate
  float xAcc, yAcc, zAcc; // the coordinate under the gravity
} particles;

#define MAX_PARTICLES 500000
#define X_INITIAL 500
#define Y_INITIAL 500
#define Z_INITIAL 0
#define GRAVITY 0.0098

particles particle[MAX_PARTICLES];

int AXIS_SIZE = 200;
int axisEnabled;
int snowEnabled = 1;
int fountainEnabled = 0;
int rainbowEnabled = 0;
int textureEnabled = 0;
int sphereEnabled = 0;
int multiSourceEnabled = 0;

float tick;
float time_pass;
int lifeEnd = -500;
int particleNum = 500;
char num[10];

float snowXini[MAX_PARTICLES]; // to store the initial x for snow

GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_position0[] = {50.0, 200.0, 600.0, 0.0};
// GLfloat light_position0[] = {0, 0, 800, 600};
GLfloat light_position1[] = {-100.0, -50.0, 600.0, 0.0};
GLfloat ambient_Light[] = {0.5,0.5,0.5,1.0};

GLfloat matSpecular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat matShininess[] = {100.0};

BITMAPINFO *TexInfo; // Texture bitmap information
GLubyte    *TexBits; // Texture bitmap pixel bits
GLint texture;

double myRandom();
void drawParticle(int i);
void move(int i, float xMove, float yMove, float zMove);
void display();
void keyboard(unsigned char key);
void reshape(int width, int height);
void makeAxes();
void drawString (void *font, float x, float y, char *str);
void menu (int menuentry);
void init();
void initGraphics(int argc, char *argv[]);
int main(int argc, char *argv[]);

#endif /* ex1_h */

