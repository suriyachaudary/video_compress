/* For each pixel, find 2 basis pixels each from the
 previous row and previous column of the pixel.
 Encode possible pairs of bases (b, a),..., (b, f),..., (b, u),..., (d, p), ..., (e, u)
 as 37 lines at angle between 0 and 180 degeree.

a b c d e
f g h i j
k l m n o
p q r s t
u v w x y

Use multi threading to speed up and process all pixels in all blocks in parallel.
Bit desciption. Each pixel is represented by 10 bits indicating the selected line from 37 lines using 6 bits
and cosine distance from the line in 4 bits. Another 10 bits is used for encoding residual.
How to discard potential bases in large blocks? What distance function to use?
0. Given a block of size BxB start processing from (2,2) onwards
1. for each pixel in the block do
	a. find possible bases of pixel pairs from previous row and column
	b. 

Reference
1. libpng
2. https://www.youtube.com/watch?v=kfLE57ljoEE&t=15
*/

#include<thread>
#include<eigen-5.0.0/Eigen/Dense>

struct Results{
	int x, y;
	int value;
};

void find_basis(Mat *img, Vec3b value, int y, int x, vector<Results> *results)
{
	Results res;
	res.x = x;
	res.y = y;
	res.value = 0;

	// 4 bits for position on the line
	float alpha = 1.0/pow(2, 4);

	vector<float> distances;
	for(int i=y-1, j=x-1;j<img->cols;j++)
	{
		for(int k=y, l=x-1;k<img->rows;k++)
		{
			
		}
	}

	results->push_back(res);
}

void filter(Blocks block)
{	
	vector<Results> results;
	vector<thread> workers;

	int num_threads = thread::hardware_concurrency();
	cout<<"Number of possible threads "<<num_threads<<"\n";

	cout<<"Block resolution "<<block.img.rows<<"x"<<block.img.cols<<"\n";
	cout<<"Number of pixels to process "<<(block.img.rows-1)*(block.img.cols-1)<<"\n";

	// start from (2,2)
	for(int i=1;i<block.img.rows; i++)
	{
		for(int j= 1;j<block.img.cols; j++)
		{
			Vec3b value = block.img.at<Vec3b>(i,j);
			workers.push_back(thread(find_basis, &(block.img), value, i, j, &results));
			
		}
	}

	for(auto& w : workers)
	{
		if(w.joinable())
		{
			w.join();
		}
	}

	cout<<"Number of pixels processed "<<results.size()<<"\n";
}