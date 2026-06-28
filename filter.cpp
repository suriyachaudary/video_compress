/* For each pixel, find 2 basis pixels each from the
 previous row and previous column of the pixel.
 Encode possible pairs of bases (b, a) (b, f),..., (b, u),..., (d, p), ..., (e, u)
 as 37 lines at angle between 0 and 180 degeree.

a b c d e
f g h i j
k l m n o
p q r s t
u v w x y

Use multi threading to speed up and process all pixels in all blocks in parallel.

Reference
1. libpng
*/

#include<thread>
#include<eigen-5.0.0/Eigen/Dense>

