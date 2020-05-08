#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using a toon shading model

in vec3 position_in_eye_space;
in vec3 normal_in_eye_space;

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;

uniform sampler2D diffuse_ramp;
uniform sampler2D specular_ramp;


void main() {
  vec3 N=normalize(normal_in_eye_space);
  vec3 lightdir=normalize(light_in_eye_space-position_in_eye_space);
  vec4 diffuseC=kd*Id*texture(diffuse_ramp,vec2(.5*dot(lightdir,N)+.5,0));
  vec4 ambC=ka*Ia;
  vec3 reflectdir=reflect(-lightdir,N);
  vec3 view=normalize(-position_in_eye_space);
  vec3 h=normalize(lightdir+view);

  float spec=max(dot(h,N),0.0);
  if(dot(lightdir,N)<=0)
    spec=0;
  vec4 specC=ks*Is*texture(specular_ramp,vec2(pow(spec,s),0));
  color=vec4(ambC+diffuseC+specC);
}
