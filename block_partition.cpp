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

class Quadtree{

public:

	float mean=0, stsndard_deviation=0;
	cv::Mat quad_tree_image;
	//from [1]
	Quadtree *northwest, *northeast, *southwest, *southeast;

	Quadtree()
	{
		return;
	}
	
	Quadtree(cv::Mat *img)
	{
		printf("image resolution %dx%d", img->rows, img->cols);
		quad_tree_image = cv::Mat::zeros(img->rows, img->cols, CV_8UC3);
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