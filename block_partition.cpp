/* 
dividde image into blocks 
1. KD tree - such that if standard deviation of a block
is greater than threshold the block is split into K blocks.
2. Quadtree - such that if standard deviation of a block
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

class Quadtree2{
	float block_mean=0, block_std_dev = 0, threshold = 10;
	int block_width = 0, block_height = 0;
	Mat image, block, quad_tree_image;
	vector<Mat> coords;
	vector<Blocks> blocks;
	Rect region_in_image;
	
	public:


		void set_image(Mat img, Mat quad_tree_image_temp){
			image =  img;
			quad_tree_image = quad_tree_image_temp;
		}

	vector<Blocks> get_blocks(Mat img, int quad = -1)
	{
		Quadtree2 northwest, northeast, southeast, southwest;
		cout<<"here";
		
		if(quad == -1)
		{
			coords = create_mesh_grid(img.rows, img.cols);	
			block = image;
			quad_tree_image = img.clone();
		} else if (quad == 0)
		{
			Rect quad_0_roi = Rect(0, 0, img.cols/2, img.rows/2);
			coords[0] = coords[0](quad_0_roi);
			coords[1] = coords[1](quad_0_roi);
			block = img(quad_0_roi);
		}
		 else if (quad == 1)
		{
			Rect quad_1_roi = Rect(img.cols/2, 0, img.cols/2, img.rows/2);
			imshow("x",coords[0]/coords[0].cols);
			waitKey(0);
			coords[0] = coords[0](quad_1_roi);
			imshow("x", coords[0]/coords[0].cols);
			waitKey(0);
			coords[1] = coords[1](quad_1_roi);
			block = img(quad_1_roi);
		}
		 else if (quad == 2)
		{
			Rect quad_2_roi = Rect(0, img.rows/2, img.cols/2, img.rows/2);
			coords[0] = coords[0](quad_2_roi);
			coords[1] = coords[1](quad_2_roi);
			block = img(quad_2_roi);
		}
		 else if (quad == 3)
		{
			Rect quad_3_roi = Rect(img.cols/2, img.rows/2, img.cols/2, img.rows/2);
			cout<<quad_3_roi;
			coords[0] = coords[0](quad_3_roi);
			coords[1] = coords[1](quad_3_roi);
			block = img(quad_3_roi);
		}


		region_in_image = Rect(Point(coords[0].at<float>(0,0) -1, coords[1].at<float>(0,0) -1),
		 Point(coords[0].at<float>(coords[0].rows -1,coords[0].cols-1)-1, coords[1].at<float>(coords[1].rows-1,coords[1].cols-1)-1));
		cout<<region_in_image;
		imshow("block", block);
		waitKey(1);
		block_width = block.cols;
		block_height = block.rows;
		Scalar mean, standard_deviation;

		meanStdDev(block, mean, standard_deviation);
		block_mean = (mean[0]*0.1140 + mean[1]*0.5870 + mean[2]*0.2989);
		block_std_dev = sqrt((standard_deviation[0]*standard_deviation[0]*0.1140 +
	 					standard_deviation[1]*standard_deviation[1]*0.5870 +
	  					standard_deviation[2]*standard_deviation[2]*0.2989));

		if(block_width > 4 && block_height > 4 && block_std_dev > threshold){

			// printf("block resolution: %dx%d at %dx%dx%dx%d, standard_deviation : %f",
			 // block_width, block_height, region_in_image.x, region_in_image.y, region_in_image.width, region_in_image.height,
			  // block_std_dev);

			rectangle(quad_tree_image, region_in_image, mean, -1);
			imshow("quad_tree_image", quad_tree_image);
			waitKey(0);

			/*split the block*/

			
			// northwest.set_image(image, quad_tree_image);
			// vector<Blocks> northwest_blocks = northwest.get_blocks(block, 0);

			northeast.set_image(image, quad_tree_image);
			vector<Blocks> northeast_blocks = northeast.get_blocks(block, 1);

			// southeast.set_image(image, quad_tree_image);
			// vector<Blocks> southeast_blocks = southeast.get_blocks(block, 2);

			// southwest.set_image(image, quad_tree_image);
			// vector<Blocks> southwest_blocks = southwest.get_blocks(block, 3);

		}else{
	
		// rectangle(quad_tree_image, region_in_image, mean, -1);
		// 	imshow("quad_tree_image", quad_tree_image);
		// 	waitKey(1);

		imshow("block", block);
		waitKey(1);
	
		// Blocks block_temp;
		// block_temp.img = block;
		// block_temp.region_in_image = region_in_image;
		// blocks.push_back(block_temp);
	}
		// cout<<block_temp.img.rows<<block_temp.img.cols<<block_temp.region_in_image;
		return blocks;
	}
};

