/* 
Takes filter result and return arithmatic code of the block.


Reference
1. gemini
*/

#include <dlib/compress_stream.h>
#include <sstream>
#include <string>


void arithmatic_code(vector<Results> result)
{
    // Calculate total byte size
    size_t total_bytes = result.size() * sizeof(Results);

    // 2. Convert to char buffer (Serialization)
    vector<char> char_buffer(total_bytes);
    memcpy(char_buffer.data(), result.data(), total_bytes);

    string original_data = string(char_buffer.begin(), char_buffer.end());
    stringstream input_stream(original_data);
    stringstream compressed_stream;
   	stringstream decompressed_stream;

   	dlib::compress_stream::kernel_2a compressor;

    // 3. Compress the stream
    compressor.compress(input_stream, compressed_stream);
    std::cout << "Original size: " << original_data.size() << " bytes\n";
    std::cout << "Compressed size: " << compressed_stream.str().size() << " bytes\n";

    compressed_stream.seekg(0); 
    compressor.decompress(compressed_stream, decompressed_stream);
    std::cout << "Decompressed size: " << decompressed_stream.str().size() << " bytes\n";

    vector<char> vec(decompressed_stream.str().size());

    // 3. Convert back to vector of structures (Deserialization)
    size_t num_elements = vec.size() / sizeof(Results);
    vector<Results> restored(num_elements);
    memcpy(restored.data(), decompressed_stream.str().data(), decompressed_stream.str().size());

    cout<<"Number of pixels restored "<<restored.size()<<"\n";

}