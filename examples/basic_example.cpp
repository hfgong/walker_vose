#include <walker_vose.hpp>
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>

int main() {
    std::cout << "Walker-Vose Sampler Example\n";
    std::cout << "==========================\n\n";
    
    // Example 1: Basic usage
    std::cout << "Example 1: Basic weighted sampling\n";
    std::vector<double> weights = {1.0, 2.0, 3.0, 4.0};
    walker_vose::WalkerVoseSampler<> sampler(weights);
    
    std::cout << "Weights: ";
    for (double w : weights) {
        std::cout << w << " ";
    }
    std::cout << "\n10 samples: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << sampler.sample() << " ";
    }
    std::cout << "\n\n";
    
    // Example 2: Frequency distribution
    std::cout << "Example 2: Sampling distribution (10,000 samples)\n";
    std::map<size_t, int> counts;
    const int num_samples = 10000;
    
    for (int i = 0; i < num_samples; ++i) {
        counts[sampler.sample()]++;
    }
    
    double total_weight = 10.0;
    for (size_t i = 0; i < weights.size(); ++i) {
        double expected = weights[i] / total_weight;
        double actual = static_cast<double>(counts[i]) / num_samples;
        std::cout << "Index " << i << ": "
                  << "expected=" << std::fixed << std::setprecision(3) << expected
                  << ", actual=" << actual
                  << ", count=" << counts[i] << "\n";
    }
    std::cout << "\n";
    
    // Example 3: Custom RNG
    std::cout << "Example 3: Using custom random number generator\n";
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::cout << "5 samples with fixed seed: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << sampler(gen) << " ";
    }
    std::cout << "\n\n";
    
    // Example 4: Different types
    std::cout << "Example 4: Using float type\n";
    std::vector<float> float_weights = {0.1f, 0.2f, 0.3f, 0.4f};
    walker_vose::WalkerVoseSampler<float> float_sampler(float_weights);
    std::cout << "5 samples: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << float_sampler.sample() << " ";
    }
    std::cout << "\n";
    
    return 0;
}