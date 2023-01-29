
#include "../src/vector.h"
#include "../src/Types/row.h"

#include <gtest/gtest.h>

#include <chrono>
#include <random>

struct VectorTest : public testing::Test
{
    constexpr static std::size_t gRows{20};

    Vector<double> vector{Vector<double>::create(Row{gRows})};

    void SetUp() override
    {
    }

    const Vector<double> &Get() const noexcept
    {
        return vector;
    }

    static void randomize(Vector<double> &xVector) noexcept
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution dist(0.0, 1.0);

        for (auto &tElem : xVector)
        {
            tElem = dist(rng);
        }
    }
};


TEST_F(VectorTest, create)
{
    EXPECT_EQ(gRows, Get().getRows().get());
}

TEST_F(VectorTest, copy)
{
    auto tVector = Get();
    EXPECT_TRUE(tVector == Get());
}

TEST_F(VectorTest, move)
{
    auto tVector = Get();
    auto tVector2 = std::move(tVector);
    EXPECT_TRUE(tVector2 == Get());
}

TEST_F(VectorTest, randomize)
{
    auto tVector = Get();
    randomize(tVector);

    std::vector<double> tControl;
    tControl.reserve(tVector.getRows().get());

    for (const auto &tRow : tVector)
    {
        EXPECT_LT(0.0, tRow);
        EXPECT_GE(1.0, tRow);
        for (const auto &tElem : tControl)
        {
            EXPECT_NE(tElem, tRow);
        }
        tControl.emplace_back(tRow);
    }
}

TEST_F(VectorTest, create_from_std_vector)
{
    auto tVector = Get();
    randomize(tVector);

    auto tVectorToControl = Vector<double>::create(tVector.getData());

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        EXPECT_DOUBLE_EQ(tVectorToControl[i], tVector.at(i));
    }
}

TEST_F(VectorTest, create_from_other_vector)
{
    auto tVector = Get();
    randomize(tVector);

    auto tVectorToControl = Vector<double>::create(tVector);

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        EXPECT_DOUBLE_EQ(tVectorToControl.at(i), tVector.at(i));
    }
}

TEST_F(VectorTest, erase)
{
    auto tVector = Get();
    randomize(tVector);

    tVector.erase();

    EXPECT_EQ(0, tVector.getRows());
    EXPECT_EQ(0, tVector.size());
}

TEST_F(VectorTest, opposite)
{
    auto tVector = Get();
    randomize(tVector);

    auto tVectorToControl = tVector;

    for (auto &tElem : tVector)
    {
        tElem *= -1;
    }

    EXPECT_TRUE(tVector.opposite(tVectorToControl));
    EXPECT_FALSE(tVector.opposite(tVector));
}

TEST_F(VectorTest, normalize)
{
    auto tVector = Get();

    for (auto &tElem : tVector)
    {
        tElem = static_cast<double>((std::rand()) % 10000000);
    }

    tVector.normalize();

    for (const auto &tElem : tVector)
    {
        EXPECT_GE(1.0, tElem);
    }

    EXPECT_DOUBLE_EQ(1.0, tVector.magnitude());
}

TEST_F(VectorTest, parallel)
{
    auto tVector = Get();
    randomize(tVector);

    constexpr double tTarget{-3.3};

    auto tVectorToControl = tVector;
    auto tVectorNegativ = tVector;
    randomize(tVectorNegativ);

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        tVectorToControl.at(i) = tTarget * tVector.at(i);
    }

    EXPECT_TRUE(tVector.parallel(tVectorToControl));
    EXPECT_FALSE(tVector.parallel(tVectorNegativ));
}

TEST_F(VectorTest, anti_parallel)
{
    auto tVector = Get();
    randomize(tVector);

    constexpr double tTarget{-3.3};

    auto tVectorToControl = tVector;
    auto tVectorNegativ = tVector;
    randomize(tVectorNegativ);

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        tVectorToControl.at(i) = tTarget * tVector.at(i) * -1;
    }

    EXPECT_TRUE(tVector.antiParallel(tVectorToControl));
    EXPECT_FALSE(tVector.antiParallel(tVectorNegativ));
}

TEST_F(VectorTest, operator_plus_equals_vector)
{
    auto tVector = Get();
    randomize(tVector);

    auto tVectorToControl = Get();

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        tVectorToControl.at(i) = tVector.at(i) + tVector.at(i);
    }

    tVector += tVector;

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        EXPECT_DOUBLE_EQ(tVector.at(i), tVectorToControl.at(i));
    }
}

TEST_F(VectorTest, operator_plus_equals_scalar)
{
    auto tVector = Get();
    randomize(tVector);

    auto tVectorToControl = Get();
    constexpr double tScalar{42.0};

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        tVectorToControl.at(i) = tVector.at(i) + tScalar;
    }

    tVector += tScalar;

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        EXPECT_DOUBLE_EQ(tVector.at(i), tVectorToControl.at(i));
    }
}

TEST_F(VectorTest, operator_minus_equals_vector)
{
    auto tVector = Get();
    randomize(tVector);

    auto tVectorToControl = Get();

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        tVectorToControl.at(i) = tVector.at(i) - tVector.at(i);
    }

    tVector -= tVector;

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        EXPECT_DOUBLE_EQ(tVector.at(i), tVectorToControl.at(i));
    }
}

TEST_F(VectorTest, operator_minus_equals_scalar)
{
    auto tVector = Get();
    randomize(tVector);

    auto tVectorToControl = Get();
    constexpr double tScalar{42.0};

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        tVectorToControl.at(i) = tVector.at(i) - tScalar;
    }

    tVector -= tScalar;

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        EXPECT_DOUBLE_EQ(tVector.at(i), tVectorToControl.at(i));
    }
}

TEST_F(VectorTest, operator_multiply_equals_scalar)
{
    auto tVector = Get();
    randomize(tVector);

    auto tVectorToControl = Get();
    constexpr double tScalar{42.0};

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        tVectorToControl.at(i) = tVector.at(i) * tScalar;
    }

    tVector *= tScalar;

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        EXPECT_DOUBLE_EQ(tVector.at(i), tVectorToControl.at(i));
    }
}

TEST_F(VectorTest, dot_product)
{
    auto tVector = Get();
    randomize(tVector);
    auto tVector2 = Get();
    randomize(tVector2);

    double tSum{0};
    for (std::size_t i = 0; i < tVector.size(); i++)
    {
        tSum += tVector.at(i) * tVector2.at(i);
    }

    auto tToControl = tVector.dotProduct(tVector2);

    EXPECT_DOUBLE_EQ(tSum, tToControl);
}

TEST_F(VectorTest, cross_product)
{
    auto tVector = Vector<double>::create(Row{3});
    auto tVector2 = Vector<double>::create(Row{3});
    auto tVectorResult = Vector<double>::create(Row{3});

    tVector.at(0) = 1;
    tVector.at(1) = 2;
    tVector.at(2) = 3;
    tVector2.at(0) = -7;
    tVector2.at(1) = 8;
    tVector2.at(2) = 9;

    tVectorResult.at(0) = -6;
    tVectorResult.at(1) = -30;
    tVectorResult.at(2) = 22;

    auto tVectorToControl = tVector.crossProduct(tVector2);

    EXPECT_EQ(3, tVectorToControl.size());

    for (std::size_t i = 0; i < tVectorToControl.size(); i++)
    {
        EXPECT_DOUBLE_EQ(tVectorResult.at(i), tVectorToControl.at(i));
    }
}