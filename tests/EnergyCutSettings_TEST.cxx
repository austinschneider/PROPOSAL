
#include "gtest/gtest.h"

#include "PROPOSAL/EnergyCutSettings.h"
#include "PROPOSAL/json.h"

using namespace PROPOSAL;

TEST(Constructor)
{
    nlohmann::json config;
    config["e_cut"] = 500;
    config["v_cut"] = 0.05;
    config["cont_rand"] = true;

    EnergyCutSettings A;
    EnergyCutSettings B(500, 0.05, true);
    EnergyCutSettings c(config);
}

TEST(Assignment, Copyconstructor)
{
    EnergyCutSettings A(100, 1, false);
    EnergyCutSettings B(A);

    EXPECT_TRUE(A == B);
}

TEST(Comparison, Comparison_equal)
{
    EnergyCutSettings A;
    EnergyCutSettings B(500, 0.05, true);
    EXPECT_TRUE(A == B);
}

TEST(Comparison, Comparison_not_equal)
{
    EnergyCutSettings A;
    EnergyCutSettings B(200, 0.09);
    EXPECT_TRUE(A != B);
}

TEST(Exceptions, wrong ecut)
{
    bool thrown {false};

    try {
        EnergyCutSettings cut(-500, 0.05, true);
    } catch(const& std::invalid_argument except) {
        thrown = true;
    }

    EXPECT_TRUE(thrown);
}

TEST(Exceptions, wrong vcut)
{
    bool thrown {false};

    try {
        EnergyCutSettings cut(500, -0.05, true);
    } catch(const& std::invalid_argument except) {
        thrown = true;
    }

    EXPECT_TRUE(thrown);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
