#version 300 es
in vec4 vPosition;   
uniform mat4 RotationMatrix;
void main()                 
{                           
    gl_Position = RotationMatrix * vPosition;  
}                           
