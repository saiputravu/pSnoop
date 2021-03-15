#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <vector>
#include <utility>
#include <iostream>

/********************** Tree Data Structure **************************/
struct tree_node_t {
	std::string name;
	std::string value;

	std::vector<struct tree_node_t *> next_nodes;
};

struct tree_t {
	std::vector<struct tree_node_t *> root_nodes;
};

class TreeStructure {
	public:
		TreeStructure() {
			this->tree = new struct tree_t();
		} 
		~TreeStructure() {}

		// Getters 
		struct tree_t *get_tree() { return this->tree; }
		unsigned int get_tree_root_count() { return this->tree->root_nodes.size(); }

		struct tree_node_t *get_root_node(unsigned int index) {
			if (index < this->tree->root_nodes.size())
				return this->tree->root_nodes[index];
			return nullptr;
		}
		
		struct tree_node_t *get_child_node(std::vector<unsigned int> indexes) {
			// Loop through each index 
			struct tree_node_t *cur_node = this->get_root_node(indexes[0]);
			for (unsigned int i = 1; i < indexes.size(); ++i) {
				if (i < cur_node->next_nodes.size())
					cur_node = cur_node->next_nodes[indexes[i]];
				else 
					return nullptr; // Bad indexes provided
			}
			// Return the final node - will be nullptr if get_root_node fails
			return cur_node;
		}
		
		// Setters 
		void add_root_node(std::string name, std::string value) {
			struct tree_node_t *node = new struct tree_node_t();
			node->name = name;
			node->value = value;

			// Push back to root nodes 
			this->tree->root_nodes.push_back(node);
		}

		bool add_child_node(std::vector<unsigned int> indexes, std::string name, std::string value) {
			struct tree_node_t *node = new struct tree_node_t();
			node->name = name;
			node->value = value; 

			// Find the child node, check it is valid and push back to its next nodes
			struct tree_node_t *cur_node = this->get_child_node(indexes);
			if (cur_node == nullptr)
				return false;

			cur_node->next_nodes.push_back(node);
			return true;
		}

	private:
		struct tree_t *tree = nullptr;
};

#endif
