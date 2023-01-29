#include "../src/matrix.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <random>

struct MatrixTest : public testing::Test
{
    constexpr static std::size_t gRows{20};
    constexpr static std::size_t gColumns{30};
    Matrix<double> matrix{Matrix<double>::create(Row{gRows}, Column{gColumns})};
    Matrix<double> matrixWithData{matrix};

    void SetUp() override
    {
        RandomizeMatrix(matrixWithData);
    }

    Matrix<double> &Get()
    {
        return matrix;
    }

    static void RandomizeMatrix(Matrix<double> &_In) noexcept
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution dist(0.0, 1.0);

        for (auto &column : _In.getMatrix())
        {
            for (auto &element : column)
            {
                element = dist(rng);
            }
        }
    }
};

TEST_F(MatrixTest, create)
{
    EXPECT_EQ(gRows, Get().getRows());
    EXPECT_EQ(gColumns, Get().getCols());
}

TEST_F(MatrixTest, copy)
{
    auto tMatrix = Get();
    EXPECT_TRUE(tMatrix == Get());
}

TEST_F(MatrixTest, move)
{
    auto tMatrix = Get();
    auto tMatrix2 = std::move(tMatrix);
    EXPECT_TRUE(tMatrix2 == Get());
}

TEST_F(MatrixTest, randomize)
{
    auto tMatrix = Get();
    RandomizeMatrix(tMatrix);

    std::vector<double> tControl;
    tControl.reserve(tMatrix.getCols().get() * tMatrix.getRows().get());

    for (const auto &tColumn : tMatrix.getMatrix())
    {
        for (const auto &tRow : tColumn)
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
}

TEST_F(MatrixTest, ceate_from_array)
{
    auto tMatrix = Get();
    RandomizeMatrix(tMatrix);

    Matrix<double> tMatrixToControl = Matrix<double>::create(tMatrix.getMatrix().at(0));

    for (size_t i = 0; i < tMatrix.getCols(); i++)
    {
        EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(0).at(i), tMatrixToControl.getMatrix().at(i).at(0));
    }
}

TEST_F(MatrixTest, create_from_vector_matrix)
{
    auto tMatrix = Get();
    RandomizeMatrix(tMatrix);

    auto tMatrixToControl = Matrix<double>::create(tMatrix.getMatrix());

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl.getMatrix().at(i).at(j));
        }
    }
}

TEST_F(MatrixTest, create_from_other_matrix)
{
    auto tMatrix = Get();
    RandomizeMatrix(tMatrix);

    auto tMatrixToControl = Matrix<double>::create(tMatrix);

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl.getMatrix().at(i).at(j));
        }
    }
}

TEST_F(MatrixTest, transpose)
{
    auto tMatrix = Get();
    RandomizeMatrix(tMatrix);
    auto tMatrix_control = tMatrix;
    tMatrix.transpose();

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrix_control.getMatrix().at(j).at(i));
        }
    }
}

TEST_F(MatrixTest, erase)
{
    auto tMatrix = Get();
    tMatrix.erase();

    EXPECT_EQ(0, tMatrix.getCols().get());
    EXPECT_EQ(0, tMatrix.getRows().get());
    EXPECT_EQ(0, tMatrix.getMatrix().size());
}

TEST_F(MatrixTest, operator_plus_equals_matrix)
{
    auto tMatrix = matrixWithData;
    auto tMatrixToControl = Get();
    auto tMatrixToControl2 = tMatrix;
    RandomizeMatrix(tMatrixToControl);

    tMatrix += tMatrixToControl;

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl.getMatrix().at(i).at(j) + tMatrixToControl2.getMatrix().at(i).at(j));
        }
    }
}

