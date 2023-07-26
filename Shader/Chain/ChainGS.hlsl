#include "Chain.hlsli"

static const int vnum = 4;

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{
	float shiftLine_a = atan2(StartPos.x - EndPos.x, StartPos.y - EndPos.y);

	float verticalLine_a = -shiftLine_a;

	float slope = atan(verticalLine_a);
	float length = Width.x / 2;

	float4 offset_array[vnum] =
	{
		float4(StartPos.x - (cos(slope) * length), StartPos.y - (sin(slope) * length), 0, 0),	// ç∂è„
		float4(StartPos.x + (cos(slope) * length), StartPos.y + (sin(slope) * length), 0, 0),	// âEè„
		float4(EndPos.x - (cos(slope) * length), EndPos.y - (sin(slope) * length), 0, 0),	// ç∂â∫
		float4(EndPos.x + (cos(slope) * length), EndPos.y + (sin(slope) * length), 0, 0),	// âEâ∫
	};

	for (uint i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		element.Pos = input[0].Pos + offset_array[i];

		float4x4 rev = (float4x4)0;
		rev[0][0] = -1;
		rev[1][1] = 1;
		rev[2][2] = -1;
		rev[3][3] = 1;

		element.Pos = mul(element.Pos, matWorld);
		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		element.Color = input[0].Color;
		
		element.Tex = offset_array[i];
		element.Tex.y *= -1;
		element.Tex += float2(0.5f,0.5f);
		element.Tex = element.Tex;
		output.Append(element);
	}
	output.RestartStrip();
}