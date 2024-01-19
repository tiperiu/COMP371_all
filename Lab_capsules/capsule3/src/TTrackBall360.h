#ifndef _T_TRACK_BALL_360_H_
#define _T_TRACK_BALL_360_H_




#include <math.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//using namespace T3D

class TTrackBall360 {
  public:

	  double m_magic_scale;
	  double m_radius;

    TTrackBall360();
    TTrackBall360(int width, int height);

    // copy constructor and assignment operator
    TTrackBall360(const TTrackBall360&);
    TTrackBall360& operator=(const TTrackBall360&);

    void Resize(int width, int height);
    void track(glm::vec2 old_p, glm::vec2 new_p, float scale = 0);
	glm::mat4& getMatrix();
    
    void reset();

  public:

    // from the old cs488 code
    void vRotateScene(float, float, float, float, glm::mat4&, glm::mat4* m =NULL);
    void vGenerateRotVec(float ,float  ,float, float, float&, float&, float&);

	glm::mat4 track_matrix;
	glm::vec4 pivot_point;
    int scrWidth, scrHeight;
};

#endif // _T_TRACK_BALL_H_
