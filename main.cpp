#define GLFW_INCLUDE_GLCOREARB
#define GLFW_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
using namespace std;
#include <GLUT/glut.h> // GLUT, include glu.h and gl.h
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Pipe.h"

//distance formula for circle

//======================================================
// CONSTANTS
//======================================================
// Square size
#define size 50.0
//======================================================
// GLOBAL VARIABLES WRITTEN TO BY reshapeCallBack( )
//======================================================
// Window size
int w_height = 800;
int w_width = 800;
//======================================================
// GLOBAL VARIABLES WRITTEN TO BY displayCallBack( )
//======================================================
int display_count = 0; //keeps count of invocations

//======================================================
// GLOBAL VARIABLES WRITTEN TO BY keyboardCallBack( )
//======================================================
float theta = 0.0; // Rotation angle of bird
int timer_period_in_ms = 100; // time between timer callbacks in milliseconds

//====================================================================
// GLOBAL VARIABLES WRITTEN TO BY motionCallBack( )
//====================================================================
int bird_r=20;
double head_r=40;
double bird_rot=20;

//mkae a vector
//updates function

//wing theta wing delta
float bird_x=40;
float bird_y=40; // position of bird
double bird_vx = 0;
double bird_vy = 0;//
double bird_ax = 0;
double bird_ay = 0;
int bird_dx = 1; // incremental change inbird_x
float dt = 0.15; //0.05;
double impulse = 0;
double impulseMax = 95;
double vyMax = 40;
double vxMax = 40;

double obj_x = 1000;
double obj_y = 200;

vector< pair <double,double> > oldPositions;
double arm_theta=0.0;
double arm_delta=5.0;
double panda_radius=20;
bool hasStarted = false;
bool isSimulating = false;
bool rotate_panda=false;
int numUpdates = 0;
Pipe p;

//======================================================
// IDLE CALLBACK ROUTINE
//======================================================
void bounce() {
  //bird_ax += 0;
  //bird_ay += 20;
  impulse += impulseMax;
  bird_ax += 2;
    
}

void boundVelocityAcceleration(double& vx, double& vy, double& ax, double& ay) {
  if( vx > vxMax ) vx = vxMax;
  if( vy > vyMax ) vy = vyMax;
  if( ax > 20 ) ax = 20;
  if( ay < -10 ) ay = -9;
}

void boundImpulse() {
  if(impulse > impulseMax) impulse = impulseMax;
    if(impulse < 0.5) impulse = 0;
}

void update()
{
  numUpdates++;
  if(numUpdates%100==0) cout << "Num updates = " << numUpdates << "bird_x = " << bird_x << " bird_y = " << bird_y << " vx = " << bird_vx << " vy = " << bird_vy << " ax = " << bird_ax << " ay = " << bird_ay << endl;
  
    //add animations aka make panda "flap"
    oldPositions.push_back(make_pair(bird_x, bird_y));
    arm_theta+=arm_delta;
    
    if(arm_theta>30){
        arm_delta *= -1;
        arm_theta+=arm_delta;
    }
    if(arm_theta<-30){
        arm_delta *= -1;
        arm_theta+=arm_delta;
    }
    
  if(hasStarted) {
    //acceleration gets updated
    bird_ay += (-4.8 + impulse ) * dt;
    //velocity gets updated
    bird_vx += bird_ax * dt;
    bird_vy += bird_ay * dt;
    //position gets updated
    bird_x += bird_vx * dt;
    bird_y += bird_vy * dt;
    boundVelocityAcceleration(bird_vx, bird_vy, bird_ax, bird_ay);
    impulse *= 0.3;
    boundImpulse();
  }

  if(bird_y-bird_r<0) { bird_y = bird_r;
    bird_vy = 0;
    bird_ay = 0;
  }
    
  //generate random circles
  if(obj_x < (bird_x-w_width/2.0)) {
      //reset circle
    //random x
    obj_x = bird_x + w_width/2.0 + (50.0*rand())/RAND_MAX;
    //random y
    obj_y = (rand())%w_height;
  }
   
    //generate random pipes
    if(p.x < (bird_x-w_width/2.0)) {
        //reset cirle
      //random x
      p.x = bird_x + w_width/2.0 + (50.0*rand())/RAND_MAX;
      //random y
      p.y = 20 + (rand())%(w_height-400);
      //random gap
        p.gap=300+rand()%100;
    }
  glutPostRedisplay();
}

//======================================================
// RESHAPE CALLBACK ROUTINE
//======================================================
void reshapeCallback(int width, int height)
{
  //Update globals containing window size
  w_width = width;
  w_height = height;
}

