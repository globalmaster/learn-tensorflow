// Intel的着色器无法编译通过
// nvidia的着色器可以编译通过
uniform vec3 lightPos[1];

void main( void )
{
    // 法线变换
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    vec3 N = normalize( gl_NormalMatrix * gl_Normal );
    vec4 V = gl_ModelViewMatrix * gl_Vertex;
    vec3 L = normalize( lightPos[0] - V.xyz );
    vec3 H = normalize( L + vec3( 0.0, 0.0,1.0 ) );
    const float specularExp = 128.0;

    // 计算散射光照
    float NdotL = max( 0.0, dot( N, L ) );
    vec4 diffuse = gl_Color * vec4( NdotL );

    // 计算镜面光照
    float NdotH = max( 0.0, dot( N, H ) );
    vec4 specular = vec4( 0.0 );
    if ( NdotL > 0.0 )
        specular = vec4( pow( NdotH, specularExp ) );

    // 求散射和镜面成分之和
    gl_FrontColor = diffuse + specular;
}
