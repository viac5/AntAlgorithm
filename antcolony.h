#ifndef ANTCOLONY_H
#define ANTCOLONY_H

#include <QObject>
#include "graph.h"
#include "ant.h"

class AntColony : public QObject
{
    Q_OBJECT
public:
    explicit AntColony(QObject *parent = nullptr);
    void optimize(Graph* graph, int antCount, int iterations,
                  double alpha, double beta, double evaporation, double Q);

    QVector<int> getBestPath() const;
    double getBestPathLength() const;

signals:
    void iterationComplete(int iteration, double bestLength);

private:
    QVector<int> bestPath;
    double bestPathLength;
    bool isValidTSPPath(const QVector<int>& path, int nodeCount);
    void updatePheromones(QVector<QVector<double>>& pheromones,
                          const QVector<QVector<double>>& distances,
                          const QVector<QVector<int>>& paths,
                          double evaporation, double Q);
};

#endif // ANTCOLONY_H