//======================================================
// MOUSE MOTION CALLBACK ROUTINE
//======================================================
void motionCallBack(int x, int y)
{
  printf("YYYMotion call back: %d, %d)\n", x, y);
  //Set bird's location to current mouse position
  bird_x = x;
  bird_y = w_height - y; //Invert mouse y (as its measured from top)

  glutPostRedisplay();
}

void Collision(){
    double pipeCase=p.x-(p.x-head_r);
    double distance=head_r+pipeCase;

    //collision between high and low plane
    double top_collision=w_height-panda_radius;//height of window- radius panda
    if(bird_y>=top_collision){
        exit(-1);
    }
    double bottom_collision=panda_radius;//height of window- radius panda
    if(hasStarted==true && bird_y<=bottom_collision){
        exit(-1);
    }

    //Collision with Pipe
    if(bird_x>p.x-4 && bird_x<p.x+p.w){
        if(bird_y<=p.y+30|| bird_y > p.y+p.gap-4){
            cout<<endl;
            cout<<"Game Over!"<<endl;
            exit(0);
        }
    }
}


//======================================================
// MOUSE CALLBACK ROUTINE
//======================================================
void mouseCallBack(int btn, int state, int x, int y)
{
  printf("Mouse call back: button=%d, state=%d, x=%d, y=%d\n", btn, state, x, y);
  if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)   exit(0);
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        hasStarted = true;
        isSimulating = true;
        rotate_panda=false;
        bounce();
    }
}


//======================================================
// KEYBOARD CALLBACK ROUTINE
//======================================================

void keyboardCallBack(unsigned char key, int x, int y)
{
  printf("Keyboard call back: key=%c, x=%d, y=%d, theta=%f\n", key, x, y, theta);
  switch (key)
  {
    case 'i':
    case 'I':
      if( !isSimulating ) {
    isSimulating = !isSimulating;
    printf("Idle function ON\n");
    break;
      }
      else {
    isSimulating = !isSimulating;
    glutIdleFunc(NULL);
    printf("Idle function OFF\n");
    break;
      }
    case ' ':
      hasStarted = true;
      isSimulating = true;
      rotate_panda=false;
      bounce();
      break;
    case 'r':
      hasStarted = true;
      isSimulating = true;
      rotate_panda=true;
      break;
    case 27:
    case 'q':
      cout << "Exiting" << endl;
      exit(-1);
    default:
      printf("Press i (Idle Off), I (Idle ON) or r (Rotate)");
  }

  glutPostRedisplay();
}

// Function to draw the shapes
void draw(GLfloat ctrlpoints[4][3])
{
    glShadeModel(GL_FLAT);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4,
            &ctrlpoints[0][0]);
 
    glEnable(GL_MAP1_VERTEX_3);
 
    // Fill the color
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
 
    // Find the coordinates
    for (int i = 0; i <= 30; i++)
        glEvalCoord1f((GLfloat)i / 30.0);
 
    glEnd();
    glFlush();
}

void DrawCircle(float r, int num_segments) {
  float theta = 2 * 3.1415926 / float(num_segments);
  glBegin(GL_POLYGON);
  for(int ii=0; ii<num_segments; ii++) {
    float xi = r * cos( (theta*ii) );
    float yi = r * sin( (theta*ii) );
    glVertex2f(xi,yi);
  }
  glEnd();
}

