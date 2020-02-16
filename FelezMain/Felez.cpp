/********************************************************************
 Felez.cpp : Defines the entry point for the console application.
 This command line program parses finite element surface mesh information
 which are in Nastran or Dyna format. Files must be in ascii format.

 Mesh could have Triangular and/or Quadrilateral shape.
 After parsing, Node and Element data structures are created.

 Node object:
	 double *m_padNodeXYZ;     // Pointer to nodes XYZ.
	 double *m_padNodeNormal;  // Pointer to nodes normal.
	 int m_nNumNodes;          // Number of nodes.
Element object:
	sElement *m_pasElements; // Pointer to the array of structure for elements.
	int m_nNumElems;         // Total number of nodes.
	int m_nElemMaxNodes;     // Maximum nodes per element.

	sElement: 
		int anElemConnectvity[MAX_NODE_PER_ELEM]; //Concectivity TODO: remove macro! Redesign!
		int nMatID;								  // Material ID
		int nNumElemNodes;                        // number of nodes
		enmElemTopol eElemTopol;                  // Element shape, Tri, Quad

 Usage: Felez.exe MeshFileName.
 A few Dyna mesh files are in "Felez/BIN/DAT/ *.k"

 By: Mehdi Paak

********************************************************************/

//--- Pre-compiled if activated ---
#include "stdafx.h"

//--- General ---
#include "generalconst.h"

//--- performance utility ---
#include "PerformanceUtil.h"

//--- Mesh ---
#include "mesh.h"

//--- Parser ---
#include "parser.h"

//--- Felez ---
#include "Felez.h"

#include<conio.h>

int main(int argc, char* argv[])
{
	// Error code from enum
  enmGeneralErrorCodes eErrorCode = eErrorNothing;
  char *pacFileName = 0;

  // Create mesh object (stack). 
  clFEMesh clFEMeshObj;

  //assert(argc >= 2 && "Insufficient input");
  try
  {
    // Read info
    if(argc < 2)
      throw eErrorInsufficientInput;

    pacFileName = argv[1];

    TimeUtil::PrintCurrentDateTime();

    fprintf(
      OUTPUT_STREAM,
      "File \"%s\" will be parsed.\n",
      pacFileName);

    // Parser.
    clFEMeshParser clParser(pacFileName);

    // Mesh.
    //pclFEMesh = clParser.ReadData();
    clParser.ReadFEData(clFEMeshObj);

  }

  //------------------------------------
  catch(enmGeneralErrorCodes eError)
  {
    fprintf(
      OUTPUT_STREAM,
      "Error %d: Insufficient command line input!\n",
      (int) eError);

    eErrorCode = eError;
  }
  //------------------------------------
  catch(const std::bad_alloc & eError)
  {
    fprintf(
      OUTPUT_STREAM,
      "Allocation failed: %s \n",
      eError.what());

    eErrorCode = eErrorMemAlloc; 

  }
  //------------------------------------
  catch(const clIOError & eError)
  {
    fprintf(
      OUTPUT_STREAM,
      "\n%s %s\n",
      eError.what(),
      eError.GetFileName());

    eErrorCode = eErrorIO;
  }
  //------------------------------------
  catch(...)
  {
    fprintf(
      OUTPUT_STREAM,
      "Unknown error!\n");

    eErrorCode = eErrorUnknown;
  }
  //------------------------------------  


  //getch();
  return eErrorCode;
}
