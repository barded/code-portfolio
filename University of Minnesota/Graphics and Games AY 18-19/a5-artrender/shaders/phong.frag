#version 330

in vec3 position_in_eye_space;
in vec3 normal_in_eye_space;

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;


void main() {
    vec3 N=normalize(normal_in_eye_space);
    vec3 lightdir=normalize(light_in_eye_space-position_in_eye_space);
    vec4 diffuseC=kd*Id*max(dot(lightdir,N),0.0);
    vec4 ambC=ka*Ia;
    vec3 view=normalize(-position_in_eye_space);
    vec3 h=normalize(lightdir+view);

    float spec=max(dot(h,N),0.0);
    if(dot(lightdir,N)<=0)
      spec=0;
    vec4 specC=ks*Is*pow(spec,s);
    color=vec4(ambC+diffuseC+specC);
}
