/**
 *        @file: greedy.cc
 *      @author: Drew VanAtta
 *        @date: November 27, 2023
 *       @brief: greedy algorithms hw 5
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
#include <istream>
#include <vector>
#include <climits>
#include <algorithm>
#include <chrono>

using namespace std;

/// function prototypes

// function that prints the 2D Vector -- Not important, just used while I was testing to see what I was working with
void print2DVec(vector<vector<int>> vector)
{
    for (size_t i = 0; i < vector.size(); i++)
    {
        for (size_t k = 0; k < vector[i].size(); k++)
        {
            if (vector[i][k] == INT_MAX)
            {
                cout << 0;
            }
            else
            {
                cout << vector[i][k];
            }
            cout << " ";
        }
        cout << endl;
    }
}

// PRIMS ALGORITHM FUNCTIONS
//   Function that returns whether or not the edge is valid
bool isValidEdge(int v1, int v2, vector<bool> inMST)
{
    if (v1 == v2) // no edge if its the same vertice
    {
        return false;
    }
    if (inMST[v1] == false && inMST[v2] == false) // if neither in MST, then we can't connect edge
    {
        return false;
    }
    else if (inMST[v1] == true && inMST[v2] == true) // if both vertices in MST then it would loop, no edge
    {
        return false;
    }
    return true; // all other cases would be one true and one false, meaning the edge is a valid edge we can connect
}

// Main prims algorithm
// I implemented prims with an adjacency matrix which I prob didn't need to do but it works
void prims(vector<vector<int>> weights, int vertices, string outputfile)
{
    outputfile = outputfile + ".txt";
    ofstream outs("tests/" + outputfile);                             // the output file
    auto start = chrono::high_resolution_clock::now();     // TIMING START
    vector<bool> inMST(vertices, false);                   // vector to hold if the vertice is added yet to avoid loops
    vector<vector<int>> MST(vertices - 1, vector<int>(3)); // Originally not needed, will for outputting to file in same format since the length has to go first

    // starting at first vertice
    inMST[0] = true;

    int edgecount = 0;                 // keep track of how many edges we have
    int totalcost = 0;                 // total cost of the MST
    while (edgecount < (vertices - 1)) // vertices - 1 will be the amount of edges in the MST always
    {
        int min = INT_MAX; // initalize min to max int so first edge is used
        int row = -1;
        int col = -1; // row and col are placeholders to save the edge positions i and j outside the for loops

        for (int i = 0; i < vertices; i++)
        {
            for (int j = 0; j < vertices; j++) // i, j indexes for the matrix
            {
                if (weights[i][j] < min)
                {
                    if (isValidEdge(i, j, inMST)) // FOUND A VALID EDGE
                    {
                        min = weights[i][j]; // keep track of min edge weight
                        // need to keep track of position
                        row = i;
                        col = j;
                    }
                }
            }
        }

        if (row != -1) // placeholder changed, a valid minimum edge was found
        {
            vector<int> tmpvec = {row, col, min};
            MST[edgecount] = tmpvec; // add the edge into the MST
            edgecount++;
            totalcost = totalcost + min;    // ADD THE MIN EDGE COST TO TOTAL
            inMST[col] = inMST[row] = true; // set so both vertices are in MST
        }
    }

    auto stop = chrono::high_resolution_clock::now(); // TIMING END
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Prim timing for " << outputfile << " is " << duration.count() << " microseconds" << endl;

    // OUTPUTTING TO THE FILE
    outs << totalcost << endl;
    for (size_t c = 0; c < MST.size(); c++)
    {
        for (size_t g = 0; g < 3; g++) // 3 since there are three ints (v1, v2, weight)
        {
            if (g < 2)
            {
                outs << MST[c][g] + 1; //+1 TO GET CORRECT VERTICE NUMBER
                outs << ",";
            }
            else // g == 2 at this point, so the output the weight without +1
            {
                outs << MST[c][g];
            }
        }
        outs << endl; // onto next edge
    }
}

// KRUSKALS IMPLEMENTATION FUNCTIONS
//  find - gets the parent of the root
int find(vector<int> &parent, int x)
{
    if (parent[x] == -1)
        return x;

    return find(parent, parent[x]);
}
// OPTIMIZED VERSION - set's the parent directly to the root
int findOpt(vector<int> &parent, int x)
{
    if (parent[x] == -1)
        return x;
    parent[x] = findOpt(parent, parent[x]); // SET PARENT[X] TO THE ROOT NODE, THEN RETURN THAT
    return parent[x];
}

// compare weights for weightSort
bool compareWeights(const vector<int> a, const vector<int> b)
{
    return a[2] < b[2];
}
// sorts the vector by weights for kruskals
vector<vector<int>> weightSort(vector<vector<int>> weights)
{
    sort(weights.begin(), weights.end(), compareWeights);
    return weights;
}

// kruskals implementation - bool flag is for the optimization in the find function
void kruskals(vector<vector<int>> weights, int vertices, bool flag, string outputfile)
{
    outputfile = outputfile + ".txt";
    ofstream outs("tests/" + outputfile);                             // the output file
    auto start = chrono::high_resolution_clock::now(); // TIMING START

    int totalcost = 0;
    vector<vector<int>> sortedWeights = weightSort(weights); // sorts the vertices by smallest weight
    // weights are now in sorted order
    vector<vector<int>> MST;          // holds the MST for output
    vector<int> parent(vertices, -1); // parent array that holds the disjoint sets parents
    int p1, p2;
    for (size_t i = 0; i < weights.size(); i++)
    {
        if (flag == false) // THIS IS WHERE THE OPTIMIZATION IS -- FALSE FOR NO OPTIMIZATION
        {
            p1 = find(parent, sortedWeights[i][0] - 1);
            p2 = find(parent, sortedWeights[i][1] - 1);
        }
        else // THIS IS THE OPTIMIZED ROUTE FOR THE FUNCTION
        {
            p1 = findOpt(parent, sortedWeights[i][0] - 1);
            p2 = findOpt(parent, sortedWeights[i][1] - 1);
        }

        if (p1 != p2) // PARENTS ARE DIFFERENT
        {
            parent[p2] = p1;
            totalcost = totalcost + sortedWeights[i][2];
            MST.push_back(sortedWeights[i]);
        }
    }

    auto stop = chrono::high_resolution_clock::now(); // TIMING END
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Kruskal timing for " << outputfile << " is " << duration.count() << " microseconds" << endl;

    outs << totalcost << endl;
    for (size_t c = 0; c < MST.size(); c++)
    {
        for (size_t g = 0; g < 3; g++) // 3 since there are three ints (v1, v2, weight)
        {
            if (g < 2)
            {
                outs << MST[c][g];
                outs << ",";
            }
            else // g == 2 at this point, so the output the weight without +1
            {
                outs << MST[c][g];
            }
        }
        outs << endl; // onto next edge
    }
}

int main(int argc, char const *argv[])
{
    ifstream fin;
    ofstream fout;
    string fileArr[6] = {"graph1", "graph_v100_e150", "graph_v200_e300", "graph_v400_e1200", "graph_v800_e3200", "graph_v1600_e6400"};
    string tmp;
    string filename;
    string outputfileprims = "prim_output_mst";
    string outputfilekruskals = "output_kruskals_unoptimized";
    string outputfilekruskals2 = "output_kruskals_optimized";
    for (size_t i = 0; i < 6; i++) // GOES THROUGH EACH OF THE FILES DOING THE THREE TESTS
    {

        filename = fileArr[i]; // INPUT FILE NAME
        // filename = "graph_v100_e150";
        filename = filename + ".txt";
        filename = "graphs/" + filename;

        // open the file
        fin.open(filename.c_str());
        if (fin.fail())
        {
            cout << "error, file not found" << endl;
        }

        getline(fin, tmp);
        int vecSize = stoi(tmp);                                  // getes the first number in the txt file
        vector<vector<int>> vec2d(vecSize, vector<int>(vecSize)); // 2d vector used for prims algorithm
        vector<vector<int>> edges;                                // list of edges for kruskals

        // initialize the vector for prims to INT_MAX
        for (int i = 0; i < vecSize; i++)
        {
            for (int k = 0; k < vecSize; k++)
            {
                vec2d[i][k] = INT_MAX;
            }
        }
        // loading the rest of the rest of the file after the first line
        while (getline(fin, tmp)) // WHILE IT CAN GET THE LINE FROM FILE
        {
            string int1, int2, int3;                // read from file will see them as strings, will convert to ints later
            int com1 = 0, com2 = 0;                 // holds position of commas for substring in function
            for (size_t i = 0; i < tmp.size(); i++) // GOES THROUGH AND FINDS COMMA
            {
                if (tmp[i] == ',')
                {
                    if (com1) // we already have first comma, so its the second comma
                    {
                        com2 = i;
                    }
                    else // we don't have first comma yet
                    {
                        com1 = i;
                    };
                };
            }
            // We have the comma spots now
            // GET THE THREE NUMBERS, ADD THEM TO VECTORS FOR PRIMS AND KRUSKALS
            int tmp1, tmp2, tmp3;
            tmp1 = stoi(tmp.substr(0, com1));                     // gets v1 from file, converts to int
            tmp2 = stoi(tmp.substr(com1 + 1, (com2 - com1) - 1)); // gets v2
            tmp3 = stoi(tmp.substr(com2 + 1, tmp.size() - com2)); // gets weight
            vec2d[tmp1 - 1][tmp2 - 1] = tmp3;                     //-1 since index starts at 0
            vec2d[tmp2 - 1][tmp1 - 1] = tmp3;                     // BOTH WAYS - ADJACENCY MATRIX
            vector<int> tmpvec;                                   // vec2d was for prims, edges is for kruskals!!!!!
            tmpvec.push_back(tmp1);
            tmpvec.push_back(tmp2);
            tmpvec.push_back(tmp3);
            edges.push_back(tmpvec);
        } // end of while loop getting from file
        // at this point, the graph has been loaded fully from the file into vec2d

        string filenum = to_string(i);
        outputfileprims = "prim_output_mst" + filenum;
        outputfilekruskals = "output_kruskals_unoptimized" + filenum;
        outputfilekruskals2 = "output_kruskals_optimized" + filenum;
        cout << i+1 << "." << endl;
        cout << "THE INPUT FILE THIS TIME IS " << filename << endl;
        prims(vec2d, vecSize, outputfileprims);
        cout << "The MST for prims has been output to " << outputfileprims << endl;

        kruskals(edges, vecSize, false, outputfilekruskals);
        cout << "The MST for prims has been output to " << outputfilekruskals << endl;
        kruskals(edges, vecSize, true, outputfilekruskals2);
        cout << "The MST for prims has been output to " << outputfilekruskals2 << endl
             << endl;

        fin.close();
    }

    cout << "Program Finished." << endl; // if this runs then woohoo!!

    return 0;
} // main
