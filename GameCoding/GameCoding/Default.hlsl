
// Vertex Shader에 들어오는 구조체
struct VS_INPUT
{
    // POSITION은 InputLayout에서의 의미와 동일하게
    float4 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    // SV_Position는 반드시 있어야 함
    float4 position : SV_Position; // System Value
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// IA -> VS(정점 작업) -> RS(보간 작업) -> PS -> OM
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    //output.color = input.color;
    output.uv = input.uv;
    
    
    return output;
}

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0 : register(s0);

// PS(픽셀 작업)
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.uv);
    
    return color;
}