TEST_F(MatrixTest, operator_minus_equals_matrix)
{
    auto tMatrix = matrixWithData;
    auto tMatrixToControl = Get();
    auto tMatrixToControl2 = tMatrix;
    RandomizeMatrix(tMatrixToControl);

    tMatrix -= tMatrixToControl;

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl2.getMatrix().at(i).at(j) - tMatrixToControl.getMatrix().at(i).at(j));
        }
    }
}
TEST_F(MatrixTest, operator_multiply_equals_matrix_normal)
{
    auto tMatrix = matrixWithData;

    auto tMatrixToControl = Matrix<double>::create(Row{gColumns}, Column{gRows});
    RandomizeMatrix(tMatrixToControl);

    std::vector<std::vector<double>> tMatrixToControl2;
    tMatrixToControl2.resize(tMatrix.getRows().get());
    for (size_t i = 0; i < tMatrix.getRows().get(); i++)
    {
        tMatrixToControl2.at(i).resize(tMatrixToControl.getCols().get());
        for (size_t j = 0; j < tMatrixToControl.getCols().get(); j++)
        {
            double sum = 0;
            for (size_t k = 0; k < tMatrix.getCols().get(); k++)
            {
                sum += tMatrix.getMatrix().at(i).at(k) * tMatrixToControl.getMatrix().at(k).at(j);
            }
            tMatrixToControl2.at(i).at(j) = sum;
        }
    }

    tMatrix *= tMatrixToControl;

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl2.at(i).at(j));
        }
    }
}
TEST_F(MatrixTest, operator_multiply_equals_matrix_powerOf)
{
    auto tMatrix = matrixWithData;

    auto tMatrixToControl = tMatrix;

    EXPECT_TRUE(tMatrix == tMatrixToControl);

    tMatrix *= tMatrix;

    EXPECT_EQ(tMatrix.getCols().get(), tMatrixToControl.getCols().get());
    EXPECT_EQ(tMatrix.getRows().get(), tMatrixToControl.getRows().get());

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl.getMatrix().at(i).at(j) * tMatrixToControl.getMatrix().at(i).at(j));
        }
    }
}
TEST_F(MatrixTest, operator_multiply_equals_matrix_Zeilenvektor_mal_Spaltenvektor)
{
    auto tMatrix = Matrix<double>::create(Row{1}, Column{4});
    RandomizeMatrix(tMatrix);

    auto tMatrixToControl = Matrix<double>::create(Row{4}, Column{1});
    RandomizeMatrix(tMatrixToControl);

    std::vector<std::vector<double>> tMatrixToControl2;
    tMatrixToControl2.resize(tMatrix.getRows().get());
    for (size_t i = 0; i < tMatrix.getRows().get(); i++)
    {
        tMatrixToControl2.at(i).resize(tMatrixToControl.getCols().get());
        for (size_t j = 0; j < tMatrixToControl.getCols().get(); j++)
        {
            double sum = 0;
            for (size_t k = 0; k < tMatrix.getCols().get(); k++)
            {
                sum += tMatrix.getMatrix().at(i).at(k) * tMatrixToControl.getMatrix().at(k).at(j);
            }
            tMatrixToControl2.at(i).at(j) = sum;
        }
    }

    tMatrix *= tMatrixToControl;

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl2.at(i).at(j));
        }
    }
}
TEST_F(MatrixTest, operator_multiply_equals_matrix_Spaltenvektor_mal_Zeilenvektor)
{
    auto tMatrix = Matrix<double>::create(Row{4}, Column{1});
    RandomizeMatrix(tMatrix);

    auto tMatrixToControl = Matrix<double>::create(Row{1}, Column{4});
    RandomizeMatrix(tMatrixToControl);

    std::vector<std::vector<double>> tMatrixToControl2;
    tMatrixToControl2.resize(tMatrix.getRows().get());
    for (size_t i = 0; i < tMatrix.getRows().get(); i++)
    {
        tMatrixToControl2.at(i).resize(tMatrixToControl.getCols().get());
        for (size_t j = 0; j < tMatrixToControl.getCols().get(); j++)
        {
            double sum = 0;
            for (size_t k = 0; k < tMatrix.getCols().get(); k++)
            {
                sum += tMatrix.getMatrix().at(i).at(k) * tMatrixToControl.getMatrix().at(k).at(j);
            }
            tMatrixToControl2.at(i).at(j) = sum;
        }
    }

    tMatrix *= tMatrixToControl;

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl2.at(i).at(j));
        }
    }
}
TEST_F(MatrixTest, operator_plus_equals_double)
{
    auto tMatrix = matrixWithData;

    auto tMatrixToControl = tMatrix;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution dist(0.0, 1.0);
    auto tElem = dist(rng);

    tMatrix += tElem;

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl.getMatrix().at(i).at(j) + tElem);
        }
    }
}
TEST_F(MatrixTest, operator_minus_equals_double)
{
    auto tMatrix = matrixWithData;

    auto tMatrixToControl = tMatrix;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution dist(0.0, 1.0);
    auto tElem = dist(rng);

    tMatrix -= tElem;

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl.getMatrix().at(i).at(j) - tElem);
        }
    }
}
TEST_F(MatrixTest, operator_multiply_equals_double)
{
    auto tMatrix = matrixWithData;

    auto tMatrixToControl = tMatrix;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution dist(0.0, 1.0);
    auto tElem = dist(rng);

    tMatrix *= tElem;

    for (size_t i = 0; i < tMatrix.getRows(); i++)
    {
        for (size_t j = 0; j < tMatrix.getCols(); j++)
        {
            EXPECT_DOUBLE_EQ(tMatrix.getMatrix().at(i).at(j), tMatrixToControl.getMatrix().at(i).at(j) * tElem);
        }
    }
}

TEST_F(MatrixTest, operator_not_equals)
{
    auto tMatrix = matrixWithData;

    auto tMatrixToControl = Get();
    RandomizeMatrix(tMatrixToControl);

    EXPECT_TRUE(tMatrix != tMatrixToControl);
}