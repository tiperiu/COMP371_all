#ifndef _T_CAMERA_H__
#define _T_CAMERA_H__


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TTrackBall.h"
#include "TTrackBall360.h"
#include "TTrackBall360google.h"

class TCamera {
    
  public:

    TCamera();
    TCamera(const TCamera&);
    TCamera& operator=(const TCamera&);

    // reset functions
    void resetview();// resets viewing matrix to the identity
    void resetproj();// resets the projection matrix to identity

    void ApplyView(int pick=0);

    void rotateview(int axis /* 0, 1, 2 = x, y, z */, float degree); 	  
    void translateview(float dx,float dy, float dz);
    void translateview(glm::vec3 &d);

    void setperspective(int weight, int height,float fov,float aspect,  float near, float far, int pick = 0);

    void setorthographic(int x1, int y1, int x21, int y2);
    void setorthographic();

    
    // trackball API
    void Resize(int, int);
    void Track(glm::vec2 op, glm::vec2 np);
    
    void Scroll(double y);

    /// project/unproject business
    static glm::vec3 World2Screen(glm::vec3 P, int weight, int height);
	static glm::vec3 Screen2World(double x, double y, double model[], double projection[]);

  public:
	  glm::mat4 view;
	  glm::mat4 proj;

    glm::mat4 zoom;
    
	  glm::mat4 gview;
    
    double scroll_zoom ;

    TTrackBall tball;
	//TTrackBall360 tball;
	//TTrackBall360G tball;
	
};

#endif // _T_CAMERA_H__
