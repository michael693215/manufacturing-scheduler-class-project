#include "mqueue.hpp"
#include <stdexcept>
// Function: Constructor
MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) : m_heap(nullptr), m_size(0), m_priorFunc(priFn), m_heapType(heapType), m_structure(structure){}
// Function: Destructor
MQueue::~MQueue(){clear();}
// Function: clear
void MQueue::clear(){
    // pop off the root until the queue is empty
    while (m_heap != nullptr){getNextOrder();}
}
// Function: Copy Constructor
MQueue::MQueue(const MQueue &rhs){
    // copy static members
    m_size = rhs.numOrders(); 
    m_priorFunc = rhs.getPriorityFn();
    m_heapType = rhs.getHeapType();
    m_structure = rhs.getStructure();
    // copy the nodes
    m_heap = copyHeap(rhs.m_heap);
}
// Function: Assignment Operator
MQueue& MQueue::operator=(const MQueue& rhs){
    // rhs object and this object are the same, return this object
    if (this->m_heap == rhs.m_heap){return *this;}
    // clear this object
    clear();
    // copy static members
    m_size = rhs.numOrders(); 
    m_priorFunc = rhs.getPriorityFn();
    m_heapType = rhs.getHeapType();
    m_structure = rhs.getStructure();
    // copy the nodes
    m_heap = copyHeap(rhs.m_heap);
    return *this;
}
// Function: insertOrder
bool MQueue::insertOrder(const Order& input){
    // new order has an invalid priority 
    if (getPriorityFn()(input) == 0){return false;}
    // insert the node; merge will handle special operations for heap types
    merge(m_heap, buildNode(input));
    // increment size by 1
    m_size++;
    return true;
}
// Function: getNextOrder
Order MQueue::getNextOrder(){
    // if the heap is empty, throw out_of_range exception
    if (m_heap == nullptr){throw out_of_range("heap is empty");}
    // root order information
    Order order = m_heap->getOrder();
    Node* root = m_heap;
    // replace the root with the nxt highest priority order and decrease size by 1
    m_heap = merge(root->m_left, root->m_right);
    m_size--; 
    // delete the old root and return its order information
    delete root;
    root = nullptr;
    return order;
}
// Function: mergeWithQueue
void MQueue::mergeWithQueue(MQueue& rhs){
    // merging with itself results in abortion
    if (m_heap == rhs.m_heap){return;}
    // merging with an incompatible queue throws exception
    if (getPriorityFn() != rhs.getPriorityFn() or
        getHeapType() != rhs.getHeapType() or
        getStructure() != rhs.getStructure()){
        throw domain_error("incompatible data type");
    }
    // merge the two heaps and adjust size 
    merge(m_heap, rhs.m_heap);
    m_size += rhs.numOrders();
    // make rhs empty 
    rhs.m_heap = nullptr;
    rhs.m_size = 0;
}
// Function: setPriorityFn
void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType){
    // store the heap in an array  
    Node* heapArr[numOrders()];
    int index = 0;
    saveHeap(heapArr, index, m_heap);     
    // unlink all the heap elements
    m_heap = unlinkHeap(m_heap);
    // change the function and type
    m_priorFunc = priFn;
    m_heapType = heapType;
    // reinsert all the elements
    for (int i = 0; i < numOrders(); i++){merge(m_heap, heapArr[i]);}
}
// Function: setStructure
void MQueue::setStructure(STRUCTURE structure){
    // if structure is being changed to itself do not do anything
    if (structure == getStructure()){return;}
    // if structure is being changed from leftist to skew, just change the structure and return
    if (getStructure() == LEFTIST){m_structure = structure; return;}
    // store the heap in an array  
    Node* heapArr[numOrders()];
    int index = 0;
    saveHeap(heapArr, index, m_heap);     
    // unlink all the heap elements
    m_heap = unlinkHeap(m_heap);
    // change the structure
    m_structure = structure;
    // reinsert all the elemenets
    for (int i = 0; i < numOrders(); i++){merge(m_heap, heapArr[i]);}
}
// GETTERS
// Function: numOrders
int MQueue::numOrders()const{return m_size;}
// Function: getPriorityFn()
prifn_t MQueue::getPriorityFn()const{return m_priorFunc;}
// Function: getHeapType()
HEAPTYPE MQueue::getHeapType()const{return m_heapType;}
// Function: getStructure()
STRUCTURE MQueue::getStructure()const{return m_structure;}
// dump 
void MQueue::dump() const{
  if (m_size == 0){
    cout << "Empty heap.\n";
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void MQueue::dump(Node *pos) const{
  if (pos != nullptr){
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer;
    else
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}
ostream& operator<<(ostream& sout, const Order& order){
  sout  << "Customer: " << order.getCustomer()
        << ", importance: " << order.getImportance() 
        << ", quantity: " << order.getQuantity() 
        << ", order placement time: " << order.getFIFO();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node){
  sout << node.m_order;
  return sout;
}
// Function: printOrderQueue() const
void MQueue::printOrderQueue() const{
    printPreorder(m_heap);
}
// PRIVATE FUNCTIONS
// Function: buildNode(Order order)
// Preconditions: none
// Postconditions: a pointer to the newly allocated node is returned
Node* MQueue::buildNode(Order order){Node* node = new Node(order); return node;}
// Function: copyHeap(Node* rhs)
// Preconditions: none 
// Postconditions: a deep copy of rhs heap is returned
Node* MQueue::copyHeap(Node* rhs){
    // current node is null, return nullptr
    if (rhs == nullptr){return nullptr;}
    // build the current node
    Node* node = buildNode(rhs->getOrder());
    // recurse for left child
    node->m_left = copyHeap(rhs->m_left);
    // recurse for right child
    node->m_right = copyHeap(rhs->m_right);
    // return node for recursion
    return node;
}
// Function: merge(Node* h1, Node* h2)
// Preconditions: initial call has h1 as root of heap
// Postconditions: h1 and h2 nodes will be merged into one heap, root is returned for recursive purposes
Node* MQueue::merge(Node* h1, Node* h2){
    // heap is empty, set h2 as root, return h2
    if (m_heap == nullptr){m_heap = h2; h2->m_npl = 0; return h2;}
    // if h1 is nullptr, return h2
    if (h1 == nullptr){return h2;}
    // if h2 is nullptr, return h1
    if (h2 == nullptr){return h1;}
    // h1 node has higher (or equal) priority than h2 node
    if (h1 == highestPriority(h1, h2)){
        // if heap is a skew heap, swap the children before merging to the left child
        if (getStructure() == SKEW){
            swapChildren(h1, h1->m_left, h1->m_right);
            h1->m_left = merge(h1->m_left, h2);
        }
        // otherwise merge h2 with h1's right child
        else{h1->m_right = merge(h1->m_right, h2);}
        // recalculate the npl, as adding a new node down the right child can affect npl
        if (getStructure() == LEFTIST){calculateNPL(h1);}
        // return h1 as the root of this subheap
        return h1;
    }
    // h2 node has higher priority than h1 node
    else{
        // if h1 was the root, make h2 the new root
        if (m_heap == h1){m_heap = h2;}
        // if heap is a skew heap, swap the children before merging to the left child
        if (getStructure() == SKEW){
            swapChildren(h2, h2->m_left, h2->m_right);
            h2->m_left = merge(h2->m_left, h1);
        }
        // otherwise merge h1 with h2's right child
        else{h2->m_right = merge(h2->m_right, h1);}
        // recalculate the npl, as adding a new node down the right child can affect npl
        calculateNPL(h2);
        // return h2 as root of this subheap
        return h2;
    }
}
// Function: calculateNPL(Node* node)
// Preconditions: node isn't a nullptr
// Postconditions: node's npl is stored in m_npl and returned.
//   also if heapType is leftist, convention is maintained
int MQueue::calculateNPL(Node* node){
    // -1 so that a node with no children will have a 0 npl
    if (node == nullptr){return -1;}
    // npl is the lesser of the right and left subheap npls
    int leftNPL = 1 + calculateNPL(node->m_left); 
    int rightNPL = 1 + calculateNPL(node->m_right);
    // if heap is leftist, and npl violates convention, swap the children
    if (getStructure() == LEFTIST and leftNPL < rightNPL){swapChildren(node, node->m_left, node->m_right);}
    // set the npl of the node to be the lesser of the left and right npls
    node->setNPL(leftNPL < rightNPL ? leftNPL : rightNPL);
    // return the npl to facilitate recursion
    return node->getNPL();
}
// Function: swapChildren(Node* parent, Node* lchild, Node* rchild)
// Preconditions: 1st param is parent, 2nd param is left child, 3rd param is right child
// Postconditions: left and right children are swapped
void MQueue::swapChildren(Node* parent, Node* lchild, Node* rchild){
    parent->m_left = rchild;
    parent->m_right = lchild;
}
// Function: highestPriority(Node* h1, Node* h2)
// Preconditions: if the root is being compared, it should be h1
// Postconditions: node with highest priority is returned
Node* MQueue::highestPriority(Node* h1, Node* h2){
    if (h1 == nullptr){return h2;}
    if (h2 == nullptr){return h1;}
    // h1 node has higher (or equal) priority than h2 node
    if ((getPriorityFn()(h1->getOrder()) >= getPriorityFn()(h2->getOrder()) and getHeapType() == MAXHEAP) or
        (getPriorityFn()(h1->getOrder()) <= getPriorityFn()(h2->getOrder()) and getHeapType() == MINHEAP)){
        return h1;
    }
    // h2 has higher priority than h2 node
    return h2;
}
// Function: saveHeap(Node** array, int& index, Node* heap)
// Preconditions: 
//   _index starts at 0
//   _array has as many positions as the heap has elemenets
// Postconditions: the heap is stored in array
void MQueue::saveHeap(Node** array, int& index, Node* heap){
    if (heap == nullptr){return;}
    saveHeap(array, index, heap->m_left);
    saveHeap(array, index, heap->m_right);
    array[index] = heap;
    index++;
}
// Function: unlinkHeap(Node* heap)
// Preconditions: none
// Postconditions: all links under heap are destroyed 
Node* MQueue::unlinkHeap(Node* heap){
    if (heap == nullptr){return nullptr;}
    heap->m_left = unlinkHeap(heap->m_left);
    heap->m_right = unlinkHeap(heap->m_right);
    return nullptr;
}
// Function: printPreorder(Node* heap)
// Preconditions: none
// Postconditions: all the stuff in the heap is printed in preorder fashion
void MQueue::printPreorder(Node* heap) const{
    // can't print a null pointer
    if (heap == nullptr){return;}
    // print the stuff
    cout << "[" << getPriorityFn()(heap->getOrder()) << "] " 
        << "Customer: " << heap->getOrder().getCustomer() 
        << ", importance: " << heap->getOrder().getImportance()
        << ", quantity: " << heap->getOrder().getQuantity()
        << ", order placement time: " << heap->getOrder().getFIFO() << endl;
    // print the left and then the right
    printPreorder(heap->m_left);
    printPreorder(heap->m_right);
}

// Preconditions: none
// Postconditions: call a function to write heap to json object
void MQueue::writeHeap(json& j){
    writeHeapPreorder(m_heap, j);
    writeHeapInorder(m_heap, j);
}
// Preconditions: none
// Postconditions: all the stuff in the heap is written to a json object in
// preorder fashion
void MQueue::writeHeapPreorder(Node* heap, json& j){
    // can't read a nullptr
    if (heap == nullptr){return;}
    // write the elements of the heap into json j
    j[0].push_back({{"priority", getPriorityFn()(heap->getOrder())}, {"name", heap->getOrder().getCustomer()}});
    
    // print the left then the right
    writeHeapPreorder(heap->m_left, j);
    writeHeapPreorder(heap->m_right, j);
}
// Preconditions: none
// Postconditions: all the stuff in the heap is written to a json object in
// inorder fashion
void MQueue::writeHeapInorder(Node* heap, json& j){
    // can't read a nullptr
    if (heap == nullptr){return;}

    // print left first
    writeHeapInorder(heap->m_left, j);

    // write the elements of the heap into json j
    j[1].push_back({{"priority", getPriorityFn()(heap->getOrder())}, {"name", heap->getOrder().getCustomer()}});
    
    // print the right
    writeHeapInorder(heap->m_right, j);
}
