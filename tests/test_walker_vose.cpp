#include <gtest/gtest.h>
#include <walker_vose.hpp>
#include <random>
#include <map>
#include <cmath>

using namespace walker_vose;

class WalkerVoseTest : public ::testing::Test {
protected:
    std::mt19937 gen{42};
};

TEST_F(WalkerVoseTest, ConstructorDefault) {
    WalkerVoseSampler<> sampler;
    EXPECT_TRUE(sampler.empty());
    EXPECT_EQ(sampler.size(), 0);
}

TEST_F(WalkerVoseTest, ConstructorWithIterators) {
    std::vector<double> weights = {1.0, 2.0, 3.0, 4.0};
    WalkerVoseSampler<> sampler(weights.begin(), weights.end());
    EXPECT_FALSE(sampler.empty());
    EXPECT_EQ(sampler.size(), 4);
}

TEST_F(WalkerVoseTest, ConstructorWithContainer) {
    std::vector<double> weights = {1.0, 2.0, 3.0, 4.0};
    WalkerVoseSampler<> sampler(weights);
    EXPECT_FALSE(sampler.empty());
    EXPECT_EQ(sampler.size(), 4);
}

TEST_F(WalkerVoseTest, InitializeWithEmptyWeights) {
    WalkerVoseSampler<> sampler;
    std::vector<double> weights;
    EXPECT_THROW(sampler.initialize(weights.begin(), weights.end()), std::invalid_argument);
}

TEST_F(WalkerVoseTest, InitializeWithNegativeWeights) {
    WalkerVoseSampler<> sampler;
    std::vector<double> weights = {1.0, -2.0, 3.0};
    EXPECT_THROW(sampler.initialize(weights.begin(), weights.end()), std::invalid_argument);
}

TEST_F(WalkerVoseTest, InitializeWithZeroSum) {
    WalkerVoseSampler<> sampler;
    std::vector<double> weights = {0.0, 0.0, 0.0};
    EXPECT_THROW(sampler.initialize(weights.begin(), weights.end()), std::invalid_argument);
}

TEST_F(WalkerVoseTest, SampleFromUninitialized) {
    WalkerVoseSampler<> sampler;
    EXPECT_THROW(sampler(gen), std::runtime_error);
}

TEST_F(WalkerVoseTest, UniformDistribution) {
    std::vector<double> weights = {1.0, 1.0, 1.0, 1.0};
    WalkerVoseSampler<> sampler(weights);
    
    const int num_samples = 100000;
    std::map<size_t, int> counts;
    
    for (int i = 0; i < num_samples; ++i) {
        counts[sampler(gen)]++;
    }
    
    double expected_prob = 1.0 / weights.size();
    double tolerance = 0.01;
    
    for (const auto& [index, count] : counts) {
        double actual_prob = static_cast<double>(count) / num_samples;
        EXPECT_NEAR(actual_prob, expected_prob, tolerance);
    }
}

TEST_F(WalkerVoseTest, NonUniformDistribution) {
    std::vector<double> weights = {1.0, 2.0, 3.0, 4.0};
    WalkerVoseSampler<> sampler(weights);
    
    const int num_samples = 100000;
    std::map<size_t, int> counts;
    
    for (int i = 0; i < num_samples; ++i) {
        counts[sampler(gen)]++;
    }
    
    double total_weight = 10.0;
    double tolerance = 0.01;
    
    for (size_t i = 0; i < weights.size(); ++i) {
        double expected_prob = weights[i] / total_weight;
        double actual_prob = static_cast<double>(counts[i]) / num_samples;
        EXPECT_NEAR(actual_prob, expected_prob, tolerance);
    }
}

TEST_F(WalkerVoseTest, SingleElement) {
    std::vector<double> weights = {5.0};
    WalkerVoseSampler<> sampler(weights);
    
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(sampler(gen), 0);
    }
}

TEST_F(WalkerVoseTest, LargeWeightDifferences) {
    std::vector<double> weights = {0.001, 1000.0, 0.001};
    WalkerVoseSampler<> sampler(weights);
    
    const int num_samples = 100000;
    std::map<size_t, int> counts;
    
    for (int i = 0; i < num_samples; ++i) {
        counts[sampler(gen)]++;
    }
    
    EXPECT_GT(counts[1], num_samples * 0.99);
    EXPECT_LT(counts[0] + counts[2], num_samples * 0.01);
}

TEST_F(WalkerVoseTest, FloatType) {
    std::vector<float> weights = {1.0f, 2.0f, 3.0f};
    WalkerVoseSampler<float> sampler(weights);
    
    size_t index = sampler(gen);
    EXPECT_LT(index, weights.size());
}

TEST_F(WalkerVoseTest, SampleMethods) {
    std::vector<double> weights = {1.0, 2.0, 3.0};
    WalkerVoseSampler<> sampler(weights);
    
    size_t index1 = sampler.sample(gen);
    EXPECT_LT(index1, weights.size());
    
    size_t index2 = sampler.sample();
    EXPECT_LT(index2, weights.size());
}

TEST_F(WalkerVoseTest, ClearAndReinitialize) {
    std::vector<double> weights1 = {1.0, 2.0, 3.0};
    WalkerVoseSampler<> sampler(weights1);
    EXPECT_EQ(sampler.size(), 3);
    
    sampler.clear();
    EXPECT_TRUE(sampler.empty());
    EXPECT_EQ(sampler.size(), 0);
    
    std::vector<double> weights2 = {4.0, 5.0};
    sampler.initialize(weights2.begin(), weights2.end());
    EXPECT_EQ(sampler.size(), 2);
}

TEST_F(WalkerVoseTest, TypeAlias) {
    std::vector<double> weights = {1.0, 2.0};
    Sampler<> sampler(weights);
    
    size_t index = sampler(gen);
    EXPECT_LT(index, weights.size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}