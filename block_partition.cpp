/* 
dividde image into blocks 
1. KD tree - such that if standard deviation of a block
is greater than threshold the block is split into K blocks.
2. Quadtree - such that if standard deviation of a block
is greater than threshold the block is split into 2x2 blocks.
Set threshold to 0 for lossless compression [1]. 
Quadtree speed up processing (intersection and union) of masks [1].

Reference 
1. https://en.wikipedia.org/wiki/Quadtree
*/
#include<cmath>
using namespace std;
using namespace cv;

class Quadtree{

public:

	Scalar mean, standard_deviation;
	float image_mean, image_std;
	//from [1]
	Quadtree *northwest, *northeast, *southwest, *southeast;

	Quadtree()
	{
		return;
	}
	
	Quadtree(Mat *img, float threshold, int quad = -1)
	{
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

		if(roi.width > 1 && roi.height > 1)
		{
			Mat block = img->clone()(roi);
			imshow("blocks", block);
			waitKey(1);
			meanStdDev(block, mean, standard_deviation);
			image_mean = (mean[0] + mean[1] + mean[2])/3;
			image_std = sqrt((standard_deviation[0]*standard_deviation[0] +
		 					standard_deviation[1]*standard_deviation[1] +
		  					standard_deviation[2]*standard_deviation[2])/3);
	

			// cout<<mean<<standard_deviation;
			printf("mean : %f, standard deviation : %f\n", image_mean, image_std);
		
			printf("image resolution %dx%d\n", img->rows, img->cols);


			if (image_std > threshold)
			{
				northwest = new Quadtree(&block, threshold, quad = 0);
				northeast = new Quadtree(&block, threshold, quad = 1);
				southwest = new Quadtree(&block, threshold, quad = 2);
				southeast = new Quadtree(&block, threshold, quad = 3);
			}
			else
			{
				// put block for processing
			}
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