#include<iostream>
using namespace std;

//定义
//1.每个节点都有颜色，不是黑色就是红色
//2.root必须是黑色
//3.不能有连续的红色
//4.从任意节点到其每一个叶子节点的所有简单路径包含相同数目的黑色节点
//5.nullptr是黑色

template<class T>
class RBTree
{
public:
	RBTree()
		:root_(nullptr)
	{

	}

public:
	void inOrder()
	{
		inOrder(root_);
		cout << endl;
	}

	void insert(const T& val)
	{
		if (root_ == nullptr)
		{
			root_ = new Node(val);
			return;
		}

		Node* parent = nullptr;
		Node* cur = root_;
		while (cur != nullptr)
		{
			if (cur->data_ > val)
			{
				parent = cur;
				cur = cur->left_;
			}
			else if (cur->data_ < val)
			{
				parent = cur;
				cur = cur->right_;
			}
			else
			{
				return;
			}
		}

		Node* node = new Node(val, parent, nullptr, nullptr, RED);
		if (parent->data_ > val)
		{
			parent->left_ = node;
		}
		else
		{
			parent->right_ = node;
		}

		if (color(parent) == RED)
		{
			fixAfterInsert(node);
		}
	}

	void remove(const T& val)
	{
		if (root_ == nullptr)
		{
			return;
		}

		Node* cur = root_;
		while (cur != nullptr)
		{
			if (cur->data_ > val)
			{
				cur = cur->left_;
			}
			else if (cur->data_ < val)
			{
				cur = cur->right_;
			}
			else
			{
				break;
			}
		}

		if (cur == nullptr)
		{
			return;
		}

		if (cur->left_ != nullptr && cur->right_ != nullptr)
		{
			Node* pre = cur->left_;
			while (pre->right_ != nullptr)
			{
				pre = pre->right_;
			}
			cur->data_ = pre->data_;
			cur = pre;
		}

		Node* child = cur->left_;
		if (child == nullptr)
		{
			child = cur->right_;
		}

		if (child != nullptr)
		{
			child->parent_ = cur->parent_;
			if (cur->parent_ == nullptr)
			{
				//cur为root
				root_ = child;
			}
			else
			{
				if (cur->parent_->left_ == cur)
				{
					cur->parent_->left_ = child;
				}
				else
				{
					cur->parent_->right_ = child;
				}
			}

			Color c = color(cur);
			delete cur;

			if (c == BLACK)
			{
				fixAfterRemove(child);
			}
		}
		else
		{
			if (color(cur) == BLACK)
			{
				fixAfterRemove(cur);
			}

			if (cur->parent_->left_ == cur)
			{
				cur->parent_->left_ = nullptr;
			}
			else
			{
				cur->parent_->right_ = nullptr;
			}

			delete cur;
		}
	}

private:
	enum Color
	{
		RED,
		BLACK
	};

	struct Node
	{
		Node(T data = T(), Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr, Color color = BLACK)
			:data_(data)
			, parent_(parent)
			, left_(left)
			, right_(right)
			, color_(color)
		{

		}

		T data_;
		Node* left_;
		Node* right_;
		Node* parent_;
		Color color_;
	};

	Node* root_;

private:
	Color color(Node* node)
	{
		return node == nullptr ? BLACK : node->color_;
	}

	void setColor(Node* node, Color color)
	{
		node->color_ = color;
	}

	Node* left(Node* node)
	{
		return node->left_;
	}

	Node* right(Node* node)
	{
		return node->right_;
	}

	Node* parent(Node* node)
	{
		return node->parent_;
	}

	void inOrder(Node* node)
	{
		if (node != nullptr)
		{
			inOrder(node->left_);
			cout << node->data_ << " ";
			inOrder(node->right_);
		}
	}

	void leftRotate(Node* node)
	{
		Node* child = node->right_;
		child->parent_ = node->parent_;
		if (node->parent_ == nullptr)
		{
			root_ = child;
		}
		else
		{
			if (node->parent_->left_ == node)
			{
				node->parent_->left_ = child;
			}
			else
			{
				node->parent_->right_ = child;
			}
		}

		node->right_ = child->left_;
		if (child->left_ != nullptr)
		{
			child->left_->parent_ = node;
		}

		child->left_ = node;
		node->parent_ = child;
	}

