# Walker-Vose Sampler

A header-only C++ library implementing the Walker-Vose alias method for efficient weighted random sampling.

## Features

- **Header-only**: No compilation required, just include and use
- **Generic templates**: Works with any numeric type and random number generator
- **O(1) sampling**: Constant time sampling after O(n) preprocessing
- **Modern C++**: Written in C++17 with clean, idiomatic code
- **Thread-safe**: Const operations are thread-safe
- **Comprehensive testing**: Full unit test coverage with Google Test

## Quick Start

```cpp
#include <walker_vose.hpp>
#include <vector>
#include <iostream>

int main() {
    // Define weights for sampling
    std::vector<double> weights = {1.0, 2.0, 3.0, 4.0};
    
    // Create the sampler
    walker_vose::WalkerVoseSampler<> sampler(weights);
    
    // Sample indices
    for (int i = 0; i < 10; ++i) {
        std::cout << "Sampled index: " << sampler.sample() << std::endl;
    }
    
    return 0;
}
```

## Installation

This is a header-only library. Simply copy the `include` directory to your project or install it system-wide:

```bash
# Clone the repository
git clone https://github.com/hfgong/walker_vose.git

# Copy header to your project
cp walker_vose/include/walker_vose.hpp /path/to/your/project/include/

# Or install system-wide (Linux/macOS)
sudo cp walker_vose/include/walker_vose.hpp /usr/local/include/
```

## Building Tests

Tests require Google Test. To build and run tests:

```bash
mkdir build && cd build
cmake ..
make
./tests/test_walker_vose
```

## Usage Examples

### Basic Usage

```cpp
#include <walker_vose.hpp>
#include <vector>

using namespace walker_vose;

// Create weights
std::vector<double> weights = {10, 20, 30, 40};

// Create and use sampler
WalkerVoseSampler<> sampler(weights);

// Sample with default thread-local RNG
size_t index = sampler.sample();

// Sample with custom RNG
std::mt19937 gen(12345);
size_t index2 = sampler(gen);
```

### Custom Types and RNG

```cpp
// Use float precision
std::vector<float> weights = {0.1f, 0.2f, 0.3f, 0.4f};
WalkerVoseSampler<float> float_sampler(weights);

// Use custom random number generator
WalkerVoseSampler<double, std::minstd_rand> custom_rng_sampler(weights);
```

### Dynamic Weight Updates

```cpp
WalkerVoseSampler<> sampler;

// Initialize with first distribution
std::vector<double> morning_weights = {0.7, 0.2, 0.1};  // coffee, tea, juice
sampler.initialize(morning_weights.begin(), morning_weights.end());

// Later, switch to evening distribution
sampler.clear();
std::vector<double> evening_weights = {0.1, 0.3, 0.6};  // coffee, tea, juice
sampler.initialize(evening_weights.begin(), evening_weights.end());
```

### Practical Example: Weighted Random Selection

```cpp
#include <walker_vose.hpp>
#include <string>
#include <map>

// Weighted random selection of items
std::map<std::string, double> item_weights = {
    {"common", 70.0},
    {"uncommon", 20.0},
    {"rare", 8.0},
    {"legendary", 2.0}
};

// Extract weights
std::vector<double> weights;
std::vector<std::string> items;
for (const auto& [item, weight] : item_weights) {
    items.push_back(item);
    weights.push_back(weight);
}

// Create sampler
walker_vose::WalkerVoseSampler<> sampler(weights);

// Generate loot
for (int i = 0; i < 10; ++i) {
    size_t index = sampler.sample();
    std::cout << "You found: " << items[index] << std::endl;
}
```

## API Reference

See [docs/API.md](docs/API.md) for detailed API documentation.

## Algorithm Details

The Walker-Vose alias method creates two tables during preprocessing:
- **Probability table**: Stores the probability of selecting each element directly
- **Alias table**: Stores alternative elements for rejection cases

This allows constant-time sampling by:
1. Randomly selecting an index
2. Generating a uniform random number
3. Comparing with the probability table to decide between the original or alias element

## Performance

- **Preprocessing**: O(n) time and space
- **Sampling**: O(1) time
- **Memory**: 2n values (probability and alias tables)

## Requirements

- C++17 or later
- Standard library with `<random>` support
- Google Test (for building tests only)

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## References

- Walker, A. J. (1977). "An Efficient Method for Generating Discrete Random Variables with General Distributions"
- Vose, M. D. (1991). "A Linear Algorithm for Generating Random Numbers with a Given Distribution"