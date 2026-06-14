/* 
dividde image into blocks 
1. KD tree - such that if standard deviation of a block
is greater than threshold the block is split into K blocks.
2. Quadtree - such that if standard deviation of a block
is greater than threshold the block is split into 4x4 blocks.
Set threshold to 0 for lossless compression [1]. 
Quadtree speed up processing (intersection and union) of masks [1].

Reference 
1. https://en.wikipedia.org/wiki/Quadtree
*/