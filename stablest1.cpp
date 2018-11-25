// File Compression using Huffman Compression
// by Shardul Kulkarni and Aditya Ujalambkar



#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<fstream>
using namespace std;

int wordCount=0;
float newCount=0  ;
string bitcodeHold[256];
int maxHold=-1;

void init(){
  for(int i=0;i<255;i++)
    bitcodeHold[i]="-1";
}


#define MAX_HT 1000

// A Huffman tree node
struct MinHeapNode {
    char data;
    unsigned freq;

    struct MinHeapNode *left, *right;
};


// MinHeap type to store all the nodes at one place
struct MinHeap {

    // Size of Minheap,,
    unsigned size;

    unsigned capacity;
    struct MinHeapNode** array; //Stores address of all nodes
};


//creates newNode ,, defines character and frequency values to node
struct MinHeapNode* newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

// create MinHeap, as collection of minHeapNodes
struct MinHeap* createMinHeap(unsigned capacity)

{

    struct MinHeap* minHeap = new MinHeap;

    //initially size is 0
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to
// swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a,struct MinHeapNode** b)

{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// minHeapify
void minHeapify(struct MinHeap* minHeap, int idx)

{

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
      smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

//
// check if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}

// to get min valued node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

// insert
// a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)

{

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

//  build min heap
void buildMinHeap(struct MinHeap* minHeap)

{

    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// to print the individual bitcode generated during traversal in tree
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        cout<<arr[i];

    cout<<endl;
}

// to check whether the node is leaf or not
int isLeaf(struct MinHeapNode* root)

{

    return !(root->left) && !(root->right);
}

// creates a minheap , with nodes equal to number of capacity,
// inserts the data[] of character and fq in it.
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)

{

    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// The function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)

{
    struct MinHeapNode *left, *right, *top;
    //Creating minheap of capacity = size
    //initially number of node =size

    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    //continue loop till no.of node become 1 in heap
    while (!isSizeOne(minHeap)) {

        //get two nodes with min fq from heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Creating a parent node stores sum of child leaf fq
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // the last remaining node will be root node..
    //thus tree is complete
    return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[], int top)

{

    // Assign 0 to left edge and recur
    if (root->left) {

        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right) {

        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node,
  //print the leaf character and bitcode
  //saving the the character and bitcode for encoding purpose
    if (isLeaf(root)) {
        /*long int bitcode="";
        for(int k=0;k<top;k++)
          bitcode=bitcode*10 + arr[k];
        maxHold++;*/
        string bit="";
        for(int k=0; k<top;k++)
          bit=bit + (char)(arr[k] + '0');


        //charHold[maxHold]=root->data;


        // bitcode stored to its ascii valued index
        //saves furter searching complexity
        bitcodeHold[root->data]=bit;

        cout<<root->data<<"=";
        printArr(arr, top);


    }
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size)

{

    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    // Print Huffman bitcodes using
    // the Huffman tree built above
    int arr[MAX_HT], top = 0;

    printCodes(root, arr, top);

    wordCount=root->freq;
}

// counts the frequency of character in file
int count(string inFile,char *arr, int* freq )
{
  int i=0;
  ifstream file;
  file.open(inFile,ios::in);
  int temp[256]={0};

  while(!file.eof()){
    temp[file.get()]++;
  //  cout<<endl<<char(file.get());
  }
int j=-1;
for( i=0; i<255;i++){
      if(temp[i] > 0){
        j++;
        arr[j]=char(i);
        freq[j]=temp[i];
      }
    }



    return j+1;
}

float getCompressionRation(){
  ifstream output;
  output.open("huffout.txt",ios::in);

  while(!output.eof()){
      output.get();
      newCount++;
    }
    cout<<"\n\n Earlier="<<wordCount*8<<" bits";
    cout<<"\n After compression="<<newCount<<" bits";

    newCount=(float)newCount/8;
  float ratio = (((float)wordCount - (float)newCount)/(float)wordCount)*100.00;


  return ratio;

}

//encodes the file by substituting bitcode at place of its char
void encode(string inFile)
{
  ifstream input;
  input.open(inFile,ios::in);

  ofstream output;
  output.open("huffout.txt",ios::out);

  for(int i=0; i<255; i++){
      if(bitcodeHold[i]!="-1")
      output<<char(i)<<bitcodeHold[i];
  }
  output<<endl;

  while(!input.eof()){
    output<<bitcodeHold[input.get()];

  }
  output.close();
  input.close();
}


int main()
{
    init();
    string inFile;
    cout<<"\n=================================================";
    cout<<"\n\t\tFile Compression";
    cout<<"\n=================================================";
    cout<<"\n\n\t Enter file location (.txt):";
    cin>>inFile;
    char arr[256];
    int freq[256] = {0};

    int size = count(inFile, arr, freq);
  //  cout<<size;


    cout<<"\nHuffman codes for each character are :\n";
    HuffmanCodes(arr, freq, size);
    encode(inFile);
    cout<<"\n\n\a Compressed file name: 'huffout.txt'";

    float r=getCompressionRation();
    cout<<"\n\tCompression Ratio= "<<r;
    cout<<endl<<endl;

      /*for(int i=0;i<255;i++){
        if(bitcodeHold[i] !="-1")
          cout<<endl<<char(i)<<bitcodeHold[i];
      }*/


    return 0;
}


/*
shardulind@advaita:~/Abhyas/school/project/HUffmanBeta1$ ls
a.out  huffout.txt  stablest1.cpp  test1.txt  test.txt
shardulind@advaita:~/Abhyas/school/project/HUffmanBeta1$ ^C
shardulind@advaita:~/Abhyas/school/project/HUffmanBeta1$ ./a.out

=================================================
		File Compression
=================================================

	 Enter file location (.txt):test.txt

Huffman codes for each character are :
c=0
b=10

=110
a=111


 Compressed file name: 'huffout.txt'

 Earlier=56 bits
 After compression=28 bits
	Compression Ratio= 50

shardulind@advaita:~/Abhyas/school/project/HUffmanBeta1$ ./a.out

=================================================
		File Compression
=================================================

	 Enter file location (.txt):test1.txt

Huffman codes for each character are :
r=0000
m=00010
l=00011
e=001
s=0100
o=0101
b=011000
y=011001
d=01101
a=0111
p=10000
x=10001000
q=100010010
E=1000100110
I=1000100111
+=1000101
u=100011
n=1001
t=1010
i=1011
 =110
c=11100
O=1110100000
P=1110100001
S=1110100010
W=1110100011
k=111010010
T=111010011
w=11101010
A=111010110
v=111010111
h=111011

=1111000
.=1111001
,=11110100
-=11110101
j=11110110
C=11110111
f=111110
g=111111


 Compressed file name: 'huffout.txt'

 Earlier=7576 bits
 After compression=4501 bits
	Compression Ratio= 59.4113

*/


/*
This files were compressed
test.txt
--------------------------------
abbccc
*/

/*
test1.txt
-----------------------------------
C++ is a general purpose programming language designed to make programming more enjoyable
for the serious programmer. Except for minor details, C++ is a superset of the C programming lan-
guage. In addition to the facilities provided by C, C++ provides flexible and efficient facilities for
defining new types. A programmer can partition an application into manageable pieces by defining
new types that closely match the concepts of the application. This technique for program construc-
tion is often called data abstraction. Objects of some user-defined types contain type information.
Such objects can be used conveniently and safely in contexts in which their type cannot be deter-
mined at compile time. Programs using objects of such types are often called object based. When
used well, these techniques result in shorter, easier to understand, and easier to maintain programs.
The key concept in C++ is class. A class is a user-defined type

*/


/* huffout.txt OUtput file (COmpressed of test1.txt)

1111000 110+1000101,11110100-11110101.1111001A111010110C11110111E1000100110I1000100111O1110100000P1110100001S1110100010T111010011W1110100011a0111b011000c11100d01101e001f111110g111111h111011i1011j11110110k111010010l00011m00010n1001o0101p10000q100010010r0000s0100t1010u100011v111010111w11101010x10001000y011001
11110111100010110001011101011010011001111101111110011001001000001110001111010000100011000010000010101000011101000000000101111111000001110001000010101110011111111100001101111001111111100011011111111100111001101001010010111111111001001011011101010010111000010011111101001000111010000000001011111110000011100010000101011100111111111000010010100000011100011001111101100101011001011101100000011001111100011111001010000110101011101100111001000010000101101011000110100110100000000010111111100000111000100001000100001111001110100010011010001000111000011000010101101111100101000011000010101110010101000011001101001101001111011000110100111101001101111011110001011000101110101101001100111110010010001110000001000001000011010110010111111011010101110110011101111011111010000000001011111110000011100010000101011100111111111000011011110011111010111110001111111000110111111111001111100111010001001111001110011101101011011011101010110101100111010100101110101011101100111011111001111110010110001110111010101100101001101000000000101111010111101101101001011011100110000110011101111011111110100110111101111000101100010111010000000001011110101111011011010010100110111110000110011000100010110110000001100111001111001011011100011111101111101011111001011001100110101101111100111111001011000111011101010110010100110111110010100001111000011010011111101011100110111001111111110100100111101010110101001100110000001010011110011101110101101101000000000101111111000001110001000010001000011011100011110011101000001110000101010111010101101011001110011110011100111100001000000011101111100011110101011010110011101011100110100101110000100111100101111111110010111011000000110011101000010110011110000101001100110000110011100110100111111010111001101110011111111111000100100111101010110101001100110000001010011010101110110111101011011100000110101010000100011011001110000100111101011100111011110101011101100111011100010110011110000110000101001001100101111110110101011101100111001111000010000000111011111000111101010110101100111110011101110100111110111011010011010100011110011101110011011100010010100011001110111110010100001101000000000101111111000001110001011011100010110010100101000001000111110011110101111100010101011010110011101011010011001011111101010001100111011100011100011000110010110111001101011110100111110011101100001001010000001111110010101011010110011111001110111010000001100011110110001111001010010011001011111101100100010100010001110100011010000100001111010101101001111110101110010010110111010100110011000000101001101110001011001101001111011100111010100110011000000111010111001111110010100000001001111010101101011001111100111110001110100010100011111001110111100101011000111101100011110010100100110111000111100111001100000111010001101000010110111011100010110011110101110011001101100110011010000110110011100111100101101110010001111111100010001101100111010111001110111000101100110100011000100010100100110101110011101110101011101110111110011101111010101110110011011000011010100110011000000111011100011110011001010110101100110000011100110100110100010000111101011111000000101011100100101101110011110101101110001010001010000101100011001110101010110001000111110011101110100001000001011111110000011100010010011010001101001011100111111111001010110001111011000111100101001001100101111110110010010001111100111011110101001100110000001010011001110000001110010111111010100011001110111000111000110001100101101110010101100011110110001111001010110011000011101000010110111110011101110100011111011001100111110001000110100001011011101110101000100011000111111010011010101110110010100001110101000111100111011100110111000100101000110010100110000000101001000110001110101101011100111001001110110101000010100010000111101001100010111010010110010000110101001011101000111001011010010000010010100111100101101111101001100111100101101110001011101001011001000011010100101110000100111101110011010011110111001110100000000010111111100000111000100100111100111110001110100111110110011101110100100010110011101110001011001111000011000010101101011100111011110111100010110001011101011010011011100000110111010001001111001110111010110110111000001101110100010011010110100110011111010001101000010000111101010110100111111010111001001011011101010011001100000011111000

*/
