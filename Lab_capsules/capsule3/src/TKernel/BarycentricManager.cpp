

#include "BarycentricManager.h"

#include <iostream>

using namespace std;


namespace T3D {


    
  ///////////// Utility functions - barycentric related
  TVector3d BarycentricManager::barCoords(const TPoint2d& p, const TPoint2d& p0, const TPoint2d& p1, const TPoint2d& p2){
		const double b0 =  area(p, p1, p2);
		const double b1 =  area(p, p2, p0);
		const double b2 =  area(p, p0, p1);
		const double sum = (b0 + b1 + b2);

		// special case - a bit hacked
		if(sum<1.0e-10){
		  cout<<"Degenerate triangle A! "<<sum<<endl;
		  //cout<<p0<<endl;
		 // cout<<p1<<endl;
		 // cout<<p2<<endl;
		  
		  TVector3d ret(1, 0 ,0);
		  return ret;
		}
		
		const double sumr = 1.0 / sum;

		TVector3d ret(b0*sumr, b1*sumr, b2*sumr);
		return ret;
	}

  TVector3d BarycentricManager::barCoords3d(const TPoint3d& p, const TPoint3d& p0, const TPoint3d& p1, const TPoint3d& p2){
		const double b0 =  area(p, p1, p2);
		const double b1 =  area(p, p2, p0);
		const double b2 =  area(p, p0, p1);
		const double sum = (b0 + b1 + b2);

		// special case - a bit hacked
		if(sum<1.0e-10){
		  cout<<"Degenerate triangle B! "<<sum<<endl;
		//  cout<<p0<<endl;
		 // cout<<p1<<endl;
		 // cout<<p2<<endl;

		  TVector3d ret(1, 0 ,0);
		  return ret;
		}

		const double sumr = 1.0 / sum;

		TVector3d ret(b0*sumr, b1*sumr, b2*sumr);
		return ret;
	}

    
    

  TPoint2d BarycentricManager::bar2Coord(const TVector3d& b, const TPoint2d& p0, const TPoint2d& p1, const TPoint2d& p2){
		TPoint2d P(0, 0);
		for(int i=0;i<2;++i)
			P[i] = p0[i]*b[0] + p1[i]*b[1] + p2[i]*b[2];
		return P;
	}

  TPoint3d BarycentricManager::bar2Coord(const TVector3d& b, const TPoint3d& p0, const TPoint3d& p1, const TPoint3d& p2){
    TPoint3d P(0, 0, 0);
    for(int i=0;i<3;++i)
      P[i] = p0[i]*b[0] + p1[i]*b[1] + p2[i]*b[2];
    return P;
  }

  double BarycentricManager::area(const TPoint3d& p0, const TPoint3d& p1, const TPoint3d& p2){
    double A = 0.5 * cross((p1 - p0),(p2 - p0)).length();
    
    // signed area!!!
    

    return A;
  }

  double BarycentricManager::area(const TPoint2d& p0, const TPoint2d& p1, const TPoint2d& p2){
    TPoint3d P0(p0[0], p0[1], 0);
    TPoint3d P1(p1[0], p1[1], 0);
    TPoint3d P2(p2[0], p2[1], 0);

    return area(P0, P1, P2);
  }

  bool BarycentricManager::is_inside(const TPoint2d& p, const TPoint2d& p0, const TPoint2d& p1, const TPoint2d& p2){
    double a1 = area(p, p0, p1);
    double a2 = area(p, p1, p2);
    double a3 = area(p, p2, p0);
    double a = area(p0, p1, p2);
	
    // important to be small enough
    if(fabs(a1+a2+a3-a)<1.0e-8){	
      return true;
    } else {
      return false;
    }
    
    return false;
  }
  
  bool BarycentricManager::is_inside(const TPoint3d& p, const TPoint3d& p0, const TPoint3d& p1, const TPoint3d& p2){
    double a1 = area(p, p0, p1);
    double a2 = area(p, p1, p2);
    double a3 = area(p, p2, p0);
    double a = area(p0, p1, p2);
    
    // important to be small enough
    if(fabs(a1+a2+a3-a)<1.0e-8){	
      return true;
    } else {
      return false;
    }
    
    return false;
  }
  
  

   void BarycentricManager::project_onto_plane(const TPoint3d& P1,const TPoint3d& P2,const TPoint3d& P3, TPoint2d& p1, TPoint2d& p2, TPoint2d& p3){
  	
	 TVector3d V1 = P2 - P1;
	 TVector3d V2 = P3 - P1;
	 
	 double l1 = length(V1);
	 V1=normalize(V1);
	 double l2 = length(V2);
	 V2=normalize(V2);

	 double ca = dot(V1,V2);
	 
	 bool over90 = (ca < 0);

	 double alpha = acos(dot(V1 , V2));
	 TVector2d d;

	 if(over90){
	     d = TVector2d(-1, fabs(tan(alpha)));
	 } else {
	     d = TVector2d(1, tan(alpha));
	 }
	 
	 d=normalize(d);

	 p1 = TPoint2d(0, 0);
	 p2 = TPoint2d(l1, 0);
	 p3 = p1 + l2 * d;
   }

}// namespace
