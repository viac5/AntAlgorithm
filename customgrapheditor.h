#ifndef CUSTOMGRAPHEDITOR_H
#define CUSTOMGRAPHEDITOR_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QMap>
#include <QVector>
#include <QPointF>

namespace Ui {
class CustomGraphEditor;
}

class CustomGraphEditor : public QDialog
{
    Q_OBJECT

public:
    explicit CustomGraphEditor(QWidget *parent = nullptr);
    ~CustomGraphEditor();

    QVector<QPointF> getNodes() const;
    QVector<QVector<double>> getEdges() const;

signals:
    void graphCreated(const QVector<QPointF>& nodes, const QVector<QVector<double>>& edges);

private:
    Ui::CustomGraphEditor *ui;
    QGraphicsScene* scene;

    QVector<QGraphicsItemGroup*> nodeItems;
    QVector<QPointF> nodePositions;
    QVector<QVector<double>> edgeWeights;

    QGraphicsEllipseItem* selectedNode1 = nullptr;
    QGraphicsEllipseItem* selectedNode2 = nullptr;

    QPointF getNodeCenter(QGraphicsItem* item);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void on_addNodeButton_clicked();
    void on_connectNodesButton_clicked();
    void on_finishButton_clicked();
};

#endif
