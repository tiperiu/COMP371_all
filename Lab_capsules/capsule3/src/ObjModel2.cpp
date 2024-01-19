
#include "ObjModel.h"
#include "ObjParser.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//#include "debug.h"

using namespace std;
using namespace T3D;


void ObjModel::add_spherical_coordinates(double radius, double width, double height){

    if(QTexture()){
        // cout textures are already creted
       // cout<<"Textures are already allocated!"<<endl;
    } else {
        texture.resize(vertex.size());
        txfaces = faces;
    }
    
    
    texture.resize(vertex.size());
    txfaces = faces;
    
    double ratio = height / width;
    
  //  cout<<"Ratio: "<<ratio<<" "<<width<<" "<<height<<endl;
    
    for(int i=0;i<vertex.size();++i){
    
        TPoint P = vertex[i];
        double l = sqrt(P[1]*P[1]+P[0]*P[0]+P[2]*P[2]);
        if(fabs(l-1.0)>1.0e-5){
            cout<<"This is not a sphere!!! "<<l<<endl;
        }
        
        
        double theta = atan(P[2] / P[0]); // x in the texture
        
        if(P[0]<0){
            theta = theta + M_PI;
        }
        
        theta = (theta + M_PI/2) / (2*M_PI);
        
        
        double psi = acos(P[1] / radius); // y in texture
       
        
        psi = (psi)/ (M_PI);
        
      
        psi = 1.0 - psi;
        
        // psi in [0, 1]
        // want 0 maps to
    
    
        double c = 1.0f / (8*ratio);
        
        double b = -c;
        double a = c+1-b;
        
        psi = a * psi + b;
        
        if(psi<0)
            psi = 0;
        if(psi>1)
            psi = 1;
        
        
#if 0
        if(psi>1)
            psi = 1;
        if(psi<-1)
            psi = -1;
#endif
        
        TVector txt;
        txt[0] = theta;
        txt[1] = psi;
        txt[2] = 0;
        
        texture[i] = txt;
    }
}




int ObjModel::NormalizeFacetIndex(){
  int min = faces[0][0];

  for(int i=0;i<faces.size();++i)
    for(int j=0;j<faces[i].size();++j)
      if(min>faces[i][j])
	min = faces[i][j];

  
  for(int i=0;i<faces.size();++i)
    for(int j=0;j<faces[i].size();++j)
      faces[i][j]-=min;

  return 0;
}


int ObjModel::AbsFacetIndex(){
  for(int i=0;i<faces.size();++i)
    for(int j=0;j<faces[i].size();++j)
      faces[i][j] = abs(faces[i][j] + 1)-1;

  return 0;
}


int ObjModel::GenerateNormals(int bAlways){
    
    if(!bAlways && QNormals())
        return 1;
    
    vector<int> nf;
    
    normal.clear();
    for(int i=0;i<vertex.size();i++){
        normal.push_back(TVector(0, 0, 0));
        nf.push_back(0);
    }
    
    for(int i=0;i<faces.size();i++)
        for(int j=0;j<faces[i].size();j++){
            normal[faces[i][j]] =  normal[faces[i][j]] + fnormal[i];
            nf[faces[i][j]]+=1;
        }
    
    for(int i=0;i<normal.size();i++){
        double f = 1.0 / (double) nf[i];
        normal[i] = f *  normal[i];
        normal[i] = normalize(normal[i]);
    }
    
    nfaces = faces;
    return 0;
}


TVector ObjModel::ComputeFaceNormals(int face){
  TPoint ret[3];
  for(int i=0;i<3;i+=1)
    ret[i] = vertex[faces[face][i]];

  TVector t1 = (ret[1] - ret[0]);
  TVector t2 = (ret[2] - ret[0]);
  t1 = normalize(t1);
  t2 = normalize(t2);
    TVector r = normalize(t1^t2);

    
  return r;
}


void ObjModel::ComputeAllFaceNormals(){
  for(int i=0;i<faces.size();i++)
    fnormal.push_back(ComputeFaceNormals(i));
}

