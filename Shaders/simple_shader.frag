#version 450

float vertexData[] = {
-0.8, -0.8, 0.0,
0.8, -0.8, 0.0,
0.0, 0.8, 0.0
};

float colorData[] = {
1.0, 0.0, 0.0,
0.0, 1.0, 0.0, 
0.0, 0.0, 1.0
};
layout(location = 0) out  vec4 outColor;

void main(){
	outColor = vec4(1.0,1.0,0.0,1.0);

}