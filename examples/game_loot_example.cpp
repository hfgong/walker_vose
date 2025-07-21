#include <walker_vose.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>

struct LootItem {
    std::string name;
    std::string rarity;
    double weight;
};

int main() {
    std::cout << "Game Loot Drop Simulation using Walker-Vose Sampler\n";
    std::cout << "==================================================\n\n";
    
    // Define loot table with weights
    std::vector<LootItem> loot_table = {
        {"Rusty Sword", "Common", 50.0},
        {"Health Potion", "Common", 30.0},
        {"Iron Shield", "Uncommon", 15.0},
        {"Magic Ring", "Rare", 4.0},
        {"Dragon Scale", "Legendary", 1.0}
    };
    
    // Extract weights
    std::vector<double> weights;
    for (const auto& item : loot_table) {
        weights.push_back(item.weight);
    }
    
    // Create sampler
    walker_vose::WalkerVoseSampler<> sampler(weights);
    
    // Display loot table
    std::cout << "Loot Table:\n";
    std::cout << std::setw(20) << "Item" 
              << std::setw(15) << "Rarity" 
              << std::setw(10) << "Weight" 
              << std::setw(15) << "Drop Chance\n";
    std::cout << std::string(60, '-') << "\n";
    
    double total_weight = 100.0;
    for (const auto& item : loot_table) {
        std::cout << std::setw(20) << item.name
                  << std::setw(15) << item.rarity
                  << std::setw(10) << item.weight
                  << std::setw(14) << std::fixed << std::setprecision(2) 
                  << (item.weight / total_weight * 100) << "%\n";
    }
    
    // Simulate opening treasure chests
    std::cout << "\n\nOpening 20 treasure chests:\n";
    std::cout << std::string(40, '-') << "\n";
    
    std::map<std::string, int> loot_counts;
    
    for (int chest = 1; chest <= 20; ++chest) {
        size_t index = sampler.sample();
        const auto& item = loot_table[index];
        loot_counts[item.name]++;
        
        std::cout << "Chest " << std::setw(2) << chest << ": " 
                  << item.name << " (" << item.rarity << ")\n";
    }
    
    // Show summary statistics
    std::cout << "\n\nLoot Summary (1000 chests simulation):\n";
    std::cout << std::string(50, '-') << "\n";
    
    loot_counts.clear();
    const int num_simulations = 1000;
    
    for (int i = 0; i < num_simulations; ++i) {
        size_t index = sampler.sample();
        loot_counts[loot_table[index].name]++;
    }
    
    std::cout << std::setw(20) << "Item" 
              << std::setw(10) << "Count" 
              << std::setw(10) << "Actual %" 
              << std::setw(12) << "Expected %\n";
    
    for (size_t i = 0; i < loot_table.size(); ++i) {
        const auto& item = loot_table[i];
        int count = loot_counts[item.name];
        double actual_percent = (count * 100.0) / num_simulations;
        double expected_percent = item.weight / total_weight * 100;
        
        std::cout << std::setw(20) << item.name
                  << std::setw(10) << count
                  << std::setw(9) << std::fixed << std::setprecision(1) << actual_percent << "%"
                  << std::setw(11) << expected_percent << "%\n";
    }
    
    return 0;
}