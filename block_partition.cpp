/* 
divide image into blocks such that if standard deviation of a block
is greater than threshold the block is split into 2x2 blocks.
Set threshold to 0 for lossless compression [1]. 
Quadtree speed up processing (intersection and union) of masks [1].
3. use weighted average to compute standard deviation [2]. Split block in order based on their standard deviation [2].
	a. Initialize root node at depth 0 with roi (0, 0, w, h) and error sum = standard deviation * node area. 
	b. Push the root node into heap with score = -standard deviation * (node area ** 0.25)
	c. Compute color of the node as mean color and standard deviation from histogram of the node.
	d. check if the node is leaf node i.e., w <= 4 or h<=4
4. append meshgrid (code from gemini) to image to know the coordinate of the block

Reference 
1. https://en.wikipedia.org/wiki/Quadtree
2. https://github.com/fogleman/Quads/blob/master/main.py
*/
#include<cmath>
#include "image_utils.cpp"

using namespace std;
using namespace cv;

struct Blocks{
	Mat img;
	Rect region_in_image;
};

int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); 
VideoWriter writer;

class Quadtree2{
	float block_mean=0, block_std_dev = 0, threshold = 10;
	int block_width = 0, block_height = 0;
	int block_width_threshold=4, block_height_threshold=4;
	Mat image, block;
	vector<Mat> coords;
	vector<Blocks> blocks;
	Rect region_in_image;
	
	public:
		Mat quad_tree_image;

		void set_image(Mat img, Mat quad_tree_image_temp){
			image =  img;
			quad_tree_image = quad_tree_image_temp;
		}

		void set_threshold(float thresh, int block_width_thresh, int block_height_thresh){
			threshold = thresh;
			block_width_threshold = block_width_thresh;
			block_height_threshold = block_height_thresh;
		}

	vector<Blocks> get_blocks(Mat img, vector<Mat> coords, int quad = -1)
	{
		Quadtree2 northwest, northeast, southeast, southwest;
		
		if(quad == -1)
		{
			writer.open("block_partitioning.mp4", fourcc, 120, img.size(), true);
			if (!writer.isOpened()) {
        		cerr << "Error: Could not open the video writer for output.\n";
    		}

			coords = create_mesh_grid(img.rows, img.cols);	
			block = image;
			quad_tree_image = img.clone();
		} else if (quad == 0)
		{
			Rect quad_roi = Rect(0, 0, img.cols/2, img.rows/2);
			coords[0] = coords[0](quad_roi);
			coords[1] = coords[1](quad_roi);
			block = img(quad_roi);
		}
		 else if (quad == 1)
		{
			Rect quad_roi = Rect(img.cols/2, 0, img.cols/2, img.rows/2);
			coords[0] = coords[0](quad_roi);
			coords[1] = coords[1](quad_roi);
			block = img(quad_roi);
		}
		 else if (quad == 2)
		{
			Rect quad_roi = Rect(0, img.rows/2, img.cols/2, img.rows/2);
			coords[0] = coords[0](quad_roi);
			coords[1] = coords[1](quad_roi);
			block = img(quad_roi);
		}
		 else if (quad == 3)
		{
			Rect quad_roi = Rect(img.cols/2, img.rows/2, img.cols/2, img.rows/2);
			coords[0] = coords[0](quad_roi);
			coords[1] = coords[1](quad_roi);
			block = img(quad_roi);
		}


		region_in_image = Rect(Point(coords[0].at<float>(0,0) -1, coords[1].at<float>(0,0) -1),
		 Point(coords[0].at<float>(coords[0].rows -1,coords[0].cols-1)-1, coords[1].at<float>(coords[1].rows-1,coords[1].cols-1)-1));
		block_width = block.cols;
		block_height = block.rows;
		Scalar mean, standard_deviation;

		meanStdDev(block, mean, standard_deviation);
		block_mean = (mean[0]*0.1140 + mean[1]*0.5870 + mean[2]*0.2989);
		block_std_dev = sqrt((standard_deviation[0]*standard_deviation[0]*0.1140 +
	 					standard_deviation[1]*standard_deviation[1]*0.5870 +
	  					standard_deviation[2]*standard_deviation[2]*0.2989));

		if(block_width > block_width_threshold && block_height > block_height_threshold && block_std_dev > threshold){

			/*split the block*/

			northwest.set_image(image, quad_tree_image);
			northwest.set_threshold(threshold, block_width_threshold, block_height_threshold);
			vector<Blocks> northwest_blocks = northwest.get_blocks(block, coords, 0);

			northeast.set_image(image, quad_tree_image);
			northeast.set_threshold(threshold, block_width_threshold, block_height_threshold);
			vector<Blocks> northeast_blocks = northeast.get_blocks(block, coords, 1);

			southeast.set_image(image, quad_tree_image);
			southeast.set_threshold(threshold, block_width_threshold, block_height_threshold);
			vector<Blocks> southeast_blocks = southeast.get_blocks(block, coords, 2);

			southwest.set_image(image, quad_tree_image);
			southwest.set_threshold(threshold, block_width_threshold, block_height_threshold);
			vector<Blocks> southwest_blocks = southwest.get_blocks(block, coords, 3);

			blocks.insert(blocks.end(), northwest_blocks.begin(), northwest_blocks.end());
			blocks.insert(blocks.end(), northeast_blocks.begin(), northeast_blocks.end());
			blocks.insert(blocks.end(), southeast_blocks.begin(), southeast_blocks.end());
			blocks.insert(blocks.end(), southwest_blocks.begin(), southwest_blocks.end());

		}else{
	
		rectangle(quad_tree_image, region_in_image, Scalar(255,255,255), 0.01);
		writer.write(quad_tree_image); 
		Blocks block_temp;
		block_temp.img = block;
		block_temp.region_in_image = region_in_image;
		blocks.push_back(block_temp);
	}
		
		return blocks;
	}
};
