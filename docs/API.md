# Walker-Vose Sampler API Documentation

## Overview

The Walker-Vose Sampler is a header-only C++ library that implements the Walker-Vose alias method for efficient weighted random sampling. This algorithm allows O(1) sampling after O(n) preprocessing time.

## Namespace

All classes and functions are contained within the `walker_vose` namespace.

## Classes

### `WalkerVoseSampler<T, RNG>`

The main template class that implements the Walker-Vose alias method.

#### Template Parameters

- `T` (default: `double`): The numeric type used for weights and probabilities. Must support basic arithmetic operations.
- `RNG` (default: `std::mt19937`): The random number generator type.

#### Type Aliases

```cpp
using value_type = T;
using size_type = std::size_t;
```

#### Constructors

##### Default Constructor
```cpp
WalkerVoseSampler();
```
Creates an empty sampler. Must be initialized before use.

##### Iterator Constructor
```cpp
template<typename Iterator>
WalkerVoseSampler(Iterator first, Iterator last);
```
Constructs and initializes the sampler with weights from the range `[first, last)`.

##### Container Constructor
```cpp
template<typename Container>
explicit WalkerVoseSampler(const Container& weights);
```
Constructs and initializes the sampler with weights from a container.

#### Member Functions

##### `initialize`
```cpp
template<typename Iterator>
void initialize(Iterator first, Iterator last);
```
Initializes or reinitializes the sampler with weights from the range `[first, last)`.

**Throws:**
- `std::invalid_argument` if the range is empty, contains negative weights, or sums to zero or less.

##### `operator()`
```cpp
template<typename Generator>
size_type operator()(Generator& gen) const;
```
Samples an index according to the weight distribution using the provided random number generator.

**Returns:** An index in the range `[0, size())`.

**Throws:**
- `std::runtime_error` if the sampler is not initialized.

##### `sample`
```cpp
size_type sample(RNG& gen) const;
```
Alternative interface for sampling with a specific generator instance.

```cpp
size_type sample() const;
```
Samples using a thread-local static generator instance.

##### `empty`
```cpp
bool empty() const;
```
Returns `true` if the sampler is not initialized or has been cleared.

##### `size`
```cpp
size_type size() const;
```
Returns the number of elements in the distribution.

##### `clear`
```cpp
void clear();
```
Clears the sampler, returning it to an uninitialized state.

### Type Aliases

```cpp
template<typename T = double>
using Sampler = WalkerVoseSampler<T>;
```
Convenience alias using the default random number generator.

## Example Usage

### Basic Usage
```cpp
#include <walker_vose.hpp>
#include <vector>
#include <random>

using namespace walker_vose;

// Create weights
std::vector<double> weights = {1.0, 2.0, 3.0, 4.0};

// Create sampler
WalkerVoseSampler<> sampler(weights);

// Sample with custom generator
std::mt19937 gen(42);
size_t index = sampler(gen);

// Sample with default generator
size_t index2 = sampler.sample();
```

### Custom Types
```cpp
// Using float precision
std::vector<float> weights = {0.1f, 0.3f, 0.6f};
WalkerVoseSampler<float> sampler(weights);

// Using custom RNG
std::vector<double> weights2 = {1.0, 2.0, 3.0};
WalkerVoseSampler<double, std::minstd_rand> sampler2(weights2);
```

### Reinitialization
```cpp
WalkerVoseSampler<> sampler;

// Initialize with first set of weights
std::vector<double> weights1 = {1.0, 1.0, 1.0};
sampler.initialize(weights1.begin(), weights1.end());

// Clear and reinitialize with different weights
sampler.clear();
std::vector<double> weights2 = {1.0, 2.0, 3.0, 4.0};
sampler.initialize(weights2.begin(), weights2.end());
```

## Performance Characteristics

- **Preprocessing**: O(n) time and space complexity
- **Sampling**: O(1) time complexity
- **Memory**: O(n) space for storing probability and alias tables

## Thread Safety

- Const member functions are thread-safe for concurrent access
- Non-const member functions require external synchronization
- The `sample()` method without parameters uses thread-local storage for the RNG