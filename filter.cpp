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

#include<cmath>
#include<thread>

struct Results{
	unsigned char x, y;
	unsigned char min_col_1;
	unsigned char min_row_2;
	unsigned char min_dist_alpha=0;
	unsigned char min_dist_r=0, min_dist_g=0, min_dist_b = 0;
};

void find_basis(Mat *img, Vec3b value, int y, int x, vector<Results> *results, Mat *new_img, int *filter_count)
{
	Results res;
	res.x = x;
	res.y = y;
	// 4 bits for position on the line
	float alpha_step = 1.0/pow(2, 4);
	float min_dist = numeric_limits<float>::max();
	unsigned char min_row_1 = y-1;
	unsigned char min_col_2 = x-1;
	
	for(int i=y-1, j=x-1;j<img->cols;j+=1)
	{
		for(int k=y, l=x-1;k<img->rows;k+=1)
		{
			for(float alpha = 0, alpha_=0; alpha<=1; alpha+=alpha_step, alpha_+=1)
			{
				Vec3b a, b;
				a = img->at<Vec3b>(i,j);
				b = img->at<Vec3b>(k,l);
				float dist = sqrt(pow((value[0] - alpha*a[0] + (1-alpha)*b[0]), 2)
							+ pow((value[1] - alpha*a[1] + (1-alpha)*b[1]), 2)
							+ pow((value[2] - alpha*a[2] + (1-alpha)*b[2]), 2))/3;

				
				if(dist < min_dist)
				{	
					min_dist = dist;
					res.min_dist_alpha = alpha_;
					res.min_col_1 = j;
					res.min_row_2 = k;

					res.min_dist_b = value[0] - (alpha*a[0] + (1-alpha)*b[0]);
					res.min_dist_g = value[1] - (alpha*a[1] + (1-alpha)*b[1]);
					res.min_dist_r = value[2] - (alpha*a[2] + (1-alpha)*b[2]);
				}
			}
		}
	}

	float min_dist_alpha = 1/pow(2,4)*res.min_dist_alpha;

	Vec3b a = img->at<Vec3b>(min_row_1,res.min_col_1);
	Vec3b b = img->at<Vec3b>(res.min_row_2,min_col_2 );

	new_img->at<Vec3b>(y, x)[0] = min_dist_alpha*a[0] + (1-min_dist_alpha)*b[0];
	new_img->at<Vec3b>(y, x)[1] = min_dist_alpha*a[1] + (1-min_dist_alpha)*b[1];
	new_img->at<Vec3b>(y, x)[2] = min_dist_alpha*a[2] + (1-min_dist_alpha)*b[2];

	results->at(*filter_count) = res;
	*filter_count = *filter_count + 1;
}

void defilter(Results result, Blocks *block, int *count)
{
	float min_dist_alpha = 1/pow(2,4)*result.min_dist_alpha;
	Vec3b a, b;
	unsigned char min_row_1 = result.y-1;
	unsigned char min_col_2 = result.x-1;
	a = block->img.at<Vec3b>(min_row_1, result.min_col_1);
	b = block->img.at<Vec3b>(result.min_row_2, min_col_2);
	block->img.at<Vec3b>(result.y, result.x)[0] = result.min_dist_b + min_dist_alpha*a[0] + (1-min_dist_alpha)*b[0];
	block->img.at<Vec3b>(result.y, result.x)[1] = result.min_dist_g + min_dist_alpha*a[1] + (1-min_dist_alpha)*b[1];
	block->img.at<Vec3b>(result.y, result.x)[2] = result.min_dist_r + min_dist_alpha*a[2] + (1-min_dist_alpha)*b[2];

	*count = *count+1;
}

vector<Results> filter(Blocks block)
{	
	vector<Results> results((block.img.rows-1)*(block.img.cols-1));
	vector<thread> workers;

	Mat new_img = Mat::zeros(block.img.rows, block.img.cols, CV_8UC3);

	for(int i=0;i<block.img.rows; i++)
	{
		for(int j= 0;j<block.img.cols; j++)
		{
			Vec3b value = block.img.at<Vec3b>(i,j);
			if(i==0 || j ==  0)
			{
				new_img.at<Vec3b>(i, j) = value;
			}
		}
	}

	int num_threads = thread::hardware_concurrency();
	cout<<"Number of possible threads "<<num_threads<<"\n";

	cout<<"Block resolution "<<block.img.rows<<"x"<<block.img.cols<<"\n";
	cout<<"Number of pixels to process "<<(block.img.rows-1)*(block.img.cols-1)<<"\n";

	int filter_count = 0;
	// start from (2,2)
	for(int i=1;i<block.img.rows; i++)
	{
		for(int j= 1;j<block.img.cols; j++)
		{
			Vec3b value = block.img.at<Vec3b>(i,j);
			workers.push_back(thread(find_basis, &(block.img), value, i, j, &results, &new_img, &filter_count));
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

	imshow("img", block.img);
	imshow("new_img", new_img);
	imwrite("block_img.png", block.img);
	imwrite("block_img_after_filter.png", new_img);
	waitKey(0);

	Blocks reconstruct;
	int count = 0;
	reconstruct.img = Mat::zeros(block.img.rows, block.img.cols, CV_8UC3);
	reconstruct.region_in_image = block.region_in_image;


	// start from (1,1)
	for(int i=0;i<block.img.rows; i++)
	{
		for(int j= 0;j<block.img.cols; j++)
		{
			Vec3b value = block.img.at<Vec3b>(i,j);
			if(i==0 || j ==  0)
			{
				reconstruct.img.at<Vec3b>(i, j) = value;
			}
		}
	}
	for(int i=1;i<block.img.rows; i++)
	{
		for(int j= 1;j<block.img.cols; j++)
		{
			for(int k=0;k<results.size();k++)
			{	
				if(results[k].x==j && results[k].y == i)
				{
					defilter(results[k], &reconstruct, &count);
				}
			}
		}
	}

	cout<<"Number of pixels reconstructed "<<count<<"\n";

	imwrite("reconstructed_block.png", reconstruct.img);
	imshow("reconstructed_block.png", reconstruct.img);
	waitKey(0);

	return results;
}
