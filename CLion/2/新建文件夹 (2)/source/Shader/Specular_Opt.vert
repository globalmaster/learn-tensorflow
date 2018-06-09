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

    // 把散射颜色放在主颜色中
    float NdotL = max( 0.0, dot( N, L ) );
    gl_FrontColor = gl_Color * vec4( NdotL );
	
    // 把镜面颜色放到辅助颜色中
    float NdotH = max( 0.0, dot( N, H ) );
    gl_FrontSecondaryColor = ( NdotL > 0.0 )?
        vec4( pow( NdotH, specularExp ) ):
        vec4( 0.0 );
}
