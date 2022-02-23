#ifndef Pipe_h
#define Pipe_h

class Pipe{

public:
    //default constructor
    Pipe(){
        x=1400;
        y=100;
        w=75;
        gap=200;

    }
    //paramertized constructer
    Pipe(double _x, double _y, double _w, double _gap){
        x=_x;
        y=_y;
        w=_w;
        gap=_gap;
    }
    
    // (p.Draw==bird radius
      //  if distance between (x-r)+
    
   void Draw(){
       //bottom pipe
        glPushMatrix();
        glColor3f(0,0.6,0);
        glBegin(GL_QUADS);
            glVertex2f(x,y);
            glVertex2f(x,0);
            glVertex2f(x+w,0);
            glVertex2f(x+w,y);
       glEnd();
       //bottom lip
       double lipLength=10;
       glColor3f(0,0.3,0);
       glBegin(GL_QUADS);
           glVertex2f(x-lipLength,y+10);//top left
           glVertex2f(x-lipLength,y); //bottom left
           glVertex2f(x+w+lipLength,y);//bottom right
           glVertex2f(x+w+lipLength,y+10);//top right
      glEnd();
       //lines on bottom pipes
       glColor3f(0,0.3,0);
       glLineWidth(4);
       glBegin(GL_LINES);
           glVertex2f(x,y-45);
           glVertex2f(x+w,y-45);
           glVertex2f(x,y-100);
           glVertex2f(x+w,y-100);
           glVertex2f(x,y-220);
           glVertex2f(x+w,y-220);
       glEnd();

      glColor3ub(0, 0, 0);
       glBegin(GL_POLYGON);
       for(int ii=0; ii<20; ii++) {
         float x = 20 * cos( (theta*ii) );
         float y = 20 * sin( (theta*ii) );
        glVertex2f(x-lipLength,y+10);
       }
       glEnd();
       //top pipes
       glColor3f(0,0.6,0);
       glBegin(GL_QUADS);
           glVertex2f(x,y+gap);
           glVertex2f(x,y+gap+700);
           glVertex2f(x+w,y+gap+700);
           glVertex2f(x+w,y+gap);
       glEnd();
       //top lip
       glColor3f(0,0.3,0);
       glBegin(GL_QUADS);
           glVertex2f(x-lipLength,y+gap);//bottom left
           glVertex2f(x-lipLength,y+gap+10);
           glVertex2f(x+w+lipLength,y+gap+10);
           glVertex2f(x+w+lipLength,y+gap);//bottom right
       glEnd();
       //top line decor
       glColor3f(0,0.3,0);
       glLineWidth(4);
       glBegin(GL_LINES);
           glVertex2f(x,y+gap+50);
           glVertex2f(x+w,y+gap+50);
           glVertex2f(x,y+gap+125);
           glVertex2f(x+w,y+gap+125);
           glVertex2f(x,y+gap+225);
           glVertex2f(x+w,y+gap+225);
       glEnd();
       
    }
    

    double theta=0.0;
    double x;
    double y;
    double w;
    double gap;
    
};


#endif /* Pipe_h */
