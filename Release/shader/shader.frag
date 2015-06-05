varying vec3 vertex_light_position;
varying vec3 vertex_light_half_vector;
varying vec3 vertex_normal;
void main() {
    vec3 fragment_normal = normalize(vertex_normal);
    
    // Calculate the ambient term
    vec4 ambient_color = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
    
    // Calculate the diffuse term
    vec4 diffuse_color = gl_FrontMaterial.diffuse * (gl_LightSource[0].diffuse / 2 + gl_LightSource[1].diffuse / 2);
    
    // Calculate the specular value
    vec4 specular_color = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(max(dot(fragment_normal,vertex_light_half_vector), 0.0) , gl_FrontMaterial.shininess);
    
    // Set the diffuse value (darkness). This is done with a dot product betweenthe normal and the light
    // and the maths behind it is explained in the maths section of the site.
    float diffuse_value = max(dot(fragment_normal, vertex_light_position), 0.0);
    
    // Set the output color of our current pixel
    gl_FragColor = ambient_color + diffuse_color * diffuse_value + specular_color;
}