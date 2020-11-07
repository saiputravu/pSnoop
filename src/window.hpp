#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDesktopWidget>

class Window : public QWidget {
	Q_OBJECT;

	public:
		Window(QWidget *parent = nullptr);
		~Window();

	private:
		void init();

	signals:

	private slots:

};

#endif // WINDOW_H
