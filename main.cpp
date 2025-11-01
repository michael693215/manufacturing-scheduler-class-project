// CMSC 341 - Fall 2024 - Project 3
#include <iostream>
#include <fstream>
#include "mqueue.hpp"
#include "Random.hpp"

const int TRUE = 0;
const int FALSE = 1;
const int MINSIZE = 1;
const int MAXSIZE = 50;

// function to write the heap into a json file
void writeHeap(MQueue heap);
// function to reduce repetition in getting order parameters from input
void getInput(std::string prompt, int& answer, int min, int max);
// function to populate heap with random orders
void populateHeapRandom(MQueue& heap);
// function to create the demo heap that is to be displayed 
MQueue buildHeap();
// main program loop
void loop();
/* priority functions */
int priorityFn1(const Order & order);
int priorityFn2(const Order & order);

int main()
{
    loop();    
    return 0;
}

// function to write the heap to a json file
void writeHeap(MQueue heap){
    // create the json file and object that gets written to 
    std::ofstream file("data.json");
    json j = json::array();
    j.push_back(json::array());
    j.push_back(json::array());

    // write the heap to json file 
    heap.writeHeap(j);
    file << j << std::endl;
}

// function to reduce repetition in getting order parameters from input
void getInput(std::string prompt, int& answer, int min, int max){
    do
    {
        std::cout << prompt << std::endl;
        std::cin >> answer;
    } while(answer < min || answer > max);
}

// function that inserts 'size' number of random elements into the heap
void populateHeapRandom(MQueue& heap){
    int strlen = 5; // arbitrary length of the 'name' string
    int size = 15;  // number of insertions to be made
    // order parameters are created with Random 
    Random str(97, 122); // for ascii
    Random importance(MINONE, MAX100);
    Random material(MINZERO, MAX100);
    Random workForce(MAX100, MAX200);
    Random quantity(MIN1000, MAX10000);
    Random time(MINONE, MAX12);
    Random fifo(MINONE, MAX50);
   
    // insert random orders into the heap
    std::cout << "Inserting " << size << " orders into the heap..." << std::endl;
    for (int i = 0; i < size; i++){
        Order newOrder(str.getRandString(strlen), fifo.getRandNum(), time.getRandNum(), time.getRandNum(), time.getRandNum(), material.getRandNum(), importance.getRandNum(), workForce.getRandNum(), quantity.getRandNum());
        heap.insertOrder(newOrder);
    }
}

// function to create the demo heap that is to be displayed 
MQueue buildHeap(){
    int indexer = -1;        // enum indexer
    HEAPTYPE heapType;       // heap type
    STRUCTURE heapStructure; // data structure
                        
    // get heap type 
    getInput("Enter the heap type ('0' for a MINHEAP, '1' for MAXHEAP)", indexer, MINHEAP, MAXHEAP);
    heapType = static_cast<HEAPTYPE>(indexer);

    // get structure
    indexer = -1;
    getInput("Enter the heap structure ('0' for SKEW, '1' for LEFTIST)", indexer, SKEW, LEFTIST); 
    heapStructure = static_cast<STRUCTURE>(indexer);

    // return the heap object
    return MQueue(heapType == MAXHEAP ? &priorityFn1 : &priorityFn2, heapType, heapStructure);
}

// main program loop
void loop(){
    int input; // to be used for yes or no queries
    // for "heap created" message 
    std::string msg = "Successfully created: ";
    // create the heap
    MQueue demo = buildHeap();

    // populate the heap
    populateHeapRandom(demo);
    msg += (demo.getHeapType() == MAXHEAP ? "MAX " : "MIN ");
    msg += (demo.getStructure() == SKEW ? "SKEW HEAP." : "LEFTIST HEAP.");
    std::cout << msg << std::endl; 

    // print the queue
    demo.printOrderQueue();

    // optionally load the queue into a json file for visualization
    msg = "Would you like to see a visualization of the heap? ('0' for NO, '1' for YES)";
    getInput(msg, input, TRUE, FALSE);
    if (input) writeHeap(demo); 
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

