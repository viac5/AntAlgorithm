#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Инициализация
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    graph = new Graph(this);
    colony = new AntColony(this);

    connect(colony, &AntColony::iterationComplete,
            this, &MainWindow::onIterationComplete);
}

MainWindow::~MainWindow()
{
    delete ui;  // Освобождаем ресурсы интерфейса
    delete scene;
    delete graph;
    delete colony;
}

void MainWindow::drawGraph()
{
    scene->clear();

    auto nodes = graph->getNodes();
    auto edges = graph->getEdges();

    QRectF boundingRect; // Для масштабирования

    // Рисуем ребра
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = i + 1; j < nodes.size(); ++j) {
            if (edges[i][j] > 0) {
                QLineF line(nodes[i], nodes[j]);
                auto lineItem = scene->addLine(line, QPen(Qt::gray, 1));

                // Подпись веса ребра
                QPointF midPoint = (nodes[i] + nodes[j]) / 2;
                auto textItem = scene->addText(QString::number(edges[i][j]));
                textItem->setDefaultTextColor(Qt::blue);
                textItem->setFont(QFont("Arial", 10, QFont::Bold));
                textItem->setPos(midPoint.x() - 10, midPoint.y() - 10);
                textItem->setZValue(1); // Поверх линий
            }
        }
    }

    // Рисуем узлы
    for (int i = 0; i < nodes.size(); ++i) {
        QPointF pos = nodes[i];
        QRectF ellipseRect(pos.x() - 15, pos.y() - 15, 30, 30);

        scene->addEllipse(ellipseRect, QPen(Qt::black), QBrush(Qt::lightGray));

        // Текст с номером узла
        auto textItem = scene->addText(QString::number(i));
        textItem->setDefaultTextColor(Qt::black);
        textItem->setFont(QFont("Arial", 10, QFont::Bold));
        textItem->setPos(pos.x() - 6, pos.y() - 10);
        textItem->setZValue(2); // Поверх всего

        boundingRect |= ellipseRect;
    }

    // Добавим немного отступа
    boundingRect.adjust(-50, -50, 50, 50);
    scene->setSceneRect(boundingRect);

    // Подогнать граф по размеру окна
    ui->graphicsView->fitInView(boundingRect, Qt::KeepAspectRatio);
}


void MainWindow::drawPath(const QVector<int>& path, QColor color)
{
    auto nodes = graph->getNodes();
    QPen pen(color, 2);

    for (int i = 0; i < path.size() - 1; ++i) {
        int from = path[i];
        int to = path[i+1];
        QLineF line(nodes[from], nodes[to]);
        scene->addLine(line, pen);
    }
}

void MainWindow::on_generateGraphButton_clicked()
{
    int nodeCount = ui->nodeCountSpinBox->value();
    graph->generateRandomGraph(nodeCount, 500, 400);
    drawGraph();
}

void MainWindow::on_runAlgorithmButton_clicked()
{
    if (graph->getNodes().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала создайте граф");
        return;
    }

    int antCount = ui->antCountSpinBox->value();
    int iterations = ui->iterationSpinBox->value();
    double alpha = ui->alphaSpinBox->value();
    double beta = ui->betaSpinBox->value();
    double evaporation = ui->evaporationSpinBox->value();
    double Q = ui->qSpinBox->value();

    colony->optimize(graph, antCount, iterations, alpha, beta, evaporation, Q);

    // Отображаем лучший путь
    auto bestPath = colony->getBestPath();
    if (bestPath.size() != graph->getNodes().size() + 1) {
        QMessageBox::warning(this, "Ошибка", "Не удалось найти полный путь");
        return;
    }
    drawPath(bestPath, Qt::red);
}

void MainWindow::on_clearButton_clicked()
{
    scene->clear();
    graph = new Graph(this);

    delete colony;
    colony = new AntColony(this);
}

void MainWindow::onIterationComplete(int iteration, double bestLength)
{
    ui->statusBar->showMessage(
        QString("Итерация: %1, Лучшая длина: %2").arg(iteration).arg(bestLength));

    // Обновляем отображение каждые 10 итераций
    if (iteration % 10 == 0) {
        drawGraph();
        drawPath(colony->getBestPath(), Qt::red);
    }
}


