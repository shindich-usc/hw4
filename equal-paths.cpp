#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int pathLength(Node * root);

// Returns length of tree path given equal paths
int pathLength(Node * root)
{
    if (root->left == nullptr && root->right == nullptr) {return 1;}
    if (root->left == nullptr) {return pathLength(root->right) + 1;}
    return pathLength(root->left) + 1;
}


bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr) {return true;}
    // Base Case: If node has no children (leaf), return true
    if (root->left == nullptr && root->right == nullptr) {return true;}
    // If node only has one child, then node has equal paths only if child does
    if (root->left == nullptr) {return equalPaths(root->right);}
    if (root->right == nullptr) {return equalPaths(root->left);}
    // If node has two children, then:
    // Return false if either child has unequal paths
    if (!(equalPaths(root->left) && equalPaths(root->right))) {return false;}
    // Otherwise, check if any left path has equal length to any right path
    return pathLength(root->left) == pathLength(root->right);
}

