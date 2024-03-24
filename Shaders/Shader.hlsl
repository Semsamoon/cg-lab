cbuffer TransformBuffer : register(b0)
{
float4x4 world;
float4x4 world_view_projection;
float4 view_position;
}

struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float4 world_pos : TEXCOORD1;
    float4 view_pos : TEXCOORD2;
};

cbuffer LightBuffer : register(b1)
{
float4 direction;
float4 color;
float4 k;
}

cbuffer DynamicLightBuffer : register(b2)
{
float4 dyn_position;
float4 dyn_color;
float4 dyn_k;
}

Texture2D Texture : TEXTURE : register(t0);
SamplerState Sampler : SAMPLER : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output;

    output.normal = mul(float4(input.normal.xyz, 0), world);
    output.world_pos = mul(float4(input.pos.xyz, 1), world);
    output.pos = mul(float4(input.pos.xyz, 1), world_view_projection);
    output.tex = input.tex.xy;
    output.view_pos = view_position;

    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    const float3 tex_color = Texture.Sample(Sampler, input.tex);
    const float3 normal = normalize(input.normal.xyz);

    const float3 view_direction = normalize(input.view_pos.xyz - input.world_pos.xyz);
    const float3 light_direction = -direction.xyz;
    const float3 dyn_light_direction = normalize(dyn_position.xyz - input.world_pos.xyz);
    const float3 reflection_vector = normalize(reflect(light_direction, normal));
    const float3 dyn_reflection_vector = normalize(reflect(dyn_light_direction, normal));

    const float3 diffuse = max(0, dot(light_direction, normal)) * tex_color.xyz;
    const float3 dyn_diffuse = max(0, dot(dyn_light_direction, normal)) * tex_color.xyz;
    const float3 ambient = tex_color * k.x;
    const float3 specular = pow(max(0, dot(-view_direction, reflection_vector)), k.y) * k.z;
    const float3 dyn_specular = pow(max(0, dot(-view_direction, dyn_reflection_vector)), dyn_k.y) * dyn_k.z;

    float4 col = float4(color.xyz * (diffuse + specular)
                        + dyn_color * (dyn_diffuse + dyn_specular) / pow(
                            distance(dyn_position.xyz, input.world_pos.xyz), 2) + ambient, 1);
    col.rgb = pow(col.rgb, 1 / 2.2f);
    return col;
}
