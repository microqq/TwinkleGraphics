
#ifndef TW_SIZEPOLICY_H
#define TW_SIZEPOLICY_H

namespace TwinkleGraphics
{
    // Qt QSizePolicy
    class SizePolicy
    {
    public:
        enum PolicyFlag
        {
            GrowFlag = 1,
            ShrinkFlag = 2,
            ExpandingFlag = 4,
            IgnoreFlag = 8
        };

        enum Policy
        {
            Fixed = 0,
            Minimum = GrowFlag,
            Maximum = ShrinkFlag,
            Preferred = GrowFlag | ShrinkFlag,
            Expanding = GrowFlag | ShrinkFlag | ExpandingFlag,
            MinimumExpading = GrowFlag | ExpandingFlag,
            Ignored = GrowFlag | ShrinkFlag | IgnoreFlag
        };

        SizePolicy(Policy horPolicy = Preferred, Policy verPolicy = Preferred);
        ~SizePolicy();

        void SetHorizontalPolicy(Policy policy);
        void SetVerticalPolicy(Policy policy);
        void SetHeightForWidth(bool dependent);

    private:
        Policy _horPolicy = Preferred;
        Policy _verPolicy = Preferred;

        bool _heightDependWidth = true;
    };
} // namespace TwinkleGraphics

#endif