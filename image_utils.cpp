// code from gemini

#include <opencv2/opencv.hpp>
#include <iostream>
#include <numeric>

using namespace std;
using namespace cv;
// Reusable function mimicking np.meshgrid(x, y) with default 'xy' indexing
void meshgrid(const Mat& x_range, const Mat& y_range, Mat& X, Mat& Y) {
    int width = x_range.total();
    int height = y_range.total();

    // Ensure ranges are flat 1D rows/columns for the repeat step
    Mat x_row = x_range.reshape(1, 1);          // 1 x width
    Mat y_col = y_range.reshape(1, height);      // height x 1

    // Repeat the 1D vectors to fill the 2D grid planes
    repeat(x_row, height, 1, X);
    repeat(y_col, 1, width, Y);
}

vector<Mat> create_mesh_grid(int rows, int cols)
{

    vector<float> x_data(cols);
    vector<float> y_data(rows);

    iota(begin(x_data), end(x_data), 1);
    iota(begin(y_data), end(y_data), 1);

    Mat x_range = Mat(x_data).t(); // 1x5 row vector
    Mat y_range = Mat(y_data);     // 3x1 column vector

    // 2. Generate the Meshgrid matrices
    Mat X, Y;
    meshgrid(x_range, y_range, X, Y);

    vector<Mat> mesh;
    mesh.push_back(X);
    mesh.push_back(Y);

    return mesh;
}

// int main() {
//     int cols = 100, rows = 100;

//     // 1. Define the 1D coordinate axes ranges
//     // Equivalent to x = [0, 1, 2, 3, 4] and y = [10, 20, 30]
//     // vector<float> x_data = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
//     // vector<float> y_data = {10.0f, 20.0f, 30.0f};
//     vector<float> x_data(cols);
//     vector<float> y_data(rows);

//     iota(begin(x_data), end(x_data), 1);
//     iota(begin(y_data), end(y_data), 1);

//     Mat x_range = Mat(x_data).t(); // 1x5 row vector
//     Mat y_range = Mat(y_data);     // 3x1 column vector

//     // 2. Generate the Meshgrid matrices
//     Mat X, Y;
//     meshgrid(x_range, y_range, X, Y);

//     imshow("X", X/cols);
//     imshow("Y", Y/rows);
//     waitKey(0);

//     // 3. Print results to verify structure
//     // cout << "X Matrix (X-coordinates over the grid):\n" << X << "\n\n";
//     // cout << "Y Matrix (Y-coordinates over the grid):\n" << Y << "\n";

//     return 0;
// }
