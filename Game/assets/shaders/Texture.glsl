#type vertex
#version 330 core
            
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;

uniform mat4 uViewProjection;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;
out float vTilingFactor;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    vTilingFactor = aTilingFactor;
    gl_Position = uViewProjection * vec4(aPosition, 1.0);
}
   
#type fragment
#version 330 core
            
layout(location = 0) out vec4 color;
in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in float vTilingFactor;
            
uniform sampler2D uTexture[32];

void main()
{
    color = texture(uTexture[int(vTexIndex)], vTexCoord * vTilingFactor) * vColor;
}