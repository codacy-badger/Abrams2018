#pragma once

#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Vector3.hpp"

#include <string>
#include <vector>

class Rgba {
public:

    static const Rgba White;
    static const Rgba Black;
    static const Rgba Red;
    static const Rgba Pink;
    static const Rgba Green;
    static const Rgba ForestGreen;
    static const Rgba Blue;
    static const Rgba NavyBlue;
    static const Rgba Cyan;
    static const Rgba Yellow;
    static const Rgba Magenta;
    static const Rgba Orange;
    static const Rgba Violet;
    static const Rgba LightGrey;
    static const Rgba LightGray;
    static const Rgba Grey;
    static const Rgba Gray;
    static const Rgba DarkGrey;
    static const Rgba DarkGray;
    static const Rgba Olive;
    static const Rgba SkyBlue;
    static const Rgba Lime;
    static const Rgba Teal;
    static const Rgba Turquoise;
    static const Rgba Periwinkle;
    static const Rgba NormalZ;
    static const Rgba NoAlpha;

    static Rgba Random();
    static Rgba RandomGreyscale();
    static Rgba RandomGrayscale();
    static Rgba RandomWithAlpha();

    Rgba() = default;
    Rgba(const Rgba& rhs) = default;
    Rgba(Rgba&& rhs) = default;
    Rgba& operator=(const Rgba& rhs) = default;
    Rgba& operator=(Rgba&& rhs) = default;
    ~Rgba() = default;

    explicit Rgba(std::string name);
    explicit Rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0xFF);

    void SetAsBytes(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
    void SetAsFloats(float normalized_red, float normalized_green, float normalized_blue, float normalized_alpha);
    void GetAsFloats(float& out_normalized_red, float& out_normalized_green, float& out_normalized_blue, float& out_normalized_alpha) const;
    Vector4 GetRgbaAsFloats() const;
    Vector3 GetRgbAsFloats() const;
    void ScaleRGB(float scale);
    void ScaleAlpha(float scale);

    unsigned int GetAsRawValue() const;
    void SetFromRawValue(unsigned long value);
    void SetRGBAFromRawValue(unsigned long value);
    void SetRGBFromRawValue(unsigned long value);

    bool IsRgbEqual(const Rgba& rhs) const;
    bool operator==(const Rgba& rhs) const;
    bool operator!=(const Rgba& rhs) const;
    bool operator<(const Rgba& rhs) const;

    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
    unsigned char a = 255;

protected:
private:
    void SetValueFromName(std::string name);
};