#include<bits/stdc++.h>
using namespace std;

// Define the graph structure
struct Graph{
    int numVertices;
    vector<vector<int>> adjacencyMatrix;
};

// Define the particle structure
struct Particle {
    vector<int> position; // Binary encoding of colors for each vertex
    vector<int> velocity;
    vector<int> personalBest; // Personal best position found by the particle
    int fitness; // Fitness value of the personal best position

    Particle(int numVertices, int numColors) {
        position.resize(numVertices);
        velocity.resize(numVertices);
        personalBest.resize(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            position[i] = rand() % numColors; // Random initialization of colors
            velocity[i] = rand() % 2; // Random initialization of velocity
            personalBest[i] = position[i];
        }
        fitness = INT_MAX; // Initialize fitness to maximum
    }
};

// Fitness function: Evaluates the quality of a coloring
int fitnessFunction(const Graph& graph, const vector<int>& coloring) {
    int conflicts = 0;
    for (int i = 0; i < graph.numVertices; ++i) {
        for (int j = i + 1; j < graph.numVertices; ++j) {
            if (graph.adjacencyMatrix[i][j] && coloring[i] == coloring[j]) {
                conflicts++;
            }
        }
    }
    return conflicts;
}

// Update particle velocity
void updateVelocity(Particle& particle, const vector<int>& globalBest) {
    // Define inertia weight and acceleration constants
    double inertiaWeight = 0.9;
    double cognitiveAcceleration = 2.0;
    double socialAcceleration = 2.0;

    // Update velocity for each dimension
    for (size_t i = 0; i < particle.velocity.size(); ++i) {
        // Calculate cognitive and social components
        double cognitiveComponent = cognitiveAcceleration * (rand() / (RAND_MAX + 1.0)) * (particle.personalBest[i] - particle.position[i]);
        double socialComponent = socialAcceleration * (rand() / (RAND_MAX + 1.0)) * (globalBest[i] - particle.position[i]);

        // Update velocity
        particle.velocity[i] = inertiaWeight * particle.velocity[i] + cognitiveComponent + socialComponent;

        // Ensure velocity remains within range (0 or 1 for binary representation)
        //particle.velocity[i] = max(0.0, min(1.0, particle.velocity[i]));
        particle.velocity[i] = max(0, min(1, particle.velocity[i])); // Ensure velocity remains either 0 or 1

    }
}

// Update particle position
void updatePosition(Particle& particle) {
    // Update position for each dimension
    for (size_t i = 0; i < particle.position.size(); ++i) {
        // Update position based on velocity (binary representation)
        if ((rand() / (RAND_MAX + 1.0)) < particle.velocity[i]) {
            particle.position[i] = 1 - particle.position[i]; // Flip the bit with a probability based on velocity
        }
    }
}


// BPSO algorithm
void BPSO(Graph& graph, int numColors, int numParticles, int maxIterations) {
    vector<Particle> particles;

    // Initialize particles
    for (int i = 0; i < numParticles; ++i) {
        particles.push_back(Particle(graph.numVertices, numColors));
    }

    vector<int> globalBestPosition(graph.numVertices);
    int globalBestFitness = INT_MAX;

    // Main loop
    for (int iter = 0; iter < maxIterations; ++iter) {
        // Update personal and global best
        for (auto& particle : particles) {
            particle.fitness = fitnessFunction(graph, particle.position);
            if (particle.fitness < fitnessFunction(graph, particle.personalBest)) {
                particle.personalBest = particle.position;
            }
            if (particle.fitness < globalBestFitness) {
                globalBestPosition = particle.position;
                globalBestFitness = particle.fitness;
            }
        }

        // Update velocities and positions
        for (auto& particle : particles) {
            updateVelocity(particle, globalBestPosition);
            updatePosition(particle);
        }

        // Output intermediate results (optional)
        cout << "Iteration: " << iter + 1 << ", Fitness: " << globalBestFitness << endl;
    }

    // Output the best solution found
    cout << "Best coloring found: ";
    for(int i = 0; i < graph.numVertices; ++i) {
        cout << globalBestPosition[i] << " ";
    }
    cout << endl;
    cout << "Fitness: " << globalBestFitness << endl;
}

int main() {
    // Define a simple graph (adjacency matrix representation)
    Graph graph;
    graph.numVertices = 5;
    graph.adjacencyMatrix = {
        {0, 1, 1, 0, 1},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1},
        {1, 0, 0, 1, 0}
    };

    int numColors = 3; // Number of colors available
    int numParticles = 50; // Number of particles in the swarm
    int maxIterations = 100; // Maximum number of iterations

    // Run BPSO algorithm
    BPSO(graph, numColors, numParticles, maxIterations);

    return 0;
}
