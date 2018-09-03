#version 300 es
in vec3 v_Position;   
in vec3 v_Color;
out vec3 f_Color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVPMatrix;

void main()                 
{                           
    //gl_Position = ModelMatrix * vec4(vPosition, 1);  
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(v_Position, 1);  
    //gl_Position = MVPMatrix * vec4(vPosition, 1);

    f_Color = v_Color;
}                           
