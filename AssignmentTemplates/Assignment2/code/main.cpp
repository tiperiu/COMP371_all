//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#ifdef STUDENT_SOLUTION
#include "src/A2solution.h"
#endif

#include <iostream>

using namespace std;

int main(int argc, char*argv[])
{
    
#ifndef STUDENT_SOLUTION
    cout<<"Compiled and run. Good job. Now define STUDENT_SOLUTION, create the src folder and start implementing your solution in A1solution.h (class A2solution with a method run!!! You can start with the code in capsule1 or capsule2"<<endl;
#endif
    
    if(argc==1){
        cout<<"No input files!"<<endl;
        return -1;
    }
    
#ifdef STUDENT_SOLUTION
    A2solution sol;
    sol.run(argv[1]);
#endif
    
    
	return 0;
}
