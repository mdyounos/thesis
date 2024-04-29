#include<bits/stdc++.h>
using namespace std;

//Constant declration
const int MAX_VERTICES = 100;
const int POPULATION_SIZE = 50;
const int MAX_GENERATIONS = 100;
const double MUTATION_RATE = 0.2;

class GCGA {
private:
    int vertices;
    vector<vector<int>> adjacencyMatrix;

    //For a single row
    struct Chromosome {
        vector<int> genes;
        int fitness;

        Chromosome(){
            fitness = 0;
        }
    };

    vector<Chromosome> population;

public:
    GCGA(int v): vertices(v) {
    adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
};

    void addEdge(int v, int w){
    adjacencyMatrix[v][w] = 1;
    adjacencyMatrix[w][v] = 1;
    }

    void initializePopulation();
    void calculateFitness(Chromosome &chromosome);
    void crossover();
    void mutate(Chromosome &chromosome);
    void evolve();
    void printSolution(const Chromosome &chromosome);
};


void GCGA::initializePopulation() {
    //change its size automatically
    population.resize(POPULATION_SIZE);

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].genes.resize(vertices);

        for (int j = 0; j < vertices; ++j) {
            population[i].genes[j] = rand() % vertices;
        }

        calculateFitness(population[i]);
    }
}

void GCGA::calculateFitness(Chromosome &chromosome) {
    chromosome.fitness = 0;

    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (adjacencyMatrix[i][j] && chromosome.genes[i] == chromosome.genes[j]) {
                // If two adjacent vertices have the same color, increase fitness
                chromosome.fitness++;
            }
        }
    }
}

void GCGA::crossover() {
    // Perform one-point crossover
    for (int i = 0; i < POPULATION_SIZE; i += 2) {
        int crossoverPoint = rand() % vertices;

        for (int j = crossoverPoint; j < vertices; ++j) {
            swap(population[i].genes[j], population[i + 1].genes[j]);
        }

        calculateFitness(population[i]);
        calculateFitness(population[i + 1]);
    }
}

void GCGA::mutate(Chromosome &chromosome) {
    // Perform mutation with a certain probability for each gene
    for (int i = 0; i < vertices; ++i) {
        if ((rand() / static_cast<double>(RAND_MAX)) < MUTATION_RATE) {
            chromosome.genes[i] = rand() % vertices;
        }
    }

    calculateFitness(chromosome);
}

void GCGA::evolve() {
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        // Sort the population based on fitness
        sort(population.begin(), population.end(), [](const Chromosome &a, const Chromosome &b) {
            return a.fitness > b.fitness;
        });

        // Print the best solution of each generation
        cout << "Generation " << generation + 1 << ", Best Fitness: " << population[0].fitness << endl;
        printSolution(population[0]);

        // Perform crossover and mutation
        crossover();
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            mutate(population[i]);
        }
    }
}

void GCGA::printSolution(const Chromosome &chromosome) {
    cout << "Vertex\tColor\n";
    for (int i = 0; i < vertices; ++i) {
        cout << i << "\t" << chromosome.genes[i] << endl;
    }
    cout << endl;
}

int main() {
    srand(static_cast<int>(time(nullptr)));

    // Example usage
    GCGA graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(3, 4);

    cout << "Graph Coloring using Genetic Algorithm:\n";
    graph.initializePopulation();
    graph.evolve();

    return 0;
}
