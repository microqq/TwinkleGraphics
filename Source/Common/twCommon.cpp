#include <iostream>

#include "twCommon.h"

namespace TwinkleGraphics
{
std::vector<std::string> GLExtensions;

bool CheckSupportExtension(std::string extension)
{
    for(auto e : GLExtensions)
    {
        if(e == extension)
        {
            return true;
        }
    }

    return false;
}

}