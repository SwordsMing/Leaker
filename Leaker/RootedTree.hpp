#ifndef LEAKER_ROOTEDTREE_H_
#define LEAKER_ROOTEDTREE_H_

/*

	此结构用于存储各层协议信息
*/
#include <string>
#include <queue>

template <typename T>
class RootedTree
{
public:
	RootedTree():root_(NULL){}
	~RootedTree(){
		destroy();
	}

	typedef struct tagNode{

		T t_;
		struct tagNode * parent_;
		struct tagNode * brother_;
		struct tagNode * child_;
	}Node;

	Node * getRoot()const{return root_;}

	Node * insert(Node* parent,const T & t){

		Node * node = new Node;
		node->t_ = t;
		node->brother_ = NULL;
		node->parent_  = parent;
		node->child_   = NULL;
		if(parent== NULL){  //插入根结点
			root_ = node;
		}else{
			Node * first = parent->child_;
			if(first!= NULL){
				while (first->brother_!= NULL){first = first->brother_;}
				first->brother_ = node;
			}
			else{
				parent->child_ = node;
			}
		}
		return node;
	}
private:
	Node * root_;
	void destroy(){
		if(root_!= NULL)
		{
			std::queue<Node*> record_;
			Node * top,child,child2;
			record_.push(root_);
			while (!record_.empty())
			{
				top = record_.front();
				record_.pop_front();
				if(top->child_!= NULL){
					record_.push(top->child_);
				}
				child = top->brother_;
				delete top;
				while (child){
					if(child->child_!= NULL){
						record_.push(child->child_);
					}
					child2 = child->brother_;
					delete child;
					child = child2;
				}
			}
		}
	}
};

typedef RootedTree<std::string>::Node *HITEM;


#endif