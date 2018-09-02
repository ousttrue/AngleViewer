#version 300 es
in vec3 vPosition;   
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVPMatrix;
void main()                 
{                           
    //gl_Position = ModelMatrix * vec4(vPosition, 1);  
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vPosition, 1);  
    //gl_Position = MVPMatrix * vec4(vPosition, 1);
}                           