void DrawPanda(float r, int num_segments) {
  float theta = 2 * 3.1415926 / float(num_segments);
    //ears
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 9 * cos( (arm_delta*ii) );
      float yi = 7.5 * sin( (arm_delta*ii) );
      glVertex2f(xi+17.5,yi+16.5);
    }
    glEnd();
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 9 * cos( (arm_delta*ii) );
      float yi = 7.5 * sin( (arm_delta*ii) );
      glVertex2f(xi-17.5,yi+16.5);
    }
    glEnd();
    //arm
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 17 * cos( (arm_delta*ii) );
      float yi = 7.5 * sin( (arm_delta*ii) );
       
      glVertex2f(xi-15,yi-30);
    }
    glEnd();
    
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 17 * cos( (arm_delta*ii) );
      float yi = 7.5 * sin( (arm_delta*ii) );
      glVertex2f(xi+15,yi-30);
    }
    glEnd();
    //body
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  glBegin(GL_POLYGON);
  for(int ii=0; ii<num_segments; ii++) {
    float xi = 17 * cos( (theta*ii) );
    float yi = 22 * sin( (theta*ii) );
    glVertex2f(xi,yi-35);
  }
    glEnd();
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);//white
  glBegin(GL_POLYGON);
  for(int ii=0; ii<num_segments; ii++) {
    float xi = 10 * cos( (theta*ii) );
    float yi = 12 * sin( (theta*ii) );
    glVertex2f(xi,yi-35);
  }
    glEnd();
    //head
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);//white
  glBegin(GL_POLYGON);
  for(int ii=0; ii<num_segments; ii++) {
    float xi = panda_radius * cos( (theta*ii) );
    float yi = panda_radius * sin( (theta*ii) );
    glVertex2f(xi,yi);
  }
  glEnd();
    //eyes
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 6 * cos( (theta*ii) );
      float yi = 7.5 * sin( (theta*ii) );
      glVertex2f(xi-8,yi+5);
    }
    glEnd();
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 6 * cos( (theta*ii) );
      float yi = 7.5 * sin( (theta*ii) );
      glVertex2f(xi+8,yi+5);
    }
    glEnd();
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 3 * cos( (theta*ii) );
      float yi = 3 * sin( (theta*ii) );
      glVertex2f(xi+8,yi+5);
    }
    glEnd();
    glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 3 * cos( (theta*ii) );
      float yi = 3 * sin( (theta*ii) );
      glVertex2f(xi-8,yi+5);
    }
    glEnd();
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 1.5 * cos( (theta*ii) );
      float yi = 1.5 * sin( (theta*ii) );
      glVertex2f(xi-8,yi+5);
    }
    glEnd();
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 1.5 * cos( (theta*ii) );
      float yi = 1.5 * sin( (theta*ii) );
      glVertex2f(xi+8,yi+5);
    }
    glEnd();
    //mouth
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 3 * cos( (theta*ii) );
      float yi = 2 * sin( (theta*ii) );
      glVertex2f(xi,yi-3.5);
    }
    glEnd();
    
    glColor3ub(0, 1, 0);
    glBegin(GL_LINES);
        glVertex2f(.5, -3.5);
        glVertex2f(.5, -10);
    
        glVertex2f(.5, -10);
        glVertex2f(3.5, -8.5);
    
        glVertex2f(-.5, -10);
        glVertex2f(-3.5, -8.5);
    glEnd();
    //feet
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 8 * cos( (theta*ii) );
      float yi = 8 * sin( (theta*ii) );
      glVertex2f(xi-10,yi-53);
    }
    glEnd();
    
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    for(int ii=0; ii<num_segments; ii++) {
      float xi = 8 * cos( (theta*ii) );
      float yi = 8 * sin( (theta*ii) );
      glVertex2f(xi+10,yi-53);
    }
    glEnd();
}
double angle=0;
void timer(int){
    glutPostRedisplay();
    glutTimerFunc(1000/60,timer,0);
    
    angle+=0.8;
    if(angle>90){
        angle=angle-90;
    }
}

//======================================================
// DISPLAY CALL BACK ROUTINE
//======================================================
void displayCallBack(void)
{
  printf("Display call back %d\n", display_count++);
  cout << "pos of bird [" << bird_x << "," << bird_y << "]"<< endl;

  if( bird_x > w_width/2 ) {
    // Set Projection Mode and Volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0.0, w_width, 0.0, w_height, -1.0, 1.0);
    glOrtho(bird_x-w_width/2, bird_x+w_width/2, 0.0, w_height, -1.0, 1.0);
  }
  glClearColor(0.5, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
    
  //draw panda
  glPushMatrix();
  glTranslatef(bird_x, bird_y, 0.0);
    if(rotate_panda==true){
        glRotatef(bird_rot, 0, 0, 1);
    }
  Collision();
  DrawPanda(head_r,20);
  glPopMatrix();

   //draw obj
  glPushMatrix();
  glTranslatef(obj_x, obj_y, 0.0);
  glColor3f(0, 1, 0);
  Collision();
  DrawCircle(bird_r,20);
  glPopMatrix();
    
  //draw pipes
  p.Draw();

  //Swap double buffers
  glutSwapBuffers();
}
    

//======================================================
// MAIN PROGRAM
//======================================================
int main(int argc, char** argv)
{
  // Allow cmd line arguments to be passed to the glut
  glutInit(&argc, argv);

  // Create and name window
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(w_width, w_height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow("FLAPPY PANDA :)");
    glutMouseFunc(mouseCallBack);
  // Set Projection Mode and Volume
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, w_width, 0.0, w_height, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);

  // Set clear color to black and clear window
  glClearColor(1.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
  // Set up callbacks
  glutKeyboardFunc(keyboardCallBack);
  glutDisplayFunc(displayCallBack);
  glutReshapeFunc(reshapeCallback);
  glutIdleFunc(update);
  // Enter main event loop
  glutMainLoop();
}
