#include "tree.hpp"

PacketTree::PacketTree(QWidget *parent) : QTreeWidget(parent) {
	this->setColumnCount(2);
}

PacketTree::~PacketTree() {

}

QTreeWidgetItem *PacketTree::add_tree_root(QString name, QString value) {
	QTreeWidgetItem *item = new QTreeWidgetItem(this);

	item->setText(0, name);
	item->setText(1, value);

	return item;
}

QTreeWidgetItem *PacketTree::add_tree_child(std::vector<unsigned int> indexes, QString name, QString value) {
	// Take the root level node 
	// Indexes will just find the indexed tree child item in a sequence 
	QTreeWidgetItem *cur_item = this->topLevelItem(indexes[0]);
	for (int ind = 1; ind < indexes.size(); ++ind) {
		if (cur_item == nullptr)
			return nullptr; // Bad indexes
		cur_item = cur_item->child(indexes[ind]);
	}
	
	// Set values to new child item 
	// Set child item to end
	QTreeWidgetItem *child_item = new QTreeWidgetItem();
	child_item->setText(0, name);
	child_item->setText(1, value);
	cur_item->addChild(child_item);

	return child_item;
}

// Overloaded if parent pointer known 
QTreeWidgetItem *PacketTree::add_tree_child(QTreeWidgetItem *parent_item, QString name, QString value) {
	QTreeWidgetItem *child_item = new QTreeWidgetItem();
	child_item->setText(0, name);
	child_item->setText(1, value);
	parent_item->addChild(child_item);

	return child_item;
}

void PacketTree::load_node(QTreeWidgetItem *parent, struct tree_node_t *parent_node) { 
	// The top node is passed and the next_nodes are then parsed
	// Top
	// -> A | B | C individually parsed and then recursively parsed
	// Parent_node name and value is assumed to already have been added
	
	if (parent == nullptr || parent_node == nullptr)
		return;
	
	// Recursively add each node
	QTreeWidgetItem *item;
	for (struct tree_node_t *n : parent_node->next_nodes) {
		if (n != nullptr) {
			// If child node exists, add it 
			item = this->add_tree_child(parent, 
					QString::fromStdString(n->name), 
					QString::fromStdString(n->value));
			this->load_node(item, n);
		}
	}
}

void PacketTree::load_tree(TreeStructure *tree) {
	// Clear previous nodes 
	this->clear();
	
	QTreeWidgetItem *root = nullptr;
	// Fill up all the roots 
	for (struct tree_node_t *i : tree->get_tree()->root_nodes) {
		root = this->add_tree_root(QString::fromStdString(i->name),
				QString::fromStdString(i->value));
		this->load_node(root, i);
	}
}