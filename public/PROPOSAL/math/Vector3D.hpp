#include <array>
#include <cmath>
#include <iostream>
#include <sstream>

using std::array;
using std::cos;
using std::sin;

namespace PROPOSAL {
template <typename T> class Vector3D {
protected:
    std::array<T, 3> values;

public:
    Vector3D() = default;
    Vector3D(array<T, 3> val)
        : values(std::move(val)){};

    bool operator==(const Vector3D&&) const;
    T& operator[](size_t idx);
    std::ostream& print(std::ostream& os) const;

    virtual T magnitude() const = 0;

    array<T, 3> GetArray() const { return values; }
};
} // namespace PROPOSAL

namespace PROPOSAL {
template <typename T> class Spherical3D;
template <typename T> class Cartesian3D : public Vector3D<T> {
public:
    Cartesian3D(array<T, 3> val)
        : Vector3D<T>(std::move(val)){};

    Cartesian3D<T>& operator+=(const Cartesian3D&);

    T magnitude() const override;
    Spherical3D<T> ToSpherical() const;
};
} // namespace PROPOSAL

namespace PROPOSAL {
template <typename T> class Spherical3D : public Vector3D<T> {
public:
    Spherical3D(array<T, 3> val)
        : Vector3D<T>(std::move(val)){};

    T magnitude() const override;
    Cartesian3D<T> ToCartesian() const;
};
} // namespace PROPOSAL

namespace PROPOSAL {
template <typename T> bool Vector3D<T>::operator==(const Vector3D&& rhs) const
{
    for (size_t i = 0; i < 3; ++i) {
        if (values[i] != rhs[i])
            return false;
    }
    return true;
}

template <typename T> T& Vector3D<T>::operator[](size_t idx)
{
    return values[idx];
}

template <typename T> T& Vector3D<T>::print(ostream& os) const
{
    os << "x0: " << values[0] << "\t";
    os << "x1: " << values[1] << "\t";
    os << "x2: " << values[2] << "\n";
    return os;
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const Vector3D<U>& vec)
{
    os << "================ Vector3D (" << &vec << ") ================\n";
    os << vec.print(os);
    os << "===========================================================\n";
    return os;
}

template <typename T>
Cartesian3D<T> operator*(Cartesian3D<T> vec, double factor)
{
    for (auto& i : Vector3D<T>::values)
        i *= factor;
    return vec;
}

template <typename U>
U operator*(const Cartesian3D<U>& l_vec, const Cartesian3D<U>& r_vec)
{
    U accum = 0;
    for (size_t i = 0; i < 3; ++i)
        accum += l_vec[i] * r_vec[i];
    return accum;
}

template <typename U>
Cartesian3D<U> operator+(Cartesian3D<U> l_vec, Cartesian3D<U>& r_vec)
{
    for (size_t i = 0; i < 3; ++i)
        l_vec[i] += r_vec[i];
    return l_vec;
}

template <typename U>
Cartesian3D<U> operator-(Cartesian3D<U> l_vec, const Cartesian3D<U>& r_vec)
{
    for (size_t i = 0; i < 3; ++i)
        l_vec[i] -= r_vec[i];
    return l_vec;
}

template <typename T>
Cartesian3D<T>& Cartesian3D<T>::operator+=(const Cartesian3D<T>& rhs)
{
    for (size_t i = 0; i < 3; ++i)
        Vector3D<T>::values[i] += rhs[i];
    return *this;
}

template <typename U>
Cartesian3D<U> cross_prod(
    const Cartesian3D<U>& l_vec, const Cartesian3D<U>& r_vec)
{
    Cartesian3D<U> vec(0, 0, 0);
    for (size_t i = 0; i < 3; ++i) {
        vec.values[i] = l_vec[(1 + i) % 3] * r_vec[(2 + i) % 3]
            + l_vec[(i + 2) % 3] * r_vec[(i + 1) % 3];
    }
    return vec;
}

template <typename T> T Cartesian3D<T>::magnitude() const
{
    T accum = 0;
    for (const auto& i : Vector3D<T>::values)
        accum += i * i;
    return std::sqrt(accum);
}

template <typename T> Spherical3D<T> Cartesian3D<T>::ToSpherical() const
{
    auto radius = magnitude();

    if (radius == 0)
        return Spherical3D<T>({ 0, 0, 0 });

    auto azimuth = std::atan2(Vector3D<T>::values[1], Vector3D<T>::values[0]);
    auto zenith = std::acos(Vector3D<T>::values[2] / radius);

    return Spherical3D<T>({ radius, azimuth, zenith });
}

template <typename T>
Spherical3D<T> operator*(Spherical3D<T> vec, double factor)
{
    vec[0] *= factor;
    return vec;
}

template <typename T> T operator*(Spherical3D<T> l_vec, Spherical3D<T> r_vec)
{
    return l_vec[0] * r_vec[0]
        * (sin(l_vec[1]) * sin(r_vec[1]) * cos(l_vec[2] - r_vec[2])
              + cos(l_vec[1]) * cos(r_vec[1]));
}

template <typename T> T Spherical3D<T>::magnitude() const
{
    return Vector3D<T>::values[0];
}

template <typename T> Cartesian3D<T> Spherical3D<T>::ToCartesian() const
{
    auto cos_a = cos(Vector3D<T>::values[1]);
    auto sin_a = sin(Vector3D<T>::values[1]);
    auto cos_z = cos(Vector3D<T>::values[2]);
    auto sin_z = sin(Vector3D<T>::values[2]);

    auto x = Vector3D<T>::values[0] * cos_a * sin_z;
    auto y = Vector3D<T>::values[0] * sin_a * sin_z;
    auto z = Vector3D<T>::values[0] * cos_z;

    return Cartesian({ x, y, z });
}
} // namespace PROPOSAL
