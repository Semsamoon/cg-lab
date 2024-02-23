#pragma once

#include "../../Lib/Types.h"
#include "../../Engine/Render/Able.h"
#include "../../Engine/Render/Pipeline.h"

namespace Basic
{
    namespace Components
    {
        class Rendering : public Engine::Render::Able
        {
        public:
            Rendering()
                : pPipeline_(), pVertexShaderByteCode_(), pPixelShaderByteCode_(), pVertexShader_(), pPixelShader_(),
                  pInputLayout_(), pPoints_(), pIndices_(), pVertexBuffer_(), pIndexBuffer_(), pRasterizerState_(),
                  indicesAmount_(0)
            {
            }

            void Compose(Engine::Render::Pipeline* pPipeline) override;
            void Render() override;

        protected:
            void CreateVertexShader(const char16* pPath, const DXShaderMacros* pDefines, DXIncludes* pIncludes);
            void CreatePixelShader(const char16* pPath, const DXShaderMacros* pDefines, DXIncludes* pIncludes);
            void CreateLayout();
            void CreateVertexBuffer(uint32 pointsAmount);
            void CreateIndexBuffer(uint32 indicesAmount);
            void CreateRasterizerState();

            Engine::Render::Pipeline* pPipeline_;
            DXBlob* pVertexShaderByteCode_;
            DXBlob* pPixelShaderByteCode_;
            DXVertexShader* pVertexShader_;
            DXPixelShader* pPixelShader_;
            DXInputLayout* pInputLayout_;
            float4* pPoints_;
            int32* pIndices_;
            DXBuffer* pVertexBuffer_;
            DXBuffer* pIndexBuffer_;
            DXRasterizerState* pRasterizerState_;
            int32 indicesAmount_;
        };
    }
}
