/********************************************************************

Mesh information: Nodes and Elements

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

#pragma once

#ifdef MESHDLL_EXPORTS
#define MESH_LIBRARY_API __declspec(dllexport) 
#else
#define MESH_LIBRARY_API __declspec(dllimport) 
#endif

//--- Dimensions ---
enum 
{
  dimX = 0,
  dimY,
  dimZ,
  NumDim
};

//--- Single element topology ---
enum enmElemTopol
{
  eNoShape = 0,
  eTri3 ,
  eQuad,
};

//--- Defines ---
#define MAX_NODE_PER_ELEM 8

//--- Forward declarations ---
class clNodes;
class clElems;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class FE Mesh.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MESH_LIBRARY_API clFEMesh
{

public:

  // Constructor.
  explicit clFEMesh();
  explicit clFEMesh(const int i_nNumElems, const int i_nNumNodes);

  // Copy/Assignment: Compiler generated.
 
  // Destructor.
  ~clFEMesh();

  //--- Get info ---
  // Get pointer to Node class.
  const clNodes * GetclNodes() const {return m_pclNodes;}
  clNodes * GetclNodes() {return m_pclNodes;}

  // Get pointer to Element class.
  const clElems * GetclElems() const {return m_pclElems;}
  clElems * GetclElems() {return m_pclElems;}

  // Get number of nodes.
  const int GetNumNodes() {return m_nNumNodes;}

  // Get number of elements.
  const int GetNumElems() {return m_nNumElems;}

  //--- Set Data ---
  // Allocate memory for elems and node class.
  void SetClassElemsandNodes(const int i_nNumElems, const int i_nNumNodes);



private:

  clNodes *m_pclNodes; // Pointer to Node class.
  clElems *m_pclElems; // Pointer to element class.
  int m_nNumElems;     // Number of elements.
  int m_nNumNodes;     // Number of nodes.

};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class Nodes.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MESH_LIBRARY_API clNodes
{

public:

  // Constructor.
  explicit clNodes();
  explicit clNodes(const int i_nNumNodes);

  // Copy/Assignment: Compiler generated.

  // Destructor.
  ~clNodes();

  //--- Set data ---
  // Set number of nodes.
  void SetNumNodes(const int i_nNumNodes) {m_nNumNodes = i_nNumNodes;}

  // Allocate memory for XYZ.
  void SetNodeXYZArray(const int i_nNumNodes);

  // Allocate memory for node normals.
  void SetNodeNormalArray(const int i_nNumNodes);

  //--- Get data ---
  // Get number of nodes.
  int GetNumNodes() const {return m_nNumNodes;}
  
  // Get nodes coordinate array.
  const double * GetNodesXYZ() const {return m_padNodeXYZ;}
  double * GetNodesXYZ() {return m_padNodeXYZ;}
  
  // Get nodes normal array.
  const double * GetNodesNormal() const {return m_padNodeXYZ;}
  double * GetNodesXYZNormal() {return m_padNodeXYZ;}
  
private:

  double *m_padNodeXYZ;     // Pointer to nodes XYZ.
  double *m_padNodeNormal;  // Pointer to nodes normal.
  int m_nNumNodes;          // Number of nodes.
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class elements.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MESH_LIBRARY_API clElems
{

public:

  // Constructor.
  explicit clElems();
  explicit clElems(const int i_nNumElem, const int i_nElemMaxNodes);
  
  // Copy/Assignment: Compiler generated.

  // Destructor.
  ~clElems();

  // Element structure declaration.
  struct sElement
  {
    int anElemConnectvity[MAX_NODE_PER_ELEM]; //TODO: remove macro! Redesign!
    int nMatID;
    int nNumElemNodes;
    enmElemTopol eElemTopol;
  };

  //--- Set data ---
  // Set number of elements.
  void SetNumElem(const int i_nNumElem) {m_nNumElems = i_nNumElem;}

  // Set Max number of nodes for element.
  void SetElemMaxNumNodes(const int i_nElemMaxNodes) {m_nElemMaxNodes = i_nElemMaxNodes;}

  // Allocate memory for Elem class.
  void SetclElem(const int i_nNumElem, const int i_nElemMaxNodes);

  //--- Get data ---
  // Get number of elements.
  int GetNumElems() {return m_nNumElems;}

  // Get max number of nodes for element.
  int GetElemMaxNodes() {return m_nElemMaxNodes;}

  // Get pointer to the array of element structure.
  const sElement * GetElement() const {return m_pasElements;}
  sElement * GetElement()  {return m_pasElements;}

  //---------------------------------------------------------
  // Get pointer the array of an element connectivity.
  // IN:
  //   i_nElemIndx: Element index.
  // RETURN:
  //   pointer to anElemConnectivity[MAX_NUM_NODE_PER_ELEM].
  //---------------------------------------------------------
  const int * GetElemConnectArray(const int i_nElemIndx) const {return m_pasElements[i_nElemIndx].anElemConnectvity;}
  int * GetElemConnectArray(const int i_nElemIndx) {return m_pasElements[i_nElemIndx].anElemConnectvity;}

  //---------------------------------------------------------
  // Get material ID.
  // IN:
  //   i_nElemIndx: Element index.
  // RETURN:
  //   Integer material ID.
  //---------------------------------------------------------
  int GetMatID(const int i_nElemIndx) const {return m_pasElements[i_nElemIndx].nMatID;}

  //---------------------------------------------------------
  // Get the number of nodes of an element.
  // IN:
  //   i_nElemIndx: Element index.
  // RETURN:
  //   Element's number of nodes.
  //---------------------------------------------------------
  int GetNumElemNodes(const int i_nElemIndx) const {return m_pasElements[i_nElemIndx].nNumElemNodes;}

private:

  sElement *m_pasElements; // Pointer to the array of structure for elements.
  int m_nNumElems;         // Total number of nodes.
  int m_nElemMaxNodes;     // Maximum nodes per element.

};