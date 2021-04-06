/**
 * @file main.cpp
 *
 * @brief Finding Single Source Shortest Path on a Directed Acyclic Graph using Topological Sorting
 *
 * @author Khayal Huseynov
 * Contact: khayalhus@gmail.com
 *
 * INSTRUCTIONS:
 * The program is written on version: c++11
 * Run with an input file: "./a.out filename" (Eg: ./a.out input.txt)
 * Input file should only contain the numbers in a pyramid or orthogonal triangle form.
 * Run if you want to give input from terminal: "./a.out"
 * 
 * METHOD:
 * At the top is an extra start (source) node that is connected to the top-most level of the pyramid.
 * The weight of this edge is the negative value of top-most node.
 * The nodes in-between are also connected in this manner.
 * At the end is a stop (destination) node.
 * The bottom-most nodes which are not prime numbers are connected to this stop node.
 * Weight of these edges are zero.
 */

#include <iostream> // cerr, cin, cout
#include <climits> // INT_MAX
#include <string> // string, getline
#include <list> // STL List
#include <stack> // STL Stack
#include <fstream> // ifstream

using namespace std;

struct Edge {
    int vertexNum;
    int edgeWeight; // in negative

    Edge (int vertexNum, int edgeWeight);
    ~Edge () {}
};

Edge::Edge (int vertexNum, int edgeWeight) {
    this->vertexNum = vertexNum;
    this->edgeWeight = edgeWeight;
}

struct DAG {
    int vertexAmount;   // count of vertices
    list<Edge> *edgeList; // holds edges between non-prime numbers
    bool * visited; // holds whether a vertex has been visited (for sorting)
    stack<int> s;

    DAG(int edgeAmount);
    ~DAG();

    void topologicalSortRecursive(int v);
    void addEdge(int source, int destination, int positiveWeight);
    void maximumSum(); // topologicalSort()
};
  
DAG::DAG(int vertexAmount) {
    this->vertexAmount = vertexAmount;
    this->edgeList = new list<Edge>[vertexAmount];
}

DAG::~DAG() {
    delete [] edgeList;
}

void DAG::addEdge(int source, int destination, int positiveWeight)
{
    Edge newEdge(destination, -positiveWeight); // hold the negative
    edgeList[source].push_back(newEdge); // put it to the edge list
}

void DAG::topologicalSortRecursive(int vertex)
{
    visited[vertex] = true;
  
    list<Edge>::iterator it;
    for (it = this->edgeList[vertex].begin(); it != edgeList[vertex].end(); it++)
    {
        Edge edge = *it;
        if ( visited[edge.vertexNum] == false) {
            topologicalSortRecursive(edge.vertexNum);
        }
    }
    s.push(vertex);
}
  
void DAG::maximumSum()
{
    // Time Complexity: O(V + E) where V are Vertices and E are Edges
    int *sum = new int[vertexAmount];
  
    // Mark all the vertices as not visited
    this->visited = new bool[this->vertexAmount] { false };
  
    for (int i = 0; i < this->vertexAmount; i++) {
        if (visited[i] == false) {
            topologicalSortRecursive(i);
        }
    }

    delete [] this->visited;
  
    for (int i = 0; i < this->vertexAmount; i++) {
        sum[i] = INT_MAX;
    }
    sum[0] = 0; // Initialize the sum to 0
  
    while (this->s.empty() == false)
    {
        int source = s.top(); // get source
        s.pop(); // remove source from stack
  
        list<Edge>::iterator i;
        if (sum[source] != INT_MAX)
        {
            for (i = this->edgeList[source].begin(); i != this->edgeList[source].end(); i++) {
                if (sum[i->vertexNum] > sum[source] + i->edgeWeight) {
                    sum[i->vertexNum] = sum[source] + i->edgeWeight;
                }
            }
        }
    }

    // print sum
    for (int i = this->vertexAmount - 1; i >= 0; i--) {
        if (sum[i] != INT_MAX) {
            cout << "Maximum Sum: " << -sum[i] << endl;
            delete [] sum;
            return;
        }
    }
    cout << "Maximum sum does not exist." << endl;
    delete [] sum;
    return;
}

