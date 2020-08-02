#ifndef TW_HIERARCHY_H
#define TW_HIERARCHY_H

#include <vector>
#include <string>

#include "imgui.h"

namespace TwinkleGraphics
{
    struct HierarchyItem
    {
        std::string name;
    };

    class Hierarchy
    {
        public:
            Hierarchy();
            virtual ~Hierarchy();
        
        private:
            std::vector<HierarchyItem> _items;

    };


} // namespace TwinkleGraphics

#endif