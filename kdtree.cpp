/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cfloat>
#include <cmath>
//#include <math.h>    //pow
//#include <stdlib.h> //abs

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first, const Point<Dim>& second, int curDim) const{
    //tie breaker
    if(first[curDim] == second[curDim])
      return first < second;

    //return smaller Dimension Value
    return first[curDim] < second[curDim];

}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest, const Point<Dim>& potential) const{

    //calculate the cur distance between target and best
    double cur = 0.0;
    for(int i = 0; i < Dim; i++){
      cur += (target[i]-currentBest[i])*(target[i]-currentBest[i]);
    }

    //calculate the distance between target and potential
    double pot = 0.0;
    for(int i = 0; i < Dim; i++){
      pot += (target[i]-potential[i])*(target[i]-potential[i]);
    }
    //tie breaker
    if(pot == cur)
      return potential < currentBest;
    else
    //return the smaller distance
     return pot < cur;
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& points, int left, int right, int pivotIndex, int currDim){
  //swap right and pivot index
  Point<Dim> temp = points[right], pivotval = points[pivotIndex];
  points[right] = pivotval;
  points[pivotIndex] = temp;

  int storeIndex = left;
  //move all smaller elements to the left and larger to the right
    for (int i = left; i < right; i++) {
        if (smallerDimVal(points[i], pivotval, currDim)){ //points[i][currDim] < pivotval
            //swap(points[storeIndex], points[i]);
            temp = points[storeIndex];
            points[storeIndex] = points[i];
            points[i] = temp;
            storeIndex++;
        }
    }
    // Move pivot to its final position
    //swap(points[right], points[storeIndex]); 
    temp = points[right];
    points[right] = points[storeIndex];
    points[storeIndex] = temp;
    return storeIndex;
}

template <int Dim>
int KDTree<Dim>::Qselect(vector<Point<Dim>>& points, int left, int right, int k, int currDim){
    int pivotIndex;
    // If the list contains only one element,
    if (left == right)        
      return left;  // return that element

    // select a pivotIndex between left and right,
    pivotIndex = (left + right) / 2;    

    pivotIndex = partition(points, left, right, pivotIndex, currDim);
     // The pivot is in its final sorted position
    if (k == pivotIndex)
        return k;
    else if (k < pivotIndex)
        return Qselect(points, left, pivotIndex - 1, k, currDim);
    else
        return Qselect(points, pivotIndex + 1, right, k, currDim);

}

//function to handle bulding the tree recursicvely.
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(KDTreeNode*& subroot, vector<Point<Dim>>& points, int left, int right, int currDim){

//BASE CASE... just in case I guess
if(left > right)
  return NULL;

//BASE CASE... A leaf, create new node
else if(left == right){
    subroot = new KDTreeNode(points[left]);
    subroot->left = NULL;
    subroot->right = NULL;
    return subroot;
  }
else{
  int idx;
  int median = (left + right) / 2;
  //find the median and partition the left and right
  idx = Qselect(points, left, right, median, currDim); 

  //create new internal node
  subroot = new KDTreeNode(points[idx]);

  //incrememnt splitting dimension and call recursively
  subroot->left = buildTree(subroot->left, points, left, idx - 1, (currDim + 1) % Dim);
  subroot->right = buildTree(subroot->right, points, idx + 1, right, (currDim + 1) % Dim);
  return subroot;
  }
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
  //initialize root to NULL otheriwise the destructor on an empty tree will SEG FAULT
  root = NULL;
  //empty tree
  if(newPoints.empty())
    return;
  else{
    int left = 0, right = newPoints.size() - 1;
    int currDim = 0;

      //copy over the vector
      points = newPoints;
      //build tree recrusively
      root = buildTree(root, points, left, right, currDim);
    }

}

//helper function for Operator= and copy constructor
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copyHelper(KDTreeNode*& subroot){
  if(subroot == NULL)
    return NULL;

  //make a new node and copy over data then call recursively
  KDTreeNode* copy = new KDTreeNode(subroot->point);
  copy->left = copyHelper(subroot->left);
  copy->right = copyHelper(subroot->right);
  return copy;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree& other) {
  size = other.size;
  //build identical tree recrusively
  root = copyHelper(other.root);

}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree& rhs) {
  if(this != &rhs){ //avoid self copy
    if(root != NULL) // root is not empty ->> delete it
      destroyTree(root);

    if(rhs.root == NULL) //other is empty
      root = NULL;
    else
      //build identical tree recursively
      root = copyHelper(rhs.root);
      size = rhs.size();
  }

  return *this;
}

//helper function for Destructor
template <int Dim>
void KDTree<Dim>::destroyTree(KDTreeNode*& subroot){
  if(subroot != NULL){
    //delete left and right subtrees if needed
  if(subroot->left != NULL)
    destroyTree(subroot->left);
  if(subroot->right != NULL)
    destroyTree(subroot->right);

  delete subroot;
  subroot = NULL;
  }
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  //destroy tree recursively
  destroyTree(root);
}

//Helper function for find nearest neighbor
template <int Dim>
void KDTree<Dim>::FNNHelper(KDTreeNode* subroot, const Point<Dim>& target, Point<Dim>& CurBest, int curDim, double& curRad) const{
  Point<Dim> curPoint;
  double newRad;
  if(subroot == NULL)
    return;
  //leaf node
  if(subroot->left == NULL && subroot->right == NULL){ 
    //update curBest pt, and curRad if needed
    if(shouldReplace(target, CurBest, subroot->point)){ 
      CurBest = subroot->point;
      newRad = 0;
      for(int i = 0; i < Dim; i++){
        newRad += pow((target[i] - CurBest[i]), 2);
      }
      newRad = sqrt(newRad);
      curRad = newRad;
      return;
    }
  }
  //traverse down the tree comparing with current dimension.
  if(target == subroot->point){
    CurBest = subroot->point;
    curRad = 0;
    return;
  }
  //if target point is smaller -> traverse left subtree
  if(smallerDimVal(target, subroot->point, curDim)) 
    FNNHelper(subroot->left, target, CurBest, (curDim+1) % Dim, curRad);
  else //traverse right subtree
    FNNHelper(subroot->right, target, CurBest, (curDim+1) % Dim, curRad);

  //climbing back up the tree now
  //update curBest, and curRad if needed
  if(shouldReplace(target, CurBest, subroot->point)){ 
      CurBest = subroot->point;
      newRad = 0;
      for(int i = 0; i < Dim; i++){
        newRad += pow((target[i] - CurBest[i]), 2);
      }
      newRad = sqrt(newRad);
      curRad = newRad;
    }
    //splitting plane is within the raduis, search other subtree
    if(abs(subroot->point[curDim] - target[curDim]) <= curRad){ 
      //search the opposite subtree than what was already searched
      if(smallerDimVal(target, subroot->point, curDim))
        FNNHelper(subroot->right, target, CurBest, (curDim+1) % Dim, curRad);
      else //traverse right subtree
        FNNHelper(subroot->left, target, CurBest, (curDim+1) % Dim, curRad);
    }

  return;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    //call helper function
    Point<Dim> Best;
    double temp = -1.0;
    FNNHelper(root, query, Best, 0, temp);
    return Best;
}
