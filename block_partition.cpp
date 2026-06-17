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
	float image_mean= 99999, image_std = 99999;
	Mat quad_tree_image, temp;
	int tree_level = 1;
	//from [1]
	Quadtree *northwest, *northeast, *southwest, *southeast;

	Quadtree()
	{
		return;
	}
	
	Quadtree(Mat *img, int threshold, int level = 1, int quad = 0)
	{
		tree_level = level;
		Rect roi;
		if(quad == 0)
		{
			roi = Rect(0, 0, img->cols/(pow(2,level)), img->rows/(pow(2,level)));
		}

		if(roi.width >= 2 && roi.height >= 2)
		{
			Mat temp = img->clone()(roi);
		// temp = temp(roi);
		meanStdDev(temp, mean, standard_deviation);
		image_mean = (mean[0] + mean[1] + mean[2])/3;
		image_std = sqrt((standard_deviation[0]*standard_deviation[0] +
		 standard_deviation[1]*standard_deviation[1] +
		  standard_deviation[2]*standard_deviation[2])/3);
	

		// cout<<mean<<standard_deviation;
		printf("mean : %f, standard deviation : %f\n", image_mean, image_std);
		
		printf("image resolution %dx%d\n", img->rows, img->cols);
		quad_tree_image = Mat::zeros(img->rows, img->cols, CV_8UC3);

		if (image_std > threshold)
		{
			line(quad_tree_image, Point(quad_tree_image.cols/2, 0.0),
			 Point(quad_tree_image.cols/2, quad_tree_image.rows),
			  Scalar(255/tree_level, 255, 255));
			
			line(quad_tree_image, Point(0.0, quad_tree_image.rows/2),
			 Point(quad_tree_image.cols, quad_tree_image.rows/2),
			  Scalar(255/tree_level, 255, 255));

			imshow("quad tree image", quad_tree_image);
			waitKey(0);
			tree_level +=1;
			printf("tree level %d\n", tree_level);
			northwest = new Quadtree(img, threshold, tree_level, quad = 0);
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