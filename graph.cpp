#include "graph.h"
#include <QPointF>
#include <QRandomGenerator>
#include <QtMath>
#include <algorithm>

Graph::Graph(QObject *parent) : QObject(parent) {}

void Graph::generateRandomGraph(int nodeCount, int width, int height)
{
    const int MIN_DISTANCE = 60; // Минимальное расстояние между узлами в пикселях
    const int MAX_ATTEMPTS = 100; // Максимальное количество попыток размещения узла

    nodes.clear();
    edges.clear();

    // Генерация случайных позиций узлов с проверкой расстояния
    for (int i = 0; i < nodeCount; ++i) {
        int attempts = 0;
        bool positionAccepted = false;

        while (!positionAccepted && attempts < MAX_ATTEMPTS) {
            attempts++;

            qreal x = QRandomGenerator::global()->bounded(20, width - 20);
            qreal y = QRandomGenerator::global()->bounded(20, height - 20);
            QPointF newPoint(x, y);

            // Проверяем расстояние до всех уже размещенных узлов
            positionAccepted = true;
            for (const QPointF& point : nodes) {
                qreal dx = point.x() - newPoint.x();
                qreal dy = point.y() - newPoint.y();
                qreal distance = qSqrt(dx * dx + dy * dy);

                if (distance < MIN_DISTANCE) {
                    positionAccepted = false;
                    break;
                }
            }

            if (positionAccepted) {
                nodes.append(newPoint);
            }
        }

        // Если не удалось найти подходящее место - размещаем в любом случае
        if (!positionAccepted) {
            qreal x = QRandomGenerator::global()->bounded(20, width - 20);
            qreal y = QRandomGenerator::global()->bounded(20, height - 20);
            nodes.append(QPointF(x, y));


        }
    }

    // Инициализация матрицы смежности
    edges.resize(nodeCount);
    for (int i = 0; i < nodeCount; ++i) {
        edges[i].resize(nodeCount);
    }

    // Создание ребер между узлами (полносвязный граф)
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = i + 1; j < nodeCount; ++j) {
            qreal dx = nodes[i].x() - nodes[j].x();
            qreal dy = nodes[i].y() - nodes[j].y();
            qreal distance = qSqrt(dx * dx + dy * dy);

            // Добавляем ребро с небольшим случайным весом
            edges[i][j] = static_cast<int>(distance * (0.8 + 0.4 * QRandomGenerator::global()->generateDouble()));
            if (edges[i][j]<=1e-6)edges[i][j]=9999;
            edges[j][i] = edges[i][j]; // Граф неориентированный
        }
    }
}

QVector<QPointF> Graph::getNodes() const
{
    return nodes;
}

QVector<QVector<double>> Graph::getEdges() const
{
    return edges;
}

QVector<int> Graph::findShortestPath() const
{
    // Базовый алгоритм для поиска кратчайшего пути (для сравнения с муравьиным)
    // Здесь реализован простой перебор, для больших графов нужно использовать оптимизированные алгоритмы

    if (nodes.empty()) return QVector<int>();

    QVector<int> path;
    for (int i = 0; i < nodes.size(); ++i) {
        path.append(i);
    }

    QVector<int> shortestPath = path;
    double shortestLength = calculatePathLength(path);

    // Генерация всех перестановок (очень медленно для n > 10)
    while (std::next_permutation(path.begin(), path.end())) {
        double currentLength = calculatePathLength(path);
        if (currentLength < shortestLength) {
            shortestLength = currentLength;
            shortestPath = path;
        }
    }

    return shortestPath;
}

double Graph::calculatePathLength(const QVector<int>& path) const
{
    if (path.size() < 2) return 0.0;

    double length = 0.0;
    for (int i = 0; i < path.size() - 1; ++i) {
        length += edges[path[i]][path[i+1]];
    }
    // Замыкаем путь (возвращаемся в начальную точку)
    length += edges[path.last()][path.first()];

    return length;
}
