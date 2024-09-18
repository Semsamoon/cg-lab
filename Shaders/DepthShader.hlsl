cbuffer TransformBuffer : register(b0)
{
float4x4 world;
float4x4 world_view_proj;
}

struct VertexShader_Input
{
    float3 pos : POSITION;
};

struct PixelShader_Input
{
    float4 pos : SV_POSITION;
    float4 depth_pos : TEXTURE0;
};

PixelShader_Input VSMain(VertexShader_Input input)
{
    PixelShader_Input output;

    output.pos = mul(mul(float4(input.pos, 1), world), world_view_proj);
    output.depth_pos = output.pos;

    return output;
}

float4 PSMain(PixelShader_Input input) : SV_TARGET
{
    //float depthValue = 0.0001 / (1.0001 - input.depth_pos.z / input.depth_pos.w);
    float depthValue = input.depth_pos.z / input.depth_pos.w;
    return float4(depthValue, depthValue, depthValue, 1.0f);
}
