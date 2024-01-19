
#ifndef BARY_TKMANAGER_MESH_JE
#define BARY_TKMANAGER_MESH_JE


#include <vector>

#include "TTuple.h"

namespace T3D {

  class BarycentricManager {
  public:

    // utility functions
    static TVector3d barCoords(const TPoint2d& p, const TPoint2d& p0, const TPoint2d& p1, const TPoint2d& p2);

    static TVector3d barCoords3d(const TPoint3d& p, const TPoint3d& p0, const TPoint3d& p1, const TPoint3d& p2);


    static TPoint2d bar2Coord(const TVector3d& b, const TPoint2d& p0, const TPoint2d& p1, const TPoint2d& p2);    
    static TPoint3d bar2Coord(const TVector3d& b, const TPoint3d& p0, const TPoint3d& p1, const TPoint3d& p2);
    

    static double area(const TPoint3d& p0, const  TPoint3d& p1, const  TPoint3d& p2);
    static double area(const  TPoint2d& p0, const  TPoint2d& p1, const  TPoint2d& p2);
  
    static bool is_inside(const TPoint2d& p, const TPoint2d& p0, const TPoint2d& p1, const TPoint2d& p2);

    static bool is_inside(const TPoint3d& p, const TPoint3d& p0, const TPoint3d& p1, const TPoint3d& p2);
    
     static void project_onto_plane(const TPoint3d& P1,const TPoint3d& P2,const TPoint3d& P3, TPoint2d& p1, TPoint2d& p2, TPoint2d& p3);

  };    

}// namespace


#endif

