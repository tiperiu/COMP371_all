
#include "ObjParser.h"
#include "ObjModel.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace T3D;



bool loadOBJT(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	ObjModel& obj,
              int txt_width, int txt_height
) {


	Load(obj, path);
    
 //   obj.add_spherical_coordinates(1, txt_width, txt_height );
    
	if (obj.nfaces.size() == 0 || obj.txfaces.size() == 0) {
		cout << "I do not have both normals and textures" << endl;
		//return false;
	}

    
    
    
	for ( int i = 0; i < obj.faces.size(); i++) {
        
        if(obj.faces[i].size()!=3){
            cout<<"Error!"<<endl;
        }
        
		for (int j = 0; j < obj.faces[i].size(); ++j){

            
            
			// Get the attributes thanks to the index
			TPoint P = obj.vertex[obj.faces[i][j]];
			TVector N = obj.normal[obj.nfaces[i][j]];
		//	TVector txt = obj.texture[obj.txfaces[i][j]];

			glm::vec3 vertex(P[0], P[1], P[2]);
			//glm::vec2 uv(txt[0], txt[1]);
			glm::vec3 normal(N[0], N[1], N[2]);

			// Put the attributes in buffers
			out_vertices.push_back(vertex);
			//out_uvs.push_back(uv);
			out_normals.push_back(normal);

		}

	}
	return true;
}

int Tokenize(std::string &dst, std::string &source, std::string k){
    int pos1 = source.find(k);
    if(pos1==std::string::npos){
	if(source.length()==0) return -1;
	dst = source;
	source = "";
	return 0;
    }

    int pos2 = pos1;
    // trail characters
    while(source[pos2]==k[0] && pos2<source.length()) pos2+=1;

    //if(pos2==source.length()) return -1;

    dst = source.substr(0, pos1);
    int test = source.length()-pos2;
    source = source.substr(pos2, test);

    char a[100];

#ifdef _WIN32
    strcpy_s(a,  source.c_str());
#else
    strcpy(a, source.c_str());
#endif

    return 0;
}

int ParseVertexLine(ObjModel* model, std::string line){

  TPoint P;

  std::string v;
  Tokenize(v, line)==0;
  P[0] = atof(v.c_str());
  Tokenize(v, line)==0;
  P[1] = atof(v.c_str()); 
  Tokenize(v, line)==0;
  P[2] = atof(v.c_str()); 
      
  model->AddVertex(P);

  return 0;
}

int ParseTextureLine(ObjModel* model, std::string line){

  TVector T;
  std::string v;
  Tokenize(v, line);
  T[0] = atof(v.c_str());
  Tokenize(v, line);
  T[1] = atof(v.c_str()); 
    
    // should be optional
    //Tokenize(v, line)==0;
    //T.z = atof(v.c_str()); 
    T[2] = 0;

    model->AddTexture(T);
	return 0;
}


int ParseNormalLine(ObjModel* model, std::string line){

  TVector N;
  std::string v;
  Tokenize(v, line);
  N[0] = atof(v.c_str());
  Tokenize(v, line);
  N[1] = atof(v.c_str()); 
  Tokenize(v, line);
  N[2] = atof(v.c_str()); 

  model->AddNormal(N);    

  return 0;
}

int ParseTangentLine(ObjModel* model, std::string line){

  TVector Tg;
  std::string v;
  Tokenize(v, line);
  Tg[0] = atof(v.c_str());
  Tokenize(v, line);
  Tg[1] = atof(v.c_str()); 
  Tokenize(v, line);
  Tg[2] = atof(v.c_str()); 

  model->AddTangent(Tg);    

  return 0;
}


int ParseMaterialLib(ObjModel* model, std::string line){
  std::string v;
  Tokenize(v, line);
  model->matlib = v;
  std::cout<<model->matlib<<std::endl;

  return 0;
}

int ParseMaterial(ObjModel* model, std::string line, std::string& mat){
  std::string v;
  Tokenize(v, line);
  mat = v;

  return 0;
}



