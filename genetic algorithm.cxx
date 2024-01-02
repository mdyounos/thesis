#include<bits/stdc++.h>
using namespace std;

const int MAX_WEIGHT = 15; // Maximum weight capacity of the knapsack
const int POPULATION_SIZE = 50; // Population size
const int MAX_GENERATIONS = 100; // Maximum number of generations
const float MUTATION_RATE = 0.1; // Mutation rate

// Structure to represent an item
struct Item {
    int weight;
    int value;
};

// Generate random items (for demonstration)
vector<Item> generateItems(int numItems) {
    vector<Item> items;
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < numItems; ++i) {
        Item newItem;
        newItem.weight = rand() % 10 + 1; // Random weight between 1 and 10
        newItem.value = rand() % 20 + 1;  // Random value between 1 and 20
        items.push_back(newItem);
    }
    return items;
}

// Fitness function: Evaluate fitness of an individual (chromosome)
int evaluateFitness(const vector<int>& chromosome, const vector<Item>& items) {
    int totalWeight = 0;
    int totalValue = 0;

    for (unsigned int i = 0; i < chromosome.size(); ++i) {
        if (chromosome[i] == 1) {
            totalWeight += items[i].weight;
            totalValue += items[i].value;
        }
    }

    // Penalize if weight exceeds the maximum allowed weight
    if (totalWeight > MAX_WEIGHT) {
        totalValue = 0;
    }

    return totalValue;
}

// Genetic Algorithm
vector<int> geneticAlgorithm(const vector<Item>& items) {
    vector<vector<int>> population(POPULATION_SIZE, vector<int>(items.size()));

    // Initialize population randomly (table)
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        for (unsigned int j = 0; j < items.size(); ++j) {
            population[i][j] = rand() % 2; // 0 or 1
        }
    }

    int generation = 0;
    while (generation < MAX_GENERATIONS) {
        // Evaluate fitness of each individual in the population
        vector<pair<int, int>> fitnessValues; // (Fitness, Index) pairs
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            int fitness = evaluateFitness(population[i], items);
            fitnessValues.push_back({fitness, i});
        }

        // Sort by fitness in descending order
        sort(fitnessValues.rbegin(), fitnessValues.rend());

        // Select top individuals for reproduction (tournament selection)
        vector<vector<int>> newPopulation;
        for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
            int idx1 = fitnessValues[i].second;
            int idx2 = fitnessValues[i + 1].second;

            // Perform crossover
            int crossoverPoint = rand() % items.size();
            vector<int> child1, child2;
            child1.insert(child1.end(), population[idx1].begin(), population[idx1].begin() + crossoverPoint);
            child1.insert(child1.end(), population[idx2].begin() + crossoverPoint, population[idx2].end());
            child2.insert(child2.end(), population[idx2].begin(), population[idx2].begin() + crossoverPoint);
            child2.insert(child2.end(), population[idx1].begin() + crossoverPoint, population[idx1].end());

            // Perform mutation
            for (int j = 0; j < items.size(); ++j) {
                if ((float)rand() / RAND_MAX < MUTATION_RATE) {
                    child1[j] = 1 - child1[j]; // Flip bit
                }
                if ((float)rand() / RAND_MAX < MUTATION_RATE) {
                    child2[j] = 1 - child2[j]; // Flip bit
                }
            }

            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }

        // Replace the old population with the new population
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            population[i] = newPopulation[i];
        }

        ++generation;
    }

    // Select the best individual from the final population
    int bestFitness = 0;
    int bestIdx = -1;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        int fitness = evaluateFitness(population[i], items);
        if (fitness > bestFitness) {
            bestFitness = fitness;
            bestIdx = i;
        }
    }

    // Return the best individual's chromosome
    return population[bestIdx];
}

int main() {
    // Generate random items for the knapsack problem
    vector<Item> items = generateItems(10); // Generating 10 random items

    cout << "Weight of knapsack: "<<MAX_WEIGHT<<endl;
    cout << "Value of selected items: ";
    for (unsigned int i = 0; i < items.size(); ++i) {
        //if (solution[i] == 1) {
            cout << items[i].value<< " ";
        //}
    }
    cout << endl;


    cout << "Weight of selected items: ";
    for (unsigned int i = 0; i < items.size(); ++i) {
        //if (solution[i] == 1) {
            cout << items[i].weight<< " ";
        //}
    }
    cout << endl;


    // Solve the knapsack problem using genetic algorithm
    vector<int> solution = geneticAlgorithm(items);

    // Display the selected items in the knapsack
    cout << "Selected items:           ";
    for (unsigned int i = 0; i < solution.size(); ++i) {
       // if (solution[i] == 1) {
            cout << solution[i] << " ";
        //}
    }
    cout << endl;


    cout << "Selected items: ";
    for (unsigned int i = 0; i < solution.size(); ++i) {
        if (solution[i] == 1) {
            cout << i << " ";
        }
    }
    cout << endl;

    return 0;
}

