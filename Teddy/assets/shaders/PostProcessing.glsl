#type vertex
#version 450 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoord;

struct VertexOutput
{
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;

void main()
{
    gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0, 1.0); 
    Output.TexCoord = a_TexCoord;
}  

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
  
struct VertexOutput
{
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 0) uniform sampler2D screenTexture;

void main()
{ 
    float redOffset   =  -0.0005;
    float greenOffset = 0.0005;
    float blueOffset  =  0.0005;

    o_Color.r  = texture(screenTexture, Input.TexCoord + vec2(redOffset  )).r;
    o_Color.g  = texture(screenTexture, Input.TexCoord + vec2(greenOffset)).g;
    o_Color.ba = texture(screenTexture, Input.TexCoord + vec2(blueOffset )).ba;

    //o_Color = texture(screenTexture, Input.TexCoord);
}