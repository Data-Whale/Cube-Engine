#ifndef TZW_VEC3_H
#define TZW_VEC3_H

#include <string>
namespace tzw {

class vec3
{
public:
    vec3();
    vec3(float the_x,float the_y,float the_z);
    void set(float the_x,float the_y, float the_z);
    vec3 operator + (const vec3 & other) const;
    vec3 operator - (const vec3 & other) const;
    vec3 operator * (float a) const;
    vec3 operator / (float a) const;
    vec3 operator - () const;
    vec3 operator +=(const vec3 & other);
    vec3 operator *=(float value);
    static float DotProduct(const vec3 & left, const vec3 & right);
    static vec3 CrossProduct(const vec3 & left, const vec3 & right);
    float x,y,z;
    float distance(const vec3 &other)const;
    float getY() const;
    void setY(float value);
    float getX() const;
    void setX(float value);
    float getZ() const;
    void setZ(float value);
    float length() const;
    void normalize();
    static vec3 lerp(const vec3& from, const vec3 & to, float the_time);
    std::string getStr();
};

} // namespace tzw

#endif // TZW_VEC3_H
