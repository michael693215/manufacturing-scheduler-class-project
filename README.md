# manufacturing scheduler demo
Class project for Data Structures class to simulate a manufacturing scheduler. 

Uses skew/leftist min/max heaps to arrange orders based on priority, and supports basic heap operations such insert and pop. Also supports merging multiple heap structures into one, which is what insert does with one of the heaps having just one node.

The priority function is not part of the class definition; main.cpp provides two priority functions. 

Additionally, a web-based visualization script can be found in index.html and bst.js. bst.js will read data.json, which contains the data structure created in main.cpp. Directions to use it are as follows:

1. Run main.cpp. This will prompt you for the heap type (MIN/MAX) and heap structure (SKEW/LEFTIST), then prompt you for whether you want to visualize it or not.
2. Open the index.html file in your browser. This will call bst.js, which reads the data.json file created in main.cpp.
3. Each node in the visualization will display the name as a tooltip if you hover your mouse over the yellow portion of it. The priority value in the center of the node is a separate html entity, so hovering on the number doesn't do anything. 
