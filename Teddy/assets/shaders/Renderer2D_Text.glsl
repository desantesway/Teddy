// MSDF text shader //

#type vertex
#version 450 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in vec4  a_OutlineColor;
layout(location = 4) in float a_OutlineThickness;
layout(location = 5) in float a_AtlasIndex;
layout(location = 6) in int   a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	vec4 OutlineColor;
	float OutlineThickness;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat float v_AtlasIndex;
layout (location = 5) out flat int v_EntityID;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.OutlineColor = a_OutlineColor;
	Output.OutlineThickness = a_OutlineThickness;
	v_AtlasIndex = a_AtlasIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	vec4 OutlineColor;
	float OutlineThickness;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat float v_AtlasIndex;
layout (location = 5) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_FontAtlas[32];

void sampleAtlasAndSize(int idx, vec2 uv, out vec3 color, out ivec2 size)
{
	switch (idx)
	{
	case 0:  color = texture(u_FontAtlas[0],  uv).rgb; size = textureSize(u_FontAtlas[0],  0); break;
	case 1:  color = texture(u_FontAtlas[1],  uv).rgb; size = textureSize(u_FontAtlas[1],  0); break;
	case 2:  color = texture(u_FontAtlas[2],  uv).rgb; size = textureSize(u_FontAtlas[2],  0); break;
	case 3:  color = texture(u_FontAtlas[3],  uv).rgb; size = textureSize(u_FontAtlas[3],  0); break;
	case 4:  color = texture(u_FontAtlas[4],  uv).rgb; size = textureSize(u_FontAtlas[4],  0); break;
	case 5:  color = texture(u_FontAtlas[5],  uv).rgb; size = textureSize(u_FontAtlas[5],  0); break;
	case 6:  color = texture(u_FontAtlas[6],  uv).rgb; size = textureSize(u_FontAtlas[6],  0); break;
	case 7:  color = texture(u_FontAtlas[7],  uv).rgb; size = textureSize(u_FontAtlas[7],  0); break;
	case 8:  color = texture(u_FontAtlas[8],  uv).rgb; size = textureSize(u_FontAtlas[8],  0); break;
	case 9:  color = texture(u_FontAtlas[9],  uv).rgb; size = textureSize(u_FontAtlas[9],  0); break;
	case 10: color = texture(u_FontAtlas[10], uv).rgb; size = textureSize(u_FontAtlas[10], 0); break;
	case 11: color = texture(u_FontAtlas[11], uv).rgb; size = textureSize(u_FontAtlas[11], 0); break;
	case 12: color = texture(u_FontAtlas[12], uv).rgb; size = textureSize(u_FontAtlas[12], 0); break;
	case 13: color = texture(u_FontAtlas[13], uv).rgb; size = textureSize(u_FontAtlas[13], 0); break;
	case 14: color = texture(u_FontAtlas[14], uv).rgb; size = textureSize(u_FontAtlas[14], 0); break;
	case 15: color = texture(u_FontAtlas[15], uv).rgb; size = textureSize(u_FontAtlas[15], 0); break;
	case 16: color = texture(u_FontAtlas[16], uv).rgb; size = textureSize(u_FontAtlas[16], 0); break;
	case 17: color = texture(u_FontAtlas[17], uv).rgb; size = textureSize(u_FontAtlas[17], 0); break;
	case 18: color = texture(u_FontAtlas[18], uv).rgb; size = textureSize(u_FontAtlas[18], 0); break;
	case 19: color = texture(u_FontAtlas[19], uv).rgb; size = textureSize(u_FontAtlas[19], 0); break;
	case 20: color = texture(u_FontAtlas[20], uv).rgb; size = textureSize(u_FontAtlas[20], 0); break;
	case 21: color = texture(u_FontAtlas[21], uv).rgb; size = textureSize(u_FontAtlas[21], 0); break;
	case 22: color = texture(u_FontAtlas[22], uv).rgb; size = textureSize(u_FontAtlas[22], 0); break;
	case 23: color = texture(u_FontAtlas[23], uv).rgb; size = textureSize(u_FontAtlas[23], 0); break;
	case 24: color = texture(u_FontAtlas[24], uv).rgb; size = textureSize(u_FontAtlas[24], 0); break;
	case 25: color = texture(u_FontAtlas[25], uv).rgb; size = textureSize(u_FontAtlas[25], 0); break;
	case 26: color = texture(u_FontAtlas[26], uv).rgb; size = textureSize(u_FontAtlas[26], 0); break;
	case 27: color = texture(u_FontAtlas[27], uv).rgb; size = textureSize(u_FontAtlas[27], 0); break;
	case 28: color = texture(u_FontAtlas[28], uv).rgb; size = textureSize(u_FontAtlas[28], 0); break;
	case 29: color = texture(u_FontAtlas[29], uv).rgb; size = textureSize(u_FontAtlas[29], 0); break;
	case 30: color = texture(u_FontAtlas[30], uv).rgb; size = textureSize(u_FontAtlas[30], 0); break;
	case 31: color = texture(u_FontAtlas[31], uv).rgb; size = textureSize(u_FontAtlas[31], 0); break;
	}
}

float screenPxRange(ivec2 atlasSize)
{
	const float pxRange = 2.0; // set to distance field's pixel range
	vec2 unitRange = vec2(pxRange) / vec2(atlasSize);
	vec2 screenTexSize = vec2(1.0) / fwidth(Input.TexCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
	vec3 msd;
	ivec2 atlasSize;
	sampleAtlasAndSize(int(v_AtlasIndex), Input.TexCoord, msd, atlasSize);

	float sd = median(msd.r, msd.g, msd.b);
	float spr = screenPxRange(atlasSize);
	float screenPxDistance = spr * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

	if (abs(screenPxDistance) < (Input.OutlineThickness / 100.0) * spr)
	{
		o_Color = Input.OutlineColor;
	}
	else
	{
		if (opacity == 0.0)
			discard;

		o_Color = mix(Input.OutlineColor, Input.Color, opacity);
	}

	if (o_Color.a == 0.0)
		discard;

	o_EntityID = v_EntityID;
}
