#include <bits/stdc++.h>
using namespace std;

const int MAX_VERTICES = 100;
const int POPULATION_SIZE = 50;
const int MAX_ITERATIONS = 100;
const double INERTIA_WEIGHT = 0.7;
const double C1 = 1.5; // Cognitive coefficient
const double C2 = 1.5; // Social coefficient
const double THRESHOLD = 0.5;

struct Particle {
    vector<int> position; // Binary representation (0 or 1)
    vector<double> velocity;
    vector<int> personal_best;
    int personal_best_fitness;
    int fitness; // Add fitness member here

    Particle() {
        personal_best_fitness = INT_MAX;
        fitness = INT_MAX; // Initialize fitness to a high value
    }
};


void initializeSwarm() {
    swarm.resize(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        swarm[i].position.resize(vertices);
        swarm[i].velocity.resize(vertices);

        // Initialize position and velocity
        for (int j = 0; j < vertices; ++j) {
            // Random binary position
            swarm[i].position[j] = rand() % 2;

            // Random velocity between -1 and 1
            swarm[i].velocity[j] = (rand() / static_cast<double>(RAND_MAX)) * 2 - 1;
        }

        calculateFitness(swarm[i]);
        // Initialize fitness
        swarm[i].fitness = swarm[i].personal_best_fitness;
        // Update personal best
        swarm[i].personal_best = swarm[i].position;
        swarm[i].personal_best_fitness = swarm[i].fitness;

        // Update global best
        if (swarm[i].personal_best_fitness < global_best_fitness) {
            global_best = swarm[i].personal_best;
            global_best_fitness = swarm[i].personal_best_fitness;
        }
    }
}



class BPSO {
private:
    int vertices;
    vector<vector<int>> adjacencyMatrix;

    struct Particle {
        vector<int> position; // Binary representation (0 or 1)
        vector<double> velocity;
        vector<int> personal_best;
        int personal_best_fitness;

        Particle() {
            personal_best_fitness = INT_MAX;
        }
    };

    vector<Particle> swarm;
    vector<int> global_best;
    int global_best_fitness;

public:
    BPSO(int v): vertices(v) {
        adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
        global_best_fitness = INT_MAX;
    }

    void addEdge(int v, int w) {
        adjacencyMatrix[v][w] = 1;
        adjacencyMatrix[w][v] = 1;
    }

    void initializeSwarm() {
        swarm.resize(POPULATION_SIZE);
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            swarm[i].position.resize(vertices);
            swarm[i].velocity.resize(vertices);

            // Initialize position and velocity
            for (int j = 0; j < vertices; ++j) {
                // Random binary position
                swarm[i].position[j] = rand() % 2;

                // Random velocity between -1 and 1
                swarm[i].velocity[j] = (rand() / static_cast<double>(RAND_MAX)) * 2 - 1;
            }

            calculateFitness(swarm[i]);
            // Update personal best
            swarm[i].personal_best = swarm[i].position;
            swarm[i].personal_best_fitness = swarm[i].fitness;

            // Update global best
            if (swarm[i].personal_best_fitness < global_best_fitness) {
                global_best = swarm[i].personal_best;
                global_best_fitness = swarm[i].personal_best_fitness;
            }
        }
    }

    void calculateFitness(Particle &particle) {
        particle.fitness = 0;

        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                if (adjacencyMatrix[i][j] && particle.position[i] == particle.position[j]) {
                    // If two adjacent vertices have the same color, increase fitness
                    particle.fitness++;
                }
            }
        }
    }

    void updateVelocityAndPosition(Particle &particle) {
        for (int i = 0; i < vertices; ++i) {
            // Update velocity using the BPSO formula
            double r1 = static_cast<double>(rand()) / RAND_MAX;
            double r2 = static_cast<double>(rand()) / RAND_MAX;

            particle.velocity[i] = INERTIA_WEIGHT * particle.velocity[i]
                                + C1 * r1 * (particle.personal_best[i] - particle.position[i])
                                + C2 * r2 * (global_best[i] - particle.position[i]);

            // Update position using sigmoid function and threshold
            double sigmoid = 1 / (1 + exp(-particle.velocity[i]));
            if (sigmoid > THRESHOLD) {
                particle.position[i] = 1;
            } else {
                particle.position[i] = 0;
            }
        }
    }

    void optimize() {
        for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
            // Update the swarm
            for (int i = 0; i < POPULATION_SIZE; ++i) {
                updateVelocityAndPosition(swarm[i]);
                calculateFitness(swarm[i]);

                // Update personal best
                if (swarm[i].fitness < swarm[i].personal_best_fitness) {
                    swarm[i].personal_best = swarm[i].position;
                    swarm[i].personal_best_fitness = swarm[i].fitness;

                    // Update global best
                    if (swarm[i].personal_best_fitness < global_best_fitness) {
                        global_best = swarm[i].personal_best;
                        global_best_fitness = swarm[i].personal_best_fitness;
                    }
                }
            }

            // Print the best solution of each iteration
            cout << "Iteration " << iteration + 1 << ", Best Fitness: " << global_best_fitness << endl;
            printSolution(global_best);
        }
    }

    void printSolution(const vector<int> &solution) {
        cout << "Vertex\tColor\n";
        for (int i = 0; i < vertices; ++i) {
            cout << i << "\t" << solution[i] << endl;
        }
        cout << endl;
    }
};

int main() {
    srand(static_cast<int>(time(nullptr)));

    // Example usage
    BPSO graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(3, 4);

    cout << "Graph Coloring using Binary Particle Swarm Optimization:\n";
    graph.initializeSwarm();
    graph.optimize();

    return 0;
}
