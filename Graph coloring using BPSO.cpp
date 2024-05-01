#include<bits/stdc++.h>
using namespace std;

constexpr int numColor = 2;
constexpr int numVertex = 5;
constexpr int MaxIteration = 100;
constexpr int populationSize = 50;

//Define the population
int populationMatrix[populationSize][numVertex];
constexpr int Graph[numVertex][numVertex] = {
        {0, 0, 0, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0}
    };

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis(0, numColor - 1);
void initializePopulation()
{
    for(int i=0;i<populationSize;i++)
    {
        for(int j=0;j<numVertex;j++)
        {
            populationMatrix[i][j]=dis(gen);
        }
    }
}

// Function to calculate the fitness of a particle
int calculateFitness(int iter) {
    int conflicts = 0;
    for (int i = 0; i < numVertex; ++i) {
        for (int j = i + 1; j < numVertex; ++j) {
            if (Graph[i][j] && populationMatrix[iter][i] == populationMatrix[iter][j]) {
                conflicts++; // Increment conflicts if adjacent vertices have the same color
            }
        }
    }
    // lower conflicts indicate better solutions
    return conflicts;
}


int main()
{


    initializePopulation();
    for(int i=0;i<populationSize;i++)
    {

    }

    for(int i=0;i<populationSize;i++)
    {
        cout<<calculateFitness(i)<<" [";
        for(int j=0;j<numVertex;j++)
        {
            cout<<populationMatrix[i][j]<<" ";
        }
        cout<<"]"<<endl;
    }
    return 0;
}
