Node* buildBalanced(const std::vector<T>& start, int l, int end)
{
        if (start > end) return nullptr;
        int m = (start + end) / 2;
        Node* node = new Node(a[m]);
        node->left  = buildBalanced(a, stara, m - 1);
        node->right = buildBalanced(a, m + 1, end);
        return node;
}



// C++ Program to Implement Balanced Binary Tree
#include <algorithm>
#include <iostream>
using namespace std;

template <typename T> class BalancedBinaryTree {
private:
    // Node structure definition
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;

        Node(T value) : data(value), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    // Function to get the height of the node
    int height(Node* node)
    {
        return node ? node->height : 0;
    }

    // Function to get the balance factor of the node
    int balanceFactor(Node* node)
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // Function to update the height of the node
    void updateHeight(Node* node)
    {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    // Right rotation function
    Node* rotateRight(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left rotation function
    Node* rotateLeft(Node* x)
    {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Function to insert a node
    Node* insert(Node* node, T key)
    {
        // Perform the normal BST insertion
        if (!node)
            return new Node(key);

        if (key < node->data)
            node->left = insert(node->left, key);
        else if (key > node->data)
            node->right = insert(node->right, key);
        else
            return node; // Duplicate keys are not allowed

        // Update height of this ancestor node
        updateHeight(node);

        // Get the balance factor to check whether this node
        // became unbalanced
        int balance = balanceFactor(node);

        // If the node becomes unbalanced, there are 4 cases

        // Left Left Case
        if (balance > 1 && key < node->left->data)
            return rotateRight(node);

        // Right Right Case
        if (balance < -1 && key > node->right->data)
            return rotateLeft(node);

        // Left Right Case
        if (balance > 1 && key > node->left->data) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->data) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // Function to find the node with the minimum value
    // (used in deletion)
    Node* findMin(Node* node)
    {
        while (node->left)
            node = node->left;
        return node;
    }

    // Function to delete a node
    Node* remove(Node* node, T key)
    {
        // Perform standard BST delete
        if (!node)
            return nullptr;

        if (key < node->data)
            node->left = remove(node->left, key);
        else if (key > node->data)
            node->right = remove(node->right, key);
        else
        {
            if (!node->left || !node->right)
            {
                Node* temp
                    = node->left ? node->left : node->right;
                if (!temp)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
              
                delete temp;
            }
            else
            {
                Node* temp = findMin(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }

        if (!node)
            return nullptr;

        // Update height of the current node
        updateHeight(node);

        // Get the balance factor
        int balance = balanceFactor(node);

        // Balance the node if it has become unbalanced

        // Left Left Case
        if (balance > 1 && balanceFactor(node->left) >= 0)
            return rotateRight(node);

        // Left Right Case
        if (balance > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Right Case
        if (balance < -1 && balanceFactor(node->right) <= 0)
            return rotateLeft(node);

        // Right Left Case
        if (balance < -1
            && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // Function to search for a key in the tree
    bool search(Node* node, T key)
    {
        if (!node)
            return false;
        if (node->data == key)
            return true;
        if (key < node->data)
            return search(node->left, key);
      
        return search(node->right, key);
    }

    // Function for inorder traversal of the tree
    void inorderTraversal(Node* node)
    {
        if (node)
        {
            inorderTraversal(node->left);
            cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

public:
    // Constructor
    BalancedBinaryTree() : root(nullptr) {}

    // Public insert function
    void insert(T key) { root = insert(root, key); }

    // Public remove function
    void remove(T key) { root = remove(root, key); }

    // Public search function
    bool search(T key) { return search(root, key); }

    // Public function to print the inorder traversal
    void printInorder()
    {
        inorderTraversal(root);
        cout << endl;
    }
};

int main()
{
    BalancedBinaryTree<int> tree;

    // Insert elements
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    cout << "Inorder traversal of the constructed Balanced "
            "Binary Tree"
         << endl;
    tree.printInorder();

    // Search for a key
    int searchKey = 30;
    cout << "Searching for key " << searchKey << ": "
         << (tree.search(searchKey) ? "Found" : "Not Found")
         << endl;

    // Remove a key
    int removeKey = 20;
    tree.remove(removeKey);
    cout << "Inorder traversal after removing " << removeKey
         << ": ";
    tree.printInorder();

    return 0;
}
