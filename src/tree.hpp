#ifndef TREE_H
#define TREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QString>

#include <vector>

#include "capture/structure.hpp"

class PacketTree : public QTreeWidget {
	Q_OBJECT;

	public:
		PacketTree(QWidget *parent = nullptr);
		~PacketTree();
		
		// Setters 
		QTreeWidgetItem *add_tree_root(QString name, QString value);
		QTreeWidgetItem *add_tree_child(std::vector<unsigned int> indexes, QString name, QString value);
		QTreeWidgetItem *add_tree_child(QTreeWidgetItem *parent_item, QString name, QString value); // Overloaded

		// Methods
		void load_node(QTreeWidgetItem *parent, struct tree_node_t *node);
		void load_tree(TreeStructure *tree);

	private:
	
	signals:

	public slots:

};


#endif // TREE_H
