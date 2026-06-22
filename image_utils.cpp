#include <opencv2/opencv.hpp>
#include <iostream>
#include <numeric>

// Reusable function mimicking np.meshgrid(x, y) with default 'xy' indexing
void meshgrid(const cv::Mat& x_range, const cv::Mat& y_range, cv::Mat& X, cv::Mat& Y) {
    int width = x_range.total();
    int height = y_range.total();

    // Ensure ranges are flat 1D rows/columns for the repeat step
    cv::Mat x_row = x_range.reshape(1, 1);          // 1 x width
    cv::Mat y_col = y_range.reshape(1, height);      // height x 1

    // Repeat the 1D vectors to fill the 2D grid planes
    cv::repeat(x_row, height, 1, X);
    cv::repeat(y_col, 1, width, Y);
}

int main() {
    const int cols = 100, rows = 100;

    // 1. Define the 1D coordinate axes ranges
    // Equivalent to x = [0, 1, 2, 3, 4] and y = [10, 20, 30]
    // std::vector<float> x_data = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    // std::vector<float> y_data = {10.0f, 20.0f, 30.0f};
    std::vector<float> x_data(cols);
    std::vector<float> y_data(rows);

    iota(begin(x_data), end(x_data), 1);
    iota(begin(y_data), end(y_data), 1);

    cv::Mat x_range = cv::Mat(x_data).t(); // 1x5 row vector
    cv::Mat y_range = cv::Mat(y_data);     // 3x1 column vector

    // 2. Generate the Meshgrid matrices
    cv::Mat X, Y;
    meshgrid(x_range, y_range, X, Y);

    cv::imshow("X", X/cols);
    cv::imshow("Y", Y/rows);
    cv::waitKey(0);

    // 3. Print results to verify structure
    std::cout << "X Matrix (X-coordinates over the grid):\n" << X << "\n\n";
    std::cout << "Y Matrix (Y-coordinates over the grid):\n" << Y << "\n";

    return 0;
}
