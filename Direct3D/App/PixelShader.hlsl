
struct input
{
    float3 pos;
    float4 color;
};

float4 main(float4 color : COLOR) : SV_TARGET
{
    color.x = 1.f;
	
	return color;
}