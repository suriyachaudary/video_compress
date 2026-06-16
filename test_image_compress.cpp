/* Run the scripts for a given image.
 The image is passed as command line argument.
 */
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<eigen-5.0.0/Eigen/Dense>
#include "block_partition.cpp"
 
using Eigen::MatrixXd;
using namespace std;

int main(int argc, char **argv)
{
	// from gemini
	bool verbose = false;

	char *image_path = NULL;
	float threshold = 10;

	for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [-v | --verbose]\n"
            <<" [-i | --image_path]\n"<<" [-t | --threshold]\n";
            return 0;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "-i" || arg == "--image_path") {
            image_path = argv[i+1];
        } else if (arg == "-t" || arg == "--threshold") {
            threshold = stof(argv[i+1]);
        }
    }

    // from gemini

	printf("image path: %s\n", image_path);
	cv::Mat img = cv::imread(image_path);
	cv::imshow("image", img);
	cv::waitKey(0);
	Quadtree quad(&img);
	cv::imshow("quad tree", quad.quad_tree_image);
	cv::waitKey(0);
	MatrixXd m(2, 2);
  	m(0, 0) = 3;
  	m(1, 0) = 2.5;
  	m(0, 1) = -1;
  	m(1, 1) = m(1, 0) + m(0, 1);
  	std::cout << m << std::endl;
	return 0;
}