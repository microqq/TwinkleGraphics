
#include "twRenderContext.h"

namespace TwinkleGraphics
{

/*------------------------------Simple Uniform--------------------------*/

template <>
void SimpleUniform<bool, 1>::BindLocation(uint32 location)
{
    glUniform1i(location, u0);
}
template<>
void SimpleUniform<float32, 1>::BindLocation(uint32 location)
{
    glUniform1f(location, u0);
}
template<>
void SimpleUniform<int32, 1>::BindLocation(uint32 location)
{
    glUniform1i(location, u0);
}
template<>
void SimpleUniform<uint32, 1>::BindLocation(uint32 location)
{
    glUniform1ui(location, u0);
}
template<>
void SimpleUniform<float64, 1>::BindLocation(uint32 location)
{
    glUniform1d(location, u0);
}



template<>
void SimpleUniform<float32, 2>::BindLocation(uint32 location)
{
    glUniform2f(location, u0, u1);
}
template<>
void SimpleUniform<int32, 2>::BindLocation(uint32 location)
{
    glUniform2i(location, u0, u1);
}
template<>
void SimpleUniform<uint32, 2>::BindLocation(uint32 location)
{
    glUniform2ui(location, u0, u1);
}
template<>
void SimpleUniform<float64, 2>::BindLocation(uint32 location)
{
    glUniform2d(location, u0, u1);
}



template<>
void SimpleUniform<float32, 3>::BindLocation(uint32 location)
{
    glUniform3f(location, u0, u1, u2);
}
template<>
void SimpleUniform<int32, 3>::BindLocation(uint32 location)
{
    glUniform3i(location, u0, u1, u2);
}
template<>
void SimpleUniform<uint32, 3>::BindLocation(uint32 location)
{
    glUniform3ui(location, u0, u1, u2);
}
template<>
void SimpleUniform<float64, 3>::BindLocation(uint32 location)
{
    glUniform3d(location, u0, u1, u2);
}



template<>
void SimpleUniform<float32, 4>::BindLocation(uint32 location)
{
    glUniform4f(location, u0, u1, u2, u3);
}
template<>
void SimpleUniform<int32, 4>::BindLocation(uint32 location)
{
    glUniform4i(location, u0, u1, u2, u3);
}
template<>
void SimpleUniform<uint32, 4>::BindLocation(uint32 location)
{
    glUniform4ui(location, u0, u1, u2, u3);
}
template<>
void SimpleUniform<float64, 4>::BindLocation(uint32 location)
{
    glUniform4d(location, u0, u1, u2, u3);
}



/*------------------------------Vector Uniform--------------------------*/

// template<>
// void VecUniform<float32, 1>::BindLocation(uint32 location)
// {
//     glUniform1fv(location, 1, glm::value_ptr(vector));
// }
// template<>
// void VecUniform<int32, 1>::BindLocation(uint32 location)
// {
//     glUniform1iv(location, 1, glm::value_ptr(vector));
// }
// template<>
// void VecUniform<uint32, 1>::BindLocation(uint32 location)
// {
//     glUniform1uiv(location, 1, glm::value_ptr(vector));
// }
// template<>
// void VecUniform<float64, 1>::BindLocation(uint32 location)
// {
//     glUniform1dv(location, 1, glm::value_ptr(vector));
// }

template<>
void VecUniform<bool, 2>::BindLocation(uint32 location)
{
    ivec2 ivector(vector.x, vector.y);
    glUniform2iv(location, 1, glm::value_ptr(ivector));
}
template<>
void VecUniform<float32, 2>::BindLocation(uint32 location)
{
    glUniform2fv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<int32, 2>::BindLocation(uint32 location)
{
    glUniform2iv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<uint32, 2>::BindLocation(uint32 location)
{
    glUniform2uiv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<float64, 2>::BindLocation(uint32 location)
{
    glUniform2dv(location, 1, glm::value_ptr(vector));
}

template<>
void VecUniform<bool, 3>::BindLocation(uint32 location)
{
    ivec3 ivector(vector.x, vector.y, vector.z);
    glUniform3iv(location, 1, glm::value_ptr(ivector));
}
template<>
void VecUniform<float32, 3>::BindLocation(uint32 location)
{
    glUniform3fv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<int32, 3>::BindLocation(uint32 location)
{
    glUniform3iv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<uint32, 3>::BindLocation(uint32 location)
{
    glUniform3uiv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<float64, 3>::BindLocation(uint32 location)
{
    glUniform3dv(location, 1, glm::value_ptr(vector));
}


template<>
void VecUniform<bool, 4>::BindLocation(uint32 location)
{
    ivec4 ivector(vector.x, vector.y, vector.z, vector.w);
    glUniform4iv(location, 1, glm::value_ptr(ivector));
}
template<>
void VecUniform<float32, 4>::BindLocation(uint32 location)
{
    glUniform4fv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<int32, 4>::BindLocation(uint32 location)
{
    glUniform4iv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<uint32, 4>::BindLocation(uint32 location)
{
    glUniform4uiv(location, 1, glm::value_ptr(vector));
}
template<>
void VecUniform<float64, 4>::BindLocation(uint32 location)
{
    glUniform4dv(location, 1, glm::value_ptr(vector));
}



/*------------------------------Matrix Uniform--------------------------*/

template<>
void MatUniform<float32, 2, 2>::BindLocation(uint32 location)
{
    glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(matrix));    
}
template<>
void MatUniform<float64, 2, 2>::BindLocation(uint32 location)
{
    glUniformMatrix2dv(location, 1, transpose, glm::value_ptr(matrix));    
}


template<>
void MatUniform<float32, 3, 3>::BindLocation(uint32 location)
{
    glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(matrix));    
}
template<>
void MatUniform<float64, 3, 3>::BindLocation(uint32 location)
{
    glUniformMatrix3dv(location, 1, transpose, glm::value_ptr(matrix));    
}



template<>
void MatUniform<float32, 4, 4>::BindLocation(uint32 location)
{
    glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(matrix));    
}
template<>
void MatUniform<float64, 4, 4>::BindLocation(uint32 location)
{
    glUniformMatrix4dv(location, 1, transpose, glm::value_ptr(matrix));    
}



} // namespace TwinkleGraphics
