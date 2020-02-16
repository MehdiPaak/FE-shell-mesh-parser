/********************************************************************

Mesh class Implementation

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


By: Mehdi Paak

********************************************************************/

//--- General headers ---
#include "stdafx.h"
#include "generalconst.h"
#include "PerformanceUtil.h"
//--- mesh header ---
//#define MESHDLL_EXPORTS, in preprocessor definition
#include "mesh.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class FE Mesh.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------
// FE Mesh Constructor.
//
//----------------------------------------------
clFEMesh::clFEMesh():m_nNumElems(0), m_nNumNodes(0), m_pclElems(0), m_pclNodes(0){}

//----------------------------------------------
// FE Mesh Constructor.
//
//----------------------------------------------
clFEMesh::clFEMesh(const int i_nNumElems, const int i_nNumNodes)
{
  // Check input.
  assert(i_nNumElems > 0 && i_nNumNodes > 0);

  // Set number of nodes and elements.
  m_nNumElems = i_nNumElems;
  m_nNumNodes = i_nNumNodes;

  // allocations for clElems and clNodes.
  try
  {
    m_pclElems = new clElems(i_nNumElems, MAX_NODE_PER_ELEM);
    m_pclNodes = new clNodes(i_nNumNodes);
  }

  catch(...)
  {
    delete m_pclElems;
    delete m_pclNodes;
    throw;
  }

}

//----------------------------------------------
// FE Mesh Destructor.
//
//----------------------------------------------
clFEMesh::~clFEMesh()
{
  
  fprintf(OUTPUT_STREAM, "\nDeleting FEMesh:\n");

  if(m_pclElems)
  {
    delete m_pclElems;
    m_pclElems = 0;
  }

  if(m_pclNodes)
  {
    delete m_pclNodes;
    m_pclNodes = 0;
  }

}

//----------------------------------------------
// Allocate space for Element and Node class.
//
//----------------------------------------------
void clFEMesh::SetClassElemsandNodes(const int i_nNumElems, const int i_nNumNodes)
{
  // Check input.
  assert(i_nNumElems > 0 && i_nNumNodes > 0);

  // Set number of nodes and elements.
  m_nNumElems = i_nNumElems;
  m_nNumNodes = i_nNumNodes;

  // allocations for clElems and clNodes.
  try
  {
    m_pclElems = new clElems(i_nNumElems, MAX_NODE_PER_ELEM);
    m_pclNodes = new clNodes(i_nNumNodes);
  }

  catch(...)
  {
    delete m_pclElems;
    delete m_pclNodes;
    throw;
  }

}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class Nodes.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------
// Node Constructor.
//
//----------------------------------------------
clNodes::clNodes():m_padNodeNormal(0), m_padNodeXYZ(0), m_nNumNodes(0){}

//----------------------------------------------
// Node Constructor.
//
//----------------------------------------------
clNodes::clNodes(const int i_nNumNodes)
{
  assert(i_nNumNodes > 0);

  try
  {
    SetNumNodes(i_nNumNodes);
    SetNodeXYZArray(i_nNumNodes);
    SetNodeNormalArray(i_nNumNodes);
  }

  catch(...)  
  {
    throw;
  }
}

//----------------------------------------------
// Node Destructor.
//
//----------------------------------------------
clNodes::~clNodes()
{

  fprintf(OUTPUT_STREAM, "- Deleting Nodes\n");

  if(m_padNodeXYZ)
  {
    delete [] m_padNodeXYZ;
    m_padNodeXYZ = 0;
  }

  if(m_padNodeNormal)
  {
    delete [] m_padNodeNormal;
    m_padNodeNormal = 0;
  }

}

//----------------------------------------------
// Allocate memory for Node XYZ array.
// IN:
//  i_nNumNodes: Number of nodes.
// DESCRIPTION:
//  XYZ=[N1x N1y N1z N2x N2y N2z ...]
//----------------------------------------------
void clNodes::SetNodeXYZArray(const int i_nNumNodes)
{
  // Check input.
  assert(i_nNumNodes > 0);
  try
  {
    // Allocate and zero initialize.
    m_padNodeXYZ = new double [NumDim * i_nNumNodes]();
  }

  catch(...)
  {
    fprintf(OUTPUT_STREAM, "Insufficient Memory for Node allocation\n");
    throw;
  }
}

//----------------------------------------------
// Allocate memory for Node Normals array.
// IN:
//  i_nNumNodes: Number of nodes.
// DESCRIPTION:
//  XYZ=[n1x n1y n1z n2x n2y n2z ...]
//  n is the unit normal vector.
//----------------------------------------------
void clNodes::SetNodeNormalArray(const int i_nNumNodes)
{
  // Check input.
  assert(i_nNumNodes > 0);
  try
  {
    // Allocate and zero initialize.
    m_padNodeNormal = new double [NumDim * i_nNumNodes]();
  }

  catch(...)
  {
    fprintf(OUTPUT_STREAM, "Insufficient Memory for Node Normals allocation\n");
    throw;
  }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class Elements.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------
// Elem Constructor.
//
//----------------------------------------------
clElems::clElems():m_nNumElems(0), m_nElemMaxNodes(0), m_pasElements(0){};

//----------------------------------------------
// Elem Constructor.
//
//----------------------------------------------
clElems::clElems(
  const int i_nNumElem, 
  const int i_nElemMaxNodes)
{
  //
  assert(i_nNumElem > 0 && i_nElemMaxNodes);

  try
  {
    m_nNumElems = i_nNumElem;
    m_nElemMaxNodes = i_nElemMaxNodes;
    SetclElem(i_nNumElem, i_nElemMaxNodes);
  }

  catch(...)
    {
    fprintf(OUTPUT_STREAM, "Insufficient Memory for Elements allocation\n");
    throw;
  }
}

//----------------------------------------------
// Elem Destructor.
//
//----------------------------------------------
clElems::~clElems()
{
  
  fprintf(OUTPUT_STREAM, "- Deleting Elems\n");
  
  if(m_pasElements)
  {
    delete [] m_pasElements;
    m_pasElements = 0;
  }
}

//----------------------------------------------
// Allocate memory array of element structure.
// IN:
//  i_nNumElem:       Number of elements.
//  i_nElemMaxNodes:  Max. number of nodes per element.
// DESCRIPTION:
//  for each element,
//  anElemConnectivity[N0 N1 N2 N3 N4 -1 -1 ... MAX_NUM_NODE]
//  this array is initialized with -1. 
//----------------------------------------------
void clElems::SetclElem(
  const int i_nNumElem, 
  const int i_nElemMaxNodes)
{
  //
  assert(i_nNumElem > 0 && i_nElemMaxNodes > 0);

  try
  {
    // Allocate mem.
    m_pasElements = new sElement [i_nNumElem];

    // Initialize element structure.
    for(int i = 0; i < i_nNumElem; i++)
    {
      m_pasElements[i].nMatID = -1;
      m_pasElements[i].nNumElemNodes = -1;
      m_pasElements[i].eElemTopol = eNoShape;
      for(int j = 0; j < MAX_NODE_PER_ELEM; j++)
      {
        m_pasElements[i].anElemConnectvity[j] = -1;
      }
    }
  }

  catch(...)
  {
    throw;
  }
}