varying vec2 Pos;
varying vec3 lightPos;

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    // Set the position of the current vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
	lightPos = normalize(gl_LightSource[0].position.xyz);

    Pos = vec2(gl_Vertex.x,gl_Vertex.y);
}
