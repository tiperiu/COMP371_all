#ifndef _T_TRACK_BALL_360_G_H_
#define _T_TRACK_BALL_360_G_H_




#include <math.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//using namespace T3D

class TTrackBall360G {
  public:

	  double m_magic_scale;
	  double m_radius;

    TTrackBall360G();
    TTrackBall360G(int width, int height);

    // copy constructor and assignment operator
    TTrackBall360G(const TTrackBall360G&);
    TTrackBall360G& operator=(const TTrackBall360G&);

    void Resize(int width, int height);
    void track(glm::vec2 old_p, glm::vec2 new_p, float scale = 0);
	glm::mat4& getMatrix();
    
    void reset();

  public:

    // from the old cs488 code
    void vRotateScene(float, float, float, float, glm::mat4&, glm::mat4* m =NULL);
    void vGenerateRotVec(float ,float  ,float, float, float&, float&, float&);

	glm::mat4 track_matrix;

	glm::mat4 track_y, track_x, track_initial;

	glm::vec4 pivot_point;
    int scrWidth, scrHeight;
};

#endif // _T_TRACK_BALL_H_
