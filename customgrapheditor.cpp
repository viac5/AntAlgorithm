#include "customgrapheditor.h"
#include "ui_customgrapheditor.h"
#include <QInputDialog>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>

CustomGraphEditor::CustomGraphEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomGraphEditor)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

CustomGraphEditor::~CustomGraphEditor()
{
    delete ui;
}

void CustomGraphEditor::on_addNodeButton_clicked()
{
    QPointF position = ui->graphicsView->mapToScene(ui->graphicsView->width() / 2, ui->graphicsView->height() / 2);
    int nodeNumber = nodeItems.size();

    // Эллипс
    QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem(-15, -15, 30, 30);
    ellipse->setBrush(QBrush(Qt::yellow));
    ellipse->setPen(QPen(Qt::black));

    // Текст
    QGraphicsTextItem* label = new QGraphicsTextItem(QString::number(nodeNumber));
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    label->setFont(font);
    label->setDefaultTextColor(Qt::black);

    // Центрируем текст в эллипсе
    QRectF textRect = label->boundingRect();
    label->setPos(-textRect.width() / 2, -textRect.height() / 2);

    // Группа
    QGraphicsItemGroup* group = scene->createItemGroup({ellipse, label});
    group->setPos(position);
    group->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(group);

    // Сохраняем как QGraphicsItem*
    nodeItems.append(group);
    nodePositions.append(position);

    // Обновим матрицу смежности
    int size = nodeItems.size();
    edgeWeights.resize(size);
    for (int i = 0; i < size; ++i)
        edgeWeights[i].resize(size);
}

QPointF CustomGraphEditor::getNodeCenter(QGraphicsItem* item)
{
    // Предполагаем, что это группа с первым элементом — эллипсом
    auto group = dynamic_cast<QGraphicsItemGroup*>(item);
    if (!group) return item->scenePos(); // fallback

    for (auto* child : group->childItems()) {
        if (auto* ellipse = dynamic_cast<QGraphicsEllipseItem*>(child)) {
            return group->scenePos() + ellipse->rect().center();
        }
    }

    return group->scenePos();
}

void CustomGraphEditor::on_connectNodesButton_clicked()
{
    if (nodeItems.size() < 2) return;

    bool ok;
    int from = QInputDialog::getInt(this, "Соединение", "Введите номер 1 узла (0...N-1):", 0, 0, nodeItems.size()-1, 1, &ok);
    if (!ok) return;

    int to = QInputDialog::getInt(this, "Соединение", "Введите номер 2 узла (0...N-1):", 0, 0, nodeItems.size()-1, 1, &ok);
    if (!ok || from == to) return;

    double weight = QInputDialog::getDouble(this, "Вес ребра", "Введите вес:", 1.0, 0.1, 1e6, 1, &ok);
    if (!ok) return;

    QPointF p1 = getNodeCenter(nodeItems[from]);
    QPointF p2 = getNodeCenter(nodeItems[to]);

    scene->addLine(QLineF(p1, p2), QPen(Qt::gray));
    auto text = scene->addText(QString::number(weight));
    text->setPos((p1 + p2) / 2);
    text->setDefaultTextColor(Qt::blue);

    edgeWeights[from][to] = weight;
    edgeWeights[to][from] = weight;
}

void CustomGraphEditor::on_finishButton_clicked()
{
    for (int i = 0; i < nodeItems.size(); ++i) {
        nodePositions[i] = getNodeCenter(nodeItems[i]);
    }

    emit graphCreated(nodePositions, edgeWeights);
    accept();
}

QVector<QPointF> CustomGraphEditor::getNodes() const
{
    return nodePositions;
}

QVector<QVector<double>> CustomGraphEditor::getEdges() const
{
    return edgeWeights;
}

void CustomGraphEditor::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Mouse clicked at" << event->pos();
    QDialog::mousePressEvent(event); // не забудь вызывать базовый метод
}
