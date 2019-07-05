#version 300 es
precision mediump float;                   
in vec3 f_Normal;
in vec2 f_TexCoord;

out vec4 out_Color;

uniform sampler2D Color;

void main()                                
{                                          
    // out_Color = vec4(f_Color, 1.0); 
    out_Color = texture(Color, f_TexCoord);
}                                          
