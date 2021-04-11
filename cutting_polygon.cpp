#include "cutting_polygon.h"

cutting_polygon::cutting_polygon(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.reset_button, SIGNAL(clicked()), this, SLOT(reset_clicked()));
    connect(ui.main_button, SIGNAL(clicked()), this, SLOT(main_clicked()));
    connect(ui.cut_button, SIGNAL(clicked()), this, SLOT(cut_clicked()));
    connect(ui.cut_start, SIGNAL(clicked()), this, SLOT(cut_polygon()));

}

void cutting_polygon::main_clicked() {
    mode = 1;
}

void cutting_polygon::reset_clicked() {
    main_poly.clear();
    cut_poly.clear();
    cutted_poly.clear();
}

void cutting_polygon::cut_clicked() {
    mode = 2;
}

void cutting_polygon::cut_polygon() {
    vector<wa_node*> m_wa, c_wa;
    int k = 2;
    wa_node* head, * temp, * temp2;
    for (int i = 0; i < main_poly.size(); i++) {
        head = new wa_node;
        head->x = main_poly[i].edge[0].x;
        head->y = main_poly[i].edge[0].y;
        head->type = vertex;
        temp = head;
        for (int j = 0; j < main_poly[i].edge.size(); j++) {
            temp2 = temp;
            for (int k = 0; k < cut_poly.size(); k++) {
                for (int l = 0; l < cut_poly[k].edge.size(); l++) {
                    float x = 0, y = 0;
                    if (get_line_intersection(main_poly[i].edge[j].x, main_poly[i].edge[j].y, main_poly[i].edge[(j + 1) % main_poly[i].edge.size()].x, main_poly[i].edge[(j + 1) % main_poly[i].edge.size()].y,
                        cut_poly[k].edge[l].x, cut_poly[k].edge[l].y, cut_poly[k].edge[(l + 1) % cut_poly[k].edge.size()].x, cut_poly[k].edge[(l + 1) % cut_poly[k].edge.size()].y, &x, &y) == true) {
                        temp->next = new wa_node();
                        temp = temp->next;
                        temp->x = x;
                        temp->y = y;
                        temp->type = cross;
                    }
                }
            }
            node_sort(temp2->next, main_poly[i].edge[j]);
            if (j == main_poly[i].edge.size() - 1) {
                temp->next = head;
            }
            else
            {
                temp->next = new wa_node();
                temp = temp->next;
                temp->x = main_poly[i].edge[(j + 1)].x;
                temp->y = main_poly[i].edge[(j + 1)].y;
                temp->type = vertex;
            }
        }
        m_wa.push_back(head);
    }
    int check = 0, check2 = 0;

    for (int i = 0; i < cut_poly.size(); i++) {
        head = new wa_node;
        head->x = cut_poly[i].edge[0].x;
        head->y = cut_poly[i].edge[0].y;
        head->type = vertex;
        temp = head;
        for (int j = 0; j < cut_poly[i].edge.size(); j++) {
            temp2 = temp;
            for (int k = 0; k < main_poly.size(); k++) {
                for (int l = 0; l < main_poly[k].edge.size(); l++) {
                    float x = 0, y = 0;
                    if (get_line_intersection(cut_poly[i].edge[j].x, cut_poly[i].edge[j].y, cut_poly[i].edge[(j + 1) % cut_poly[i].edge.size()].x, cut_poly[i].edge[(j + 1) % cut_poly[i].edge.size()].y,
                        main_poly[k].edge[l].x, main_poly[k].edge[l].y, main_poly[k].edge[(l + 1) % main_poly[k].edge.size()].x, main_poly[k].edge[(l + 1) % main_poly[k].edge.size()].y, &x, &y) == true) {
                        temp->next = new wa_node();
                        temp = temp->next;
                        temp->x = x;
                        temp->y = y;
                        temp->type = cross;
                        check++;
                    }
                }
            }
            node_sort(temp2->next, cut_poly[i].edge[j]);
            if (j == cut_poly[i].edge.size() - 1) {
                temp->next = head;
            }
            else {
                temp->next = new wa_node();
                temp = temp->next;
                temp->x = cut_poly[i].edge[(j + 1)].x;
                temp->y = cut_poly[i].edge[(j + 1)].y;
                temp->type = vertex;
            }
        }
        c_wa.push_back(head);
    }
    if (check == 0) {
        cutted_poly = cut_poly;
        return;
    }

    for (int i = 0; i < m_wa.size(); i++) {
        temp = m_wa[i]->next;
        while (1) {
            if ((temp->x == m_wa[i]->x) && (temp->y == m_wa[i]->y))
                break;
            for (int j = 0; j < c_wa.size(); j++) {
                temp2 = c_wa[j]->next;
                while (1) {
                    if ((temp2->x == c_wa[j]->x) && (temp2->y == c_wa[j]->y))
                        break;
                    if ((abs(temp->x - temp2->x) < 1) && (abs(temp->y - temp2->y) < 1)) {
                        temp->ano_poly = temp2;
                        temp2->ano_poly = temp;
                        break;
                    }
                    temp2 = temp2->next;
                }
            }
            temp = temp->next;
        }
    }

    for (int i = 0; i < c_wa.size(); i++) {
        temp = c_wa[i]->next;
        while (1) {
            if ((temp->x == c_wa[i]->x) && (temp->y == c_wa[i]->y))
                break;
            if (temp->type == cross) {
                temp2 = temp->ano_poly;
                if (is_enter_point(temp, temp2)) {
                    temp->type = out;
                    temp2->type = enter;
                }
                else {
                    temp->type = enter;
                    temp2->type = out;
                }
            }
            temp = temp->next;
        }
    }
    while (1) {
        wa_node* start = m_wa[0];
        int i = 0;
        polygon tplg;

        while (1) {
            if (start->is_use == false && start->type != vertex) {
                break;
            }
            start = start->next;
            if (start == m_wa[i]) {
                start = m_wa[++i];
            }
        }

        temp = start;
        while (1) {
            if (temp->type == out) {
                if (temp->is_use == false) {
                    temp->is_use = true;
                    Pos tp;
                    tp.x = temp->x;
                    tp.y = temp->y;
                    tplg.edge.push_back(tp);
                    temp = temp->ano_poly->next;
                    check2++;
                }
            }
            else if (temp->type == enter) {
                if (temp->is_use == false) {
                    temp->is_use = true;
                    Pos tp;
                    tp.x = temp->x;
                    tp.y = temp->y;
                    tplg.edge.push_back(tp);
                    temp = temp->next;
                    check2++;
                }
            }
            else {
                Pos tp;
                tp.x = temp->x;
                tp.y = temp->y;
                tplg.edge.push_back(tp);
                temp = temp->next;
            }
            if (temp == start || temp->ano_poly == start)
                break;
        }
        cutted_poly.push_back(tplg);
        if (check == check2) {
            break;
        }
    }
}

