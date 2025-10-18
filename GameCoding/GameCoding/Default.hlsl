
// Vertex Shader�� ������ ����ü
struct VS_INPUT
{
    // POSITION�� InputLayout������ �ǹ̿� �����ϰ�
    float4 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    // SV_Position�� �ݵ�� �־�� ��
    float4 position : SV_Position; // System Value
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// IA -> VS(���� �۾�) -> RS(���� �۾�) -> PS -> OM
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

// PS(�ȼ� �۾�)
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.uv);
    
    return color;
}

