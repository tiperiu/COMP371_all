#ifndef _T_TRACK_BALL_H_
#define _T_TRACK_BALL_H_

#define MAGIC_SCALE 4.3


#include <math.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//using namespace T3D

class TTrackBall {
  public:

    TTrackBall();
    TTrackBall(int width, int height);

    // copy constructor and assignment operator
    TTrackBall(const TTrackBall&);
    TTrackBall& operator=(const TTrackBall&);

    void Resize(int width, int height);
    void track(glm::vec2 old_p, glm::vec2 new_p, float scale = MAGIC_SCALE);
	glm::mat4& getMatrix();
    
    void reset();

  public:

    // from the old cs488 code
    void vRotateScene(float, float, float, float, glm::mat4&);
    void vGenerateRotVec(float ,float  ,float, float, float&, float&, float&);

	glm::mat4 track_matrix;
	glm::vec4 pivot_point;
    int scrWidth, scrHeight;
};

#endif // _T_TRACK_BALL_H_
