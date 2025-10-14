
// Vertex Shader에 들어오는 구조체
struct VS_INPUT
{
    // POSITION은 InputLayout에서의 의미와 동일하게
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    // SV_Position는 반드시 있어야 함
    float4 position : SV_Position; // System Value
    float4 color : COLOR;
};

// IA -> VS(정점 작업) -> RS(보간 작업) -> PS -> OM
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.color = input.color;
    
    
    
    return output;
}

// PS(픽셀 작업)
float4 PS(VS_OUTPUT input) : SV_Target
{
    
    return input.color;
}

