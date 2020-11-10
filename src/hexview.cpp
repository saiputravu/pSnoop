#include "hexview.hpp"

HexView::HexView(QWidget *parent) : QAbstractScrollArea(parent) {
	this->setStyleSheet("background-color: red");
}

HexView::~HexView() {

}
