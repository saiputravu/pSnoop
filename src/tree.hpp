#ifndef TREE_H
#define TREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QString>
#include <QColor>
#include <QDebug>

#include <vector>

#include "capture/structure.hpp"
#include "settings.hpp"

class PacketTree : public QTreeWidget {
	Q_OBJECT;

	public:
		PacketTree(Settings *settings, QWidget *parent = nullptr);
		~PacketTree();
		
		// Setters 
		QTreeWidgetItem *add_tree_root(QString name, QString value);
		QTreeWidgetItem *add_tree_child(std::vector<unsigned int> indexes, QString name, QString value);
		QTreeWidgetItem *add_tree_child(QTreeWidgetItem *parent_item, QString name, QString value); // Overloaded

		// Methods
		void load_node(QTreeWidgetItem *parent, struct tree_node_t *node);

	private:
		Settings *settings;
	
	signals:

	public slots:
		void load_tree(TreeStructure *tree);
		void resize_all(); // Resize columns

};


#endif // TREE_H
