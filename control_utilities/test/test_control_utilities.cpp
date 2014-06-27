#include <gtest/gtest.h>

#include <control_utilities/limits.hpp>

using namespace control_utilities;

#define LENGTH(x) sizeof(x) / sizeof(x[0])

TEST(limits_test, clamp_values)
{
    // Check a series of values
    double values[] = {-1.5, -0.5, 0., 0.5, 1.5};
    double min = -1.;
    double max = 1.;
    double clamped_expected[] = {-1., -0.5, -0., 0.5, 1.};
    int results_expected[] = {-1, 0, 0, 0, 1};

    int count = LENGTH(values);
    for (int i = 0; i < count; ++i)
    {
        int result = -2;
        double clamped = clamp(values[i], min, max, &result);
        EXPECT_EQ(clamped_expected[i], clamped);
        EXPECT_EQ(results_expected[i], result);

        double clamped_no_result = clamp(values[i], min, max);
        EXPECT_EQ(clamped, clamped_no_result);
    }
}

TEST(limits_test, clamp_nan)
{
    // Check clamping NAN
    double min = -1.;
    double max = 1.;
    int result = -2;
    EXPECT_TRUE(std::isnan(clamp(NAN, min, max, &result)));
    EXPECT_EQ(0, result);
}

TEST(limits_test, clamp_no_min)
{
    // No lower bound
    double min = -1.;
    double max = 1.;
    int result = -2;
    EXPECT_EQ(2 * min, clamp(2 * min, NAN, max, &result));
    EXPECT_EQ(0, result);
}

TEST(limits_test, clamp_no_max)
{
    // No upper bound
    double min = -1.;
    double max = 1.;
    int result = -2;
    EXPECT_EQ(2 * max, clamp(2 * max, min, NAN, &result));
    EXPECT_EQ(0, result);
}

TEST(limits_test, clamp_no_bounds)
{
    // No bounds
    double max = 1.;
    int result = -2;
    EXPECT_EQ(2 * max, clamp(2 * max, NAN, NAN, &result));
    EXPECT_EQ(0, result);
}

TEST(limits_test, rate_limit)
{
    double prev_value = 1.;
    double cur_values[] = {-1., 0., 1., 3.};
    double dt = 0.005;
    double rate_min = -100;
    double rate_max = 200;
    double clamped_expected[] = {0.5, 0.5, 1., 2.};
    int results_expected[] = {-1, -1, 0, 1};

    int count = LENGTH(cur_values);
    for (int i = 0; i < count; ++i)
    {
        int result = -2;
        double clamped = rate_limit(prev_value, cur_values[i], dt, rate_min, rate_max, &result);
        EXPECT_EQ(clamped_expected[i], clamped);
        EXPECT_EQ(results_expected[i], result);

        double clamped_no_result = rate_limit(prev_value, cur_values[i], dt, rate_min, rate_max);
        EXPECT_EQ(clamped, clamped_no_result);
    }

    {
        // Check initialization
        int result = -2;
        EXPECT_EQ(prev_value, rate_limit(NAN, prev_value, dt, rate_min, rate_max, &result));
        EXPECT_EQ(0, result);
    }

    {
        // Check bad values
        int result = -2;
        EXPECT_TRUE(std::isnan(rate_limit(prev_value, NAN, dt, rate_min, rate_max, &result)));
        EXPECT_EQ(0, result);
    }
}

TEST(limits_test, RateLimiter)
{
    double prev_value = 1.;
    double cur_values[] = {-1., 0., 1., 3.};
    double dt = 0.005;
    double rate_min = -100;
    double rate_max = 200;
    double clamped_expected[] = {0.5, 0.5, 1., 2.};
    int results_expected[] = {-1, -1, 0, 1};

    RateLimiter limiter(rate_min, rate_max);
    StampedRateLimiter stamped(rate_min, rate_max);
    EXPECT_TRUE(std::isnan(limiter.getPreviousValue()));
    EXPECT_TRUE(std::isnan(limiter.getPreviousRateSaturation()));
    EXPECT_FALSE(limiter.wasSaturated());

    ros::Time dummy_dt;
    dummy_dt.fromSec(dt); // From zero

    int count = LENGTH(cur_values);
    for (int i = 0; i < count; ++i)
    {
        limiter.reset(prev_value);
        stamped.reset(prev_value);
        double clamped = limiter.update(dt, cur_values[i]);

        EXPECT_EQ(clamped_expected[i], clamped);
        // Check state
        EXPECT_EQ(clamped, limiter.getPreviousValue());

        int result_expected = results_expected[i];
        EXPECT_EQ(result_expected, limiter.wasSaturated());
        double rate_saturation = limiter.getPreviousRateSaturation();
        if (result_expected == -1)
            EXPECT_EQ(rate_min, rate_saturation);
        if (result_expected == 1)
            EXPECT_EQ(rate_max, rate_saturation);

        double stamped_clamped = stamped.update(dummy_dt, cur_values[i]);
        EXPECT_EQ(clamped, stamped_clamped);
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
