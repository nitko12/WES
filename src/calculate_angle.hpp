#include <Arduino.h>

byte lut[10][11] = {
    {10, 18, 25, 34, 38, 42, 42, 42, 42, 42, 42},
    {10, 15, 23, 30, 40, 40, 40, 40, 40, 40, 40},
    {10, 14, 21, 28, 38, 38, 38, 38, 38, 38, 38},
    {10, 14, 20, 25, 36, 36, 36, 36, 36, 36, 36},
    {10, 13, 20, 24, 35, 35, 35, 35, 35, 35, 35},
    {10, 13, 19, 24, 33, 33, 33, 33, 33, 33, 33},
    {10, 13, 19, 23, 32, 32, 32, 32, 32, 32, 32},
    {10, 13, 19, 22, 30, 30, 30, 30, 30, 30, 30},
    {10, 13, 18, 21, 28, 28, 28, 28, 28, 28, 28},
    {10, 13, 18, 20, 25, 25, 25, 25, 25, 25, 25},
};

int x_dist[] = {1000, 1400, 1800, 2500, 3000, 4000, 6000, 8000, 10000, 12000, 14000};
int y_dist[] = {15, 25, 30, 40, 50, 60, 70, 80, 90, 100};

byte find_x_index(int val)
{
    for (int i = 0; i < 11; i++)
    {
        if (val < x_dist[i])
            return i;
    }
    return 10;
}

byte find_y_index(int val)
{
    for (int i = 0; i < 10; i++)
    {
        if (val < y_dist[i])
            return i;
    }
    return 9;
}

int get_value(int x, int y)
{
    int lb_x = find_x_index(x);
    int lb_y = find_y_index(y);

    // interpolate x

    // trilinear interpolation

    int x_val_lower = map(x, x_dist[lb_x - 1], x_dist[lb_x], lut[lb_y][lb_x - 1], lut[lb_y][lb_x]);

    int x_val_upper = map(x, x_dist[lb_x - 1], x_dist[lb_x], lut[lb_y + 1][lb_x - 1], lut[lb_y + 1][lb_x]);

    int x_val = map(y, y_dist[lb_y], y_dist[lb_y + 1], x_val_lower, x_val_upper);

    if (x_val > 42)
        x_val = 10;

    if (x_val < 10)
        x_val = 10;

    return x_val;
}