////////////////////////////////////////////////////////////////
// School of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons 
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Skeleton code for COMP37111 coursework, 20013-14
//
// Authors: Arturs Bekasovs and Toby Howard and Annan Wang
//
/////////////////////////////////////////////////////////////////

#include "ex1.h"

///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  // return (rand()/(double)RAND_MAX);
  return (rand()/(double)RAND_MAX * 2 - 1); // range[-1,1]
}

///////////////////////////////////////////////
void drawParticle(int i)
{
  // glPointSize(3);
  glColor3f(particle[i].r, particle[i].g, particle[i].b);

  // glBegin(GL_POINTS);
  //   glVertex3f(particle[i].x, particle[i].y, particle[i].z);
  // glEnd();
  if (fountainEnabled && sphereEnabled) // draw spheres
  {
    glPushMatrix();
      glTranslatef(particle[i].x, particle[i].y, particle[i].z);
      glRotatef(particle[i].x, 1, 0, 0);
      glRotatef(particle[i].y, 1, 0, 0);
      glRotatef(particle[i].z, 1, 0, 0);
      glutSolidSphere(10, 20, 20);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  } // else if
  else if (textureEnabled)
  { 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TexInfo->bmiHeader.biWidth,
                 TexInfo->bmiHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TexBits);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
      glTexCoord2d(0,0);glVertex3f(particle[i].x - 20, particle[i].y - 20, particle[i].z); // left bottom
      glTexCoord2d(0,1);glVertex3f(particle[i].x - 20, particle[i].y + 20, particle[i].z); // left top
      glTexCoord2d(1,1);glVertex3f(particle[i].x + 20, particle[i].y + 20, particle[i].z); // right top
      glTexCoord2d(1,0);glVertex3f(particle[i].x + 20, particle[i].y - 20, particle[i].z); // right bottom
    glEnd();

    glDisable(GL_TEXTURE_2D);
  } // if
  else
  {
    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2d(1.0,0.0); glVertex3f(particle[i].x, particle[i].y - 5, particle[i].z);  
      glTexCoord2d(0.5,1.5); glVertex3f(particle[i].x - 5, particle[i].y + 2.5, particle[i].z); 
      glTexCoord2d(1.5,1.5); glVertex3f(particle[i].x + 5, particle[i].y + 2.5, particle[i].z); 
      glTexCoord2d(1.0,2.0); glVertex3f(particle[i].x, particle[i].y + 5, particle[i].z); 
      glTexCoord2d(0.5,0.5); glVertex3f(particle[i].x - 5, particle[i].y - 2.5, particle[i].z);  
      glTexCoord2d(1.5,0.5); glVertex3f(particle[i].x + 5, particle[i].y - 2.5, particle[i].z); 
    glEnd();
  } // else
} // draw

///////////////////////////////////////////////

void move(int i, float xMove, float yMove, float zMove)
{
  tick = particle[i].life + time_pass;
  particle[i].life = tick;
  xMove = particle[i].x_ini + (particle[i].xV * tick + (float)(0.5 * particle[i].xAcc * pow(tick, 2)));
  yMove = particle[i].y_ini + (particle[i].yV * tick - (float)(0.5 * particle[i].yAcc * pow(tick, 2)));
  zMove = particle[i].z_ini + (particle[i].zV * tick + (float)(0.5 * particle[i].zAcc * pow(tick, 2)));

  particle[i].x = xMove;
  particle[i].y = yMove;
  particle[i].z = zMove;
} // move

