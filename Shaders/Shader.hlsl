cbuffer TransformBuffer : register(b0)
{
float4x4 world;
float4x4 world_view_proj;
float4 camera_pos;
}

cbuffer AmbientLightBuffer : register(b1)
{
float4 ambient_light_color;
float ambient_light_intensity;
}

cbuffer DirectionalLightBuffer : register(b2)
{
float4 dir_light_direction;
float4 dir_light_color;
float4 dir_light_diffK_specA_specK;
}

cbuffer PointLightBuffer : register(b3)
{
float4 point_light_position;
float4 point_light_color;
float4 point_light_diffK_specA_specK;
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
    float3 camera_direction : TEXCOORD2;
};

Texture2D Texture : TEXTURE : register(t0);
SamplerState Sampler : SAMPLER : register(s0);

PixelShader_Input VSMain(VertexShader_Input input)
{
    PixelShader_Input output;

    output.pos = mul(float4(input.pos, 1), world_view_proj);
    output.normal = normalize(mul(float4(input.normal, 0), world).xyz);
    output.tex = input.tex;
    output.world_pos = mul(float4(input.pos, 1), world).xyz;
    output.camera_direction = camera_pos.xyz;

    return output;
}

float4 PSMain(PixelShader_Input input) : SV_Target
{
    float4 col = float4(0, 0, 0, 1);

    //Ambient Light
    col.rgb += ambient_light_color * ambient_light_intensity;

    //Directional Light

    //Diffuse
    col.rgb += dir_light_color.xyz * max(0, dot(-dir_light_direction.xyz, input.normal)) *
        dir_light_diffK_specA_specK.x;

    //Specular
    const float3 dir_light_reflection = normalize(reflect(-dir_light_direction.xyz, input.normal));
    const float3 camera_direction = normalize(input.camera_direction - input.world_pos);
    col.rgb += dir_light_color.xyz * pow(max(0, dot(-camera_direction, dir_light_reflection)),
                                         dir_light_diffK_specA_specK.y) * dir_light_diffK_specA_specK.z;

    //Point Light
    const float3 distance_sqr = pow(distance(point_light_position.xyz, input.world_pos), 2);
    const float3 point_light_direction = normalize(point_light_position.xyz - input.world_pos);

    //Diffuse
    col.rgb += point_light_color.xyz * max(0, dot(point_light_direction, input.normal)) *
        (point_light_diffK_specA_specK.x / distance_sqr);

    //Specular
    const float3 point_light_reflection = normalize(reflect(point_light_direction, input.normal));
    col.rgb += point_light_color.xyz * pow(max(0, dot(-camera_direction, point_light_reflection)),
                                           point_light_diffK_specA_specK.y) *
        (point_light_diffK_specA_specK.z / distance_sqr);

    col.rgb = pow(col.rgb, 1 / 2.2f);
    col.rgb = saturate(col.rgb);

    const float3 tex_color = Texture.Sample(Sampler, input.tex).xyz;
    col.rgb *= tex_color;
    return col;
}
