//http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
//return vec3.x=fnear, vec3.y=fFar, vec3.z=bool : miss or hit.
//make sure ray_o is normalized!!!
//radius2: R^2
//Be carefull: if ray_o is inside sphere, vec3.x < 0, use this value at your
//own discretion.
vec3 raySphareIntersection(vec3 center, float radius2, vec3 ray_o, vec3 ray_dir)
{
	float P0Pcenter2 = length(ray_o-center);
	P0Pcenter2 = P0Pcenter2*P0Pcenter2;
	//float A=dot(ray_dir,ray_dir)=1;
	float B = 2.0 * dot(ray_dir, (ray_o-center) );
	float C = P0Pcenter2 - radius2;
	float disc = B*B - 4.0 * C;
	if (disc<0.0)
	{//discriminant < 0, ray misses sphere
		return vec3(0,0,0);
	}
	float distSqrt = sqrt(disc);
	float q=0;
    vec3  hitInfo=vec3(0,0,1);
	if (B < 0.0)
	{
		q = (-B+distSqrt)*0.5;
        hitInfo.x = C/q;//t0
        hitInfo.y = q;//t1:=q/A
	}
	else{
		q = (-B-distSqrt)*0.5;
        hitInfo.x = q;//t0:=q/A
        hitInfo.y = C/q;//t1
	}
    // if t1 is less than zero, the object is in the ray's negative
    // direction and consequently the ray misses the sphere.
    if(hitInfo.y < 0)
        hitInfo.z = 0;
    return hitInfo;
}
//input parameters:
//rayStart, rayDir define the the ray starting position and direction.
//make sure ray_o is normalized!
//bottomLeft, topRight define the box geometry
//return: vec3.x: entry distance, vec3.y: exit distance, 
// vec3.z: hit(1) or miss(0) indication.
vec3 rayBoxIntersection(vec3 rayStart, vec3 rayDir, vec3 bottomLeft, vec3 topRight)
{
	//more compact version
	vec3 txyzMin = (b.pMin - eye_o) / dir;
	vec3 txyzMax = (b.pMax - eye_o) / dir;
	vec3 txyzEntry = min(txyzMin, txyzMax);
	vec3 txyzExit = max(txyzMin, txyzMax);
	float tentry = max(max(txyzEntry.x, txyzEntry.y), txyzEntry.z);
	float texit = min(min(txyzExit.x, txyzExit.y), txyzExit.z);

	vec3 hitInfo = vec3(0, 0, 1);
	hitInfo.x = tentry;//
	hitInfo.y = texit;
	if (texit < tentry)
	{
		hitInfo.z = 0;
	}
	return hitInfo;
	/*
	//More human readable version
    //The cube is [bottomLeft]x[topRight]
    //Compute entry point and exit point of the ray
    //In x-axis.
    float tx1 = (bottomLeft.x-rayStart.x)/rayDir.x;
    float tx2 = (  topRight.x-rayStart.x)/rayDir.x;
    float txentry = min(tx1,tx2);
    //In case eye is inside the volume, ignore the negative txentry and 
    //the entry point is the eye position.
          txentry = max(txentry, 0);
    float txexit  = max(tx1,tx2);
    //In y-axis
    float ty1 = (bottomLeft.y-rayStart.y)/rayDir.y;
    float ty2 = (  topRight.y-rayStart.y)/rayDir.y;
    float tyentry = min(ty1, ty2);
         tyentry = max(tyentry, 0);
    float tyexit = max(ty1, ty2);
    //In z-axis
    float tz1 = (bottomLeft.z-rayStart.z)/rayDir.z;
    float tz2 = (  topRight.z-rayStart.z)/rayDir.z;
    float tzentry = min(tz1, tz2);
         tzentry = max(tzentry, 0);
    float tzexit = max(tz1, tz2);

    float tentry = max(max(txentry,tyentry),tzentry);
    float texit  = min(min(txexit,tyexit),tzexit);
    vec3 hitInfo = vec3(tentry, texit, 1.0);//z=1.0 means ray hit the box.
    if (tentry > texit)
    {
        hitInfo.z = 0.0;//no hit happens.
    }
    return hitInfo;
	*/
}
//input parameters:
//rayStart, rayDir define the the ray starting position and direction.
//bottomLeft, topRight define the box geometry
bool pointInsideBox(vec3 pos, vec3 bottomLeft, vec3 topRight)
{
    return all(greaterThanEqual(pos, bottomLeft)) && all(lessThanEqual(pos, topRight));
}
#define PI 3.1415926f
#define invPi 0.3183f
#define inv2Pi 0.1591f
#define  RadiusToDegree 180.0f/PI
//http://www.cplusplus.com/reference/cmath/atan2/
vec2 SphericalXYZtoUV(in vec3 xyz){
	float r		= length(xyz);
	float theta = asin(xyz.y / r);
	float fi	= atan(xyz.x, xyz.z);//[-pi,+pi]
	return vec2(fi * inv2Pi + 0.5, theta * invPi + 0.5);//normalize to [0,1]
}

vec3 fromWGS84ToSphericalXYZ(vec2 WGS84, float R)
{
	float y = R * sin(WGS84.y);//WGS84.y := latitude
	float x = R * cos(WGS84.y) * sin(WGS84.x);//WGS84.x := longitude
	float z = R * cos(WGS84.y) * cos(WGS84.x);
	return vec3(x, y, z);
}
//Take the z value, between [0,1], from Z-Buffer and convert it into 
//the depth value in eye space where z axis is pointing towards eye. 
//That means the converted z values of visible object are mostly negative. 
//It cannot be directly visualized.
float ConvertToEyeSpaceDepth(float z)
{
    float z_n = 2.0 * z - 1.0;//from [0,1] ==> NDC:[-1,1]
    //return (2.0 * n * f) / (f + n - z_n * (f - n));//	z-axis pointing away from eye.
    return (2.0 * n * f) / (z_n * (f - n) -(f + n));//z-axis pointing towards eye.
}
//texCoord: texture coordinates of the fragment.[0,1]x[0,1]
//z value from the Z-Buffer.
vec3 EyeSpacePositionFromDepth(vec2 texCoord, float z)
{
    float c_x = texCoord.x * 2.0 - 1.0;
    float c_y = (1-texCoord.y) * 2.0 - 1.0;
    float c_z = z*2.0 - 1;
    vec4 c_pos = vec4(c_x,c_y,c_z,1.0);
    // Transform by the inverse projection matrix
    vec4 e_pos = gl_ProjectionMatrixInverse * c_pos;
    return e_pos.xyz/e_pos.w;
}
//Specific for atmosphere scattering.
float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}