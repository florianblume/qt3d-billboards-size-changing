#version 150 core

// copy of phong.frag from qt3d extras

uniform sampler2D tex0;

in vec2 texCoord;

out vec4 fragColor;

#pragma include light.inc.frag

void main()
{
    // Image is flipped
    fragColor = texture( tex0, vec2(1.0, 1.0) - texCoord );
}
