#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 resolution;
uniform vec2 center;
uniform float blurRadius;
uniform float sampleNum;

vec4 blur(vec2);

void main(void)
{
    //vec4 col = blur(v_texCoord); //* v_fragmentColor.rgb;
    //gl_FragColor =  vec4(col) * v_fragmentColor;
	vec4 color = texture2D(CC_Texture0, v_texCoord);
	
	float pointRadius = 0.06;
	float linkSize = 0.04;
	float noiseStrength = 0.08; // range: 0-1
	
	float minDimension = min(resolution.x, resolution.y);
	vec2 bounds = vec2(resolution.x / minDimension, resolution.y / minDimension);
	//vec2 uv = gl_FragCoord.xy / minDimension;
	vec2 uv = (2. * gl_FragCoord.xy - center.xy) / resolution.xy;
	color.x += 0.32 * sin(1.32 );
	color.y += 0.3 * sin(1.03 );
	color.z += 0.4 * sin(1.32 );
	
	vec2 midUV = vec2(bounds.x * 0.5, bounds.y * 0.5);
	color.xy += midUV;
	vec2 vecToR = color.xy - uv;
	vec2 dirToR = normalize(vecToR.xy);
	float distToR = length(vecToR);
	
	gl_FragColor.x = 1.0 - smoothstep(distToR, 0.0, pointRadius * color.x);
	gl_FragColor.y = 1.0 - smoothstep(distToR, 0.0, pointRadius * color.y);
	gl_FragColor.z = 1.0 - smoothstep(distToR, 0.0, pointRadius * color.z);
	gl_FragColor.w = color.w;
	
	//gl_FragColor = color;
	
}

vec4 blur(vec2 p)
{
    if (blurRadius > 0.0 && sampleNum > 1.0)
    {
        vec4 col = vec4(0);
        vec2 unit = 1.0 / resolution.xy;
        
        float r = blurRadius;
        float sampleStep = r / sampleNum;
        
        float count = 0.0;
        
        for(float x = -r; x < r; x += sampleStep)
        {
            for(float y = -r; y < r; y += sampleStep)
            {
                float weight = (r - abs(x)) * (r - abs(y));
                col += texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y)) * weight;
                count += weight;
            }
        }
        
        return ((col / count) + texture2D(CC_Texture0, p));
    }
    
    return texture2D(CC_Texture0, p);
}
