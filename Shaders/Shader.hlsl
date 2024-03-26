cbuffer TransformBuffer : register(b0)
{
float4x4 world;
float4x4 world_view_proj;
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
    float3 world_pos : TEXCOORD1;
    float4 pos_in_light_view : TEXCOORD2;
    float4 pos_in_point_light_view : TEXCOORD3;
    float3 camera_direction : TEXCOORD4;
};

cbuffer LightTransformBuffer : register(b1)
{
float4x4 dir_light_view_proj;
float4x4 point_light_view_proj;
float4 camera_pos;
}

cbuffer AmbientLightBuffer : register(b2)
{
float4 ambient_light_col;
}

cbuffer DirectionalLightBuffer : register(b3)
{
float4 dir_light_direction;
float4 dir_light_col;
float4 dir_light_diffK_specA_specK;
}

cbuffer PointDynamicLightBuffer : register(b4)
{
float4 point_light_pos;
float4 point_light_col;
float4 point_light_diffK_specA_specK;
}

Texture2D Texture : TEXTURE : register(t0);
Texture2D DirLightDepthMapTexture : register(t1);
Texture2D PointLightDepthMapTexture : register(t2);

SamplerState Sampler : SAMPLER : register(s0);
SamplerState SampleTypeClamp : register(s1);

PixelShader_Input VSMain(VertexShader_Input input)
{
    PixelShader_Input output;

    output.pos = mul(float4(input.pos, 1), world_view_proj);
    output.normal = normalize(mul(float4(input.normal, 0), world).xyz);
    output.tex = input.tex;
    output.world_pos = mul(float4(input.pos, 1), world).xyz;
    output.pos_in_light_view = mul(float4(output.world_pos, 1), dir_light_view_proj);
    output.pos_in_point_light_view = mul(float4(output.world_pos, 1), point_light_view_proj);
    output.camera_direction = normalize(camera_pos.xyz - output.world_pos);

    return output;
}

float4 PSMain(PixelShader_Input input) : SV_Target
{
    const float bias = 5e-6f;
    float4 col = float4(0, 0, 0, 1);

    //Ambient Light
    col.rgb += ambient_light_col.xyz;

    //Projected to Light View
    float2 projected_tex_coord;
    projected_tex_coord.x = input.pos_in_light_view.x / input.pos_in_light_view.w / 2.0f + 0.5f;
    projected_tex_coord.y = -input.pos_in_light_view.y / input.pos_in_light_view.w / 2.0f + 0.5f;

    if ((saturate(projected_tex_coord.x) == projected_tex_coord.x) &&
        (saturate(projected_tex_coord.y) == projected_tex_coord.y))
    {
        //Depth Comparison
        const float depth = DirLightDepthMapTexture.Sample(SampleTypeClamp, projected_tex_coord).r;
        const float light_depth = input.pos_in_light_view.z / input.pos_in_light_view.w - bias;

        if (light_depth < depth)
        {
            //Directional Light

            //Diffuse
            col.rgb += dir_light_col.xyz
                * max(0, dot(-dir_light_direction.xyz, input.normal))
                * dir_light_diffK_specA_specK.x;

            //Specular
            const float3 dir_light_reflection = normalize(reflect(-dir_light_direction.xyz, input.normal));
            col.rgb += dir_light_col.xyz
                * pow(max(0, dot(-input.camera_direction, dir_light_reflection)), dir_light_diffK_specA_specK.y)
                * dir_light_diffK_specA_specK.z;
        }
    }

    projected_tex_coord.x = input.pos_in_point_light_view.x / input.pos_in_point_light_view.w / 2.0f + 0.5f;
    projected_tex_coord.y = -input.pos_in_point_light_view.y / input.pos_in_point_light_view.w / 2.0f + 0.5f;

    if ((saturate(projected_tex_coord.x) == projected_tex_coord.x) &&
        (saturate(projected_tex_coord.y) == projected_tex_coord.y))
    {
        //Depth Comparison
        const float depth = PointLightDepthMapTexture.Sample(SampleTypeClamp, projected_tex_coord).r;
        const float light_depth = input.pos_in_point_light_view.z / input.pos_in_point_light_view.w - bias;

        if (light_depth < depth)
        {
            //Point Light
            const float3 distance_sqr = pow(distance(point_light_pos.xyz, input.world_pos), 3);
            const float3 point_light_direction = normalize(point_light_pos.xyz - input.world_pos);

            //Diffuse
            col.rgb += point_light_col.xyz
                * max(0, dot(point_light_direction, input.normal))
                * (point_light_diffK_specA_specK.x / distance_sqr);

            //Specular
            const float3 point_light_reflection = normalize(reflect(point_light_direction, input.normal));
            col.rgb += point_light_col.xyz
                * pow(max(0, dot(-input.camera_direction, point_light_reflection)), point_light_diffK_specA_specK.y)
                * (point_light_diffK_specA_specK.z / distance_sqr);
        }
    }

    col.rgb = pow(col.rgb, 1 / 2.2f);

    const float3 tex_color = Texture.Sample(Sampler, input.tex).xyz;
    col.rgb *= tex_color;
    return col;
}
