

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

cbuffer finalMatix : register(b0)
{
    float4x4 worldViewProj ;
    
};

VOut main( VertexInput input)
{     
    VOut output;
    output.color = input.COLOR;
    output.position = mul(float4(input.POSITION, 1.f), worldViewProj);
	
    return output;
}
