
struct Input
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

struct Output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;

};

float4 main(Input input) : SV_TARGET
{
   // input.color.x += 0.3f;
    
    
    return input.color;
}