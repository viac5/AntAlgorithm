#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QVector>
#include <QPointF>

class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(QObject *parent = nullptr);

    void generateRandomGraph(int nodeCount, int width, int height);
    QVector<QPointF> getNodes() const;
    QVector<QVector<double>> getEdges() const;
    QVector<int> findShortestPath() const;
    void setCustomGraph(const QVector<QPointF>& newNodes, const QVector<QVector<double>>& newEdges);

private:
    QVector<QPointF> nodes;
    QVector<QVector<double>> edges;
    double calculatePathLength(const QVector<int>& path) const;
};

#endif
