uniform sampler2D texture;

varying vec2 px;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	px = vec2(gl_MultiTexCoord0);
}