void ObjModel::ComputeBoundingBox(){
  int N = vertex.size();
  C = TPoint(0, 0, 0);
  for(int i=0;i<N;++i){
    C = C + vertex[i];
  }

  double dn = 1.0 / ((double) N);

  C = C * dn;

 // cout<<C[0]<<", "<<C[1]<<", "<<C[2]<<endl;
}


int ObjModel::EliminateEmptyVertices(){

	return -1;
}


void ObjModel::dumpampl(const char* fname){
  std::ofstream out(fname);

  if(faces.size()!=2*vertex.size()-4){
    std::cout<<"Not a manifold!!!"<<std::endl;
    return;
  }

  int m = 3*vertex.size() - 6;

  

}

int ObjModel::GenerateObjModel(std::ostream& f){
    
  // slap the material lib first
  if(!matlib.empty())
    f<<"mtllib "<<matlib<<endl;
   

// generate vertices & normals first
    for(int i=0;i<vertex.size();i+=1)
	f<<"v "<<vertex[i][0]<<" "<<vertex[i][1]<<" "<<vertex[i][2]<<std::endl;

    f<<std::endl;

    // textures
    for(int i=0;i<texture.size();i+=1)
	f<<"vt "<<texture[i][0]<<" "<<texture[i][1]<<std::endl;

    f<<std::endl;

    
    for(int i=0;i<normal.size();i+=1)
	f<<"vn "<<normal[i][0]<<" "<<normal[i][1]<<" "<<normal[i][2]<<std::endl;

    f<<std::endl;

    for(int i=0;i<tangent.size();i+=1)
	f<<"r "<<tangent[i][0]<<" "<<tangent[i][1]<<" "<<tangent[i][2]<<std::endl;

    f<<std::endl;
    
    std::string mat;

    //ASSERT(sfaces.size()==faces.size());

    // now faces
    for(int i=0;i<faces.size();i+=1){

      if(!sfaces[i].empty() && sfaces[i]!=mat){
	mat = sfaces[i];
	f<<"usemtl "<<mat<<endl;
      }

	f<<"f";
	for(int j=0;j<3;j+=1){

	  f<<" "<<faces[i][j]+1;//<<"/";

	    //   if(QTexture())
	  //   f<<txfaces[i][j]+1;

	    // the normal
	  // f<<"/"<<nfaces[i][j]+1;

	}
	f<<std::endl;
    }

	return 0;
}


// flip the x and y coordinates
void ObjModel::RotateLeft(){
  for(int i=0;i<vertex.size();i+=1){
    TPoint& P = vertex[i];
    double a = P[0];
    P[0] = P[1];
    P[1] = a;
  }

}

int ObjModel::Triangulate(){
  int size = faces.size();
  for(int i=0;i<size;i++){
    std::vector<int> first, nfirst, txfirst;
    std::vector<int> tmp, ntmp, txtmp;
    first.clear();
    nfirst.clear();
    txfirst.clear();
    for(int j=0;j<faces[i].size()-2;j++){
      tmp.clear();
      tmp.push_back(faces[i][0]);

      ntmp.clear();
      ntmp.push_back(nfaces[i][0]);

      txtmp.clear();
      txtmp.push_back(txfaces[i][0]);

      for(int k=1;k<3;k++){
	tmp.push_back(faces[i][j+k]);
	ntmp.push_back(nfaces[i][j+k]);
	txtmp.push_back(txfaces[i][j+k]);
      }

      if(j==0){
	first = tmp;
	nfirst = ntmp;
	txfirst = txtmp;
      } else {
	faces.push_back(tmp);
	nfaces.push_back(ntmp);
	txfaces.push_back(txtmp);
      }

    }// for j
    
    faces[i] = first;
    nfaces[i] = nfirst;
    txfaces[i] = txfirst;
  }// for i

  ComputeAllFaceNormals();


  return 0;
}// 

void ObjModel::reverse_facet(int f){
  //assert(f>=0 && f<faces.size());

  vector<int> &v = faces[f];
  vector<int> iv = v;
  int s = v.size();
  for(int i=0;i<s;++i)
    v[s-i-1] = iv[i];
}


void ObjModel::reverse_all(){
  int s = faces.size();
  for(int i=0;i<s;++i)
    reverse_facet(i);
}

