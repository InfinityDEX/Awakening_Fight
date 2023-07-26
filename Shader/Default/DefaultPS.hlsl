#include "Default.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float2 uv;
	
	uv = input.Tex;
	float4 diff = tex.Sample(samLinear, uv);

	return diff;
}
