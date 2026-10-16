#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
using namespace std;

struct Node
{
    int id;
    int x;
    int y;
};
double calculateDistance(Node &a, Node &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

void greedyTSP(vector<Node> nodes)
{
    vector<bool> visited(nodes.size(), false);
    vector<int> tour;
    double totalDistance = 0;

    int currentIndex = 0;
    visited[0] = true;
    tour.push_back(nodes[0].id);

    for (int step = 1; step < nodes.size(); step++)
    {
        int nearestNode = -1;
        double minDistance = numeric_limits<double>::max();
        for (size_t i = 0; i < nodes.size(); i++)
        {
            if (!visited[i])
            {
                double distance = calculateDistance(nodes[currentIndex], nodes[i]);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    nearestNode = i;
                }
            }
        }
        tour.push_back(nodes[nearestNode].id);
        visited[nearestNode] = true;
        currentIndex = nearestNode;
        totalDistance += minDistance;
    }
    totalDistance += calculateDistance(nodes[currentIndex], nodes[0]);
    tour.push_back(nodes[0].id);

    cout << "Kolejnosc odwiedzania (Trasa):\n";
    for (size_t i = 0; i < tour.size(); i++) // size_t to typ int unsigned
    {
        cout << tour[i];
        if (i + 1 < tour.size())
            cout << " -> ";
    }
    cout << "\n\nCalkowity dystans: " << totalDistance << "\n";
}

int main()
{
    string filename = "testcase.txt"; // ścieżka do pliku
    ifstream inputFile(filename);

    if (!inputFile.is_open()) // otwieranie pliku
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

    vector<Node> nodes(numNodes);
    for (int i = 0; i < numNodes; ++i)
    {
        if (!(inputFile >> nodes[i].id >> nodes[i].x >> nodes[i].y))
        {
            cerr << "Error reading node at index " << i << "\n";
            return 1;
        }
    }

    inputFile.close();

    cout << "Successfully loaded " << nodes.size() << " nodes:\n";
    for (const auto &node : nodes)
    {
        cout << "Node " << node.id << ": (" << node.x << ", " << node.y << ")\n";
    }

    greedyTSP(nodes);

    return 0;
}
