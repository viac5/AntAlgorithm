#include "ant.h"
#include <QVector>
#include <cmath>
#include <random>

Ant::Ant(int startNode, int graphSize) :
    currentNode(startNode),
    visitedNodes(graphSize, false)
{
    visitedNodes[startNode] = true;
    path.push_back(startNode);
}

void Ant::move(const QVector<QVector<double>>& pheromones,
               const QVector<QVector<double>>& distances,
               double alpha, double beta)
{
    const int startNode = path.first();

    while (path.size() < visitedNodes.size()) {
        int nextNode = chooseNextNode(pheromones, distances, alpha, beta);
        if (nextNode == -1) break;

        path.push_back(nextNode);
        visitedNodes[nextNode] = true;
        currentNode = nextNode;
    }

    if (path.size() == visitedNodes.size() &&
        distances[currentNode][startNode] > 0.0) {
        path.push_back(startNode);
    }
}

QVector<int> Ant::getPath() const
{
    return path;
}

double Ant::getPathLength(const QVector<QVector<double>>& distances) const
{
    double length = 0.0;
    for (int i = 0; i < path.size() - 1; ++i) {
        length += distances[path[i]][path[i+1]];
    }
    return length;
}

int Ant::chooseNextNode(const QVector<QVector<double>>& pheromones,
                        const QVector<QVector<double>>& distances,
                        double alpha, double beta)
{
    const int n = visitedNodes.size();
    QVector<double> probabilities(n, 0.0);
    double sum = 0.0;

    for (int i = 0; i < n; ++i) {
        if (!visitedNodes[i] && distances[currentNode][i] > 0.0) {
            double pheromoneInfluence = std::pow(pheromones[currentNode][i], alpha);
            double heuristicInfluence = std::pow(1.0 / distances[currentNode][i], beta);
            double probability = pheromoneInfluence * heuristicInfluence;

            if (std::isnan(probability) || probability < 0.0 || std::isinf(probability))
                continue;

            probabilities[i] = probability;
            sum += probability;
        }
    }

    if (sum == 0.0) {
        QVector<int> available;
        for (int i = 0; i < n; ++i) {
            if (!visitedNodes[i] && distances[currentNode][i] > 0.0) {
                available.append(i);
            }
        }
        if (available.isEmpty()) return -1;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, available.size() - 1);
        return available[dis(gen)];
    }

    for (double& p : probabilities)
        p /= sum;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double r = dis(gen);

    double cumulative = 0.0;
    for (int i = 0; i < n; ++i) {
        cumulative += probabilities[i];
        if (r <= cumulative) {
            return i;
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        if (probabilities[i] > 0.0)
            return i;
    }

    return -1;
}


