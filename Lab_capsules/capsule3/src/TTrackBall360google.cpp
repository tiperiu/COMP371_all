
#include "TTrackBall360google.h"
#include <math.h>

#ifdef __APPLE__ 
#include <OpenGL/gl.h>
#else
//#include <GL/gl.h>
#include <GL/glew.h>
#endif

#include <glm/gtx/transform.hpp>



TTrackBall360G::TTrackBall360G()
    :scrWidth(0), scrHeight(0), m_radius(1), m_magic_scale(.005){

    pivot_point = glm::vec4(0, 0, 0, 1);
    reset();
}


TTrackBall360G::TTrackBall360G(int width, int height)
:scrWidth(width), scrHeight(height), m_radius(1), m_magic_scale(.005) {
    pivot_point = glm::vec4(0, 0, 0, 1);

	

    reset();
}

TTrackBall360G::TTrackBall360G(const TTrackBall360G& T){
    track_matrix = T.track_matrix;
    pivot_point = T.pivot_point;
    scrWidth = T.scrWidth;
    scrHeight = T.scrHeight;
	m_radius = T.m_radius; 
	m_magic_scale = T.m_magic_scale;
}


TTrackBall360G& TTrackBall360G::operator=(const TTrackBall360G& T){
    track_matrix = T.track_matrix;
    pivot_point = T.pivot_point;
    scrWidth = T.scrWidth;
    scrHeight = T.scrHeight;
	m_radius = T.m_radius;
	m_magic_scale = T.m_magic_scale;


	return *this;
}


void TTrackBall360G::Resize(int width, int height ){
    
    scrWidth = width;
    scrHeight = height;
}


void TTrackBall360G::reset(){

	track_y = glm::mat4(1.0);
	track_x = glm::mat4(1.0);
	track_initial = glm::mat4(1.0);
	float angle = 3.14 ;
	track_initial = glm::rotate(angle, glm::vec3(0, 1, 0));
    track_matrix = track_initial;//glm::mat4(1.0);
}

void TTrackBall360G::track(glm::vec2 old_p, glm::vec2 new_p, float scale){

	if (scale == 0)
		scale = m_magic_scale;


	track_y *= glm::rotate(-scale * (new_p[0] - old_p[0]), glm::vec3(0, 1, 0));
	track_x *= glm::rotate(-scale * (new_p[1] - old_p[1]), glm::vec3(1, 0, 0));


    track_matrix = track_x * track_y * track_initial;
}


glm::mat4& TTrackBall360G::getMatrix(){
    return track_matrix;
}
  
 /*
 * Name      : void vRotateScene(float fVecX, float fVecY, float fVecZ, 
 *                               Matrix mNewMat)
 *
 * Parameters: float  fVecX   - The X component of the rotation vector.
 *             float  fVecY   - The Y component of the rotation vector.
 *             float  fVecZ   - The Z component of the rotation vector.
 *             Matrix mNewMat - The matrix to store the rotation in.
 *
 * Returns   : void
 *
 * Purpose   : Generates a matrix based on a rotation vector.
 */
void TTrackBall360G::vRotateScene(float fVecX, float fVecY, float fVecZ,
float fScale, glm::mat4& mNewMat, glm::mat4*  mInvMat)
{
    
    float fRadians, fInvLength, fNewVecX, fNewVecY, fNewVecZ;

    fVecX *= -1.0;
    fVecY *= -1.0;
    
    // the scale from pick
    //fScale = fScale * 180.0 / 3.1415;

    /* 
     * Find the length of the vector.
     */
    fRadians = sqrt(fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ);

    /*  
     * If the vector has zero length - return the 
	 matrix.
     */
    if (fRadians > -0.000001 && fRadians < 0.000001) {
		// JEAN TODO 3
		// mNewMat.setIdentity();
		glm::mat4 mNewMat = glm::mat4(1.0f);
        return;
    }

    /* 
     * Normalize the rotation vector.
     */
    fInvLength = 1 / fRadians;
    fNewVecX   = fVecX * fInvLength;
    fNewVecY   = -fVecY * fInvLength;
    fNewVecZ   = fVecZ * fInvLength;
    fRadians  *= fScale;  

    /*
     * Use OpenGL to rotate about an arbitrary axis
     */
	 // JEAN TODO 4 **********************

	glm::mat4 I(1.0);
	mNewMat = glm::rotate(I, fRadians, glm::vec3(fNewVecX, fNewVecY, fNewVecZ));
	
}

/*
 * Name      : void vGenerateRotVec(float fCurrentX, float fCurrentY, 
 *                                  float *fVecX, float *fVecY, float *fVecZ)
 *
 * Parameters: float fOldX
 			   float fOldY
 			   float fCurrentX - The current X location of the cursor.
 *             float fCurrentY - The current Y location of the cursor.
 *             float *fVecX    - The X component of the rotation vector.
 *             float *fVecY    - The Y component of the rotation vector.
 *             float *fVecZ    - The Z component of the rotation vector.
 *
 * Returns   : void
 *
 * Purpose   : Computes a rotation vector for the trackball
 *             interaction.
 */
void TTrackBall360G::vGenerateRotVec(float fOldX, float fOldY,
					float fCurrentX, float fCurrentY, float& fVecX,
                     float& fVecY, float& fVecZ) {

    long  nXOrigin, nYOrigin;
    float fNewX, fNewY, fNewZ, fLength;
    float fLastX, fLastY, fLastZ;
    int nWinWidth, nWinHeight, nEdge;

    /*
     * Compute center of window.
     */
    
    nWinWidth = scrWidth;
    nWinHeight = scrHeight;
    if(scrWidth<scrHeight)
		nEdge = scrHeight;
    else
		nEdge = scrWidth;
   		
   
    
    nXOrigin = nWinWidth / 2;
    nYOrigin = nWinHeight / 2;

    /*
     * Calculate components.
     */
    fNewX    = (fCurrentX - (float)nXOrigin) * 2.0 / (float)nEdge;
    fNewY    = (fCurrentY - (float)nYOrigin) * 2.0 / (float)nEdge;
    fNewZ    = (m_radius*m_radius - fNewX * fNewX - fNewY * fNewY);
   
   fLastX    = (fOldX - (float)nXOrigin) * 2.0 / (float)nEdge;
   fLastY    = (fOldY - (float)nYOrigin) * 2.0 / (float)nEdge;
   fLastZ    = (m_radius*m_radius - fLastX * fLastX - fLastY * fLastY);

   /*
    * Moving around outside of window.
    */

	if (fLastZ < 0.0) {
      fLength = sqrt(1.0 - fLastZ);
      fLastZ  = 0.0;
      fLastX /= fLength;
      fLastY /= fLength;
   } else {
      fLastZ = -sqrt(fLastZ);
   }


	if (fNewZ < 0.0) {
      fLength = sqrt(1.0 - fNewZ);
      fNewZ  = 0.0;
      fNewX /= fLength;
      fNewY /= fLength;
   	} else {
      fNewZ = -sqrt(fNewZ);
    }


   /*
    * Generate rotation vector.
    */
   fVecX = fLastY * fNewZ - fNewY * fLastZ;
   fVecY = fLastZ * fNewX - fNewZ * fLastX;
   fVecZ = -(fLastX * fNewY - fNewX * fLastY);

   /*
    * Update for next time.
    */
   fLastX = fNewX;
   fLastY = fNewY;
   fLastZ = fNewZ;
}
  
