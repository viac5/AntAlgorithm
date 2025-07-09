#include "graph.h"
#include <QPointF>
#include <QRandomGenerator>
#include <QtMath>

Graph::Graph(QObject *parent) : QObject(parent) {}

void Graph::generateRandomGraph(int nodeCount, int width, int height)
{
    const int MIN_DISTANCE = 60;
    const int MAX_ATTEMPTS = 100;

    nodes.clear();
    edges.clear();

    for (int i = 0; i < nodeCount; ++i) {
        int attempts = 0;
        bool positionAccepted = false;

        while (!positionAccepted && attempts < MAX_ATTEMPTS) {
            attempts++;

            qreal x = QRandomGenerator::global()->bounded(20, width - 20);
            qreal y = QRandomGenerator::global()->bounded(20, height - 20);
            QPointF newPoint(x, y);

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
        if (!positionAccepted) {
            qreal x = QRandomGenerator::global()->bounded(20, width - 20);
            qreal y = QRandomGenerator::global()->bounded(20, height - 20);
            nodes.append(QPointF(x, y));
        }
    }

    edges.resize(nodeCount);
    for (int i = 0; i < nodeCount; ++i) {
        edges[i].resize(nodeCount);
    }

    for (int i = 0; i < nodeCount; ++i) {
        for (int j = i + 1; j < nodeCount; ++j) {
            qreal dx = nodes[i].x() - nodes[j].x();
            qreal dy = nodes[i].y() - nodes[j].y();
            qreal distance = qSqrt(dx * dx + dy * dy);

            if (distance < MIN_DISTANCE) {
                edges[i][j] = 0.0;
            } else {
                edges[i][j] = distance * (0.8 + 0.4 * QRandomGenerator::global()->generateDouble());
            }
            edges[j][i] = edges[i][j];
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

double Graph::calculatePathLength(const QVector<int>& path) const
{
    if (path.size() < 2) return 0.0;

    double length = 0.0;
    for (int i = 0; i < path.size() - 1; ++i) {
        length += edges[path[i]][path[i+1]];
    }

    return length;
}


void Graph::setCustomGraph(const QVector<QPointF>& newNodes, const QVector<QVector<double>>& newEdges)
{
    nodes = newNodes;
    edges = newEdges;
}
