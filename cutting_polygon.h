#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cutting_polygon.h"

#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

#define vertex 1
#define cross 2
#define enter 3
#define out 4
#define M_PI 3.14159265358979323846

using namespace std;

class Pos {
public:
    float x;
    float y;
};

class polygon{
public:
    vector<Pos> edge;
    bool is_made = false;
};

class wa_node{
public:
    float x, y;
    int type;
    bool is_use;
    wa_node *next;
    wa_node *ano_poly;
    wa_node(){
        next = NULL;
        ano_poly = NULL;
        is_use = false;
    }
};

float dist(float x1, float y1, float x2, float y2);

void node_sort(wa_node *&head, Pos A);

bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float* i_x, float* i_y);

bool is_enter_point(wa_node *p, wa_node *q);

class cutting_polygon : public QMainWindow
{
    Q_OBJECT

public:
    cutting_polygon(QWidget *parent = Q_NULLPTR);
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    vector<Pos> points;
    vector<polygon> main_poly;
    vector<polygon> cut_poly;
    vector<polygon> cutted_poly;
    int mode = 0;

private:
    Ui::cutting_polygonClass ui;

public slots:
    void main_clicked();
    void cut_clicked();
    void reset_clicked();
    void cut_polygon();
};

