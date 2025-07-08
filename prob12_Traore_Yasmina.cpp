#include "moregraphics.h"
#include "classUniverse.h"
#include <iostream>
#include <fstream>
#include <cstdlib> // needed for exit(1)

using namespace std;

//==============================================================================
// Class declaration for the Universe class
//==============================================================================

// The declaration is in classUniverse.h.  It's been #included in Line 2 above!

//==============================================================================
// Class implementation for the Universe class
//
// NOTE:  Part of the class implementation is in prob12_provided_fcns.cpp.
//        You need to write the remaining functions by filling in the starter
//        code given below.
//==============================================================================

// This function is a default constructor.
// It sets the width, height, and wrap parameter to 0.  Sets cells to nullptr.
// [Relevant sample code(s): classDataset_with_myCopy_V3.cpp]
Universe::Universe()
{
    width = 0;
    height = 0;
    wrap = 0;
    cells = nullptr;
}

//------------------------------------------------------------------------------

// This function is a constructor.
// It sets the width, height, and wrap parameter based on the values of the
// arguments.  Dynamically allocates memory for the cells array, which will
// store the status of each cell.  Sets each entry in the cells array to 0.
// [Relevant sample code(s): classDataset_with_myCopy_V3.cpp]
Universe::Universe(int widthInput, int heightInput, int wrapInput)
{
    width = widthInput;
    height = heightInput;
    wrap = wrapInput;

    cells = new int[width*height];
    if (cells == nullptr)
    {
        cout << "\nUnable to allocate memory for the cells array.\n";
        exit(1);
    }

    for (int i=0; i<width*height; i++) cells[i] = 0;
}

//------------------------------------------------------------------------------

// This function is the destructor.
// It's called automatically when the object goes out of scope.
// It deallocates the memory associated with the cells array.
// [Relevant sample code(s): classDataset_with_myCopy_V3.cpp]
Universe::~Universe()
{
    if (cells != nullptr)
    {
        delete[] cells;
        cells = nullptr;
    }
}

//------------------------------------------------------------------------------

// This operator function writes a Universe to a file.  The 1st, 2nd, and 3rd
// numbers in the file are width, height, and wrap setting.  After that, the
// file contains a table of 0s and/or 1s representing dead/live cells.
// [Relevant sample code(s): avgmaxFile_cppVersion.cpp and 2Darray_with_malloc.c]
void Universe::operator>>(char filename[])
{
    // Declarations
    ofstream outputFile;

    // Open the output file and make sure it opened properly.
    outputFile.open(filename);
    if (outputFile.fail())
    {
        cout << "Unable to open file " << filename << ".\n";
        exit(1);
    }

    // Write the width, height, and wrap parameter.
    outputFile << width << endl
               << height << endl
               << wrap << endl;

    // Now write the 0s and 1s.
    for (int indexrow=0; indexrow<height; indexrow++)
    {
        for (int indexcol=0; indexcol<width; indexcol++)
        {
            outputFile << cells[indexrow*width + indexcol] << " ";
        }
        outputFile << endl;
    }

    // Close the output file, since we're done writing to it.
    outputFile.close();

    return;
}

//------------------------------------------------------------------------------

