// Intel的着色器无法编译通过
// nvidia的着色器可以编译通过
uniform vec3 lightPos[1];

void main( void )
{
    // 法线的MVP变换
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec3 N = normalize( gl_NormalMatrix * gl_Normal );
    vec4 V = gl_ModelViewMatrix * gl_Vertex;
    vec3 L = normalize( lightPos[0] - V.xyz );

    // 输出散射颜色
    float NdotL = dot( N, L );
    gl_FrontColor = gl_Color * vec4( max( 0.0, NdotL ) );
}