class Quadtree{

public:

	Scalar mean, standard_deviation;
	float image_mean =0 , image_std =0;
	Mat quad_tree_image, temp;
	//from [1]
	Quadtree *northwest, *northeast, *southwest, *southeast;

	Quadtree()
	{
		return;
	}
	
	Quadtree(Mat *img, Mat quad_tree_image_temp, float threshold, int quad = -1)
	{
		quad_tree_image = quad_tree_image_temp;
		Rect roi = Rect(0, 0, img->cols, img->rows);
		if(img->rows > 4 && img-> cols > 4)
		{
			if(quad == 0)
			{
				roi = Rect(0, 0, img->cols/2, img->rows/2);
			}
			else if(quad == 1)
			{
				roi = Rect(img->cols/2, 0, img->cols/2, img->rows/2);
			}
			else if(quad == 2)
			{
				roi = Rect(0, img->rows/2, img->cols/2, img->rows/2);
			}
			else if(quad == 3)
			{
				roi = Rect(img->cols/2, img->rows/2, img->cols/2, img->rows/2);
			}

			Mat block = img->clone()(roi);

			meanStdDev(block, mean, standard_deviation);
			image_mean = (mean[0]*0.1140 + mean[1]*0.5870 + mean[2]*0.2989);
			image_std = sqrt((standard_deviation[0]*standard_deviation[0]*0.1140 +
		 					standard_deviation[1]*standard_deviation[1]*0.5870 +
		  					standard_deviation[2]*standard_deviation[2]*0.2989));
	

			// cout<<mean<<standard_deviation;
			// printf("mean : %f, standard deviation : %f\n", image_mean, image_std);
		
			// printf("image resolution %dx%d\n", img->rows, img->cols);


			if (image_std > threshold)
			{
				line(quad_tree_image, Point((roi.width)/2, roi.y),
			 		Point((roi.width)/2, roi.height),
			  		Scalar(255, 255, 255));
			
				line(quad_tree_image, Point(roi.x, (roi.height)/2),
			 		Point(roi.width, (roi.height)/2),
			  		Scalar(255, 255, 255));
					printf("mean : %f, standard deviation : %f\n", image_mean, image_std);
		
			printf("image resolution %dx%d at %dx%dx%dx%d\n", img->rows, img->cols, roi.x, roi.y, roi.width, roi.height);
			rectangle(quad_tree_image, roi, Scalar(255,0,255));

				imshow("quad tree image", quad_tree_image);
				waitKey(1);
				northwest = new Quadtree(&block, quad_tree_image, threshold, quad = 0);
				northeast = new Quadtree(&block, quad_tree_image, threshold, quad = 1);
				southwest = new Quadtree(&block, quad_tree_image, threshold, quad = 2);
				southeast = new Quadtree(&block, quad_tree_image, threshold, quad = 3);
			}
			
		}
			else
			{
				// printf("mean : %f, standard deviation : %f\n", image_mean, image_std);
		
			// printf("image resolution %dx%d at %dx%dx%dx%d\n", img->rows, img->cols, roi.x, roi.y, roi.width, roi.height);
			
				imshow("blocks", *img);
				waitKey(1);
			}
	}

	~Quadtree()
	{
		return;
	}

	int subdivide()
	{

		return 0;
	}

};