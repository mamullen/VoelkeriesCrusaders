uniform sampler2D color_texture;
uniform sampler2D normal_texture;
uniform vec3 lightDir;
varying vec2 Pos;
varying vec3 lightPos;


void main() {
    
    // Extract the normal from the normal map
    vec3 normal = normalize(texture2D(normal_texture, gl_TexCoord[0].st).rgb * 2.0 - 1.0);
    //vec3 normal = normalize(texture2D(normal_texture, Pos).rgb * 2.0 - 1.0);
    
    // Determine where the light is positioned (this can be set however you like)
    //vec3 light_pos = normalize(vec3(1.0,1.0,1.5));
	vec3 light_pos = normalize(lightPos);
    
    // Calculate the lighting diffuse value
    float diffuse = max(dot(normal, light_pos), 0.0);
    
    float intensity = dot(light_pos, normal);
    
    
    vec3 temp = vec3(0.0,0.0,0.0);
    if(intensity>0.9)
    {
     temp = vec3(1,1,1);
    }
    
    else if(intensity>0.5)
    {
        temp = vec3(0.5,0.5,0.5);
    }
    else if(intensity>0.3)
    {
       temp = vec3(0.3,0.3,0.3);
    }
    
    
    
   vec3 color = texture2D(color_texture, gl_TexCoord[0].st).rgb*temp;
   // vec3 color = (diffuse * texture2D(color_texture, Pos).rgb)*temp;
    
    // Set the output color of our current pixel
    gl_FragColor = vec4(color, 1.0) * gl_LightSource[0].diffuse;
}
