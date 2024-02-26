#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <vector>

#include "Able.h"
#include "../../Lib/Types.h"

namespace Engine
{
    namespace Render
    {
        class Able;

        class Pipeline final
        {
        public:
            void Compose(PHandlerWindow pHandlerWindow, const Point& size);
            void Render() const;
            void Destroy() const;

            void Add(Able* pRenderAble);

            DXDevice* GetDevice() const { return pDevice_.Get(); }
            DXDeviceContext* GetDeviceContext() const { return pDeviceContext_; }

        private:
            void ComposeDeviceAndSwapChain(PHandlerWindow pHandlerWindow);
            void ComposeRenderTargetView();

            Point size_{};
            DXViewport viewport_{};
            Microsoft::WRL::ComPtr<DXDevice> pDevice_;
            DXDeviceContext* pDeviceContext_ = nullptr;
            DXSwapChain* pSwapChain_ = nullptr;
            DXRenderTargetView* pRenderTargetView_ = nullptr;
            std::vector<Able*> renderAbles_{};
        };
    }
}