int ParseOneFaceString(std::string s, int &value, int &texture,  int &normal){
    std::string v;

// this method does not work, I don't delete it though..
#if 0
    // Simple implementation of face
    if(Tokenize(v, s, "/")!=0) return -1;
    value = atoi(v.c_str()) - 1;

    if(Tokenize(v, s, "/")!=0) return -1;;
    normal = atoi(v.c_str()) - 1;
#else
    int ipos = 0;
    int lpos = s.find("/");

    // first element is the vertex index
    v = s.substr(ipos, lpos - ipos);
    value = atoi(v.c_str()) - 1;
    if(value<0) 
	return -1;

    // second element is the texture coordinate
    
    s = s.substr(lpos+1, s.length() - lpos -1);
    ipos = 0;
    lpos = s.find("/");
    v = s.substr(ipos, lpos - ipos);
    texture = atoi(v.c_str()) - 1; // not supported for now


    // third element is the normal

    s = s.substr(lpos+1, s.length() - lpos -1);
    ipos = 0;
    lpos = s.find("/");
    v = s.substr(ipos, lpos - ipos);
    normal = atoi(v.c_str()) - 1; 

#endif

    return 0;
}
    
	

int ParseFaceLine(ObjModel* model, std::string line, std::string& mat){
    std::string sFace;
    int nFace = 0;
    // assumes that a face has at most 100 vertices
    int aFace[100], NFace[100], TxFace[100];

    while(Tokenize(sFace, line)==0){
	if(ParseOneFaceString(sFace, aFace[nFace], TxFace[nFace], NFace[nFace])==0) nFace+=1;
    }// while
    
    if(nFace==0) return -1;
    

    // tangent face is assumed to be the same as vertex
    model->AddFace(nFace, aFace, mat, NFace, TxFace);
    
    return 0;
}

int ParseFaceLineSmf(ObjModel* model, std::string line){

	return -1;
 

    std::string sFace;
    int nFace = 0;
    // assumes that a face has at most 100 vertices
    int aFace[100], TxFace[100], NFace[100];

    while(Tokenize(sFace, line)==0){
	int index = atoi(sFace.c_str());
	//index>0);
	if(index>0){
	    aFace[nFace] = index - 1;
	    TxFace[nFace] = index - 1;
	    NFace[nFace] = index - 1;
	    nFace+=1;
	}// if
    }// while
    
    if(nFace==0) return -1;
    
  std::string dummy;
    model->AddFace(nFace, aFace, dummy, TxFace, NFace);
    
    return 0;
}


LineType GetType(std::string s){

    if(s=="v") return VERTEX;
    // obj file
    if(s=="vn") return NORMAL;
    // smf file
    if(s=="n") return NORMAL;
    if(s=="f") return FACE;
    if(s=="#") return COMMENT;
    if(s=="vt") return TEXTURE;
    if(s=="r") return TANGENT;
    if(s=="mtllib") return MATLIB;
    if(s=="usemtl") return MAT;

    return UNKNOWN;
}

int ObjParser(std::istream* file,  ObjModel* model){

    char b[255];
    std::string s;
    std::string line;

    std::string mat;

    // process
    while(!file->eof()){
	if(!file->getline(b, 255))break;
	line = b;


//	cout<<line<<endl;

	LineType lType;
	if(line.length()!=0){
	    Tokenize(s, line);
	    lType = GetType(s);
	} else lType = EMPTY;

	switch(lType){
	
        // vertex
	case VERTEX:{
	    ParseVertexLine(model, line);
	  //  cout<<"vertex"<<endl;
	}break;
	
	// tetxure
	case TEXTURE:{
	    ParseTextureLine(model, line); 
	}break;

        // normal
	case NORMAL:{
	    ParseNormalLine(model, line); 
	}break;

	// tangent
	case TANGENT:{
	    ParseTangentLine(model, line); 
	}break;
	   
	// face
	case FACE:{
	  //   if(t_obj)
	//  cout<<"face"<<endl;
		ParseFaceLine(model, line, mat);
		//else
		//ParseFaceLineSmf(model, line);
	}break;
	// material library
	case MATLIB:{
	  ParseMaterialLib(model, line);
	}break;

	// material
	case MAT:{
	  ParseMaterial(model, line, mat);
	}break;	
	// comment
	case COMMENT:{
	}break;

	// empty line
	case EMPTY:{
	}break;
	    
	default:
	    // unknown
	    //ASSERT(0);
	    break;
	}// switch
    }// while

	return 0;
}

