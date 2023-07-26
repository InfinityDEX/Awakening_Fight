#include "Chain.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float2 uv;
	uv = (input.Tex);
	uv.y = uv.y + Time.x * 2;
	float4 diff = tex.Sample(samLinear, uv);
	diff.x *= Color.x;
	diff.y *= Color.y;
	diff.z *= Color.z;
	if (diff.w > 0)
	{
		diff.w = Color.w;
	}
	return diff;
}
