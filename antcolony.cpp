#include "antcolony.h"
#include <QDebug>


AntColony::AntColony(QObject *parent) : QObject(parent), bestPathLength(1e9) {}

void AntColony::optimize(Graph* graph, int antCount, int iterations,
                         double alpha, double beta, double evaporation, double Q)
{

    bestPathLength = std::numeric_limits<double>::max(); // Сбрасываем длину
    bestPath.clear();                                   // Очищаем предыдущий путь

    auto nodes = graph->getNodes();
    auto distances = graph->getEdges();
    int nodeCount = nodes.size();

    QVector<QVector<double>> pheromones(nodeCount, QVector<double>(nodeCount, 1.0));

    for (int iter = 0; iter < iterations; ++iter) {
        QVector<QVector<int>> validPaths;

        for (int k = 0; k < antCount; ++k) {
            int startNode = rand() % nodeCount;
            Ant ant(startNode, nodeCount);
            ant.move(pheromones, distances, alpha, beta);
            QVector<int> path = ant.getPath();

            // Проверка, что муравей прошел по всем вершинам и вернулся в начало
            if (isValidTSPPath(path, nodeCount)){
                double length = ant.getPathLength(distances);
                validPaths.append(path);

                if (length < bestPathLength) {
                    bestPathLength = length;
                    bestPath = path;
                    emit iterationComplete(iter, bestPathLength);
                }
            }
        }

        // Обновляем феромоны только по корректным путям
        updatePheromones(pheromones, distances, validPaths, evaporation, Q);
    }
}

bool AntColony::isValidTSPPath(const QVector<int>& path, int nodeCount) {
    if (path.size() != nodeCount + 1)
        return false;

    if (path.first() != path.last())
        return false;

    QSet<int> unique;
    for (int i = 0; i < path.size() - 1; ++i) {
        unique.insert(path[i]);
    }

    return unique.size() == nodeCount;
}


void AntColony::updatePheromones(QVector<QVector<double>>& pheromones,
                                 const QVector<QVector<double>>& distances,
                                 const QVector<QVector<int>>& paths,
                                 double evaporation, double Q)
{
    int nodeCount = pheromones.size();

    // Испарение феромонов
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            pheromones[i][j] = std::max(pheromones[i][j] * (1.0 - evaporation), 0.01);
        }
    }

    // Добавление новых феромонов
    for (const auto& path : paths) {
        double pathLength = 0.0;
        for (int i = 0; i < path.size() - 1; ++i) {
            pathLength += distances[path[i]][path[i+1]];
        }

        if (pathLength > 0) {
            double deltaPheromone = Q / pathLength;
            for (int i = 0; i < path.size() - 1; ++i) {
                int from = path[i];
                int to = path[i+1];
                pheromones[from][to] += deltaPheromone;
                pheromones[to][from] += deltaPheromone;
            }
        }
    }
}
QVector<int> AntColony::getBestPath() const
{
    return bestPath;
}
