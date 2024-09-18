#include "SquadComponent.h"

using namespace components;

SquadComponent::SquadComponent()
{
    VertexInputBufferElement params;
    params.normal = float3(0, 0, 1);

    params.position = float3(-1, 1, 0);
    params.uv = float2(0, 1);
    vertex_buffer_.data.push_back(params);

    params.position = float3(0, 1, 0);
    params.uv = float2(1, 1);
    vertex_buffer_.data.push_back(params);

    params.position = float3(-1, 0, 0);
    params.uv = float2(0, 0);
    vertex_buffer_.data.push_back(params);

    params.position = float3(0, 0, 0);
    params.uv = float2(1, 0);
    vertex_buffer_.data.push_back(params);

    index_buffer_.data.push_back(0);
    index_buffer_.data.push_back(1);
    index_buffer_.data.push_back(2);
    index_buffer_.data.push_back(1);
    index_buffer_.data.push_back(3);
    index_buffer_.data.push_back(2);
}
