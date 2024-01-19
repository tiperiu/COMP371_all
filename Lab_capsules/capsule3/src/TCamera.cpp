
#include "TCamera.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Include GLEW
#include <GL/glew.h>
#ifdef __APPLE__ 
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

TCamera::TCamera(){
  view = glm::mat4(1.0);
    proj = glm::mat4(1.0);
    zoom = glm::mat4(1.0);
    
    scroll_zoom = 0.0;

}


TCamera::TCamera(const TCamera& c){
    view = c.view;
    tball = c.tball;
    
}

TCamera& TCamera::operator=(const TCamera& c){
    view = c.view;
    tball = c.tball;
   
    return *this;
}

void  TCamera::ApplyView(int pick){
 
 // TA: PLay with the order by uncommenting various lines
   // view = glm::mat4(1.0);
   // view = glm::translate(view, glm::vec3(3, 3, 0));
    
    
	gview = zoom * view * tball.getMatrix();
   // gview = tball.getMatrix() * view * zoom;
   // gview = zoom * tball.getMatrix()  * view;
    // gview = view * tball.getMatrix()  * zoom;

 

}

void  TCamera::resetview(){
	view = glm::mat4(1.0);
    zoom = glm::mat4(1.0);
    scroll_zoom  = 0.0;
    
}

void  TCamera::resetproj(){

	proj = glm::mat4(1.0);
}


void  TCamera::rotateview(int axis /* 0, 1, 2 = x, y, z */, float degree){

	glm::vec3 a(0, 0, 0);
	if (axis == 0) {
		a[0] = 1;
	}
	else if (axis == 1) {
		a[1] = 1;
	}
	else {
		a[2] = 1;
	}

	view = glm::rotate(view, degree, a);
}
 	  
void  TCamera::translateview(float dx,float dy, float dz){
	view = glm::translate(view, glm::vec3(dx, dy, dz));
	
}

void TCamera:: translateview(glm::vec3 &d){
	view = glm::translate(view, d);
}

void  TCamera::setperspective(int width, int height, float fov, float aspect, float near, float far, int pick){
    // ignore the picking for the moment
  

    double ratio = (double) width / (double) height;
    
   // cout<<"A "<<width<<" "<<height<<" "<<ratio<<endl;
    
	//proj = glm::perspective(fov, aspect, near, far);
    proj = glm::perspective(glm::radians(60.0), ratio, 0.01, 100.0);
    
    
    
//    proj = glm::perspective(60.0, ratio, 0.1, 100.0);
}


void TCamera::setorthographic(int x1, int y1, int x2, int y2){
  

}
    
void TCamera::setorthographic(){
    
}

// trackball API
void TCamera::Resize(int w, int h){
    tball.Resize(w, h);
}

void TCamera::Track(glm::vec2 op, glm::vec2 np){
    tball.track(op, np);
}

void TCamera::Scroll(double y){
    
    //cout<<"O get here! zoom "<<endl;
    
    double zscale = 0.1;
    
    scroll_zoom += y * zscale;
    
    // set some boundaries
  //  if(scroll_zoom<0)
    //    scroll_zoom = 0;
    
   // if(scroll_zoom>1.0-0.01)
     //   scroll_zoom -=y * zscale;
    
    zoom = glm::translate(view, glm::vec3(0, 0, scroll_zoom));
    
}

/// project and unproject business

glm::vec3 TCamera::World2Screen(glm::vec3 P, int weight, int height) {
	glm::vec3 ret;
#if 0

	GLdouble p[16], m[16];
	GLint viewport[4] = { 0, 0, weight, height };
	double v[3], rv[3];

	glGetDoublev(GL_MODELVIEW_MATRIX, m);
	glGetDoublev(GL_PROJECTION_MATRIX, p);

	for (int i = 0; i < 3; i += 1)
		v[i] = P[i];

	gluProject(v[0], v[1], v[2], m, p, viewport, &rv[0], &rv[1], &rv[2]);

	for (int i = 0; i < 3; i += 1)
		ret[i] = rv[i];
#endif
    return ret;
}
    
/**
* Convert x,y from screen to wrold coordinate
* @param x
* @param y
* @param model
* @param projection
* @return
*/
glm::vec3 TCamera::Screen2World(double x, double y, double model[],
	double projection[])
{
	
	glm::vec3 ret;
#if 1

	//	std::cout << "S2W: " << w << " " << h << std::endl;
	GLint viewport[4]; //={ 0, 0, w, h };
	GLdouble winX, winY, winZ;
	winZ = -1.0f;
	double rv[3] =
	{ 0.0 };
	glGetIntegerv(GL_VIEWPORT, viewport);

	//winX = (GLdouble)x;
	//winY = (GLdouble)viewport[3] - (GLdouble)y;
    
    winX = (GLdouble)x;
    winY = (GLdouble)y;

    
   // cout<<"OX: "<<viewport[0]<<" OY: "<<viewport[1]<<endl;
   // cout<<"WX: "<<viewport[2]<<" HY: "<<viewport[3]<<endl;
    
	//	glReadPixels(mx, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &winZ);
	gluUnProject(winX, winY, winZ, model, projection, viewport, &rv[0], &rv[1],
		&rv[2]);
	for (int i = 0; i < 3; i += 1)
		ret[i] = rv[i];
#endif
	return ret;

}
