/* Run the scripts for a given image.
 The image is passed as command line argument.
 */
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<eigen-5.0.0/Eigen/Dense>
#include "block_partition.cpp"
 
using Eigen::MatrixXd;

int main(int argc, char **argv)
{
	char *image_path = argv[1];
	printf("image path: %s\n", image_path);
	cv::Mat img = cv::imread(image_path);
	cv::imshow("image", img);
	cv::waitKey(0);
	Quadtree quad(&img);
	cv::imshow("quad tree", *quad.quad_tree_image);
	cv::waitKey(0);
	MatrixXd m(2, 2);
  	m(0, 0) = 3;
  	m(1, 0) = 2.5;
  	m(0, 1) = -1;
  	m(1, 1) = m(1, 0) + m(0, 1);
  	std::cout << m << std::endl;
	return 0;
}