///////////////////////////////////////////////

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0.0, 200.0, 1000.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // If enabled, draw coordinate axis
  if (axisEnabled)
    makeAxes();

  int i;
  if (snowEnabled)
  {
    glColor3f(1.0, 1.0, 1.0);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    drawString(GLUT_BITMAP_8_BY_13, -800, -600, "current mode: snow");
  }
  else if (fountainEnabled)
  {
    glColor3f(1.0, 1.0, 1.0);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    drawString(GLUT_BITMAP_8_BY_13, -800, -600, "current mode: fountain");

    if (multiSourceEnabled)
    {
      glColor3f(1.0, 1.0, 1.0);
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
      drawString(GLUT_BITMAP_8_BY_13, -100, -600, "Multi-Source");
    } // if
  } // else if

  if (textureEnabled)
  {

    if (snowEnabled)
      TexBits = LoadDIBitmap("1.bmp", &TexInfo);
    else if (fountainEnabled)
      TexBits = LoadDIBitmap("snow1.bmp", &TexInfo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH); 

    glColor3f(1.0, 1.0, 1.0);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    drawString(GLUT_BITMAP_8_BY_13, -400, -600, "textured");
  } // if
  else if (rainbowEnabled)
  {
    glColor3f(1.0, 1.0, 1.0);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    drawString(GLUT_BITMAP_8_BY_13, -400, -600, "rainbow");
  } // else if
  else if (fountainEnabled && sphereEnabled)
  {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    drawString(GLUT_BITMAP_8_BY_13, -400, -600, "Sphere fountain");
    // light properties
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient_Light);
    // material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  } // else if 

  glColor3f(1.0, 1.0, 1.0);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  drawString(GLUT_BITMAP_8_BY_13, 350, -600, "Particle Number:");
  sprintf(num, "%d", particleNum);
  drawString(GLUT_BITMAP_8_BY_13, 650, -600, num);

  for (i = 0; i < particleNum; i++)
  {
    if (snowEnabled)
    {
      particle[i].x_ini = snowXini[i];
      particle[i].y_ini = Y_INITIAL;
      particle[i].z_ini = Z_INITIAL * myRandom();
    } // if

    if (fountainEnabled)
    {
      particle[i].x_ini = X_INITIAL * 0.1;
      particle[i].y_ini = Y_INITIAL * 0.1;
      particle[i].z_ini = 0.0;

      if (multiSourceEnabled)
      {
        if (i % 2 == 0)
        {
          particle[i].x_ini = -X_INITIAL;
          particle[i].xAcc = 0.0098; 
          particle[i].yAcc *= 0.1;

          particle[i].r = fabs(myRandom());
          particle[i].g = fabs(myRandom());
          particle[i].b = 1.0;
        } // if
      } // if
    } // if
  } // for

  for (i = 0; i < particleNum; i++)
  {

    if (rainbowEnabled)
    {
      particle[i].r = fabs(myRandom());
      particle[i].g = fabs(myRandom());
      particle[i].b = fabs(myRandom());
    } // if
    drawParticle(i);

    if (particle[i].y > lifeEnd)
      move(i, particle[i].x, particle[i].y, particle[i].z);
    else 
    {
      particle[i].life = 0;

      particle[i].x = particle[i].x_ini;
      particle[i].y = particle[i].y_ini;
      particle[i].z = particle[i].z_ini; 

      move(i, particle[i].x, particle[i].y, particle[i].z);
    } // else

  } // for

  glutPostRedisplay();
  glutSwapBuffers();
} // display

///////////////////////////////////////////////

