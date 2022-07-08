Texture2D tex;
SamplerState splr;

float4 main(float4 pos : SV_Position, float2 texcoord : TexCoord) : SV_TARGET
{
    return tex.Sample(splr, texcoord);
}