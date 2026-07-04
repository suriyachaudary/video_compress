/* Run the scripts for a given image.
 The image is passed as command line argument.
 */
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>

#include "block_partition.cpp"
#include "filter.cpp"
 
using Eigen::MatrixXd;
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	// from gemini
	bool verbose = false;

	char *image_path = NULL;
	float threshold = 10;
	int block_width = 16, block_height = 16;

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
	Mat img = imread(image_path);
	imshow("image", img);
	waitKey(0);
	// Quadtree quad(&img, img, threshold);
	// printf("%dx%d\n", quad.quad_tree_image.rows, quad.quad_tree_image.cols);
	// imshow("quad tree", quad.quad_tree_image);
	// waitKey(0);

	Quadtree2 quad;
	quad.set_image(img, 255*Mat::ones(img.rows, img.cols, CV_8UC3));
	quad.set_threshold(threshold, block_width, block_height);
	vector<Mat> coords;
	vector<Blocks> blocks = quad.get_blocks(img, coords);
	cout<<"Number of blocks to process "<<blocks.size()<<"\n";

	imshow("quad tree", quad.quad_tree_image);
	imwrite("quad_tree_image.png", quad.quad_tree_image);
	waitKey(0);
	
	vector<Results> results = filter(blocks[54]);

	MatrixXd m(2, 2);
  	m(0, 0) = 3;
  	m(1, 0) = 2.5;
  	m(0, 1) = -1;
  	m(1, 1) = m(1, 0) + m(0, 1);
  	std::cout << m << std::endl;
	return 0;
}