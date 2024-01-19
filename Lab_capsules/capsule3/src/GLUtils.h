
#ifndef TG2LUTILS_HPP
#define TG2LUTILS_HPP

#include <GL/glew.h>


namespace TAPP {

    // check ogl errors
    bool is_error(bool verbose = false);
    
    // Shaders
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    
    // Texture
    GLuint loadBMP_custom(const char * imagepath, int& rw, int& rh);
    
    // used for text
    GLuint loadDDS(const char * imagepath);
    
}

#endif
