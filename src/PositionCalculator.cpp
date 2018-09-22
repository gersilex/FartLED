#include "PositionCalculator.h"

MatrixConfig::MatrixConfig(int width, int height) {
        this->width = width;
        this->height = height;
}


bool Position::isFirstRow() {
    return this->row == 0;
}

bool Position::isFirstColumn() {
    return this->column == 0;
}

PositionCalculator::PositionCalculator(MatrixConfig config) {
    this->config = config;
}

/**
 * Translate a (row|column) tuple to a position in an array
 */
Position PositionCalculator::translate(int row, int column) {
    Position out;

    out.row = row;
    out.column = column;

    // serpentine mode
    if (row % 2 == 0) {
        out.current = row * this->config.width + column;
        out.left = out.current - 1;
        out.up = out.current - (2 * column) - 1;
    } else {
        out.current = (row + 1) * this->config.width - column - 1;
        out.left = out.current + 1;
        out.up = (row - 1) * this->config.width + column;
    }

    return out;
}