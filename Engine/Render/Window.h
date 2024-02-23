#pragma once

#include <windows.h>
#include <WinUser.h>

#include "../../Lib/Types.h"

namespace Engine
{
    namespace Render
    {
        class Window final
        {
        public:
            Window() : pName_(), size_(), pHandlerInstance_(), windowClassEx_(), pHandlerWindow_()
            {
            }

            void Compose(const char16* pName, const Point& size);
            void Show() const;
            void Hide() const;
            void Destroy() const;

            PHandlerWindow GetHandlerWindow() const { return pHandlerWindow_; }

        private:
            void ComposeWindowClass();
            void ComposeHandlerWindow();

            static PInt64 CALLBACK WindowProcedure(
                PHandlerWindow pHandlerWindow, uint32 message, PUint64 pUintParam, PInt64 pIntParam);

            const char16* pName_;
            Point size_;
            PHandlerInstance pHandlerInstance_;
            WindowClassEx windowClassEx_;
            PHandlerWindow pHandlerWindow_;
        };
    }
}
