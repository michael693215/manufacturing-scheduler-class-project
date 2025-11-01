#include <math.h>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>
#include "mqueue.hpp"
#include "Random.hpp"

using namespace std;
const char* PASS_STATEMENT = "TEST PASSED: ";
const char* FAIL_STATEMENT = "TEST FAILED: ";

/* TESTER */
class Tester{
public:
    bool insertLeftist(MQueue&, Order*, int);
    bool insertSkew(MQueue&, Order*, int);
    bool insertError(MQueue&);
    bool getNextOrderNormal(MQueue&);
    bool getNextOrderError(MQueue&);
    bool copyConstructorNormal(MQueue&);
    bool copyConstructorEdge(MQueue&);
    bool assignmentOperatorNormal(MQueue&, MQueue&);
    bool assignmentOperatorEdge(MQueue&, MQueue&);
    bool mergeWithQueueNormal(MQueue&, MQueue&);
    bool mergeWithQueueEdge(MQueue&, MQueue&);
    bool mergeWithQueueError(MQueue&, MQueue&);
    bool setPriorityFn(MQueue&);
    bool setStructure(MQueue&);
    // helper functions
    Node* highestPriorityT(MQueue&, Node*, Node*);
    bool highestPriorityT(MQueue&, Order, Order); // im too deep
    int getSize(Node*);
    bool isHeap(MQueue&, Node*);
    bool checkNPL(Node*);
    int calculateNPL(Node*);
    bool isLeftistHeap(Node*);
    bool isdeepCopy(Node*, Node*);
    bool notInHeap(Node*, Node*);
    void saveHeapT(Node**, int&, Node*);
};
/* PRIORITY FUNCTIONS */
int priorityFn1(const Order&);
int priorityFn2(const Order&);
/* MAIN */
int main(){
    Tester t;
    // establish ranges for order params
    Random name(97, 122);
    Random fifo(1, 50);
    Random processTime(1, 12);
    Random dueTime(1, 12);
    Random slackTime(1, 12);
    Random material(0, 100);
    Random importance(1, 100);
    Random workForce(100, 200);
    Random quantity(1000, 10000);
    // create 300 static orders for use in testing
    Order orderList[300];
    for (int i = 0; i < 300; i++){
        orderList[i] = Order(name.getRandString(5), fifo.getRandNum(), 
                                processTime.getRandNum(),dueTime.getRandNum(), 
                                slackTime.getRandNum(),material.getRandNum(), 
                                importance.getRandNum(),workForce.getRandNum(), 
                                quantity.getRandNum());
    }
    // test string
    string test;
    /* insert leftist */
    test = "insertLeftist()";
    cout << "Testing: " << test << endl;
    MQueue h1leftist(&priorityFn1, MINHEAP, LEFTIST);
    MQueue h2leftist(&priorityFn2, MAXHEAP, LEFTIST);
    // ensure insertion into a leftist heap is working for both min and max heaps
    if (t.insertLeftist(h1leftist, orderList, 300) and t.insertLeftist(h2leftist, orderList, 300)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* insertSkew */
    test = "insertSkew()";
    cout << "Testing: " << test << endl;
    MQueue h1skew(&priorityFn1, MINHEAP, SKEW);
    MQueue h2skew(&priorityFn2, MAXHEAP, SKEW);
    // ensure insertion into a skew heap is working for both min and max heaps
    if (t.insertSkew(h1skew, orderList, 300) and t.insertSkew(h2skew, orderList, 300)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* insertError */
    test = "insertError()";
    cout << "Testing: " << test << endl;
    if (t.insertError(h1skew)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* getNextOrderNormal */
    test = "getNextOrderNormal()";
    cout << "Testing: " << test << endl;
    // try to clear all the queues created up to this point
    if (t.getNextOrderNormal(h1leftist) and t.getNextOrderNormal(h1skew) and t.getNextOrderNormal(h2leftist) and t.getNextOrderNormal(h2skew)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* getNextOrderError */
    test = "getNextOrderError()";
    cout << "Testing: " << test << endl;
    // attempt to get an order off the heap for empty heaps
    if (t.getNextOrderError(h1leftist) and t.getNextOrderError(h1skew) and t.getNextOrderError(h2leftist) and t.getNextOrderError(h2skew)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* copyConstructorNormal */
    test = "copyConstructorNormal()";
    cout << "Testing: " << test << endl;
    // do some insertions on one of the empty heap objects 
    for (int i = 0; i < 50; i++){h1leftist.insertOrder(orderList[i]);}
    if (t.copyConstructorNormal(h1leftist)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* copyConstructorEdge */
    test = "copyConstructorEdge()";
    cout << "Testing: " << test << endl;
    // try to copy one of the empty heap objects
    if (t.copyConstructorEdge(h1skew)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* assignmentOperatorNormal */
    test = "assignmentOperatorNormal()";
    cout << "Testing: " << test << endl;
    MQueue ah1leftist(h1skew);
    // do some insertions so ah1leftist doesn't have an empty heap
    for (int i = 0; i < 50; i++){h1leftist.insertOrder(orderList[i]);}
    if (t.assignmentOperatorNormal(ah1leftist, h1leftist)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* assignmentOperatorEdge */
    test = "assignmentOperatorEdge()";
    cout << "Testing: " << test << endl;
    // h1skew is empty
    if (t.assignmentOperatorNormal(ah1leftist, h1skew)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* mergeWithQueueNormal */
    test = "mergeWithQueueNormal()";
    cout << "Testing: " << test << endl;
    // make two of the same kind of queue
    MQueue first(&priorityFn1, MINHEAP, LEFTIST);
    MQueue second(&priorityFn1, MINHEAP, LEFTIST);
    // insert elements into both of them 
    for (int i = 0; i < 300; i++){
        if (i < 150) first.insertOrder(orderList[i]);
        else second.insertOrder(orderList[i]);
    }
    // ok actually test now
    if (t.mergeWithQueueNormal(first, second)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* mergeWithQueueEdge */
    test = "mergeWithQueueEdge()";
    cout << "Testing: " << test << endl;
    if (t.mergeWithQueueEdge(second, first)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* mergeWithQueueError */
    test = "mergeWithQueueError()";
    cout << "Testing: " << test << endl;
    // make two different kinds of queue
    MQueue third(&priorityFn1, MINHEAP, LEFTIST);
    MQueue fourth(&priorityFn2, MAXHEAP, LEFTIST);
    // insert some elements into both of them
    for (int i = 0; i < 300; i++){
        third.insertOrder(orderList[i]);
        fourth.insertOrder(orderList[i]);
    }
    if (t.mergeWithQueueError(third, fourth)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* setPriorityFn */
    test = "setPriorityFn()";
    cout << "Testing: " << test << endl;
    // populate a heap 
    MQueue priority(&priorityFn1, MINHEAP, LEFTIST);
    MQueue priority2(&priorityFn1, MINHEAP, SKEW);
    for (int i = 0; i < 300; i++){priority.insertOrder(orderList[i]);}
    // ensure that the function works for both structure types
    if (t.setPriorityFn(priority) and t.setPriorityFn(priority2)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;}
    cout << endl;
    /* setHeapType */
    test = "setStructure()";
    cout << "Testing: " << test << endl;
    // populate another 2 heaps
    MQueue structure(&priorityFn1, MINHEAP, LEFTIST);
    MQueue structure2(&priorityFn1, MINHEAP, SKEW);
    for (int i = 0; i < 300; i++){
        structure.insertOrder(orderList[i]);
        structure2.insertOrder(orderList[i]);
    }
    if (t.setStructure(structure) and t.setStructure(structure2)){cout << PASS_STATEMENT << test;}
    else{cout << FAIL_STATEMENT << test;} 
    cout << endl;
    return 0;
    
}

/* TESTER FUNCTION DEFINITIONS */

// Function: insertLeftist(MQueue& heap, Order* arr, int arrSize)
// Preconditions: 
//   _inserting a node into a leftist heap adds a node to the heap
//   _after insertion, heap property is maintained at every node
//   _after insertion, all npl values in the heap are correct
//   _after insertion, leftist heap npl convention is maintained
//   _size of the heap is updated 
// Postconditions: insertion into a leftist heap is working as intended
bool Tester::insertLeftist(MQueue& heap, Order* arr, int arrSize){
    for (int i = 0; i < arrSize; i++){
        // insert a node into the heap
        if (!heap.insertOrder(arr[i])){return false;}
        // a node has been added to the heap
        if (heap.numOrders() != getSize(heap.m_heap)){return false;}
        // heap property is maintained at every node
        if (!isHeap(heap, heap.m_heap)){return false;}
        // all npl values are correct
        if (!checkNPL(heap.m_heap)){return false;}
        // leftist heap npl convention is maintained
        if (!isLeftistHeap(heap.m_heap)){return false;}
    }
    // size of the heap is updated
    if (heap.numOrders() != arrSize){return false;}
    // all tests pass return true
    return true;
}
// Function: insertSkew(MQueue& heap, Order* arr, int arrSize)
// Preconditions: 
//   _inserting a node into skew heap adds a node to the heap
//   _after insertion, heap property is maintained at every node
//   _after insertion, if root had higher priority than inserted node, 
//     root's left and right children are not the same as before insertion
//   _after insertion, if root had lower priority than inserted node, 
//     old root will be the left child of new root
//   _size of the heap is updated
// Postconditions: inserting in a skew heap is working as intended 
bool Tester::insertSkew(MQueue& heap, Order* arr, int arrSize){
    for (int i = 0; i < arrSize; i++){
        // root and its children from before the insertion
        Node* oroot = heap.m_heap;
        Node* lchild;
        Node* rchild;
        if (oroot != nullptr){lchild = oroot->m_left; rchild = oroot->m_right;}
        // insert a node into the heap 
        if (!heap.insertOrder(arr[i])){return false;}
        // inserting node adds node to the heap
        if (heap.numOrders() != getSize(heap.m_heap)){return false;}
        // after insertion, heap porperty is maintained at every node
        if (!isHeap(heap, heap.m_heap)){return false;}
        // root had higher (or equal) priority than inserted node, root's children are not the same
        if (oroot != nullptr and highestPriorityT(heap, oroot->getOrder(), arr[i])){
            if ((lchild != nullptr and oroot->m_left == lchild) or (rchild != nullptr and oroot->m_right == rchild)){return false;}
        }
        // root had lower priority than inserted node, root is left child of new root
        else{
            if (heap.m_heap->m_left != oroot){return false;}
        }
    }
    // size of the heap is updated
    if (heap.numOrders() != arrSize){return false;}
    // all checks passed, return true
    return true;
}
// Function: insertError(MQueue& queue)
// Preconditions: inserting an invalid order returns false and won't insert
// Postconditions: insert is owrking as intended for an error case
bool Tester::insertError(MQueue& queue){
    int size = queue.numOrders();
    // build an invalid order
    Order badOrder;
    // order has a nonexistent priority
    if (queue.getPriorityFn()(badOrder) != 0){return false;}
    // insertOrder returns false if trying to insert bad order
    if (queue.insertOrder(badOrder)){return false;}
    // heap size doesn't change
    if (queue.numOrders() != size){return false;}
    // all checks pass true
    return true;
}
// Function: getNextOrderNormal(MQueue& heap)
// Preconditions: 
//   _getNextOrder() replaces the root with the next highest priority order in the heap
//   _size of the heap decrements by 1
// Postconditions: getNextOrder is working as intended for a normal case
bool Tester::getNextOrderNormal(MQueue& heap){
    // numOrders works as intended for insertions, and only insertions have been done so far
    int expectedSize = heap.numOrders();
    int initialSize = expectedSize;
    for (int i = 0; i < initialSize; i++){
        // it's still a heap, and if it's leftist, npl conventions are maintained
        if (!isHeap(heap, heap.m_heap)){return false;}
        if (heap.getStructure() == LEFTIST and !isLeftistHeap(heap.m_heap)){return false;}
        // store the expected new root as whicherver of old root's children had higher priority
        // in case of both children having the same priority, left child will take precedence
        Node* expectedRoot = nullptr;
        if (heap.m_heap->m_right == nullptr or heap.m_heap->m_left == highestPriorityT(heap, heap.m_heap->m_left, heap.m_heap->m_right)){
            expectedRoot = heap.m_heap->m_left;
        }
        else{expectedRoot = heap.m_heap->m_right;}
        // pop the root and adjust the expected size
        heap.getNextOrder();
        expectedSize--;
        // expected root matches new root
        if (expectedRoot != heap.m_heap){return false;}
        // expected size matches actual size
        if (expectedSize != heap.numOrders()){return false;}       
    }
    // heap is empty
    if (heap.numOrders() != 0){return false;}
    // all tests pass retur ntrue
    return true;
}
// Function: getNextOrderError(MQueue& heap)
// Preconditions: heap has no elements so an out_of_range exception is thrown
// Postconditions: getNextOrder() works as intended for an error case
bool Tester::getNextOrderError(MQueue& heap){
    try{heap.getNextOrder();}
    // an exception was thrown by heap
    catch(out_of_range &e){return true;}
    // exception was not thrown
    return false;
}
// Function: copyConstructorNormal()
// Preconditions: constructing an object with copy constructor creates a deep copy
// Postconditions: copy constructor works as intended for a normal case
bool Tester::copyConstructorNormal(MQueue& rhs){
    // make a copy of rhs
    MQueue lhs(rhs); 
    // member variables are the same
    if (lhs.numOrders() != rhs.numOrders() or
        lhs.getPriorityFn() != rhs.getPriorityFn() or
        lhs.getHeapType() != rhs.getHeapType() or
        lhs.getStructure() != rhs.getStructure()){
        return false;
    }
    // lhs is a deep copy of rhs
    if (!isdeepCopy(lhs.m_heap, rhs.m_heap)){return false;}
    // all tests pass return true
    return true;
}
// Function: copyConstructorEdge(MQueue& rhs)
// Preconditions: 
//   _copying an empty object still returns a deep copy of that empty object
// Postconditions: copy constructor is working as intended for an edge case
bool Tester::copyConstructorEdge(MQueue& rhs){
    // rhs is empty
    if (rhs.m_heap != nullptr){return false;}
    // make a copy of rhs
    MQueue lhs(rhs); 
    // member variables are the same
    if (lhs.numOrders() != rhs.numOrders() or
        lhs.getPriorityFn() != rhs.getPriorityFn() or
        lhs.getHeapType() != rhs.getHeapType() or
        lhs.getStructure() != rhs.getStructure()){
        return false;
    }
    // lhs is a deep copy of rhs
    if (!isdeepCopy(lhs.m_heap, rhs.m_heap)){return false;}
    // all tests pass return true
    return true;
}
// Function: assignmentOperatorNormal(MQueue& lhs, MQueue& rhs)
// Preconditions: assignment results in two identical queues with different pointers
// Postconditions: assignment operator is working as intended for a normal case
bool Tester::assignmentOperatorNormal(MQueue& lhs, MQueue& rhs){
    // lhs and rhs are different objects
    if (lhs.numOrders() == rhs.numOrders() and
        lhs.getPriorityFn() == rhs.getPriorityFn() and
        lhs.getHeapType() == rhs.getHeapType() and
        lhs.getStructure() == rhs.getStructure()){
        return false;
    }
    // assign lhs to rhs
    lhs = rhs; 
    // lhs and rhs are now equivalent but not the exact same
    if (lhs.numOrders() != rhs.numOrders() or
        lhs.getPriorityFn() != rhs.getPriorityFn() or
        lhs.getHeapType() != rhs.getHeapType() or
        lhs.getStructure() != rhs.getStructure()){
        return false;
    }
    // i assume it's supposed to deep copy
    if (!isdeepCopy(lhs.m_heap, rhs.m_heap)){return false;}
    // all tests pass return true
    return true;
}
// Function: assignmentOperatorEdge(MQueue& lhs, MQueue& rhs)
// Preconditions: assigning to an empty queue still results in two identical empty queues with different pointers
// Postconditions: assignment operator works as intended for an edge case
bool Tester::assignmentOperatorEdge(MQueue& lhs, MQueue& rhs){
    // rhs has an empty heap and rhs doesn't
    if (rhs.m_heap != nullptr or lhs.m_heap == nullptr){return false;}
    // assign lhs to rhs
    lhs = rhs; 
    // lhs and rhs are now equivalent but not the exact same
    if (lhs.numOrders() != rhs.numOrders() or
        lhs.getPriorityFn() != rhs.getPriorityFn() or
        lhs.getHeapType() != rhs.getHeapType() or
        lhs.getStructure() != rhs.getStructure()){
        return false;
    }
    // lhs heap is empty after assignment
    if (lhs.m_heap != nullptr){return false;}
    // all tests pass return true
    return true;
}
// Function: mergeWithQueueNormal(MQueue& lhs, MQueue& rhs)
// Preconditions: 
//   _merging two of the same type of queue will result in the host queue inheriting the other queue's heap
//   _after merging, the other queue is empty
//   _after merging, the host queue maintains whatever structural conventions it had
// Postconditions: merge is working as intended for a normal case
bool Tester::mergeWithQueueNormal(MQueue& lhs, MQueue& rhs){
    // store the rhs heap's root and the expected size of lhs after merging
    Node* temp = rhs.m_heap;
    int size = lhs.numOrders() + rhs.numOrders();
    // merge the queues
    lhs.mergeWithQueue(rhs);
    // lhs structure is still a heap
    if (!isHeap(lhs, lhs.m_heap)){return false;}
    // if it's a leftist, npl conventions maintained
    if (lhs.getStructure() == LEFTIST and !isLeftistHeap(lhs.m_heap)){return false;}
    // rhs heap's former root is in lhs somewhere, and heap's size is correct
    if (notInHeap(lhs.m_heap, temp) or lhs.numOrders() != size){return false;}
    // rhs queue is empty
    if (rhs.m_heap != nullptr or rhs.numOrders() != 0){return false;}
    // all tests pass return true
    return true;
}
// Function: mergeWithQueueEdge(MQueue& lhs, MQueue& rhs)
// Preconditions: merging an empty queue with a normal queue will transfer all the stuff to the empty queue
// Postconditions: merge is working as intneded for an edge case
bool Tester::mergeWithQueueEdge(MQueue& lhs, MQueue& rhs){
    // lhs is empty
    if (lhs.m_heap != nullptr or lhs.numOrders() != 0){return false;}
    // store the expected size after merge (should be rhs)
    int size = rhs.numOrders();
    lhs.mergeWithQueue(rhs);
    // lhs structure is still a heap
    if (!isHeap(lhs, lhs.m_heap)){return false;}
    // if it's a leftist, npl conventions maintained
    if (lhs.getStructure() == LEFTIST and !isLeftistHeap(lhs.m_heap)){return false;}
    // heap size is correct
    if (lhs.numOrders() != size){return false;}
    // rhs queue is empty
    if (rhs.m_heap != nullptr or rhs.numOrders() != 0){return false;}
    // all tests pass return true
    return true;
}
// Function: mergeWithQueueError(MQueue& lhs, MQueue rhs)
// Preconditions: 
//   _merging an invalid queue doesn't do anything
//   _merging the same queue into itself throws a domain error
// Postconditions: merge is working as intended for an error case
bool Tester::mergeWithQueueError(MQueue& lhs, MQueue& rhs){
    // store the old sizes
    int size1 = lhs.numOrders();
    int size2 = rhs.numOrders();
    // try merge the queues
    try {lhs.mergeWithQueue(rhs);}
    // merging queues throws an exception
    catch (domain_error &e){
        // the queues will not change in size, and rhs queue will still have a heap
        if (lhs.numOrders() != size1 or rhs.numOrders() != size2 or rhs.m_heap == nullptr){return false;}
        // merge queue into itself
        lhs.mergeWithQueue(lhs);
        // size doesn't change and lhs still has a heap
        if (lhs.numOrders() != size1 or lhs.m_heap == nullptr){return false;}
        // all tests pass return true
        return true;
    }
    // no exception was thrown
    return false;
}
// Function: setPriorityFn(MQueue queue)
// Preconditions: 
//   _queue still has a valid heap after changing the priority function
//   _old root is still in the heap (so probably the heap has the same stuff)
// Postconditions: setPriorityFn is working as intended
bool Tester::setPriorityFn(MQueue& queue){
    int size = queue.numOrders();
    // put heap in an array
    int index = 0;
    Node* heapArr[size];
    saveHeapT(heapArr, index, queue.m_heap);
    // store priority function and type
    prifn_t pf = queue.getPriorityFn();
    HEAPTYPE ht = queue.getHeapType();
    // queue has a valid heap at the moment
    if (!isHeap(queue, queue.m_heap)){return false;}
    // queue is a leftist heap
    if (!isLeftistHeap(queue.m_heap)){return false;}
    // change the priority function (also heaptype)
    queue.setPriorityFn(pf == priorityFn1 ? priorityFn2 : priorityFn1, ht == MINHEAP ? MAXHEAP : MINHEAP);
    // size didn't change
    if (queue.numOrders() != size){return false;}
    // priority function and heap type are changed
    if (queue.getPriorityFn() == pf or queue.getHeapType() == ht){return false;}
    // all elemenets of old heap arrangement are elemenets in new heap arrangemenet
    for (int i = 0; i < size; i++){
        if (notInHeap(queue.m_heap, heapArr[i])){return false;}
    }
    // queue is still a valid heap
    if (!isHeap(queue, queue.m_heap)){return false;}
    // queue is still a valid leftist heap (if it was a leftist heap in the first place)
    if (queue.getStructure() == LEFTIST and !isLeftistHeap(queue.m_heap)){return false;}
    // all tests pass return true
    return true;
}
// Function: setStructure(MQueue queue)
// Preconditions: 
//   _if queue is a leftist heap and is being changed to a skew heap, the heap itself doesn't immediately change
//   _if queue is a skew heap being changed to a leftist heap, the heap will change
//   _queue still has a valid heap after changing the structure
//   _old root is still in the heap (so probably the heap has the sam estuff)
// Postconditions: setStructure is working as intended
bool Tester::setStructure(MQueue& queue){
    // heap was a leftist heap
    if (queue.getStructure() == LEFTIST){
        // old root
        Node* oroot = queue.m_heap;
        // set structure to itself
        queue.setStructure(LEFTIST);
        // root doesn't change
        if (queue.m_heap != oroot){return false;}
        // structure is still leftist
        if (queue.getStructure() != LEFTIST){return false;}
        if (!isLeftistHeap(queue.m_heap)){return false;}
        // set structure to skew
        queue.setStructure(SKEW);
        // root doesn't change
        if (queue.m_heap != oroot){return false;}
        // heap structure is skew now though
        if (queue.getStructure() != SKEW){return false;}
        // it's still a heap
        if (!isHeap(queue, queue.m_heap)){return false;}
        // all tests pass return true
        return true;
    }
    // heap was a skew heap
    else{
        Node* oroot = queue.m_heap;
        // store the queue in an array and also get its size
        int heapsize = queue.numOrders();
        int index = 0;
        Node* heapArr[heapsize];
        saveHeapT(heapArr, index, queue.m_heap);
        // set structure to itself
        queue.setStructure(SKEW);
        // root doesn't change
        if (queue.m_heap != oroot){return false;}
        // set structure to leftist
        queue.setStructure(LEFTIST);
        // it's a heap
        if (!isHeap(queue, queue.m_heap)){return false;}
        // heap is a leftist heap now
        if (!isLeftistHeap(queue.m_heap)){return false;}
        // size doesn't change
        if (queue.numOrders() != heapsize){return false;}
        // every element of the original queue arrangement is a element of the new queue arrangement
        for (int i = 0; i < heapsize; i++){
            if (notInHeap(queue.m_heap, heapArr[i])){return false;}
        }
        // all checks pass return true
        return true;
    }
}
/* TESTER HELPER FUNCTION DEFINITIONS */

// Function: getSize(Node* heap)
// Preconditions: none
// Postconditions: return number of nodes in the heap
int Tester::getSize(Node* heap){
    if (heap == nullptr){return 0;}
    return 1 + getSize(heap->m_left) + getSize(heap->m_right);
}
// Function: isHeap(MQueue& heap, Node* heap)
// Preconditions: none
// Postconditions: return true if heap doesn't violate min/max heap properties, false otherwise
bool Tester::isHeap(MQueue& heap, Node* node){
    if (node == nullptr){return true;}
    // left child has higher priority than the parent
    if (node->m_left != nullptr){
        if (node->m_left == highestPriorityT(heap, node, node->m_left)){
            return false;
        }
    }
    // right child has higher priority than the parent
    else if (node->m_right != nullptr){
        if (node->m_right == highestPriorityT(heap, node, node->m_right)){
            return false;
        }
    }
    // left or right child violate min/max heap properties
    if (!isHeap(heap, node->m_left) or !isHeap(heap, node->m_right)){return false;}
    // it's a valid min/max heap
    return true;
}
// Function: checkNPL(Node* heap)
// Preconditions: none
// Postconditions: return true if NPL value for each node is accurate, false otherwise
bool Tester::checkNPL(Node* node){
    if (node == nullptr){return true;}
    // npl of node doesn't match true npl
    if (node->getNPL() != calculateNPL(node)){return false;}
    // npl of children nodes don't match true npl
    if (!checkNPL(node->m_left) or !checkNPL(node->m_right)){return false;}
    // npl is good
    return true;
}
// Function: calculateNPL(Node* node)
// Preconditions: none
// Postconditions: npl value of a node is returned
int Tester::calculateNPL(Node* node){
    // -1 so that a node with no children should have a 0 npl
    if (node == nullptr){return -1;}
    // npl is the lesser of the right and left subheap npls
    int leftNPL = 1 + calculateNPL(node->m_left); 
    int rightNPL = 1 + calculateNPL(node->m_right);
    return leftNPL < rightNPL ? leftNPL : rightNPL;
}
// Function: isLeftistHeap(Node* node)
// Preconditions: none
// Postconditions: return true if left npl of every node is >= right npl, false otherwise
bool Tester::isLeftistHeap(Node* node){
    if (node == nullptr){return true;}
    // left npl is less than right npl
    if (calculateNPL(node->m_left) < calculateNPL(node->m_right)){return false;}
    // left or right subtree isn't a leftist heap
    if (!isLeftistHeap(node->m_left) or !isLeftistHeap(node->m_right)){return false;}
    // heap is a leftist heap
    return true;
}
// Function: highestPriorityT(Node* h1, Node* h2)
// Preconditions: if the root is being compared, it should be h1
// Postconditions: node with highest priority is returned
Node* Tester::highestPriorityT(MQueue& heap, Node* h1, Node* h2){
    if (h1 == nullptr){return h2;}
    if (h2 == nullptr){return h1;}
    // h1 node has higher (or equal) priority than h2 node
    if ((heap.getPriorityFn()(h1->getOrder()) >= heap.getPriorityFn()(h2->getOrder()) and heap.getHeapType() == MAXHEAP) or
        (heap.getPriorityFn()(h1->getOrder()) <= heap.getPriorityFn()(h2->getOrder()) and heap.getHeapType() == MINHEAP)){
        return h1;
    }
    // h2 has higher priority than h2 node
    return h2;
}
// Function: highestPriorityT(Order o1, Order o2)
// Preconditions: if root's order is being compared, it should be h1
// Postconditions: return true if left o1 has higher priority, false otherwise
bool Tester::highestPriorityT(MQueue& heap, Order o1, Order o2){
    // h1 node has higher (or equal) priority than h2 node
    if ((heap.getPriorityFn()(o1) >= heap.getPriorityFn()(o2) and heap.getHeapType() == MAXHEAP) or
        (heap.getPriorityFn()(o1) <= heap.getPriorityFn()(o2) and heap.getHeapType() == MINHEAP)){
        return true;
    }
    // h2 has higher priority than h2 node
    return false;
}
// Function: isdeepCopy(MQueue& lhs, MQueue& lhs, Node* lheap, Node* rheap)
// Preconditions: none
// Postconditions: return true if two heaps are deep copy, false otherwise
bool Tester::isdeepCopy(Node* lheap, Node* rheap){
    // a node is nullptr
    if (lheap == nullptr or rheap == nullptr){
        // both nodes have to be nullptr
        if (lheap != nullptr or rheap != nullptr){return false;}
        // both nodes are nullptr
        return true;
    }
    // order objects are the same
    if (lheap->getOrder().getCustomer() != rheap->getOrder().getCustomer() or
        lheap->getOrder().getFIFO() != rheap->getOrder().getFIFO() or
        lheap->getOrder().getProcessTime() != rheap->getOrder().getProcessTime() or
        lheap->getOrder().getDueTime() != rheap->getOrder().getDueTime() or
        lheap->getOrder().getSlackTime() != rheap->getOrder().getSlackTime() or
        lheap->getOrder().getMaterial() != rheap->getOrder().getMaterial() or
        lheap->getOrder().getImportance() != rheap->getOrder().getImportance() or
        lheap->getOrder().getWorkForce() != rheap->getOrder().getWorkForce() or
        lheap->getOrder().getQuantity() != rheap->getOrder().getQuantity()){
        return false;
    }
    // nodes are not the same
    if (lheap == rheap){return false;}
    // children are also deep copies
    if (!isdeepCopy(lheap->m_left, rheap->m_left) or !isdeepCopy(lheap->m_right, rheap->m_right)){return false;}
    // it's a deep copy
    return true;
}
// Function: notInHeap(Node* heap, Node* node)
// Preconditions: none
// Postconditions: true if node is not in heap, false otherwise
bool Tester::notInHeap(Node* heap, Node* node){
    // it's not here
    if (heap == nullptr){return true;}
    // it's in
    if (node == heap){return false;}
    // it's in children
    if (!notInHeap(heap->m_left, node) or !notInHeap(heap->m_right, node)){return false;}
    // it's not in heap
    return true;
}
// Function: saveHeapT(Node**, int&, Node*)
// Preconditions:
//   _index starts at zero
//   _array has as many posotions as the heap has elements
// Postconditions: the heap is saved into an array
void Tester::saveHeapT(Node** array, int& index, Node* heap){
    if (heap == nullptr){return;}
    saveHeapT(array, index, heap->m_left);
    saveHeapT(array, index, heap->m_right);
    array[index] = heap;
    index++;
}
/* priority functions */
int priorityFn1(const Order & order){
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1101-10400]
    int minValue = MIN1000 + MAX100 + MINONE + MINZERO;
    int maxValue = MAX100 + MAX100 + MAX200 + MAX10000;
    //the larger value means the higher priority
    int priority = order.getMaterial() + order.getImportance() + 
                    order.getWorkForce() + order.getQuantity();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}
int priorityFn2(const Order & order){
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [4-86]
    int minValue = MINONE + MINONE + MINONE + MINONE;
    int maxValue = MAX12 + MAX12 + MAX12 + MAX50;
    //the smaller value means the higher priority
    int priority = order.getFIFO() + order.getProcessTime() + 
                order.getDueTime() + order.getSlackTime();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}
