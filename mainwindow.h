#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "graph.h"
#include "antcolony.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_generateGraphButton_clicked();
    void on_runAlgorithmButton_clicked();
    void on_clearButton_clicked();
    void onIterationComplete(int iteration, double bestLength);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Graph *graph;
    AntColony *colony;

    void drawGraph();
    void drawPath(const QVector<int>& path, QColor color);
};

#endif // MAINWINDOW_H
