#ifndef ANT_H
#define ANT_H

#include <QVector>

class Ant
{
public:
    Ant(int startNode, int graphSize);
    void move(const QVector<QVector<double>>& pheromones,
              const QVector<QVector<double>>& distances,
              double alpha, double beta);
    QVector<int> getPath() const;
    double getPathLength(const QVector<QVector<double>>& distances) const;
    void moveToTarget(const QVector<QVector<double>>& pheromones,
                       const QVector<QVector<double>>& distances,
                           int targetNode, double alpha, double beta);

private:
    int currentNode;
    QVector<int> visited;
    QVector<int> path;
    QVector<bool> visitedNodes;
    int chooseNextNode(const QVector<QVector<double>>& pheromones,
                       const QVector<QVector<double>>& distances,
                       double alpha, double beta);
};
#endif
