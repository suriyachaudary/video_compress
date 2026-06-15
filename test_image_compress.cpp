/* Run the scripts for a given image.
 The image is passed as command line argument.
 */
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<eigen-5.0.0/Eigen/Dense>
 
using Eigen::MatrixXd;


int main(int argc, char **argv)
{
	printf("image path: %s\n", argv[1]);
	MatrixXd m(2, 2);
  	m(0, 0) = 3;
  	m(1, 0) = 2.5;
  	m(0, 1) = -1;
  	m(1, 1) = m(1, 0) + m(0, 1);
  	std::cout << m << std::endl;
	return 0;
}