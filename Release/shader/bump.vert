varying vec2 Pos;

void main() {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    // Set the position of the current vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    Pos = vec2(gl_Vertex.x,gl_Vertex.y);
}
