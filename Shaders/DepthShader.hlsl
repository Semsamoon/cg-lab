cbuffer TransformBuffer
{
float4x4 world_view_proj;
}

struct VertexShader_Input
{
    float4 position : POSITION;
};

struct PixelShader_Input
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};

PixelShader_Input VSMain(VertexShader_Input input)
{
    PixelShader_Input output;

    input.position.w = 1.0f;
    output.position = mul(input.position, world_view_proj);
    output.depthPosition = output.position;

    return output;
}

float4 PSMain(PixelShader_Input input) : SV_TARGET
{
    float depthValue = input.depthPosition.z / input.depthPosition.w;
    return float4(depthValue, depthValue, depthValue, 1.0f);
}
