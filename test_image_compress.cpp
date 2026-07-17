/* Run the scripts for a given image.
 The image is passed as command line argument.
 */
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>

#include "block_partition.cpp"
#include "filter.cpp"
#include "arithmatic_coding.cpp"
 
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	bool verbose = false;

	char *image_path = NULL;
	float threshold = 10;
	int block_width = 4, block_height = 4;

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


	printf("image path: %s\n", image_path);
	Mat img = imread(image_path, IMREAD_UNCHANGED);
	cout<<"Image Depth "<<img.depth()<<"\n";

	Quadtree2 quad;
	quad.set_image(img, 255*Mat::ones(img.rows, img.cols, CV_8UC3));
	quad.set_threshold(threshold, block_width, block_height);
	vector<Mat> coords;
	vector<Blocks> blocks = quad.get_blocks(img, coords);
	writer.release();
	cout<<"Number of blocks to process "<<blocks.size()<<"\n";

	imwrite("quad_tree_image.png", quad.quad_tree_image);
	
	for(int i = 0; i < blocks.size(); i++)
	{
		vector<Results> results = filter(blocks[i]);
		cout<<"Raw memory "<<blocks[i].img.total()*blocks[i].img.elemSize()<<" bytes\n";
		arithmatic_code(results);
		break;
	}


	return 0;
}