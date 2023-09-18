

struct VertexInput
{
    float3 POSITION : POSITION;
    float4 COLOR : COLOR;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


VOut main( VertexInput input)
{
    VOut output;
    output.color = input.COLOR;
    output.position = float4(input.POSITION, 1.f);
	
    return output;
}
