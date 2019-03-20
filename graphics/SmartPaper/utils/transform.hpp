#include "tinymatrix/matrix.hpp"
#include "tinymatrix/vector.hpp"

inline mat4 Perspective(double fovy, double Aspect, double zNear, double zFar)
{
  mat4 Result(mat4::zero());
  double f(1.0/tan((fovy/2.0)*M_PI/180.0)); 
  Result.at(0,0) = f/Aspect;
  Result.at(1,1) = f;
  Result.at(2,2) = (zFar+zNear)/(zNear-zFar);
  Result.at(2,3) = (2.0f*zFar*zNear)/(zNear-zFar);
  Result.at(3,2) = -1.0;
  return Result;
}
  
inline mat4 Frustum(double Left, double Right,
		    double Bottom, double Top,
		    double Near, double Far)
  {
    mat4 Result(mat4::zero());
    Result.at(0,0) = 2.*Near/(Right-Left);
    Result.at(1,1) = 2.*Near/(Top-Bottom);
    Result.at(2,2) = (Far+Near)/(Far-Near);
    Result.at(2,3) = 2.*Far*Near/(Far-Near);
    Result.at(3,2) = -1.;
    Result.at(0,2) = (Right+Left)/(Right-Left);
    Result.at(1,2) = (Top+Bottom)/(Top-Bottom);
    return Result;
  }

  inline mat4 Ortho(double Left, double Right,
		    double Bottom, double Top,
		    double Near, double Far)
  {
    mat4 Result(mat4::zero());
    Result.at(0,0) = 2./(Right-Left);
    Result.at(1,1) = 2./(Top-Bottom);
    Result.at(2,2) = -2./(Far-Near);
    Result.at(3,3) = 1.;

    Result.at(0,3) = -(Right+Left)/(Right-Left);
    Result.at(1,3) = -(Top+Bottom)/(Top-Bottom);
    Result.at(2,3) = -(Far+Near)/(Far-Near);
    return Result;
  }

  inline mat4 Ortho2D(double Left, double Right,
		      double Bottom, double Top)
  {
    return Ortho(Left, Right, Bottom, Top, -1., 1.);
  }

  inline mat4 Translate(vec3 Direction)
  {
    mat4 Result(mat4::identity());
    Result.at(0,3) = Direction.at(0);
    Result.at(1,3) = Direction.at(1);
    Result.at(2,3) = Direction.at(2);
    return Result;
  }

  inline mat4 Scale(vec3 Coefficients)
  {
    mat4 Result(mat4::identity());
    Result.at(0,0) = Coefficients.at(0);
    Result.at(1,1) = Coefficients.at(1);
    Result.at(2,2) = Coefficients.at(2);
    Result.at(3,3) = 1.;
    return Result;
  }
  
  //angles en degres:
  inline mat4 RotateX(double Angle)
  {
    mat4 Result(mat4::identity());
    Result.at(1,1) = cos(Angle*M_PI/180.0);
    Result.at(1,2) = -sin(Angle*M_PI/180.0);
    Result.at(2,1) = sin(Angle*M_PI/180.0);
    Result.at(2,2) = cos(Angle*M_PI/180.0);
    return Result;
  }
  inline mat4 RotateY(double Angle)
  {
    mat4 Result(mat4::identity());
    Result.at(0,0) = cos(Angle*M_PI/180.0);
    Result.at(0,2) = sin(Angle*M_PI/180.0);
    Result.at(2,0) = -sin(Angle*M_PI/180.0);
    Result.at(2,2) = cos(Angle*M_PI/180.0);
    return Result;
  }
  inline mat4 RotateZ(double Angle)
  {
    mat4 Result(mat4::identity());
    Result.at(0,0) = cos(Angle*M_PI/180.0);
    Result.at(0,1) = -sin(Angle*M_PI/180.0);
    Result.at(1,0) = sin(Angle*M_PI/180.0);
    Result.at(1,1) = cos(Angle*M_PI/180.0);
    return Result;
  }

  inline mat4 LookAt( const vec3& Eye,
		      const vec3& Center,
		      vec3 Up)
  {
    Normalize(Up);
    vec3 f(Center-Eye);
    Normalize(f);
    vec3 s(CrossProduct(f,Up));
    vec3 u(CrossProduct(s,f));
    
    mat4 r(mat4::zero());
    r.at(0,0) = s.at(0);
    r.at(0,1) = s.at(1);
    r.at(0,2) = s.at(2);

    r.at(1,0) = u.at(0);
    r.at(1,1) = u.at(1);
    r.at(1,2) = u.at(2);

    r.at(2,0) = -f.at(0);
    r.at(2,1) = -f.at(1);
    r.at(2,2) = -f.at(2);

    r.at(3,3) = 1.;
    
    return r*Translate(-Eye);
  }
