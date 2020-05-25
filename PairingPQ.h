// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure to
            // initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent { nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
			Node *parent;
            // TODO: Add one extra pointer (parent or previous) as desired.
    }; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
	explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
		BaseClass{ comp } {
		root = nullptr;
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
	PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
		BaseClass{ comp } {
		root = nullptr;
		while (start != end)
		{
			addNode(*start);
			start++;
		}
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        BaseClass{ other.compare } {
		std::deque<Node*> data;
		data.push_back(other.root);
		root = nullptr;
		// this might give u headache
		Node* currentNode = other.root;
		while (!data.empty())
		{
			currentNode = data.front();
			data.pop_front();
			if (currentNode->child)
			{
				data.push_back(currentNode->child);
			}
			if (currentNode->sibling)
			{
				data.push_back(currentNode->sibling);
			}
			addNode(currentNode->elt);
		}
		//delete currentNode;
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
		std::deque<Node*> data;
		data.push_back(rhs.root);
		root = nullptr;
		// this might give u headache
		Node* currentNode = rhs.root;
		while (!data.empty())
		{
			currentNode = data.front();
			if (currentNode->child)
			{
				data.push_back(currentNode->child);
			}
			if (currentNode->sibling)
			{
				data.push_back(currentNode->sibling);
			}
			addNode(currentNode->elt);
			data.pop_front();
		}
		//delete currentNode;
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
		std::deque<Node*> data;
		data.push_back(root);
		// this might give u headache
		while (!data.empty())
		{
			if (data.front()->child)
			{
				data.push_back(data.front()->child);
			}
			if (data.front()->sibling)
			{
				data.push_back(data.front()->sibling);
			}
			data.front()->sibling = nullptr;
			data.front()->child = nullptr;
			data.front()->parent = nullptr;
			delete data.front();
			data.pop_front();
		}
		//delete currentNode;
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
		//cout << "Entered updatePrio" << endl;
		std::deque<Node*> data;
		Node* currentNode = root;
		data.push_back(currentNode);
		root = nullptr;
		// this might give u headache
		//cout << "Before the Loop" << endl;
		while (!data.empty())
		{
			currentNode = data.front();
			data.pop_front();
			if (currentNode->child)
			{
				data.push_back(currentNode->child);
			}
			if (currentNode->sibling)
			{
				data.push_back(currentNode->sibling);
			}
			currentNode->child = nullptr;
			currentNode->sibling = nullptr;
			currentNode->parent = nullptr;
			if (!root)
			{
				root = currentNode;
			}
			else
			{
				meld(currentNode, root);
			}
		}
		//cout << "After the Loop" << endl;
		//delete currentNode;
    } // updatePriorities()


    // Description: Add a new element to the priority_queue. This is almost done,
    //              in that you should implement push functionality in the addNode()
    //              function, and this function should call addNode().
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
		addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
		std::deque<Node*> dq;
		if (!root->child)
		{
			delete root;
			root = nullptr;
			sz--;
			return;
		}
		Node* temp = root->child;
		dq.push_back(temp);
		//cout << "Before Loop 1" << endl;
		while (temp->sibling)
		{
			temp = temp->sibling;
			dq.push_back(temp);
		}
		//cout << "Before Loop 2" << endl;
		while (dq.size() != 1)
		{
			//cout << "Entered Loop 2" << endl;
			//cout << "Size: " << dq.size() << endl;
			dq[0]->sibling = nullptr;
			dq[0]->parent = nullptr;
			dq[1]->sibling = nullptr;
			dq[1]->parent = nullptr;
			//cout << "Progress 1" << endl;
			//cout << "Progress 2" << endl;
			dq.push_back(meld(dq[0], dq[1]));
			//cout << "Progress 3" << endl;
			dq.pop_front();
			dq.pop_front();
		}
		//cout << "After Loop 2" << endl;
		delete root;
		sz--;
		root = dq.front();

    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
		return root->elt;
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
		return sz;
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
		return sz == 0;
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
		node->elt = new_value;
		if (node == root)
		{
			return;
		}
		if (this->compare(new_value, node->parent->elt))
		{
			return;
		}
		if (node->parent->child == node)
		{
			if (node->sibling)
			{
				node->parent->child = node->sibling;
			}
			else
			{
				node->parent->child = nullptr;
			}
		}
		else
		{
			Node* temp = node->parent->child;
			while (temp->sibling != node)
			{
				temp = temp->sibling;
			}
			if (!node->sibling)
			{
				temp->sibling = nullptr;
			}
			else
			{
				temp->sibling = node->sibling;
			}
		}
		node->parent = nullptr;
		node->sibling = nullptr;
		meld(node, root);

    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE & val) {
		//cout << "Entered meld" << endl;
		Node* p = new Node(val);
		if (!root)
		{
			sz++;
			root = p;
			//cout << "Exited meld" << endl;
			return p;
		}
		meld(p, root);
		sz++;
		//cout << "Exited meld" << endl;
        return p; // TODO: Delete or change this line
    } // addNode()


private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
	
	Node *root = nullptr;
	size_t sz = 0;
	Node* meld(Node* a, Node* b)
	{
		if (this->compare(a->elt, b->elt))
		{
			a->sibling = b->child;
			b->child = a;
			a->parent = b;
			if (a == root)
			{
				root = b;
			}
			return b;
		}
		else
		{
			b->sibling = a->child;
			a->child = b;
			b->parent = a;
			if (b == root)
			{
				root = a;
			}
			return a;
		}
	}
};


#endif // PAIRINGPQ_H
