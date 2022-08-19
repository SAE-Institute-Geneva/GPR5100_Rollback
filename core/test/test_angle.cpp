//
// Created by efarhan on 7/26/21.
//
#include "maths/angle.h"
#include <gtest/gtest.h>

TEST(Angle, RadianToDegree)
{
    constexpr core::Radian angle{core::PI};
    constexpr core::Degree angle2 = angle;

    EXPECT_FLOAT_EQ(180.0f, angle2.value());
}

TEST(Angle, DegreeToRadian)
{
    constexpr core::Degree angle{180.0f};
    constexpr core::Radian angle2 = angle;

    EXPECT_FLOAT_EQ(core::PI, angle2.value());
}

TEST(Angle, Cosinus)
{
    constexpr core::Degree angle{180.0f};
    constexpr core::Radian angle2 = angle;
    EXPECT_FLOAT_EQ(core::Cos(angle), core::Cos(angle2));
}

TEST(Angle, Sinus)
{
    constexpr core::Degree angle{180.0f};
    constexpr core::Radian angle2 = angle;
    EXPECT_FLOAT_EQ(core::Sin(angle), core::Sin(angle2));
}

TEST(Angle, Tan)
{
    constexpr core::Degree angle{45.0f};
    constexpr core::Radian angle2 = angle;
    EXPECT_FLOAT_EQ(core::Tan(angle), core::Tan(angle2));
}

TEST(Angle, ArcCosinus)
{
    constexpr core::Degree angle{180.0f};
    const auto result = core::Cos(angle);
    const core::Degree angleResult = core::Acos(result);
    EXPECT_FLOAT_EQ(angleResult.value(), angle.value());
}

TEST(Angle, ArcSinus)
{
    constexpr core::Degree angle{90.0f};
    const auto result = core::Sin(angle);
    const core::Degree angleResult = core::Asin(result);
    EXPECT_FLOAT_EQ(angleResult.value(), angle.value());
}

TEST(Angle, ArcTan)
{
    constexpr core::Degree angle{45.0f};
    const auto result = core::Tan(angle);
    const core::Degree angleResult = core::Atan(result);
    EXPECT_FLOAT_EQ(angleResult.value(), angle.value());
}