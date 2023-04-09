#include "glb_impl.h"
// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>



using namespace tinygltf;
// void tinygltf::LoadImageData(
//     tinygltf::Image*,
//     int,
//     std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*,
//     std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*,
//     int,
//     int,
//     unsigned char const*,
//     int,
//     void*
// );

TinyGLTF loader;
