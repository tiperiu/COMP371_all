



#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

#include <sstream>

using namespace std;


int print_eigen(Eigen::Matrix4d m)
{
    // Eigen Matrices do have rule to print them with std::cout
    std::cout << m << std::endl;
    return 0;
}

int test_eigen()
{
    
    
    Eigen::Matrix4d test; //4 by 4 double precision matrix initialization

    // Let's make it a symmetric matrix
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
            test(i,j) = (i+1)*(1+j);
    }

    // Print
    print_eigen(test);

    return 0;
}




