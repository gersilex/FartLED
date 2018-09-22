#ifndef __POSITION_CALCULATOR
#define __POSITION_CALCULATOR

struct MatrixConfig{
    int width;
    int height;
    int mode = 0;

    MatrixConfig(int width, int height);
};

struct Position {
    int current;
    int up;
    int left;
    int right;
    int down;

    int row;
    int column;

    bool isFirstRow();
    bool isFirstColumn();
};


class PositionCalculator
{
    MatrixConfig config = MatrixConfig(0,0);

public:
    PositionCalculator(MatrixConfig config);
    Position translate(int row, int column);
};

#endif // __POSITION_CALCULATOR