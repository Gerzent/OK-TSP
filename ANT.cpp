#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <random>
#include <iomanip>
using namespace std;

struct Node
{
    int id, x, y;
};

double calculateDistance(Node &a, Node &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

struct Edge
{
    double pheromonLevel = 0.1;
    double distance;
};

class Ant
{
    double trust = 0.5;
    double greed = 0.5;

public:
    vector<int> path;
    double tourLength = 0.0;
    void walkGraph(const vector<vector<Edge>> &edges, int numNodes, int startingNode, mt19937 &rng)
    {
        path.clear();
        path.reserve(numNodes + 1);
        tourLength = 0.0;
        vector<bool> visited(numNodes + 1, false);
        int current = startingNode;
        visited[current] = true;
        path.push_back(current);

        for (int step = 1; step < numNodes; step++)
        {
            vector<int> candidates;
            vector<double> weights;
            double totalWeight = 0.0;
            for (int next = 1; next <= numNodes; next++)
            {
                if (!visited[next])
                {
                    double pheromonScalar = pow(edges[current][next].pheromonLevel, trust);
                    double atractivness = pow(1.0 / max(edges[current][next].distance, 1e-9), greed);
                    double weight = pheromonScalar * atractivness;
                    candidates.push_back(next);
                    weights.push_back(weight);
                    totalWeight += weight;
                }
            }
            // 2. Roulette-wheel selection
            int chosenNode = candidates.back();
            if (totalWeight > 0.0)
            {
                uniform_real_distribution<double> dist(0.0, totalWeight); // tworzymy rozkład jednostajny
                double randomNumber = dist(rng);
                double cumulative = 0.0;
                for (unsigned int i = 0; i < candidates.size(); i++)
                {
                    cumulative += weights[i];
                    if (randomNumber <= cumulative) // patrzymy w jakim przedziale jest nasza liczba, realizacja losowości ważonej
                    {
                        chosenNode = candidates[i]; // wybór następnego node
                        break;
                    }
                }
            }
            visited[chosenNode] = true;
            path.push_back(chosenNode);
            tourLength += edges[current][chosenNode].distance;
            current = chosenNode;
        }
        tourLength += edges[current][startingNode].distance; // powrót do początku
        path.push_back(startingNode);
    }
};

int main()
{
    string filename;
    cout << "podaj ścieżkę relatywną do pliku:\n";
    cin >> filename;
    ifstream inputFile(filename); // otwieranie pliku

    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open file " << filename << "\n";
        return 1;
    }

    int numNodes = 0;
    if (!(inputFile >> numNodes)) // odczytanie z pliku
    {
        cerr << "Error: Failed to read the number of nodes.\n";
        return 1;
    }

    vector<Node> nodes(numNodes + 1);
    for (int i = 1; i <= numNodes; i++)
    {
        if (!(inputFile >> nodes[i].id >> nodes[i].x >> nodes[i].y))
        {
            cerr << "Error reading node at index " << i << "\n";
            return 1;
        }
    }

    inputFile.close();

    cout << "Wczytano " << numNodes << " wierzchołków:\n";
    for (int i = 1; i <= numNodes; i++)
    {
        cout << "Node " << nodes[i].id << ": (" << nodes[i].x << ", " << nodes[i].y << ")\n";
    }

    vector<vector<Edge>> edges(numNodes + 1, vector<Edge>(numNodes + 1)); // macierz krawędzi
    for (int i = 1; i <= numNodes; i++)
    {
        for (int j = 1; j <= numNodes; j++)
        {
            edges[i][j].distance = calculateDistance(nodes[i], nodes[j]);
        }
    }

    int numAnts = 10;
    int numIterations = 100;
    double evaporationRate = 0.5;
    double Q = 100.0;   // stała ilości zostawianego feromonu

    vector<Ant> ants(numAnts);
    vector<int> bestPath;
    double bestLength = numeric_limits<double>::infinity();

    random_device rd;                                         // źródło entropii
    mt19937 rng(rd());                                        // generator losowości
    uniform_int_distribution<int> startNodeDist(1, numNodes); // rozkład do losowania punktu startowego (1..numNodes)

    // MAIN
    for (int iter = 0; iter < numIterations; iter++)
    {
        // budowanie tras przez mrówki
        for (int t = 0; t < numAnts; t++)
        {
            int randomNode = startNodeDist(rng);
            ants[t].walkGraph(edges, numNodes, randomNode, rng);

            if (ants[t].tourLength < bestLength)
            {
                bestLength = ants[t].tourLength;
                bestPath = ants[t].path;
            }
        }

        // parowanie
        for (int i = 1; i <= numNodes; i++)
        {
            for (int j = 1; j <= numNodes; j++)
            {
                edges[i][j].pheromonLevel *= (1.0 - evaporationRate);
                // zabezpieczenie przed całkowitym wyzerowaniem feromonu
                if (edges[i][j].pheromonLevel < 1e-6)
                {
                    edges[i][j].pheromonLevel = 1e-6;
                }
            }
        }

        // nowe feromony
        for (int t = 0; t < numAnts; t++)
        {
            double deposit = Q / ants[t].tourLength;
            for (size_t step = 0; step < ants[t].path.size() - 1; step++)
            {
                int u = ants[t].path[step];
                int v = ants[t].path[step + 1];
                edges[u][v].pheromonLevel += deposit;
                edges[v][u].pheromonLevel += deposit;
            }
        }
    }

    // wyświetlenie wyników końcowych
    cout << "\n  WYNIKI KOŃCOWE  \n";
    cout << fixed << setprecision(2);
    cout << "Najkrótszy znaleziony dystans: " << bestLength << "\n";
    cout << "Najlepsza trasa: ";
    for (size_t i = 0; i < bestPath.size(); i++)
    {
        cout << bestPath[i];
        if (i < bestPath.size() - 1)
        {
            cout << " -> ";
        }
    }
    cout << "\n";

    return 0;
}
