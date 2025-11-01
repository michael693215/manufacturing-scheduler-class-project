const MINHEAP = 0;
const MAXHEAP = 1;
const RADIUS = 25;

//BST class for heap nodes
class BST{
    constructor(canvasX, canvasY, heapType, preorder, inorder){
        this.mCanvasX = canvasX;
        this.mCanvasY = canvasY;
        this.mSize = 0;
        this.mHeapType = heapType;
        this.mPreorder = preorder;
        this.mInorder = inorder;
        this.mIndex = 0;   // for traversing array
        this.mRoot = this.buildTree(this.mPreorder, this.mInorder, 0);
        this.mWeightX = RADIUS; // for assigning x-coordinate
        this.mOffsetX = this.mWeightX/2; // for assigning x-coordinate
        this.mScalarX = 0; // for assigning x-coordinate
        this.mWeightY = 5 * RADIUS; // for assigning y-coordinate
        this.mOffsetY = this.mWeightY/2;
        this.setPos(this.mRoot);
    }

    // function that builds the tree from heapified preorder and inorder arrays
    buildTree(preorder, inorder, depth){
        if (!inorder || inorder.length == 0){
            return null;
        }
        // pop the next element off preorder list and let root of this subtree
        // absorb its values
        let rootVal = preorder.shift();
        let root = new Node(rootVal.priority, rootVal.name, depth);
        
        // next_depth represents the depth of the left and right child nodes
        const nextDepth = depth + 1;
        // hopefully priority and name make a primary key
        const rootIndex = inorder.findIndex(x => x.priority === rootVal.priority && x.name === rootVal.name);
        const leftTree = inorder.slice(0, rootIndex);
        const rightTree = inorder.slice(rootIndex + 1);

        // recurse into left and right subtrees
        root.mLeft = this.buildTree(preorder, leftTree, nextDepth);
        root.mRight = this.buildTree(preorder, rightTree, nextDepth);

        ++this.mSize;
        return root;
    }

    // function to assign x and y coordinate of all the nodes in the tree
    setPos(root){
        this.setX(root);
        this.setY(root);
    }

    // function that assigns x-coordinate of all the nodes in the tree
    setX(root){
        if (root == null) return;
        this.setX(root.mLeft);
        root.mX = 3 * RADIUS * this.mScalarX + this.mOffsetX;
        ++this.mScalarX;
        this.setX(root.mRight);
    }
    
    // function that assigns y-coordinate of all the nodes in the tree
    setY(root){
        if (root == null) return;
        this.setY(root.mLeft);
        root.mY = root.mDepth * 3 * RADIUS + this.mOffsetY;
        this.setY(root.mRight);
    }
}

// node class for heap BST
class Node{
    constructor(priority, name, depth = 0){
        this.mPriority = priority;
        this.mName = name;
        this.mDepth = depth;
        this.mLeft = null;
        this.mRight = null;
        this.mX = this.mY = 0;
    }

}

// function to make tree
async function loadTree(canvas){
    // put the json file into an array
    let bst = null;
    const response = await fetch('data.json');
    const bstArray = await response.json();
    const preorder = bstArray[0];
    const inorder = bstArray[1];
    
    // get the size of the canvas
    const canvasX = parseFloat(getComputedStyle(canvas).width);
    const canvasY = parseFloat(getComputedStyle(canvas).height);
    
    // put the array into a bst object
    if (preorder[0].priority < preorder[1].priority){
        bst = new BST(canvasX, canvasY, MINHEAP, preorder, inorder);
    } else{
        bst = new BST(canvasX, canvasY, MAXHEAP, preorder, inorder);
    }
    return bst;
}

// adds lines between nodes 
function drawLines(root, canvas) {
    if (root == null) return;

    if (root.mLeft) {
        const line = document.createElementNS("http://www.w3.org/2000/svg", "line");
        line.setAttribute("x1", root.mX);
        line.setAttribute("y1", root.mY);
        line.setAttribute("x2", root.mLeft.mX);
        line.setAttribute("y2", root.mLeft.mY);
        line.setAttribute("stroke", "black");
        canvas.appendChild(line);

        drawLines(root.mLeft, canvas);
    }

    if (root.mRight) {
        const line = document.createElementNS("http://www.w3.org/2000/svg", "line");
        line.setAttribute("x1", root.mX);
        line.setAttribute("y1", root.mY);
        line.setAttribute("x2", root.mRight.mX);
        line.setAttribute("y2", root.mRight.mY);
        line.setAttribute("stroke", "black");
        canvas.appendChild(line);

        drawLines(root.mRight, canvas);
    }
}

// adds nodes from bst to the svg container
function populateCanvas(root, canvas){
    if (root == null){
        return;
    }

    populateCanvas(root.mLeft, canvas);

    let x = root.mX;
    let y = root.mY;

    // node
    const order = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
    order.setAttribute('class', 'node');
    order.setAttribute('r', RADIUS);
    order.setAttribute('cx', x);
    order.setAttribute('cy', y);
    
    // priority number
    const number = document.createElementNS('http://www.w3.org/2000/svg', 'text');
    number.textContent = root.mPriority;
    number.setAttribute('x', x);
    number.setAttribute('y', y);
    number.setAttribute('text-anchor', 'middle');
    number.setAttribute('dominant-baseline', 'middle');
    number.setAttribute('fill', 'black');
    let fontSize = RADIUS * .5;
    number.setAttribute('font-size', fontSize);

    const title = document.createElementNS('http://www.w3.org/2000/svg', 'title');
    title.textContent = root.mName;

    order.appendChild(title);
    canvas.appendChild(order);
    canvas.appendChild(number);

    populateCanvas(root.mRight, canvas);
}

// recomputing canvas size to accomodate overflow
function recomputeCanvas(root){
    if (!root) return { minX: Infinity, maxX: -Infinity, minY: Infinity, maxY: -Infinity };

    const left = recomputeCanvas(root.mLeft);
    const right = recomputeCanvas(root.mRight);

    const minX = Math.min(root.mX, left.minX, right.minX);
    const maxX = Math.max(root.mX, left.maxX, right.maxX);
    const minY = Math.min(root.mY, left.minY, right.minY);
    const maxY = Math.max(root.mY, left.maxY, right.maxY);

    return { minX, maxX, minY, maxY };


}

// main function
async function driver(){
    canvas = document.getElementById('canvas');
    const bst = await loadTree(canvas);
    console.log(bst);

    // add the nodes to the canvas
    drawLines(bst.mRoot, canvas);
    populateCanvas(bst.mRoot, canvas);
    const bounds = recomputeCanvas(bst.mRoot);

    const padding = 50; 
    canvas.setAttribute('width', bounds.maxX - bounds.minX + padding*2);
    canvas.setAttribute('height', bounds.maxY - bounds.minY + padding*2);
}

// call main function
driver();