void cutting_polygon::mousePressEvent(QMouseEvent* event){
    
    if (event->button() == Qt::LeftButton) {
        Pos p;
        p.x = event->pos().x();
        p.y = event->pos().y();
        points.push_back(p);
    }

    if (event->button() == Qt::RightButton) {
        if (mode == 1) {
            polygon temp;
            temp.is_made = true;
            temp.edge = points;
            main_poly.push_back(temp);
            points.clear();
        }
        else if (mode == 2) {
            polygon temp;
            temp.is_made = true;
            temp.edge = points;
            cut_poly.push_back(temp);
            points.clear();
        }
    }
}

void cutting_polygon::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter.setPen(pen);
    if (points.size() > 0)
    {
        for (unsigned int i = 0; i < points.size(); i++)
        {
            painter.drawPoint(points[i].x, points[i].y);
        }
    }

    pen.setColor(Qt::red);
    painter.setPen(pen);
    if (main_poly.size() > 0) {
        for (int i = 0; i < main_poly.size(); i++) {
            for (int j = 0; j < main_poly[i].edge.size() - 1; j++)
                painter.drawLine(main_poly[i].edge[j].x, main_poly[i].edge[j].y, main_poly[i].edge[j + 1].x, main_poly[i].edge[j + 1].y);
            painter.drawLine(main_poly[i].edge[0].x, main_poly[i].edge[0].y, main_poly[i].edge.back().x, main_poly[i].edge.back().y);
        }
    }

    pen.setColor(Qt::green);
    painter.setPen(pen);
    if (cut_poly.size() > 0) {
        for (int i = 0; i < cut_poly.size(); i++) {
            for (int j = 0; j < cut_poly[i].edge.size() - 1; j++)
                painter.drawLine(cut_poly[i].edge[j].x, cut_poly[i].edge[j].y, cut_poly[i].edge[j + 1].x, cut_poly[i].edge[j + 1].y);
            painter.drawLine(cut_poly[i].edge[0].x, cut_poly[i].edge[0].y, cut_poly[i].edge.back().x, cut_poly[i].edge.back().y);
        }
    }

    pen.setColor(Qt::blue);
    painter.setPen(pen);
    if (cutted_poly.size() > 0) {
        for (int i = 0; i < cutted_poly.size(); i++) {
            for (int j = 0; j < cutted_poly[i].edge.size() - 1; j++)
                painter.drawLine(cutted_poly[i].edge[j].x, cutted_poly[i].edge[j].y, cutted_poly[i].edge[j + 1].x, cutted_poly[i].edge[j + 1].y);
            painter.drawLine(cutted_poly[i].edge[0].x, cutted_poly[i].edge[0].y, cutted_poly[i].edge.back().x, cutted_poly[i].edge.back().y);
        }
    }
    update();
}



bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float* i_x, float* i_y)
{
    float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
    s10_x = p1_x - p0_x;
    s10_y = p1_y - p0_y;
    s32_x = p3_x - p2_x;
    s32_y = p3_y - p2_y;

    denom = s10_x * s32_y - s32_x * s10_y;
    if (denom == 0)
        return false;
    bool denomPositive = denom > 0;

    s02_x = p0_x - p2_x;
    s02_y = p0_y - p2_y;
    s_numer = s10_x * s02_y - s10_y * s02_x;
    if ((s_numer < 0) == denomPositive)
        return false;

    t_numer = s32_x * s02_y - s32_y * s02_x;
    if ((t_numer < 0) == denomPositive)
        return false;

    if (fabs(s_numer) > fabs(denom) || fabs(t_numer) > fabs(denom))
        return false;
    t = t_numer / denom;
    if (i_x != NULL)
        *i_x = p0_x + (t * s10_x);
    if (i_y != NULL)
        *i_y = p0_y + (t * s10_y);

    return true;
}

void node_sort(wa_node *&head, Pos A)
{
    if (head == NULL || head->next == NULL)
    {
        return;
    }
    wa_node* p = NULL;
    bool is_change = true;
    while (p != head->next && is_change)
    {
        wa_node* q = head;
        is_change = false;
        for (; q->next && q->next != p; q = q->next)
        {
            if (dist(A.x, A.y, q->x, q->y) > dist(A.x, A.y, q->next->x, q->next->y))
            {
                swap(q->x, q->next->x);
                swap(q->y, q->next->y);
                is_change = true;
            }
        }
        p = q;
    }
}

float dist(float x1, float y1, float x2, float y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

bool is_enter_point(wa_node *p, wa_node *q){
    float vec1_x, vec1_y, vec2_x, vec2_y;
    vec1_x = p->next->x - p->x;
    vec1_y = p->next->y - p->y;
    vec2_x = q->next->x - q->x;
    vec2_y = q->next->y - q->y;

    float theta1, theta2;
    theta1 = atan2(vec1_y, vec1_x) * 180 / M_PI;
    theta2 = atan2(vec2_y, vec2_x) * 180 / M_PI;
    if (theta1 < 0)
    {
        theta1 += 360;
    }
    if (theta2 < 0)
    {
        theta2 += 360;
    }
    if ((theta1 - theta2 > 0) && (theta1 - theta2 < 180))
    {
        return true;
    }
    else if ((theta1 - theta2 > -360) && (theta1 - theta2 < -180))
    {
        return true;
    }
    return false;
}