#version 430

//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
  }
  
 
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 



vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise2d(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

int 	koch_Count = 3;
float 	koch_distances[5];
vec2 	koch_points[5];
int	koch_mapping[5];

void sortFragments()
{
	int inc = koch_Count / 2;
	while (inc > 0)
	{
		for (int i = inc; i < koch_Count; ++i)
		{
			float tmp_dist = koch_distances[i];
			int tmp_map = koch_mapping[i];
			vec2 tmp_point = koch_points[i];
			int j = i;
			while (j >= inc && koch_distances[j - inc] > tmp_dist)
			{
				koch_distances[j] = koch_distances[j - inc];
				koch_points[j] = koch_points[j - inc];
				koch_mapping[j] = koch_mapping[j - inc];
				j -= inc;
			}
			koch_distances[j] = tmp_dist;
			koch_points[j] = tmp_point;
			koch_mapping[j] = tmp_map;
		}
		inc = int(inc / 2.2 + 0.5);
	}
}

layout (location=0)  uniform vec2 mpos;
layout (location=3)  uniform float time;

in vec2 pos;
layout (location=0) out vec4 FragColor;


float distance_pointline(vec2 point, vec2 p0,vec2 p1) {
vec2 v = p1-p0;
vec2 w = point-p0;

float c1 = dot(w,v);
if (c1<=0)
return distance(point,p0);
float c2 = dot(v,v);
if (c2<=c1)
return distance(point,p1);
float b = c1/c2;
vec2 Pb = p0+b*v;
return distance(point,Pb);
}



void main() {

koch_Count = 3;
float updown = sin(time+100)*0.0001-0.25;

koch_points[0] = vec2(-0.25,updown);
koch_points[1] = vec2(0.25,updown);
koch_points[2] = vec2(0.0,updown)+ normalize(vec2(0.0,0.25)-vec2(0.0,updown))*(0.43301270189221932338186158537647);

koch_points[0] += mpos;
koch_points[1] += mpos;
koch_points[2] += mpos;

float sumdot = 0;
vec2 prev = vec2(0,0);
for (int k=0; k<koch_Count; k++) {

vec2 vect = koch_points[k]-pos;
if (k==0){
prev = normalize(vect);
} else {
sumdot += dot(prev, normalize(vect));
prev = normalize(vect);
}
	koch_distances[k] = length(vect);
	}
sortFragments();

koch_Count = 5;

int timelevel =int( mod(time*2,6));

//if (sumdot<0)
//timelevel=0;

for (int level=0; level<timelevel; level++) {
	//we need to get five points (by kochenizing)
	//float linelength = distance(koch_points[0],koch_points[1]);
	vec2 twopointvector = koch_points[1]-koch_points[0];
	vec2 signvector = pos-koch_points[0];
	vec3 up = cross(vec3(signvector,0.0),vec3(twopointvector,0.0));
	vec3 perpendic = cross(vec3(twopointvector,0.0),up);
	
	
	vec2 perpendic2d = normalize(perpendic.xy);

	float tri_length =length(twopointvector)*0.3333333333333;
	float l =sqrt(tri_length*tri_length- (tri_length*tri_length*0.25));

	koch_mapping[0] = 0;
	koch_mapping[1] = 1;
	koch_mapping[2] = 2;
	koch_mapping[3] = 3;
	koch_mapping[4] = 4;
	
	koch_points[2] = koch_points[0]+twopointvector*0.3333333333333;
	koch_points[3] = koch_points[0]+twopointvector*0.6666666666666;

	koch_points[4] = (koch_points[0]+twopointvector*0.5)+perpendic2d*l;
	
	for (int k=0; k<koch_Count; k++)
		koch_distances[k] = distance(koch_points[k] ,pos);
	sortFragments();
	
	
	//the following is to get rid of the possibility of an unwanted scenario - comment it to see
	if ((koch_mapping[0]==1 && koch_mapping[1]==4) || (koch_mapping[1]==1 && koch_mapping[0]==4 ) )
		koch_points[1] = koch_points[3];

	if ((koch_mapping[0]==0 && koch_mapping[1]==4) || (koch_mapping[1]==0 && koch_mapping[0]==4 ) )
	koch_points[1] = koch_points[2];
		
		//remove interior edges
	if ((koch_mapping[0]==2 && koch_mapping[1]==3  ) || (koch_mapping[1]==2 && koch_mapping[0]==3  )){
		vec2 tmp = koch_points[1] ;
		koch_points[1] = koch_points[2];
		koch_points[2] = tmp;
	}

	
} //for


float dist =    distance_pointline(pos,koch_points[0],koch_points[1]);//length(pos-koch_points[0]);//

if (dist>=1)
dist = 1.0;
float finaldist =((0.005+sin(time*0.5)*0.001)/(dist+0.005+sin(time*0.5)*0.001))*(sin(time*0.5+50.0)*0.3+0.68); //0.2-pow(dist,1.0);


//first input - 3 points
//get two closest points
//kochenize - out 5 points
//get two closest points
//kochenize - out 5 points
//until max level reached. get closest two points / or approximate by getting closest two points




float noise =  snoise(vec3(pos*5.0,0))*0.5+0.5;

vec3 col = vec3(0,finaldist,0);
float signal =snoise(vec3(pos,finaldist));

float distfrommouse = distance(mpos,pos);
if (distfrommouse>=1)
distfrommouse = 1.0;
distfrommouse = ((0.1+sin(time)*0.005)/(distfrommouse+0.1+sin(time)*0.005))*(sin(time*0.5+50.0)*0.3+0.68); //0.2-pow(dist,1.0);

float s1 = snoise(vec3(pos+vec2(100,100)*time*0.001,finaldist));
float s2 = snoise(vec3(pos+vec2(100,-100)*time*0.005,1000)+vec3(0,distfrommouse,0));
float s3 = snoise(vec3(pos+vec2(-100,100)*time*(-0.001),distfrommouse));
float s4 = snoise(vec3(pos+vec2(-100,-1000)*time*(0.0001),0)+vec3(distfrommouse,0,0));



float t1 = snoise(vec3(sin(s1)*5,s2,s3));
float t2 = snoise(vec3(s2,sin(s4*5),s1)+vec3(0,sin(distfrommouse),0));
float t3 = snoise(vec3(s3,s1,sin(s2*0.01)));
//vec3 f = vec3(signal,signal,signal);
vec3 f = vec3(t1,t2,t3);

FragColor =  vec4(f,1); //vec3(finaldist,finaldist,finaldist)


}
