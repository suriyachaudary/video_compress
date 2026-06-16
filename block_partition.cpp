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
	Mat quad_tree_image;
	//from [1]
	Quadtree *northwest, *northeast, *southwest, *southeast;

	Quadtree()
	{
		return;
	}
	
	Quadtree(Mat *img)
	{
		meanStdDev(*img, mean, standard_deviation);
		image_mean = (mean[0] + mean[1] + mean[2])/3;
		image_std = sqrt((standard_deviation[0]*standard_deviation[0] +
		 standard_deviation[1]*standard_deviation[1] +
		  standard_deviation[2]*standard_deviation[2])/3);

		// cout<<mean<<standard_deviation;
		printf("mean : %f, standard deviation : %f", image_mean, image_std);
		
		printf("image resolution %dx%d", img->rows, img->cols);
		quad_tree_image = Mat::zeros(img->rows, img->cols, CV_8UC3);

		if (standard_deviation > thresold)
		{
			
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