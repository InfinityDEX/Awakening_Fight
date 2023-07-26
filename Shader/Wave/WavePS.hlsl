struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 Time;
	float4 MainColor;
	float4 AlphaColor;
};
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);
float4 main(PS_INPUT input) : SV_TARGET
{
	float2 uv;
	uv = input.Tex;
	uv.x += Time.x * 2;
	uv.y *= 5;
	uv.y += sin(sin(uv.x + sin(Time.x)) * 10);
	float scale = tex.Sample(samLinear, uv).x;
	float4 diff = lerp(MainColor, AlphaColor, scale);
	//float4 diff = tex.Sample(samLinear, uv);
	return diff;
}
