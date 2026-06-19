/* 
dividde image into blocks 
1. KD tree - such that if standard deviation of a block
is greater than threshold the block is split into K blocks.
2. Quadtree - such that if standard deviation of a block
is greater than threshold the block is split into 2x2 blocks.
Set threshold to 0 for lossless compression [1]. 
Quadtree speed up processing (intersection and union) of masks [1].
3. use weighted average [2]

Reference 
1. https://en.wikipedia.org/wiki/Quadtree
2. https://github.com/fogleman/Quads/blob/master/main.py
*/
#include<cmath>
using namespace std;
using namespace cv;

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

		if(roi.width > 1 && roi.height > 1)
		{
			meanStdDev(block, mean, standard_deviation);
			image_mean = (mean[0] + mean[1] + mean[2])/3;
			image_std = sqrt((standard_deviation[0]*standard_deviation[0]*0.1140 +
		 					standard_deviation[1]*standard_deviation[1]*0.5870 +
		  					standard_deviation[2]*standard_deviation[2]*0.2989));
	

			// cout<<mean<<standard_deviation;
			// printf("mean : %f, standard deviation : %f\n", image_mean, image_std);
		
			// printf("image resolution %dx%d\n", img->rows, img->cols);


			if (image_std > threshold)
			{
				line(quad_tree_image, Point(roi.width/2, 0.0),
			 		Point(roi.width/2, roi.height),
			  		Scalar(255, 255, 255));
			
				line(quad_tree_image, Point(0.0, roi.height/2),
			 		Point(roi.width, roi.height/2),
			  		Scalar(255, 255, 255));
				

				imshow("quad tree image", quad_tree_image);
				waitKey(1);
				imshow("blocks", block);
				waitKey(0);
				northwest = new Quadtree(&block, quad_tree_image, threshold, quad = 0);
				northeast = new Quadtree(&block, quad_tree_image, threshold, quad = 1);
				southwest = new Quadtree(&block, quad_tree_image, threshold, quad = 2);
				southeast = new Quadtree(&block, quad_tree_image, threshold, quad = 3);
			}
		}
			else
			{
				printf("mean : %f, standard deviation : %f\n", image_mean, image_std);
		
			printf("image resolution %dx%d\n", img->rows, img->cols);
				// imshow("blocks", block);
				// waitKey(1);
				// put block for processing
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