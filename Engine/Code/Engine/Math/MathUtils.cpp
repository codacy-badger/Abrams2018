#include "Engine/Math/MathUtils.hpp"

#include <cmath>

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/Capsule3.hpp"
#include "Engine/Math/Disc2.hpp"
#include "Engine/Math/Sphere3.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/LineSegment3.hpp"
#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/Plane3.hpp"

namespace MathUtils {

namespace {
unsigned int MT_RANDOM_SEED = 0;
}

void SetRandomEngineSeed(unsigned int seed) {
    MT_RANDOM_SEED = seed;
}

std::pair<float, float> SplitFloatingPointValue(float value) {
    float frac = 0.0f;
    float int_part = 0.0f;
    frac = std::modf(value, &int_part);
    return std::make_pair(int_part, frac);
}

std::pair<double, double> SplitFloatingPointValue(double value) {
    double frac = 0.0;
    double int_part = 0.0;
    frac = std::modf(value, &int_part);
    return std::make_pair(int_part, frac);
}

std::pair<long double, long double> SplitFloatingPointValue(long double value) {
    long double frac = 0.0;
    long double int_part = 0.0;
    frac = std::modf(value, &int_part);
    return std::make_pair(int_part, frac);
}

float ConvertDegreesToRadians(float degrees) {
    return degrees * (MathUtils::M_PI / 180.0f);
}

float ConvertRadiansToDegrees(float radians) {
    return radians * (180.0f * MathUtils::M_1_PI);
}

//NOT THREAD SAFE
std::random_device& GetRandomDevice() {
    static std::random_device rd;
    return rd;
}

//NOT THREAD SAFE
std::mt19937& GetMTRandomEngine(unsigned int seed /*= 0*/) {
    static std::mt19937 e = std::mt19937(!seed ? GetRandomDevice()() : seed);
    return e;
}

//NOT THREAD SAFE
std::mt19937_64& GetMT64RandomEngine(unsigned int seed /*= 0*/) {
    static std::mt19937_64 e = std::mt19937_64(!seed ? GetRandomDevice()() : seed);
    return e;
}

int GetRandomIntLessThan(int maxValueNotInclusive) {
    std::uniform_int_distribution<int> d(0, maxValueNotInclusive - 1);
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

int GetRandomIntInRange(int minInclusive, int maxInclusive) {
    std::uniform_int_distribution<int> d(minInclusive, maxInclusive);
    return d(GetMTRandomEngine(MT_RANDOM_SEED));

}

long GetRandomLongLessThan(long maxValueNotInclusive) {
    std::uniform_int_distribution<long> d(0L, maxValueNotInclusive - 1L);
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

long GetRandomLongInRange(long minInclusive, long maxInclusive) {
    std::uniform_int_distribution<long> d(minInclusive, maxInclusive);
    return d(GetMTRandomEngine(MT_RANDOM_SEED));

}

float GetRandomFloatInRange(float minInclusive, float maxInclusive) {
    std::uniform_real_distribution<float> d(minInclusive, std::nextafter(maxInclusive, maxInclusive + 1.0f));
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

float GetRandomFloatZeroToOne() {
    std::uniform_real_distribution<float> d(0.0f, std::nextafter(1.0f, 2.0f));
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

float GetRandomFloatZeroUpToOne() {
    std::uniform_real_distribution<float> d(0.0f, 1.0f);
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

float GetRandomFloatNegOneToOne() {
    return GetRandomFloatInRange(-1.0f, 1.0f);
}

double GetRandomDoubleInRange(double minInclusive, double maxInclusive) {
    std::uniform_real_distribution<double> d(minInclusive, std::nextafter(maxInclusive, maxInclusive + 1.0));
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

double GetRandomDoubleZeroToOne() {
    std::uniform_real_distribution<double> d(0.0, std::nextafter(1.0, 2.0));
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

double GetRandomDoubleZeroUpToOne() {
    std::uniform_real_distribution<double> d(0.0, 1.0);
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

double GetRandomDoubleNegOneToOne() {
    return GetRandomDoubleInRange(-1.0, 1.0);
}

long double GetRandomLongDoubleInRange(long double minInclusive, long double maxInclusive) {
    std::uniform_real_distribution<long double> d(minInclusive, std::nextafter(maxInclusive, maxInclusive + 1.0L));
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

long double GetRandomLongDoubleZeroToOne() {
    std::uniform_real_distribution<long double> d(0.0L, std::nextafter(1.0L, 2.0L));
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

long double GetRandomLongDoubleZeroUpToOne() {
    std::uniform_real_distribution<long double> d(0.0L, 1.0L);
    return d(GetMTRandomEngine(MT_RANDOM_SEED));
}

long double GetRandomLongDoubleNegOneToOne() {
    return GetRandomDoubleInRange(-1.0L, 1.0L);
}

bool IsPercentChance(float probability) {
    float roll = GetRandomFloatZeroToOne();
    return roll < probability;
}

bool IsPercentChance(double probability) {
    double roll = GetRandomDoubleZeroToOne();
    return roll < probability;
}

bool IsPercentChance(long double probability) {
    long double roll = GetRandomLongDoubleZeroToOne();
    return roll < probability;
}

float CosDegrees(float degrees) {
    float radians = MathUtils::ConvertDegreesToRadians(degrees);
    return std::cos(radians);
}

float SinDegrees(float degrees) {
    float radians = MathUtils::ConvertDegreesToRadians(degrees);
    return std::sin(radians);
}

float Atan2Degrees(float y, float x) {
    float radians = std::atan2(y, x);
    return MathUtils::ConvertRadiansToDegrees(radians);
}

bool IsEquivalent(float a, float b, float epsilon /*= 0.00001f*/) {
    return std::abs(a - b) < epsilon;
}

bool IsEquivalent(double a, double b, double epsilon /*= 0.0001*/) {
    return std::abs(a - b) < epsilon;
}

bool IsEquivalent(long double a, long double b, long double epsilon /*= 0.0001L*/) {
    return std::abs(a - b) < epsilon;
}

bool IsEquivalent(const Vector2& a, const Vector2& b, float epsilon /*= 0.0001f*/) {
    return IsEquivalent(a.x, b.x, epsilon) && IsEquivalent(a.y, b.y, epsilon);
}

bool IsEquivalent(const Vector3& a, const Vector3& b, float epsilon /*= 0.0001f*/) {
    return IsEquivalent(a.x, b.x, epsilon) && IsEquivalent(a.y, b.y, epsilon) && IsEquivalent(a.z, b.z, epsilon);
}

bool IsEquivalent(const Vector4& a, const Vector4& b, float epsilon /*= 0.0001f*/) {
    return IsEquivalent(a.x, b.x, epsilon) && IsEquivalent(a.y, b.y, epsilon) && IsEquivalent(a.z, b.z, epsilon) && IsEquivalent(a.w, b.w, epsilon);
}

float CalcDistance(const Vector2& a, const Vector2& b) {
    return (b - a).CalcLength();
}

float CalcDistance(const Vector3& a, const Vector3& b) {
    return (b - a).CalcLength();
}

float CalcDistance(const Vector4& a, const Vector4& b) {
    return (b - a).CalcLength4D();
}

float CalcDistance(const Vector2& p, const LineSegment2& line) {
    return std::sqrt(CalcDistanceSquared(p, line));
}

float CalcDistance(const Vector3& p, const LineSegment3& line) {
    return std::sqrt(CalcDistanceSquared(p, line));
}

float CalcDistance4D(const Vector4& a, const Vector4& b) {
    return (b - a).CalcLength4D();
}

float CalcDistance3D(const Vector4& a, const Vector4& b) {
    return (b - a).CalcLength3D();
}

float CalcDistanceSquared(const Vector2& a, const Vector2& b) {
    return (b - a).CalcLengthSquared();
}

float CalcDistanceSquared(const Vector3& a, const Vector3& b) {
    return (b - a).CalcLengthSquared();
}

float CalcDistanceSquared(const Vector4& a, const Vector4& b) {
    return (b - a).CalcLength4DSquared();
}

float CalcDistanceSquared(const Vector2& p, const LineSegment2& line) {
    return CalcDistanceSquared(p, CalcClosestPoint(p, line));
}

float CalcDistanceSquared(const Vector3& p, const LineSegment3& line) {
    return CalcDistanceSquared(p, CalcClosestPoint(p, line));
}

float CalcDistanceSquared4D(const Vector4& a, const Vector4& b) {
    return CalcDistanceSquared(a, b);
}

float CalcDistanceSquared3D(const Vector4& a, const Vector4& b) {
    return (b - a).CalcLength3DSquared();
}

float DotProduct(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;
}

float DotProduct(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float DotProduct(const Vector4& a, const Vector4& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vector3 CrossProduct(const Vector3& a, const Vector3& b) {
    float a1 = a.x;
    float a2 = a.y;
    float a3 = a.z;

    float b1 = b.x;
    float b2 = b.y;
    float b3 = b.z;

    return Vector3(a2 * b3 - a3 * b2, a3 * b1 - a1 * b3, a1 * b2 - a2 * b1);
}

Vector2 Project(const Vector2& a, const Vector2& b) {
    return (DotProduct(a, b) / DotProduct(b, b)) * b;
}

Vector3 Project(const Vector3& a, const Vector3& b) {
    return (DotProduct(a, b) / DotProduct(b, b)) * b;
}

Vector4 Project(const Vector4& a, const Vector4& b) {
    return (DotProduct(a, b) / DotProduct(b, b)) * b;
}

Vector2 Reflect(const Vector2& in, const Vector2& normal) {
    return in - ((2.0f * DotProduct(in, normal)) * normal);
}

Vector3 Reflect(const Vector3& in, const Vector3& normal) {
    return in - ((2.0f * DotProduct(in, normal)) * normal);
}

Vector4 Reflect(const Vector4& in, const Vector4& normal) {
    return in - ((2.0f * DotProduct(in, normal)) * normal);
}

Vector2 ProjectAlongPlane(const Vector2& v, const Vector2& n) {
    return v - (DotProduct(v, n) * n);
}

Vector3 ProjectAlongPlane(const Vector3& v, const Vector3& n) {
    return v - (DotProduct(v, n) * n);
}

Vector4 ProjectAlongPlane(const Vector4& v, const Vector4& n) {
    return v - (DotProduct(v, n) * n);
}

unsigned int CalculateManhattanDistance(const IntVector2& start, const IntVector2& end) {
    return std::abs(end.x - start.x) + std::abs(end.y - start.y);
}

unsigned int CalculateManhattanDistance(const IntVector3& start, const IntVector3& end) {
    return std::abs(end.x - start.x) + std::abs(end.y - start.y) + std::abs(end.z - start.z);
}

unsigned int CalculateManhattanDistance(const IntVector4& start, const IntVector4& end) {
    return std::abs(end.x - start.x) + std::abs(end.y - start.y) + std::abs(end.z - start.z) + std::abs(end.w - start.w);
}

bool IsPointInside(const AABB2& aabb, const Vector2& point) {
    if(aabb.maxs.x < point.x) return false;
    if(point.x < aabb.mins.x) return false;
    if(aabb.maxs.y < point.y) return false;
    if(point.y < aabb.mins.y) return false;
    return true;
}

bool IsPointInside(const AABB3& aabb, const Vector3& point) {
    if(aabb.maxs.x < point.x) return false;
    if(point.x < aabb.mins.x) return false;
    if(aabb.maxs.y < point.y) return false;
    if(point.y < aabb.mins.y) return false;
    if(aabb.maxs.z < point.z) return false;
    if(point.z < aabb.mins.z) return false;
    return true;
}

bool IsPointInside(const Disc2& disc, const Vector2& point) {
    return CalcDistanceSquared(disc.center, point) < (disc.radius * disc.radius);
}

bool IsPointInside(const Capsule2& capsule, const Vector2& point) {
    return CalcDistanceSquared(point, capsule.line) < (capsule.radius * capsule.radius);
}

bool IsPointInside(const Sphere3& sphere, const Vector3& point) {
    return CalcDistanceSquared(sphere.center, point) < (sphere.radius * sphere.radius);
}

bool IsPointInside(const Capsule3& capsule, const Vector3& point) {
    return CalcDistanceSquared(point, capsule.line) < (capsule.radius * capsule.radius);
}

bool IsPointOn(const Disc2& disc, const Vector2& point) {
    float distanceSquared = CalcDistanceSquared(disc.center, point);
    float radiusSquared = disc.radius * disc.radius;
    return !(distanceSquared < radiusSquared || radiusSquared < distanceSquared);
}

bool IsPointOn(const LineSegment2& line, const Vector2& point) {
    return MathUtils::IsEquivalent(CalcDistanceSquared(point, line), 0.0f);
}

bool IsPointOn(const Capsule2& capsule, const Vector2& point) {
    float distanceSquared = CalcDistanceSquared(point, capsule.line);
    float radiusSquared = capsule.radius * capsule.radius;
    return !(distanceSquared < radiusSquared || radiusSquared < distanceSquared);
}

bool IsPointOn(const LineSegment3& line, const Vector3& point) {
    return MathUtils::IsEquivalent(CalcDistanceSquared(point, line), 0.0f);
}

bool IsPointOn(const Sphere3& sphere, const Vector3& point) {
    float distanceSquared = CalcDistanceSquared(sphere.center, point);
    float radiusSquared = sphere.radius * sphere.radius;
    return !(distanceSquared < radiusSquared || radiusSquared < distanceSquared);
}

bool IsPointOn(const Capsule3& capsule, const Vector3& point) {
    float distanceSquared = CalcDistanceSquared(point, capsule.line);
    float radiusSquared = capsule.radius * capsule.radius;
    return !(distanceSquared < radiusSquared || radiusSquared < distanceSquared);
}

Vector2 CalcClosestPoint(const Vector2& p, const AABB2& aabb) {
    if(IsPointInside(aabb, p)) {
        return p;
    }
    if(p.x < aabb.mins.x && aabb.maxs.y < p.y) {
        return Vector2(aabb.mins.x, aabb.maxs.y);
    }
    if(p.x < aabb.mins.x && p.y < aabb.mins.y) {
        return Vector2(aabb.mins.x, aabb.mins.y);
    }
    if(aabb.maxs.x < p.x && p.y < aabb.mins.y) {
        return Vector2(aabb.maxs.x, aabb.mins.y);
    }
    if(aabb.maxs.x < p.x && aabb.maxs.y < p.y) {
        return Vector2(aabb.maxs.x, aabb.maxs.y);
    }
    if(p.x < aabb.mins.x) {
        return Vector2(aabb.mins.x, p.y);
    }
    if(aabb.maxs.x < p.x) {
        return Vector2(aabb.maxs.x, p.y);
    }
    if(p.y < aabb.mins.y) {
        return Vector2(p.x, aabb.mins.y);
    }
    if(aabb.maxs.y < p.y) {
        return Vector2(p.x, aabb.maxs.y);
    }
    return Vector2::ZERO;
}

Vector3 CalcClosestPoint(const Vector3& p, const AABB3& aabb) {
    float nearestX = MathUtils::Clamp(p.x, aabb.mins.x, aabb.maxs.x);
    float nearestY = MathUtils::Clamp(p.y, aabb.mins.y, aabb.maxs.y);
    float nearestZ = MathUtils::Clamp(p.z, aabb.mins.z, aabb.maxs.z);

    return Vector3(nearestX, nearestY, nearestZ);
}

Vector2 CalcClosestPoint(const Vector2& p, const Disc2& disc) {
    Vector2 dir = (p - disc.center).GetNormalize();
    return disc.center + dir * disc.radius;
}

Vector2 CalcClosestPoint(const Vector2& p, const LineSegment2& line) {
    Vector2 D = line.end - line.start;
    Vector2 T = D.GetNormalize();

    Vector2 SP = p - line.start;
    float regionI = MathUtils::DotProduct(T, SP);
    if(regionI < 0.0f) {
        return line.start;
    }
    
    Vector2 EP = p - line.end;
    float regionII = MathUtils::DotProduct(T, EP);
    if(regionII > 0.0f) {
        return line.end;
    }

    Vector2 directionSE = D.GetNormalize();
    float lengthToClosestPoint = MathUtils::DotProduct(directionSE, SP);
    Vector2 C = directionSE * lengthToClosestPoint;
    Vector2 ConL = line.start + C;
    return ConL;
}

Vector2 CalcClosestPoint(const Vector2& p, const Capsule2& capsule) {
    Vector2 closestP = CalcClosestPoint(p, capsule.line);
    Vector2 dir_to_p = (p - closestP).GetNormalize();
    return closestP + (dir_to_p * capsule.radius);
}

Vector3 CalcClosestPoint(const Vector3& p, const LineSegment3& line) {
    Vector3 D = line.end - line.start;
    Vector3 T = D.GetNormalize();
    Vector3 SP = p - line.start;
    float regionI = MathUtils::DotProduct(T, SP);
    if(regionI < 0.0f) {
        return line.start;
    }

    Vector3 EP = p - line.end;
    float regionII = MathUtils::DotProduct(T, EP);
    if(regionII > 0.0f) {
        return line.end;
    }

    Vector3 directionSE = D.GetNormalize();
    float lengthToClosestPoint = MathUtils::DotProduct(directionSE, SP);
    Vector3 C = directionSE * lengthToClosestPoint;
    Vector3 ConL = line.start + C;
    return ConL;
}

Vector3 CalcClosestPoint(const Vector3& p, const Sphere3& sphere) {
    Vector3 dir = (p - sphere.center).GetNormalize();
    return sphere.center + dir * sphere.radius;
}

Vector3 CalcClosestPoint(const Vector3& p, const Capsule3& capsule) {
    Vector3 closestP = CalcClosestPoint(p, capsule.line);
    Vector3 dir_to_p = (p - closestP).GetNormalize();
    return closestP + (dir_to_p * capsule.radius);
}

bool DoDiscsOverlap(const Disc2& a, const Disc2& b) {
    return DoDiscsOverlap(a.center, a.radius, b.center, b.radius);
}

bool DoDiscsOverlap(const Vector2& centerA, float radiusA, const Vector2& centerB, float radiusB) {
    return CalcDistanceSquared(centerA, centerB) < (radiusA + radiusB) * (radiusA + radiusB);
}

bool DoDiscsOverlap(const Disc2& a, const Capsule2& b) {
    return CalcDistanceSquared(a.center, b.line) < (a.radius + b.radius) * (a.radius + b.radius);
}

bool DoSpheresOverlap(const Sphere3& a, const Sphere3& b) {
    return DoSpheresOverlap(a.center, a.radius, b.center, b.radius);
}

bool DoSpheresOverlap(const Vector3& centerA, float radiusA, const Vector3& centerB, float radiusB) {
    return CalcDistanceSquared(centerA, centerB) < (radiusA + radiusB) * (radiusA + radiusB);
}

bool DoSpheresOverlap(const Sphere3& a, const Capsule3& b) {
    return CalcDistanceSquared(a.center, b.line) < (a.radius + b.radius) * (a.radius + b.radius);
}

bool DoAABBsOverlap(const AABB2& a, const AABB2& b) {
    if(a.maxs.x < b.mins.x) return false;
    if(b.maxs.x < a.mins.x) return false;
    if(a.maxs.y < b.mins.y) return false;
    if(b.maxs.y < a.mins.y) return false;
    return true;
}

bool DoAABBsOverlap(const AABB3& a, const AABB3& b) {
    if(a.maxs.x < b.mins.x) return false;
    if(b.maxs.x < a.mins.x) return false;
    if(a.maxs.y < b.mins.y) return false;
    if(b.maxs.y < a.mins.y) return false;
    if(a.maxs.z < b.mins.z) return false;
    if(b.maxs.z < a.mins.z) return false;
    return true;
}

bool DoLineSegmentOverlap(const Disc2& a, const LineSegment2& b) {
    return CalcDistanceSquared(a.center, b) < a.radius * a.radius;
}

bool DoLineSegmentOverlap(const Sphere3& a, const LineSegment3& b) {
    return CalcDistanceSquared(a.center, b) < a.radius * a.radius;
}

bool DoCapsuleOverlap(const Disc2& a, const Capsule2& b) {
    return CalcDistanceSquared(a.center, b.line) < (a.radius + b.radius) * (a.radius + b.radius);
}

bool DoCapsuleOverlap(const Sphere3& a, const Capsule3& b) {
    return CalcDistanceSquared(a.center, b.line) < (a.radius + b.radius) * (a.radius + b.radius);
}

bool DoPlaneOverlap(const Disc2& a, const Plane2& b) {
    return std::fabs(DotProduct(a.center, b.normal) - b.dist) < a.radius;
}

bool DoPlaneOverlap(const Sphere3& a, const Plane3& b) {
    return std::fabs(DotProduct(a.center, b.normal) - b.dist) < a.radius;
}

bool DoPlaneOverlap(const Capsule2& a, const Plane2& b) {
    bool both_capsule_points_in_front = IsPointInFrontOfPlane(a.line.start, b) && IsPointInFrontOfPlane(a.line.end, b);
    bool both_capsule_points_in_back = IsPointBehindOfPlane(a.line.start, b) && IsPointBehindOfPlane(a.line.end, b);

    if(both_capsule_points_in_front || both_capsule_points_in_back) {
        return CalcDistanceSquared(Vector2::ZERO, a.line) < (a.radius + b.dist) * (a.radius + b.dist);
    }
    return true;
}

bool DoPlaneOverlap(const Capsule3& a, const Plane3& b) {
    bool both_capsule_points_in_front = IsPointInFrontOfPlane(a.line.start, b) && IsPointInFrontOfPlane(a.line.end, b);
    bool both_capsule_points_in_back = IsPointBehindOfPlane(a.line.start, b) && IsPointBehindOfPlane(a.line.end, b);

    if(both_capsule_points_in_front || both_capsule_points_in_back) {
        return CalcDistanceSquared(Vector3::ZERO, a.line) < (a.radius + b.dist) * (a.radius + b.dist);
    }
    return true;
}

bool IsPointInFrontOfPlane(const Vector3& point, const Plane3& plane) {
    return (DotProduct(point, plane.normal) > plane.dist);
}

bool IsPointBehindOfPlane(const Vector3& point, const Plane3& plane) {
    return (DotProduct(point, plane.normal) < plane.dist);
}

bool IsPointOnPlane(const Vector3& point, const Plane3& plane) {
    return !IsPointInFrontOfPlane(point, plane) && !IsPointBehindOfPlane(point, plane);
}

bool IsPointInFrontOfPlane(const Vector2& point, const Plane2& plane) {
    return (DotProduct(point, plane.normal) > plane.dist);
}

bool IsPointBehindOfPlane(const Vector2& point, const Plane2& plane) {
    return (DotProduct(point, plane.normal) < plane.dist);
}

bool IsPointOnPlane(const Vector2& point, const Plane2& plane) {
    return !IsPointInFrontOfPlane(point, plane) && !IsPointBehindOfPlane(point, plane);
}

template<>
Vector2 Clamp<Vector2>(const Vector2& valueToClamp, const Vector2& minRange, const Vector2& maxRange) {
    Vector2 result = valueToClamp;
    result.x = Clamp(valueToClamp.x, minRange.x, maxRange.x);
    result.y = Clamp(valueToClamp.y, minRange.y, maxRange.y);
    return result;
}

template<>
Vector3 Clamp<Vector3>(const Vector3& valueToClamp, const Vector3& minRange, const Vector3& maxRange) {
    Vector3 result = valueToClamp;
    result.x = Clamp(valueToClamp.x, minRange.x, maxRange.x);
    result.y = Clamp(valueToClamp.y, minRange.y, maxRange.y);
    result.z = Clamp(valueToClamp.z, minRange.z, maxRange.z);
    return result;
}

template<>
Vector4 Clamp<Vector4>(const Vector4& valueToClamp, const Vector4& minRange, const Vector4& maxRange) {
    Vector4 result = valueToClamp;
    result.x = Clamp(valueToClamp.x, minRange.x, maxRange.x);
    result.y = Clamp(valueToClamp.y, minRange.y, maxRange.y);
    result.z = Clamp(valueToClamp.z, minRange.z, maxRange.z);
    result.w = Clamp(valueToClamp.w, minRange.w, maxRange.w);
    return result;
}

template<>
IntVector2 Clamp<IntVector2>(const IntVector2& valueToClamp, const IntVector2& minRange, const IntVector2& maxRange) {
    IntVector2 result = valueToClamp;
    result.x = Clamp(valueToClamp.x, minRange.x, maxRange.x);
    result.y = Clamp(valueToClamp.y, minRange.y, maxRange.y);
    return result;
}

template<>
IntVector3 Clamp<IntVector3>(const IntVector3& valueToClamp, const IntVector3& minRange, const IntVector3& maxRange) {
    IntVector3 result = valueToClamp;
    result.x = Clamp(valueToClamp.x, minRange.x, maxRange.x);
    result.y = Clamp(valueToClamp.y, minRange.y, maxRange.y);
    result.z = Clamp(valueToClamp.z, minRange.z, maxRange.z);
    return result;
}

template<>
IntVector4 Clamp<IntVector4>(const IntVector4& valueToClamp, const IntVector4& minRange, const IntVector4& maxRange) {
    IntVector4 result = valueToClamp;
    result.x = Clamp(valueToClamp.x, minRange.x, maxRange.x);
    result.y = Clamp(valueToClamp.y, minRange.y, maxRange.y);
    result.z = Clamp(valueToClamp.z, minRange.z, maxRange.z);
    result.w = Clamp(valueToClamp.w, minRange.w, maxRange.w);
    return result;
}

template<>
Vector2 Interpolate(const Vector2& a, const Vector2& b, float t) {
    float x = Interpolate(a.x, b.x, t);
    float y = Interpolate(a.y, b.y, t);
    return Vector2(x, y);
}

template<>
Vector3 Interpolate(const Vector3& a, const Vector3& b, float t) {
    float x = Interpolate(a.x, b.x, t);
    float y = Interpolate(a.y, b.y, t);
    float z = Interpolate(a.z, b.z, t);
    return Vector3(x, y, z);
}

template<>
Vector4 Interpolate(const Vector4& a, const Vector4& b, float t) {
    float x = Interpolate(a.x, b.x, t);
    float y = Interpolate(a.y, b.y, t);
    float z = Interpolate(a.z, b.z, t);
    float w = Interpolate(a.w, b.w, t);
    return Vector4(x, y, z, w);
}

template<>
IntVector2 Interpolate(const IntVector2& a, const IntVector2& b, float t) {
    float x = Interpolate(static_cast<float>(a.x), static_cast<float>(b.x), t);
    float y = Interpolate(static_cast<float>(a.y), static_cast<float>(b.y), t);
    return IntVector2(Vector2(x, y));
}

template<>
IntVector3 Interpolate(const IntVector3& a, const IntVector3& b, float t) {
    float x = Interpolate(static_cast<float>(a.x), static_cast<float>(b.x), t);
    float y = Interpolate(static_cast<float>(a.y), static_cast<float>(b.y), t);
    float z = Interpolate(static_cast<float>(a.z), static_cast<float>(b.z), t);
    return IntVector3(Vector3(x, y, z));
}

template<>
IntVector4 Interpolate(const IntVector4& a, const IntVector4& b, float t) {
    float x = Interpolate(static_cast<float>(a.x), static_cast<float>(b.x), t);
    float y = Interpolate(static_cast<float>(a.y), static_cast<float>(b.y), t);
    float z = Interpolate(static_cast<float>(a.z), static_cast<float>(b.z), t);
    float w = Interpolate(static_cast<float>(a.w), static_cast<float>(b.w), t);
    return IntVector4(Vector4(x, y, z, w));
}

template<>
AABB2 Interpolate(const AABB2& a, const AABB2& b, float t) {
    Vector2 mins(Interpolate(a.mins, b.mins, t));
    Vector2 maxs(Interpolate(a.maxs, b.maxs, t));
    return AABB2(mins, maxs);
}

template<>
AABB3 Interpolate(const AABB3& a, const AABB3& b, float t) {
    Vector3 mins(Interpolate(a.mins, b.mins, t));
    Vector3 maxs(Interpolate(a.maxs, b.maxs, t));
    return AABB3(mins, maxs);
}

template<>
Disc2 Interpolate(const Disc2& a, const Disc2& b, float t) {
    Vector2 c(Interpolate(a.center, b.center, t));
    float r(Interpolate(a.radius, b.radius, t));
    return Disc2(c, r);
}

template<>
LineSegment2 Interpolate(const LineSegment2& a, const LineSegment2& b, float t) {
    Vector2 start(Interpolate(a.start, b.start, t));
    Vector2 end(Interpolate(a.end, b.end, t));
    return LineSegment2(start, end);
}

template<>
Capsule2 Interpolate(const Capsule2& a, const Capsule2& b, float t) {
    LineSegment2 line(Interpolate(a.line, b.line, t));
    float r(Interpolate(a.radius, b.radius, t));
    return Capsule2(line, r);
}

template<>
LineSegment3 Interpolate(const LineSegment3& a, const LineSegment3& b, float t) {
    Vector3 start(Interpolate(a.start, b.start, t));
    Vector3 end(Interpolate(a.end, b.end, t));
    return LineSegment3(start, end);
}

template<>
Sphere3 Interpolate(const Sphere3& a, const Sphere3& b, float t) {
    Vector3 c(Interpolate(a.center, b.center, t));
    float r(Interpolate(a.radius, b.radius, t));
    return Sphere3(c, r);
}

template<>
Capsule3 Interpolate(const Capsule3& a, const Capsule3& b, float t) {
    LineSegment3 line(Interpolate(a.line, b.line, t));
    float r(Interpolate(a.radius, b.radius, t));
    return Capsule3(line, r);
}

template<>
Plane2 Interpolate(const Plane2& a, const Plane2& b, float t) {
    float d = Interpolate(a.dist, b.dist, t);
    Vector2 n = Interpolate(a.normal, b.normal, t);
    return Plane2(n, d);
}

template<>
Plane3 Interpolate(const Plane3& a, const Plane3& b, float t) {
    float d = Interpolate(a.dist, b.dist, t);
    Vector3 n = Interpolate(a.normal, b.normal, t);
    return Plane3(n, d);
}

} //End MathUtils