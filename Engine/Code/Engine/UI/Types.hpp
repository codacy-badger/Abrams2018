#pragma once

#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector2.hpp"

#include "Engine/Math/MathUtils.hpp"

namespace UI {

class Ratio {
public:
    explicit Ratio(const Vector2& newValue = Vector2::ZERO);
    Ratio(const Ratio& rhs);
    Ratio(Ratio&& rhs);
    Ratio& operator=(const Ratio& rhs);
    Ratio& operator=(Ratio&& rhs);
    ~Ratio() = default;
    const Vector2& GetValue() const;
    void SetValue(const Vector2& newValue);
private:
    Vector2 value{};
};

struct Metric {
    Ratio ratio{};
    Vector2 unit{};
};

class HalfExtent {
public:
    explicit HalfExtent(const Vector2& newValue = Vector2::ZERO);
    HalfExtent(const HalfExtent& rhs);
    HalfExtent(HalfExtent&& rhs);
    HalfExtent& operator=(const HalfExtent& rhs);
    HalfExtent& operator=(HalfExtent&& rhs);
    ~HalfExtent() = default;
    const Vector2& GetValue() const;
    void SetValue(const Vector2& newValue);
private:
    Vector2 value{};
};

enum class PositionMode {
    Absolute,
    Relative,
};

enum class FillMode {
    Fill,
    Fit,
    Stretch,
    Tile,
    Center,
    Span,
    Crop = Fill,
};

enum class PivotPosition {
    First_,
    Center = First_,
    TopLeft,
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
    Last_,
};

PivotPosition& operator++(PivotPosition& mode);
PivotPosition operator++(PivotPosition& mode, int);

PivotPosition& operator--(PivotPosition& mode);
PivotPosition operator--(PivotPosition& mode, int);

} //End UI