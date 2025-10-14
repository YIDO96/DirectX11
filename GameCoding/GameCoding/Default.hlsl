
// Vertex Shader�� ������ ����ü
struct VS_INPUT
{
    // POSITION�� InputLayout������ �ǹ̿� �����ϰ�
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    // SV_Position�� �ݵ�� �־�� ��
    float4 position : SV_Position; // System Value
    float4 color : COLOR;
};

// IA -> VS(���� �۾�) -> RS(���� �۾�) -> PS -> OM
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.color = input.color;
    
    
    
    return output;
}

// PS(�ȼ� �۾�)
float4 PS(VS_OUTPUT input) : SV_Target
{
    
    return input.color;
}

