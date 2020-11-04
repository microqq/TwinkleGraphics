

#ifndef TW_EDITOR_MENU_H
#define TW_EDITOR_MENU_H

#include "twWidget.h"

namespace TwinkleGraphics
{
    class EditorMenu : public Widget
    {
    public:
        explicit EditorMenu(Widget* parent = nullptr);
        virtual ~EditorMenu();

        virtual void OnGui() override;

    private:
    };
} // namespace TwinkleGraphics

#endif