// This function returns the number of live neighbors of the cell at row,col.
int Universe::neighbors(int row, int col)
{
    // Declarations and initializations
    int numLiveNeigh = 0;


    if (!wrap) // "no wrap" case
    {
        // The four corners of the universe have three neighbors

        if (row==0 && col==0)
        {
            numLiveNeigh = getvalue(row, col+1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, col+1);
        }
        else if (row==0 && col==width-1)
        {
            numLiveNeigh = getvalue(row, width-2) +
                           getvalue(row+1, width-2) +
                           getvalue(row+1, width-1);
        }
        else if (row==height-1 && col==0)
        {
            numLiveNeigh = getvalue(height-2, col) +
                           getvalue(height-2, col+1) +
                           getvalue(height-1, col+1);
        }
        else if (row==height-1 && col==width-1)
        {
            numLiveNeigh = getvalue(height-2,width-2) +
                           getvalue(height-2,width-1) +
                           getvalue(height-1,width-2);
        }

        // The cells in the top row (except the first and last) have
        // five neighbors, West, East, South-West, South, and South-East

        else if (row==0 && (col>=1 && col<=width-2))
        {
            numLiveNeigh = getvalue(row, col-1) +
                           getvalue(row, col+1) +
                           getvalue(row+1, col-1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, col+1);
        }

        // The cells in the bottom row (except the first and last) have
        // five neighbors, North-West, North, North-East, West, and East

        else if (row==height-1 && (col>=1 && col<=width-2))
        {
            numLiveNeigh = getvalue(row-1, col-1) +
                           getvalue(row-1, col) +
                           getvalue(row-1, col+1) +
                           getvalue(row, col-1) +
                           getvalue(row, col+1);
        }

        // The cells in the first column (except the top and bottom) have
        // five neighbors, North, North-East, East, South, and South-East

        else if ((row>=1 && row <=height-2) && col==0)
        {
            numLiveNeigh = getvalue(row-1, col) +
                           getvalue(row-1, col+1) +
                           getvalue(row, col+1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, col+1);
        }

        // The cells in the last column (except the top and bottom) have
        // five neighbors, North-West, North, West, South-West, and South

        else if ((row>=1 && row <=height-2) && col==width-1)
        {
            numLiveNeigh = getvalue(row-1, col-1) +
                           getvalue(row-1, col) +
                           getvalue(row, col-1) +
                           getvalue(row+1, col-1) +
                           getvalue(row+1, col);
        }

        // All the other inner cells have eight neighbors.

        else
        {
            numLiveNeigh = getvalue(row-1, col-1) +
                           getvalue(row-1, col) +
                           getvalue(row-1, col+1) +
                           getvalue(row, col-1) +
                           getvalue(row, col+1) +
                           getvalue(row+1, col-1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, col+1);
        }
    }
    else // "wrap" case
    {
        // All cells have eight neighbors, with the corners having as neighbors
        // the adjacent cells upon wrapping or replicating and pasting the universe
        // next to itself

        if (row==0 && col==0)
        {
            numLiveNeigh = getvalue(height-1, width-1) +
                           getvalue(height-1, col) +
                           getvalue(height-1, col+1) +
                           getvalue(row, width-1) +
                           getvalue(row, col+1) +
                           getvalue(row+1, width-1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, col+1);
        }
        else if (row==0 && col==width-1)
        {
            numLiveNeigh = getvalue(height-1, width-2) +
                           getvalue(height-1, col) +
                           getvalue(height-1, 0) +
                           getvalue(row, width-2) +
                           getvalue(row, 0) +
                           getvalue(row+1, width-2) +
                           getvalue(row+1, col) +
                           getvalue(row+1, 0);
        }
        else if (row==height-1 && col==0)
        {
            numLiveNeigh = getvalue(height-2, width-1) +
                           getvalue(height-2, col) +
                           getvalue(height-2, col+1) +
                           getvalue(row, width-1) +
                           getvalue(row, col+1) +
                           getvalue(0, width-1) +
                           getvalue(0, col) +
                           getvalue(0, col+1);
        }
        else if (row==height-1 && col==width-1)
        {
            numLiveNeigh = getvalue(height-2, width-2) +
                           getvalue(height-2, col) +
                           getvalue(height-2, 0) +
                           getvalue(row, width-2) +
                           getvalue(row, 0) +
                           getvalue(0, width-2) +
                           getvalue(0, col) +
                           getvalue(0, 0);
        }
        else if (row==0 && (col>=1 && col<=width-2))
        {
            numLiveNeigh = getvalue(height-1, col-1) +
                           getvalue(height-1, col) +
                           getvalue(height-1, col+1) +
                           getvalue(row, col-1) +
                           getvalue(row, col+1) +
                           getvalue(row+1, col-1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, col+1);
        }
        else if (row==height-1 && (col>=1 && col<=width-2))
        {
            numLiveNeigh = getvalue(row-1, col-1) +
                           getvalue(row-1, col) +
                           getvalue(row-1, col+1) +
                           getvalue(row, col-1) +
                           getvalue(row, col+1) +
                           getvalue(0, col-1) +
                           getvalue(0, col) +
                           getvalue(0, col+1);
        }
        else if ((row>=1 && row <=height-2) && col==0)
        {
            numLiveNeigh = getvalue(row-1, width-1) +
                           getvalue(row-1, col) +
                           getvalue(row-1, col+1) +
                           getvalue(row, width-1) +
                           getvalue(row, col+1) +
                           getvalue(row+1, width-1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, col+1);
        }
        else if ((row>=1 && row <=height-2) && col==width-1)
        {
            numLiveNeigh = getvalue(row-1, col-1) +
                           getvalue(row-1, col) +
                           getvalue(row-1, 0) +
                           getvalue(row, col-1) +
                           getvalue(row, 0) +
                           getvalue(row+1, col-1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, 0);
        }
        else
        {
            numLiveNeigh = getvalue(row-1, col-1) +
                           getvalue(row-1, col) +
                           getvalue(row-1, col+1) +
                           getvalue(row, col-1) +
                           getvalue(row, col+1) +
                           getvalue(row+1, col-1) +
                           getvalue(row+1, col) +
                           getvalue(row+1, col+1);
        }
    }

    return numLiveNeigh;
}

//------------------------------------------------------------------------------

// This operator function calculates the new generation by applying the four
// "genetic laws."  Then it displays the new generation and pauses briefly.
// The genetic laws (rules) are as follows.
// Survival:              Any live cell with 2 or 3 live neighbors survives.
// Death by isolation:    Any live cell with fewer than 2 live neighbors dies,
// Death by overcrowding: Any live cell with more than 3 live neighbors dies.
// Birth:                 Any dead cell with exactly 3 live neighbors is born
//                        (i.e., becomes a live cell in the next generation).
// [Relevant sample code(s): None, but look at the start of Lecture 11 to
//                           refresh your memory on the most efficient way
//                           to step through a 2D array.]
void Universe::operator<<(Universe& onTheRight)
{
    // Loop over all cells in order to set the values (0 or 1) of the cells
    // in the cells array associated with the calling object.
    for (int irow=0; irow<height; irow++)
    {
        for (int icol=0; icol<width; icol++)
        {
            if (onTheRight.neighbors(irow, icol)<2 || onTheRight.neighbors(irow, icol)>3)
                setvalue(irow, icol, 0);

            else if (onTheRight.neighbors(irow, icol) == 3)
                setvalue(irow, icol, 1);
        }
    }

    // Display the live cells in the graphics window.
    display();

    return;
}