void keyboard(unsigned char key)
{
  int i;
  switch(key)
  {
    case 27 /* Escape key */:     exit(0);
    case 120 /* 'x' */:  axisEnabled = !axisEnabled; break;

    case '1' /* '1' */:  snowEnabled = 1; fountainEnabled = 0; 
                         rainbowEnabled = 0; textureEnabled = 0; 
                         multiSourceEnabled = 0; sphereEnabled = 0; break;
    case '2' /* '2' */:  fountainEnabled = 1; snowEnabled = 0; 
                         rainbowEnabled = 0; textureEnabled = 0; 
                         multiSourceEnabled = 0; sphereEnabled = 0; break;
    case '3' /* '3' */:  rainbowEnabled = !rainbowEnabled; break;
    case '4' /* '4' */:  textureEnabled = !textureEnabled; sphereEnabled = 0; break;

    if (fountainEnabled)
    {                   
      case '5' /* '5' */:  multiSourceEnabled = !multiSourceEnabled;
                           sphereEnabled = 0; break;
      case '6' /* '6' */: sphereEnabled = !sphereEnabled; textureEnabled = 0; break;
    } // if

    case 32 /* space to reset */: 

        time_pass = 0.1;
        lifeEnd = -500;
        particleNum = 500;
        for (i = 0; i < particleNum; i++)
        {
          particle[i].life = 0;

          particle[i].r = 1.0;
          particle[i].g = 1.0;
          particle[i].b = 1.0;

          particle[i].x = particle[i].x_ini;
          particle[i].y = particle[i].y_ini;
          particle[i].z = particle[i].z_ini;          

          particle[i].xV = myRandom();  
          particle[i].yV = myRandom() * 2.0;         
          particle[i].zV = myRandom();
          particle[i].xAcc = myRandom() * 0.001;
          particle[i].yAcc = GRAVITY;
          particle[i].zAcc = myRandom() * 0.001;
        } // for 
        break;

    case 97 /* 'a' */:
        for (i = 0; i < particleNum; i++)
          particle[i].xAcc -= 0.001;
        break;

    case 100 /*'d'*/:
        for (i = 0; i < particleNum; i++)
          particle[i].xAcc += 0.001; 
        break;

    case 99 /* 'c' */:
        for (i = 0; i < particleNum; i++)
        {
          particle[i].r = fabs(myRandom());
          particle[i].g = fabs(myRandom());
          particle[i].b = fabs(myRandom());
        } // for
        break;

    case 115 /* 's' */:
        for (i = 0; i < particleNum; i++)
          particle[i].yAcc *= 5;
        break;

    case 119 /* 'w' */:
        for (i = 0; i < particleNum; i++)
          particle[i].yAcc *= 0.1;
        break;

    case 107 /* 'k' */: 
          if (lifeEnd < 0)
            lifeEnd *= 0.5;
          else if (lifeEnd == 0)
            lifeEnd = 1;
          else if (lifeEnd <= 200)
            lifeEnd *= 2;
          break;

    case '=':
          if (particleNum < MAX_PARTICLES / 2)
            particleNum *= 2;
          else
            particleNum = MAX_PARTICLES;
          break;

    case '-':
          if (particleNum > 1)
          {
            double temp;
            particleNum /= 2;
            temp = (particleNum % 2) / 2;
            if (temp >= 0.5)
              particleNum++;
          } // if
          else
          {
            particleNum = 1;
            multiSourceEnabled = 0;
          }
          break;
  } // switch
} // keyboard

///////////////////////////////////////////////

void reshape(int width, int height)
{
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

///////////////////////////////////////////////

void makeAxes() {
// Create a display list for drawing coord axis
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  glLineWidth(2.0);
  glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);       // X axis - red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(AXIS_SIZE, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);       // Y axis - green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, AXIS_SIZE, 0.0);
    glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, AXIS_SIZE);
  glEnd();
  glFlush();
}

void drawString (void *font, float x, float y, char *str)
{ /* Displays the string "str" at (x,y,0), using font "font" */

  char *ch;
  glRasterPos3f(x,y,0.0);
  for(ch = str; *ch; ch++)
  {
    glutBitmapCharacter(font,(int)*ch);
  }
}

