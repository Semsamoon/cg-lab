cbuffer TransformBuffer : register(b0)
{
float4x4 world;
float4x4 world_view_proj;
}

cbuffer LightTransformBuffer : register(b1)
{
float4x4 light_view_proj;
float4 camera_pos;
}

cbuffer AmbientLightBuffer : register(b2)
{
float4 ambient_light_color;
float ambient_light_intensity;
}

cbuffer DirectionalLightBuffer : register(b3)
{
float4 dir_light_direction;
float4 dir_light_color;
float4 dir_light_diffK_specA_specK;
}

struct VertexShader_Input
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct PixelShader_Input
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float4 pos_in_light_view : TEXCOORD1;
    float3 camera_direction : TEXCOORD2;
};

Texture2D ShaderTexture : register(t0);
Texture2D DepthMapTexture : register(t1);
SamplerState SampleTypeWrap : register(s0);
SamplerState SampleTypeClamp : register(s1);

PixelShader_Input VSMain(VertexShader_Input input)
{
    PixelShader_Input output;

    output.pos = mul(float4(input.pos, 1), world_view_proj);
    output.normal = normalize(mul(float4(input.normal, 0), world).xyz);
    output.tex = input.tex;
    output.pos_in_light_view = mul(mul(float4(input.pos, 1), world), light_view_proj);
    output.camera_direction = normalize(camera_pos.xyz - mul(float4(input.pos, 1), world).xyz);

    return output;
}

float4 PSMain(PixelShader_Input input) : SV_Target
{
    const float bias = 0.001f;
    float4 col = float4(0, 0, 0, 1);

    //Projected to Light View
    float2 projected_tex_coord;
    projected_tex_coord.x = input.pos_in_light_view.x / input.pos_in_light_view.w / 2.0f + 0.5f;
    projected_tex_coord.y = -input.pos_in_light_view.y / input.pos_in_light_view.w / 2.0f + 0.5f;

    if ((saturate(projected_tex_coord.x) == projected_tex_coord.x) &&
        (saturate(projected_tex_coord.y) == projected_tex_coord.y))
    {
        //Depth Comparison
        const float depth = DepthMapTexture.Sample(SampleTypeClamp, projected_tex_coord).r;
        const float light_depth = input.pos_in_light_view.z / input.pos_in_light_view.w - bias;

        if (light_depth < depth)
        {
            //Diffuse
            col.rgb += dir_light_color.xyz * max(0, dot(-dir_light_direction.xyz, input.normal)) *
                dir_light_diffK_specA_specK.x;

            //Specular
            const float3 dir_light_reflection = normalize(reflect(-dir_light_direction.xyz, input.normal));
            col.rgb += dir_light_color.xyz * pow(max(0, dot(-input.camera_direction, dir_light_reflection)),
                                                 dir_light_diffK_specA_specK.y) * dir_light_diffK_specA_specK.z;
        }
    }

    //col.rgb = pow(col.rgb, 1 / 2.2f);
    col.rgb = saturate(col.rgb);

    const float3 tex_color = ShaderTexture.Sample(SampleTypeWrap, input.tex).xyz;
    col.rgb *= tex_color;
    // return float4(DepthMapTexture.Sample(SampleTypeWrap, projected_tex_coord).r, 0, 0, 1);
    return float4(input.tex.x, input.tex.y, 1, 1);
}