	void rightRotate(Node* node)
	{
		Node* child = node->left_;
		child->parent_ = node->parent_;
		if (node->parent_ == nullptr)
		{
			root_ = child;
		}
		else
		{
			if (node->parent_->left_ == node)
			{
				node->parent_->left_ = child;
			}
			else
			{
				node->parent_->right_ = child;
			}
		}

		node->left_ = child->right_;
		if (child->right_ != nullptr)
		{
			child->right_->parent_ = node;
		}

		child->right_ = node;
		node->parent_ = child;
	}

	void fixAfterInsert(Node* node)
	{
		while (color(parent(node)) == RED)
		{
			if (left(parent(parent(node))) == parent(node))
			{
				//插入的节点在左子树中
				Node* uncle = right(parent(parent(node)));
				if (color(uncle) == RED)//情况一
				{
					setColor(parent(node), BLACK);
					setColor(uncle, BLACK);
					setColor(parent(parent(node)), RED);
					node = parent(parent(node));
				}
				else
				{
					//先处理情况三
					if (right(parent(node)) == node)
					{
						node = parent(node);
						leftRotate(node);
					}

					//统一处理情况二
					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					rightRotate(parent(parent(node)));
					break;
				}
			}
			else
			{
				//插入的节点在右子树中
				Node* uncle = left(parent(parent(node)));
				if (color(uncle) == RED)//情况一
				{
					setColor(parent(node), BLACK);
					setColor(uncle, BLACK);
					setColor(parent(parent(node)), RED);
					node = parent(parent(node));
				}
				else
				{
					//先处理情况三
					if (left(parent(node)) == node)
					{
						node = parent(node);
						rightRotate(node);
					}

					//统一处理情况二
					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					leftRotate(parent(parent(node)));
					break;
				}
			}
		}

		//此处强制root为黑色
		setColor(root_, BLACK);
	}

	void fixAfterRemove(Node* node)
	{
		while (color(node) == BLACK)
		{
			if (left(parent(node)) == node)
			{
				Node* bro = right(parent(node));
				if (color(bro) == RED)//情况四
				{
					setColor(parent(node), RED);
					setColor(bro, BLACK);
					leftRotate(parent(node));
					bro = right(parent(node));
				}

				if (color(left(bro)) == BLACK && color(right(bro)) == BLACK)//情况三
				{
					setColor(bro, RED);
					node = parent(node);
				}
				else
				{
					if (color(right(bro)) != RED)//情况二
					{
						setColor(bro, RED);
						setColor(left(bro), BLACK);
						rightRotate(bro);
						bro = right(parent(node));
					}

					//归结到情况一
					setColor(bro, color(parent(node)));
					setColor(parent(node), BLACK);
					setColor(right(bro), BLACK);
					leftRotate(parent(node));
					break;
				}
			}
			else
			{
				Node* bro = left(parent(node));
				if (color(bro) == RED)//情况四
				{
					setColor(parent(node), RED);
					setColor(bro, BLACK);
					rightRotate(parent(node));
					bro = left(parent(node));
				}

				if (color(left(bro)) == BLACK && color(right(bro)) == BLACK)//情况三
				{
					setColor(bro, RED);
					node = parent(node);
				}
				else
				{
					if (color(left(bro)) != RED)//情况二
					{
						setColor(bro, RED);
						setColor(right(bro), BLACK);
						leftRotate(bro);
						bro = left(parent(node));
					}

					//归结到情况一
					setColor(bro, color(parent(node)));
					setColor(parent(node), BLACK);
					setColor(left(bro), BLACK);
					rightRotate(parent(node));
					break;
				}
			}
		}

		setColor(node, BLACK);
	}
};

int main()
{
	RBTree<int> rbt;

	int arr[] = { 1,2,3,4,5,6,7,8,9,10 };
	for (int v : arr)
	{
		rbt.insert(v);
	}

	rbt.inOrder();

	rbt.remove(3);

	rbt.inOrder();

	return 0;
}