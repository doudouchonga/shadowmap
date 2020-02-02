#version 330 core
out vec4 FragColor;

float LinearizeDepth(float depth)
{
	float near_plane = 1.0f;
	float far_plane = 7.5f;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return z;
    //return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    //gl_FragDepth = gl_FragCoord.z;
    //float z = LinearizeDepth(gl_FragCoord.z);
    //FragColor = vec4(z, z, z, 1.0);
}