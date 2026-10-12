# Custom H.265 / HEVC Video Compression Codec in C++

A lightweight, modular, and high-performance implementation of an H.265 (High Efficiency Video Coding) inspired video compression codec written from scratch in **C++** using **OpenCV**.



Block parition

![Block partition](https://github.com/suriyachaudary/video_compress/blob/main/quad_tree_image.png)


Block Image

![Block image](https://github.com/suriyachaudary/video_compress/blob/main/block_img.png)

Block image after filtering

![Block image after filter](https://github.com/suriyachaudary/video_compress/blob/main/block_img_after_filter.png)


Compile code with

``g++ test_image_compress.cpp -o test_image_compression `pkg-config --cflags --libs opencv4` -ldlib``

Run code with 

``./test_image_compression [path_to_image]``

# Note

Current version does not have compression improvement in case of 8 bit RGB images.


### Reference 
1. Michael Fogleman. [Quads](https://github.com/fogleman/Quads/). 2014.
2. [libpng](https://libpng.org/pub/png/).
3. [libpng history](https://www.libpng.org/pub/png/book/LCH-png-chapter.pdf)