///////////////////////////////////////////////
void menu (int menuentry) {
  int i;
  switch (menuentry) {
  case 1: axisEnabled = !axisEnabled; break;

  case 2:  snowEnabled = 1; fountainEnabled = 0; 
           rainbowEnabled = 0; textureEnabled = 0; 
           multiSourceEnabled = 0; sphereEnabled = 0; break;
  case 3:  fountainEnabled = 1; snowEnabled = 0; 
           rainbowEnabled = 0; textureEnabled = 0; 
           multiSourceEnabled = 0; sphereEnabled = 0; break;
  case 4:  rainbowEnabled = !rainbowEnabled; break;
  case 5:  textureEnabled = !textureEnabled; sphereEnabled = 0;break;

  if (fountainEnabled)
  {                   
    case 6: multiSourceEnabled = !multiSourceEnabled;
            sphereEnabled = 0; break;
    case 7: sphereEnabled = !sphereEnabled; textureEnabled = 0; break;
  }

  case 8: 
          time_pass = 0.1;
          lifeEnd = -500;
          particleNum = 500;
          for (i = 0; i < particleNum; i++)
          {
            particle[i].life = 0;

            particle[i].r = 1;
            particle[i].g = 1;
            particle[i].b = 1;

            particle[i].x = particle[i].x_ini;
            particle[i].y = particle[i].y_ini;
            particle[i].z = particle[i].z_ini;
            
            particle[i].xV = myRandom();
            particle[i].yV = myRandom() * 2.0;
            particle[i].zV = myRandom();
            particle[i].xAcc = myRandom() * 0.001;
            particle[i].yAcc = GRAVITY;
            particle[i].zAcc = myRandom() * 0.001;
          } // for
          break;

  case 9:
      for (i = 0; i < particleNum; i++)
        particle[i].xAcc -= 0.01;
      break;

  case 10:
      for (i = 0; i < particleNum; i++)
        particle[i].xAcc += 0.01; 
      break;

  case 11:
      for (i = 0; i < particleNum; i++)
      {
        particle[i].r = fabs(myRandom());
        particle[i].g = fabs(myRandom());
        particle[i].b = fabs(myRandom());
      }
      break;

  case 12:
      for (i = 0; i < particleNum; i++)
        particle[i].yAcc *= 5;
      break;

  case 13:
      for (i = 0; i < particleNum; i++)
        particle[i].yAcc *= 0.1;
      break;

  case 14: 
          if (lifeEnd < 0)
            lifeEnd *= 0.5;
          else if (lifeEnd == 0)
            lifeEnd = 1;
          else if (lifeEnd <= 200)
            lifeEnd *= 2;
          break;

  case 15:
      if (particleNum < MAX_PARTICLES / 2)
      {
        particleNum *= 2;
        time_pass *= 1.5;
      }
      else
        particleNum = MAX_PARTICLES;
      break;

  case 16:
      if (particleNum > 1)
      {
        double temp;
        particleNum /= 2;
        temp = (particleNum % 2) / 2;
        if (temp >= 0.5)
          particleNum++;
      } // if
      else
      {
        particleNum = 1;
        multiSourceEnabled = 0;
      }
      break;

  case 17: exit(0); break;
  } // switch
} // menu

///////////////////////////////////////////////
void init()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // set black as the background colour

  glutCreateMenu(menu);
  glutAddMenuEntry("Show Axis (x)", 1);

  glutAddMenuEntry("Snow (1)", 2);
  glutAddMenuEntry("Fountain (2)", 3);
  glutAddMenuEntry("Rainbow (3)", 4);
  glutAddMenuEntry("Textured (4)", 5);

  glutAddMenuEntry("Multi-Source (5) only available in mode fountain", 6);
  glutAddMenuEntry("Sphere Rendering (6) only available in mode fountain", 7);

  glutAddMenuEntry("Reset (space)", 8);
  glutAddMenuEntry("Wind from left (a)", 9);
  glutAddMenuEntry("Wind from right (d)", 10);
  glutAddMenuEntry("Change colour (c)", 11);
  glutAddMenuEntry("Increase the intensity of gravity (s)", 12);
  glutAddMenuEntry("Decrease the intensity of gravity (w)", 13);
  glutAddMenuEntry("Shorten the life-time (k)", 14);
  glutAddMenuEntry("Add particles (=)", 15);
  glutAddMenuEntry("Less particles (-)", 16);
  glutAddMenuEntry ("Quit (esc)", 17);

  glutAttachMenu (GLUT_RIGHT_BUTTON);

  int i;
  time_pass = 0.1;

  for (i = 0; i < particleNum; i++)
  {
    particle[i].life = 0;

    particle[i].r = 1;
    particle[i].g = 1;
    particle[i].b = 1;

    particle[i].x_ini = (X_INITIAL * myRandom());
    snowXini[i] = particle[i].x_ini;
    particle[i].y_ini = Y_INITIAL;
    particle[i].z_ini = Z_INITIAL * myRandom();

    particle[i].x = particle[i].x_ini;
    particle[i].y = particle[i].y_ini;
    particle[i].z = particle[i].z_ini;
    
    particle[i].xV = myRandom();
    particle[i].yV = myRandom() * 2.0;
    particle[i].zV = myRandom();
    particle[i].xAcc = myRandom() * 0.001;
    particle[i].yAcc = GRAVITY;
    particle[i].zAcc = myRandom() * 0.001;
  } // for
} // init

///////////////////////////////////////////////
void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("COMP37111 Particles");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
}

/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  srand(time(NULL));
  initGraphics(argc, argv);
  glEnable(GL_POINT_SMOOTH);
  glutMainLoop();
  return 0;
}