bool isPrime(int num)
{
    // Time Complexity: O(sqrt(n))
    if (num <= 1) {
        return false;
    }
    if (num <= 3) {
        return true;
    }

    if (num % 2 == 0 || num % 3 == 0) {
        return false;
    }

    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

void readInput(int N, DAG *& pyramid) {
    int NSum = 0;
    for (int i = 1; i <= N; i++) {
        NSum += i;
    }
    pyramid = new DAG(NSum + 2);
    
    int num = 0;
    cout << "Level 1, Number 1: ";
    cin >> num;
    if(isPrime(num) == true) {
        return;
    }
    pyramid->addEdge(0, 1, num);
    // cout << 0 << "->" << 1 << " w: " << num << endl;

    int index = 2;
    // Read the pyramid and create edges if the destination node is not prime
    for (int i = 2; i <= N; i++) {
        for (int j = 0; j < i; j++, index++) {
            cout << "Level " << i << ", Number " << j+1 << ": ";
            cin >> num;
            if(isPrime(num) == false) {
                if (j == 0) {
                    pyramid->addEdge(index-i+1, index, num);
                } else if (j + 1 == i) {
                    // cout << index-i << "->" << index << " w: " << num << endl;
                    pyramid->addEdge(index-i, index, num);
                } else {
                    // cout << index-i << "->" << index << " w: " << num << endl;
                    pyramid->addEdge(index-i, index, num);
                    // cout << index-i+1 << "->" << index << " w: " << num << endl;
                    pyramid->addEdge(index-i+1, index, num);
                }

                if (i == N) {
                    // cout << index << "->" << index+i-j << " w: " << 0 << endl;
                    pyramid->addEdge(index, index+i-j, 0);
                }
            }
        }
    }
}

void readInput(ifstream& inFile, DAG *& pyramid) {
    /* get level count from file */
    int Nsum = 0;
    int N = 0;
    string data;
    while (inFile.peek() != EOF) {
        getline(inFile, data, '\n'); // move to next line
        Nsum += ++N;
    }
    
    inFile.clear();
    inFile.seekg(0); // move cursor to start of file
    
    pyramid = new DAG(Nsum + 2);
    
    int num = 0;

    inFile >> num;
    if(isPrime(num) == true) {
        return;
    }
    pyramid->addEdge(0, 1, num);
    // cout << 0 << "->" << 1 << " w: " << num << endl;

    int index = 2;

    // Read the pyramid and create edges if the destination node is not prime
    for (int i = 2; i <= N; i++) {
        for (int j = 0; j < i; j++, index++) {
            inFile >> num;
            if(isPrime(num) == false) {
                if (j == 0) {
                    // cout << index-i+1 << "->" << index << " w: " << num << endl;
                    pyramid->addEdge(index-i+1, index, num);
                } else if (j + 1 == i) {
                    // cout << index-i << "->" << index << " w: " << num << endl;
                    pyramid->addEdge(index-i, index, num);
                } else {
                    // cout << index-i << "->" << index << " w: " << num << endl;
                    pyramid->addEdge(index-i, index, num);
                    // cout << index-i+1 << "->" << index << " w: " << num << endl;
                    pyramid->addEdge(index-i+1, index, num);
                }

                if (i == N) {
                    // cout << index << "->" << index+i-j << " w: " << 0 << endl;
                    pyramid->addEdge(index, index+i-j, 0);
                }
            }
        }
    }

}

int main (int argc, char** argv) {

    string filename = "";

    if (argc > 1) {
        filename = argv[1];
        cout << "Trying to open " << filename << "..." << endl;
    } else {
        cout << "No filename supplied." << endl;
    }

    DAG * pyramid = NULL;


    if (filename.compare("") == 0) {
        int N = 0; // level count
        cout << "Please enter the level count of pyramid: ";
        cin >> N;

        readInput(N, pyramid);
    } else {
        ifstream inFile;

        inFile.open(filename);

        if (!inFile) {
		    cerr << "ERROR: Can not open input file." << endl;
		    return 1;
	    }

        readInput(inFile, pyramid);

        inFile.close();
    }

    pyramid->maximumSum();

    delete pyramid;
    
    return 0;
}