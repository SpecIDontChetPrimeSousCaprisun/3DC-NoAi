#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 0.0);

    vec3 result = (vec3(0.0, 0.5, 1.0) + diffuse) * vec3(1.0, 1.0, 0.0);
    FragColor = vec4(result, 1.0);
} 
