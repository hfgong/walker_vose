#ifndef WALKER_VOSE_HPP
#define WALKER_VOSE_HPP

#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>

namespace walker_vose {

template<typename T = double, typename RNG = std::mt19937>
class WalkerVoseSampler {
public:
    using value_type = T;
    using size_type = std::size_t;
    
    WalkerVoseSampler() = default;
    
    template<typename Iterator>
    WalkerVoseSampler(Iterator first, Iterator last) {
        initialize(first, last);
    }
    
    template<typename Container>
    explicit WalkerVoseSampler(const Container& weights) {
        initialize(weights.begin(), weights.end());
    }
    
    template<typename Iterator>
    void initialize(Iterator first, Iterator last) {
        size_type n = std::distance(first, last);
        if (n == 0) {
            throw std::invalid_argument("Cannot initialize with empty weights");
        }
        
        std::vector<T> weights(first, last);
        
        T sum = std::accumulate(weights.begin(), weights.end(), T(0));
        if (sum <= 0) {
            throw std::invalid_argument("Sum of weights must be positive");
        }
        
        for (auto& w : weights) {
            if (w < 0) {
                throw std::invalid_argument("Weights must be non-negative");
            }
            w = w * n / sum;
        }
        
        prob_.resize(n);
        alias_.resize(n);
        
        std::vector<size_type> small, large;
        small.reserve(n);
        large.reserve(n);
        
        for (size_type i = 0; i < n; ++i) {
            if (weights[i] < T(1)) {
                small.push_back(i);
            } else {
                large.push_back(i);
            }
        }
        
        while (!small.empty() && !large.empty()) {
            size_type l = small.back();
            small.pop_back();
            size_type g = large.back();
            large.pop_back();
            
            prob_[l] = weights[l];
            alias_[l] = g;
            
            weights[g] = weights[g] + weights[l] - T(1);
            
            if (weights[g] < T(1)) {
                small.push_back(g);
            } else {
                large.push_back(g);
            }
        }
        
        while (!large.empty()) {
            size_type g = large.back();
            large.pop_back();
            prob_[g] = T(1);
        }
        
        while (!small.empty()) {
            size_type l = small.back();
            small.pop_back();
            prob_[l] = T(1);
        }
    }
    
    template<typename Generator>
    size_type operator()(Generator& gen) const {
        if (prob_.empty()) {
            throw std::runtime_error("Sampler not initialized");
        }
        
        std::uniform_int_distribution<size_type> index_dist(0, prob_.size() - 1);
        std::uniform_real_distribution<T> prob_dist(0, 1);
        
        size_type i = index_dist(gen);
        T u = prob_dist(gen);
        
        return (u < prob_[i]) ? i : alias_[i];
    }
    
    size_type sample(RNG& gen) const {
        return (*this)(gen);
    }
    
    size_type sample() const {
        static thread_local RNG gen(std::random_device{}());
        return (*this)(gen);
    }
    
    bool empty() const {
        return prob_.empty();
    }
    
    size_type size() const {
        return prob_.size();
    }
    
    void clear() {
        prob_.clear();
        alias_.clear();
    }
    
private:
    std::vector<T> prob_;
    std::vector<size_type> alias_;
};

template<typename T = double>
using Sampler = WalkerVoseSampler<T>;

} // namespace walker_vose

#endif // WALKER_VOSE_HPP