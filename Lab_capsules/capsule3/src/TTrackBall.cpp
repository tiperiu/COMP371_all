
#include "TTrackBall.h"
#include <math.h>

#ifdef __APPLE__ 
#include <OpenGL/gl.h>
#else
//#include <GL/gl.h>
#include <GL/glew.h>
#endif


//#define RADIUS2 .25
#define RADIUS2 4

TTrackBall::TTrackBall()
    :scrWidth(0), scrHeight(0){

    pivot_point = glm::vec4(0, 0, 0, 1);
    reset();
}


TTrackBall::TTrackBall(int width, int height)
:scrWidth(width), scrHeight(height){
    pivot_point = glm::vec4(0, 0, 0, 1);
    reset();
}

TTrackBall::TTrackBall(const TTrackBall& T){
    track_matrix = T.track_matrix;
    pivot_point = T.pivot_point;
    scrWidth = T.scrWidth;
    scrHeight = T.scrHeight;
}


TTrackBall& TTrackBall::operator=(const TTrackBall& T){
    track_matrix = T.track_matrix;
    pivot_point = T.pivot_point;
    scrWidth = T.scrWidth;
    scrHeight = T.scrHeight;
    return *this;
}


void TTrackBall::Resize(int width, int height ){
    
    scrWidth = width;
    scrHeight = height;
}


void TTrackBall::reset(){
    track_matrix = glm::mat4(1.0);
}

void TTrackBall::track(glm::vec2 old_p, glm::vec2 new_p, float scale){
	glm::mat4 m;
    float fx, fy, fz;

    // call the track ball code
    vGenerateRotVec(old_p[0], old_p[1], new_p[0], new_p[1], 
		    fx, fy, fz);
			
    // generate the matrix
    vRotateScene(fx, fy, fz, scale, m);
	
	glm::vec4 newP = pivot_point;
    newP =  track_matrix * pivot_point;

    // rotate around arbitrary point (pivot_point in this case)
	
    track_matrix =  glm::translate(track_matrix,  glm::vec3(-newP[0], -newP[1], -newP[2]));
		
    // multiply the matrix
    track_matrix = m *  track_matrix;

    // around an arbitrary point ch 2
	track_matrix = glm::translate(track_matrix, glm::vec3(newP[0], newP[1], newP[2]));
}


glm::mat4& TTrackBall::getMatrix(){
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
void TTrackBall::vRotateScene(float fVecX, float fVecY, float fVecZ, 
float fScale, glm::mat4& mNewMat)
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
void TTrackBall::vGenerateRotVec(float fOldX, float fOldY, 
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
    if(scrWidth>scrHeight)
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
    fNewZ    = (RADIUS2 - fNewX * fNewX - fNewY * fNewY);
   
   fLastX    = (fOldX - (float)nXOrigin) * 2.0 / (float)nEdge;
   fLastY    = (fOldY - (float)nYOrigin) * 2.0 / (float)nEdge;
   fLastZ    = (RADIUS2 - fLastX * fLastX - fLastY * fLastY);

   /*
    * Moving around outside of window.
    */

	if (fLastZ < 0.0) {
      fLength = sqrt(RADIUS2 - fLastZ);
      fLastZ  = 0.0;
      fLastX /= fLength;
      fLastY /= fLength;
   } else {
      fLastZ = sqrt(fLastZ);
   }


	if (fNewZ < 0.0) {
      fLength = sqrt(RADIUS2 - fNewZ);
      fNewZ  = 0.0;
      fNewX /= fLength;
      fNewY /= fLength;
   	} else {
      fNewZ = sqrt(fNewZ);
    }


   /*
    * Generate rotation vector.
    */
   fVecX = fLastY * fNewZ - fNewY * fLastZ;
   fVecY = fLastZ * fNewX - fNewZ * fLastX;
   fVecZ = -(fLastX * fNewY - fNewX * fLastY);


}
  
