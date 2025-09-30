#type vertex
#version 450 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in int a_ChromaticAberration;
layout(location = 3) in vec3 a_ChromaticAberrationOffset;

struct VertexOutput
{
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;
layout (location = 1) flat out int ChromaticAberration;
layout (location = 2) out vec3 ChromaticAberrationOffset;

void main()
{
    gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0, 1.0); 
    Output.TexCoord = a_TexCoord;
    ChromaticAberration = a_ChromaticAberration;
    ChromaticAberrationOffset = a_ChromaticAberrationOffset;
}  

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
  
struct VertexOutput
{
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;
layout (location = 1) flat in int ChromaticAberration;
layout (location = 2) in vec3 ChromaticAberrationOffset;

layout (binding = 0) uniform sampler2D screenTexture;

void main()
{ 
    vec4 color = texture(screenTexture, Input.TexCoord);

    if(ChromaticAberration != 0)
    {
        float maxOffset = max(max(abs(ChromaticAberrationOffset.x), abs(ChromaticAberrationOffset.y)), 
        abs(ChromaticAberrationOffset.z));

        float zoomFactor = 1.0 / (1.0 + 0.001 * maxOffset);
        vec2 center = vec2(0.5, 0.5);  // Center of the screen
        vec2 zoomedCoord = (Input.TexCoord - center) * zoomFactor + center;

        // Clamp the texture coordinates to avoid sampling outside the texture bounds
        zoomedCoord = clamp(zoomedCoord, 0.0, 1.0);

        float redOffset   =  0.0005 * ChromaticAberrationOffset.x;
        float greenOffset =  0.0005 * ChromaticAberrationOffset.y;
        float blueOffset  =  0.0005 * ChromaticAberrationOffset.z;

        color.r  = texture(screenTexture, zoomedCoord + vec2(0.0, redOffset)).r;
        color.g  = texture(screenTexture, zoomedCoord + vec2(greenOffset, 0.0)).g;
        color.ba = texture(screenTexture, zoomedCoord + vec2(blueOffset, 0.0)).ba;
    }

    o_Color = color;
}