void ObjModel::reverse_smart(){
  int s = faces.size();
  std::vector<int> process_facets;
  process_facets.resize(s);
  for(int i=0;i<s;++i)
    process_facets[i] = 0;

  while(true){
    
    queue<int> q;
    
    for(int i=0;i<faces.size();++i)
      if(process_facets[i]==0){  
	q.push(i);
	process_facets[i] = 1;
	break;
      }
      
      if(q.empty())
	break;
      
      while(!q.empty()){
	
	int f = q.front();q.pop();
	
//	cout<<"pop "<<f<<endl;
	//cout<<vfacets.size()<<" "<<vertex.size()<<endl;
	
	std::vector<int> nfaces;
	for(int i=0;i<faces[f].size();++i){
//	  cout<<faces[f].size()<<" "<<faces[f][i]<<endl;
	//  cout<<vfacets[faces[f][i]].size()<<endl;
	  for(int j=0;j<vfacets[faces[f][i]].size();++j)
	    nfaces.push_back(vfacets[faces[f][i]][j]);
	}
	
	//cout<<nfaces.size()<<endl;
	
	for(int i=0;i<nfaces.size();++i){
	  if(process_facets[nfaces[i]]==0){
	    // do the inversion if applicable
	    vector<int> &v1 = faces[f];
	    vector<int> &v2 = faces[nfaces[i]];
	    
	    int success = 0;
	    for(int k=0;k<v1.size();++k){
	      for(int l=0;l<v2.size();++l){
		if(v1[k] == v2[l]){
		  if(v1[(k+1)%v1.size()]==v2[(l+1)%v2.size()]){
		    success = 1;
		    break;
		  }
		  if(v1[(k+1)%v1.size()]==v2[(l+v2.size()-1)%v2.size()]){
		    success = 2;
		    break;
		  }
		}
	      }
	      if(success!=0) break;
	    }
	    
	    if(success==1){
	      reverse_facet(nfaces[i]);	  
	      process_facets[nfaces[i]] = 1;
	      q.push(nfaces[i]);
	    }
	    if(success==2){
	      process_facets[nfaces[i]] = 1;
	      q.push(nfaces[i]);
	    }
	  }// if process
	}// for
      }// q
  }// while true
}
    
    void  ObjModel::optimize(){
  vector<int> marker;
  int size = vertex.size();
  marker.resize(size);

  for(int i=0;i<size;++i)
    marker[i] = 0;

  vector<int> mp, mpi;
  mp.resize(size);
  int gindex = 0;
  bool suc = true;
  while(true){
    // find an unmarked vertex
    suc = false;
    int index=-1;
    for(int i=0;i<size;++i)
      if(marker[i]==0){
	suc=true;
	index = i;
	break;
      }
    if(!suc)
      break;
    
    // process vertex index
    marker[index] = 1;
    mp[index] = gindex;
    mpi.push_back(index);

    for(int i=0;i<size;++i){
      if(marker[i])
	continue;
      
      TVector d = vertex[index] - vertex[i];
      if(d.norm()<1.0e-5){
	marker[i] = 1;
	mp[i] = gindex;
      }// if
    }// for i

    gindex++;

  }// while

  // restore the data
  for(int i=0;i<faces.size();++i)
    for(int j=0;j<faces[i].size();++j)
      faces[i][j] = mp[faces[i][j]];
  
  vector<TPoint> nv;

  nv.resize(gindex);

  for(int i=0;i<gindex;++i)
    nv[i] = vertex[mpi[i]];

  vertex = nv;

}// optimize

/* external functions */
void SaveAs(ObjModel& obj, const char* file){

    std::ofstream f(file);
    if(f)
      f<<obj;
}



void Load(ObjModel& obj, const char* file){
  std::ifstream f(file);
  if(f)
    f>>obj;

  obj.ComputeAllFaceNormals();
  obj.GenerateNormals();
}

std::ostream& operator<<(std::ostream& o, ObjModel& m){
  m.GenerateObjModel(o);

  return o;
}


std::istream& operator>>(std::istream& in, ObjModel&m){
  ObjParser(&in, &m);

  return in;
} 

