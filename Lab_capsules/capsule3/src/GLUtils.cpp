#include "GLUtils.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

extern std::string global_path;

namespace TAPP {

    
    
    bool is_error(bool verbose){
        
        GLenum ret = glGetError();
        
        if(ret!=GL_NO_ERROR){
            
            if(verbose){
                
                if(ret==GL_INVALID_ENUM){
                    cout<<"GL_INVALID_ENUM"<<endl;
                } else if(ret==GL_INVALID_VALUE){
                    cout<<"GL_INVALID_VALUE"<<endl;
                } else if(ret==GL_INVALID_OPERATION){
                    cout<<"GL_INVALID_OPERATION"<<endl;
                } else if(ret==GL_INVALID_FRAMEBUFFER_OPERATION){
                    cout<<"GL_INVALID_FRAMEBUFFER_OPERATION"<<endl;
                } else if(ret==GL_OUT_OF_MEMORY){
                    cout<<"GL_OUT_OF_MEMORY"<<endl;
                    
                } else if(ret==GL_STACK_UNDERFLOW){
                    cout<<"GL_STACK_UNDERFLOW"<<endl;
                    
                } else if(ret==GL_STACK_OVERFLOW){
                    cout<<"GL_STACK_OVERFLOW"<<endl;
                    
                } else {
                    
                    cout<<"Unknown: "<<ret<<endl;
                }
            }
            
            
            
            return true;
        }
        
        return false;
    }
    
    // Shader stuff
    
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    
    
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
    
    
    
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    
    
    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    
 // HACK
//    glDetachShader(ProgramID, VertexShaderID);
   // glDetachShader(ProgramID, FragmentShaderID);
    
  //  glDeleteShader(VertexShaderID);
  //  glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}
    
    // Texture stuff
    
    GLuint loadBMP_custom(const char * imagepath, int& rw, int& rh){
        
        printf("Reading image %s\n", imagepath);
        
        // Data read from the header of the BMP file
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;
        unsigned int width, height;
        // Actual RGB data
        unsigned char * data;
        
        // Open the file
#pragma warning(disable:4996)
        FILE * file = fopen(imagepath,"rb");
        if (!file){
            printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
            //getchar();
            return 0;
        }
        
        // Read the header, i.e. the 54 first bytes
        
        // If less than 54 bytes are read, problem
        if ( fread(header, 1, 54, file)!=54 ){
            printf("Not a correct BMP file 1\n");
            fclose(file);
            return 0;
        }
        // A BMP files always begins with "BM"
        if ( header[0]!='B' || header[1]!='M' ){
            printf("Not a correct BMP file 2\n");
            fclose(file);
            return 0;
        }
        // Make sure this is a 24bpp file
        if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file 3\n");    fclose(file); return 0;}
        if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file 4\n");    fclose(file); return 0;}
        
        // Read the information about the image
        dataPos    = *(int*)&(header[0x0A]);
        imageSize  = *(int*)&(header[0x22]);
        width      = *(int*)&(header[0x12]);
        height     = *(int*)&(header[0x16]);
        
        rw = width;
        rh = height;
        
        // Some BMP files are misformatted, guess missing information
        if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
        if (dataPos==0)      dataPos=54; // The BMP header is done that way
        
        printf("ok1\n");
        
        // Create a buffer
        data = new unsigned char [imageSize];
        
        // Read the actual data from the file into the buffer
        fread(data,1,imageSize,file);
        
        // Everything is in memory now, the file can be closed.
        fclose (file);
        
        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
        
        // OpenGL has now copied the data. Free our own version
        delete [] data;
        
        // Poor filtering, or ...
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
        
        // ... nice trilinear filtering ...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // ... which requires mipmaps. Generate them automatically.
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // Return the ID of the texture we just created
        return textureID;
    }
    
    
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
    
    GLuint loadDDS(const char * imagepath){
        
        unsigned char header[124];
        
        FILE *fp;
        
        /* try to open the file */
        fp = fopen(imagepath, "rb");
        if (fp == NULL){
            printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
            return 0;
        }
        
        /* verify the type of file */
        char filecode[4];
        fread(filecode, 1, 4, fp);
        if (strncmp(filecode, "DDS ", 4) != 0) {
            fclose(fp);
            return 0;
        }
        
        /* get the surface desc */
        fread(&header, 124, 1, fp);
        
        unsigned int height      = *(unsigned int*)&(header[8 ]);
        unsigned int width	     = *(unsigned int*)&(header[12]);
        unsigned int linearSize	 = *(unsigned int*)&(header[16]);
        unsigned int mipMapCount = *(unsigned int*)&(header[24]);
        unsigned int fourCC      = *(unsigned int*)&(header[80]);
        
        
        unsigned char * buffer;
        unsigned int bufsize;
        /* how big is it going to be including all mipmaps? */
        bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
        buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
        fread(buffer, 1, bufsize, fp);
        /* close the file pointer */
        fclose(fp);
        
        unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
        unsigned int format;
        switch(fourCC)
        {
            case FOURCC_DXT1:
                format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                break;
            case FOURCC_DXT3:
                format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                break;
            case FOURCC_DXT5:
                format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                break;
            default:
                free(buffer);
                return 0;
        }
        
        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        
        unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
        unsigned int offset = 0;
        
        /* load the mipmaps */ 
        for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
        { 
            unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
            glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
                                   0, size, buffer + offset); 
            
            offset += size; 
            width  /= 2; 
            height /= 2; 
            
            // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
            if(width < 1) width = 1;
            if(height < 1) height = 1;
            
        } 
        
        free(buffer); 
        
        return textureID;
        
        
    }

    
}

