#version 300 es
precision mediump float;                   
in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

out vec3 f_Normal;
out vec2 f_TexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVPMatrix;

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(v_Position, 1);
    f_Normal = (ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(v_Normal, 0)).xyz;
    f_TexCoord = v_TexCoord;
}
