struct VertexShader_Input
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PixelShader_Input
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

Texture2D Texture : TEXTURE : register(t0);

SamplerState Sampler : SAMPLER : register(s0);

PixelShader_Input VSMain(VertexShader_Input input)
{
    PixelShader_Input output;

    output.pos = float4(input.pos, 1);
    output.tex = input.uv;

    return output;
}

float4 PSMain(PixelShader_Input input) : SV_TARGET
{
    float4 temp = Texture.Sample(Sampler, input.tex);
    temp.rgb -= 0.9f;
    return float4(temp.rgb / 0.1f, 1);
}
