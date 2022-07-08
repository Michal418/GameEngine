cbuffer CBuf
{
    float2 camsize, campos;
};

struct VSOut
{
    float4 pos : SV_Position;
    float2 tex : TexCoord;
};

VSOut main(float2 pos : Position, float2 tex : TexCoord)
{
    VSOut vo;
	vo.pos = float4
	(
		(pos.x - campos.x) / camsize.x,
		(pos.y - campos.y) / camsize.y,
		0.0f, 1.0f
	);

    vo.tex = tex;
    return vo;
}