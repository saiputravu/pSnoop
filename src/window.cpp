#include "window.hpp"

Window::Window(QWidget *parent) : QWidget(parent) {
	this->init();
}

Window::~Window() {

}

void Window::init() {
	this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}
