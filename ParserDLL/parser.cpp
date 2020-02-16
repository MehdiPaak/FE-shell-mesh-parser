/********************************************************************
Parser class implementation

By: Mehdi Paak

********************************************************************/

//--- general header ---
#include "stdafx.h"

//--- Parser header ---
//#define PARSERDLL_EXPORTS, in preprocessor definition 
#include "parser.h"

#include "generalconst.h"

//--- FE mesh header ---
#include "mesh.h"

//--- PerformanceUtil ---
#include "PerformanceUtil.h"



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class FE mesh Parser..
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Constructor
clFEMeshParser::clFEMeshParser(const char *i_pacFileName): 
  m_pacFileName(i_pacFileName), 
  m_nNumLinesTot(0), 
  m_nNumLinesBeforeElems(0), 
  m_nNumLinesBeforeNodes(0){}

//--- Set date ---

//---------------------------
// Set file info data.
// IN:
//   i_nNumLinesTot:          Total number of lines in file (ASCII).
//   i_nNumLinesBeforeElems:  Number of comment lines before element section.
//   i_nNumLinesBeforeNodes:  Number of lines before node section.
//---------------------------
void clFEMeshParser::SetNumLines(
  const int i_nNumLinesTot,
  const int i_nNumLinesBeforeElems,
  const int i_nNumLinesBeforeNodes)
{
  m_nNumLinesTot = i_nNumLinesTot;
  m_nNumLinesBeforeElems = i_nNumLinesBeforeElems;
  m_nNumLinesBeforeNodes = i_nNumLinesBeforeNodes;
}

//---------------------------
// Open File.
// IN:
//   io_ppFile: Pointer to Pointer to File.
//   i_pacMpde: File opening mode ASCII or BIN.
// RETURN:
//---------------------------
void clFEMeshParser::OpenFile(
  FILE **io_ppFile,
  const char *i_pacMode)
{
  // Check input.

  const char *pacFileName = GetFileName();

  *io_ppFile = fopen(pacFileName,"r");
  
  if(!*io_ppFile)
  {
    fprintf(OUTPUT_STREAM,"File %s could not be opened!\n", pacFileName);
    throw clIOError(clIOError::eErrorOpeningFile, pacFileName);
  }

}

//---------------------------
// Read General data from file.
// IN:
//   i_pFile: Pointer to File.
// OUT:
//   o_nNumCommentLineBeforElem
//   o_nNumCommentLineBeforeNode
//   o_nNumElem
//   o_nNumNodes
// RETURN:
//---------------------------
int clFEMeshParser::ParseGenData(
  FILE *i_pFile,
  int &o_nNumCommentLineBeforeElem,
  int &o_nNumCommentLineBeforeNode,
  int &o_nNumElem,
  int &o_nNumNodes)
{
  // TODO: Add mode ascii etc.
  // Check input.
  assert(i_pFile && "Input file NULL pointer");

  // initialize.
  o_nNumNodes = 0;
  o_nNumElem = 0;
  o_nNumCommentLineBeforeElem = 0;
  o_nNumCommentLineBeforeNode = 0;

  // Read lines.
  char acLine[MAX_CHAR_ARRAY_SIZE] = {0};
  bool bElem = false;
  bool bNode = false;
  int nLineCount = 0;

  while(fgets(acLine,MAX_CHAR_ARRAY_SIZE,i_pFile) != NULL)
  {
    
    nLineCount++;

    // Skip lines starting with:
    if(acLine[0] == '\n') continue;  
    
    if(acLine[0] == '$')  continue;
    
    if(acLine[0] == 'L')  continue;

    // Extract info from lines starting with '*'.
    if(acLine[0] == '*')
    {
      //*ELEMENT
      if(acLine[1] == 'E' && acLine[2] == 'L')
      {
        bElem = true;
        bNode = false;
        o_nNumCommentLineBeforeElem = nLineCount + 1;
        continue;
      }

      //*NODE
      else if(acLine[1] == 'N')
      {
        bElem = false;
        bNode = true;
        o_nNumCommentLineBeforeNode = nLineCount + 1;
        continue;
      }

      //*END
      else if(acLine[1] == 'E' && acLine[2] == 'N')
        break;
      else
        continue;
    }
    
    if(bElem) 
      o_nNumElem++;
    if(bNode)
      o_nNumNodes++;
  }

  // Print info.
  fprintf(
     OUTPUT_STREAM,
     "\n"
     "NLTot       %d\n"
     "NLBE        %d\n"
     "NLBN        %d\n"
     "Num. Elems  %d\n"
     "Num. Nodes  %d\n",
     nLineCount,
     o_nNumCommentLineBeforeElem,
     o_nNumCommentLineBeforeNode,
     o_nNumElem,
     o_nNumNodes);

  return nLineCount;
}

//---------------------------
// Read mesh data from file and fill mesh structure.
// IN:
//   i_pFile:          Pointer to File.
// INOUT:
//   io_pclFEMesh:     Pointer to mesh class.
// RETURN:
//---------------------------
int clFEMeshParser::ReadMeshData(
  FILE *i_pFile,
  clFEMesh *io_pclFEMesh)
{
  // Check input.
  assert(i_pFile && io_pclFEMesh);

  const int nNumNodes = io_pclFEMesh->GetNumNodes();
  const int nNumElem  = io_pclFEMesh->GetNumElems();

  // Pointer to array of element structure.
  clElems::sElement *psElement = io_pclFEMesh->GetclElems()->GetElement();
  double *padNodeXYZ = io_pclFEMesh->GetclNodes()->GetNodesXYZ();

  const int nNumLinesBeforeElems = GetNumLinsBfrElems();
  const int nNumLinesBeforeNodes = GetNumLinsBfrNodes();

  // Read file.
  int nLineCount = 0;
  int neid = 0, nnid = 0;
  int nDum = 0;
  int nN0, nN1,nN2, nN3;
  char acLine [MAX_CHAR_ARRAY_SIZE] = {0};

  while(fgets(acLine, MAX_CHAR_ARRAY_SIZE, i_pFile) != NULL)
  {
    nLineCount++;

    // Read Elements from element section.
    if((nLineCount > nNumLinesBeforeElems) 
      && 
      (nLineCount < nNumLinesBeforeElems + nNumElem + 1))
    {
     // sscanf(acLine,"%d", &neid);
     // neid--;
        
      assert(neid < nNumElem);
      sscanf(
        acLine,
        "%d %d %d %d %d %d", 
        &nDum, 
        &(psElement[neid].nMatID), 
        &nN0,
        &nN1,
        &nN2,
        &nN3); // TODO: the number of nodes read can be extended to 8 %d ...
     
      psElement[neid].anElemConnectvity[0] = nN0;
      psElement[neid].anElemConnectvity[1] = nN1;
      psElement[neid].anElemConnectvity[2] = nN2;
      psElement[neid].anElemConnectvity[3] = nN3;

      if(nN2 == nN3)
      {
        psElement[neid].eElemTopol = eTri3;
        psElement[neid].nNumElemNodes = 3;
      }
      else
      {
        psElement[neid].eElemTopol = eQuad;
        psElement[neid].nNumElemNodes = 4;
      }
      neid++;
    }

    // Read Nodes from node section.
    else if((nLineCount > nNumLinesBeforeNodes) 
      && 
      (nLineCount < nNumLinesBeforeNodes + nNumNodes + 1))
    {
      //sscanf(acLine, "%d", &nnid);
      //nnid--;
      assert(nnid < nNumNodes);
      sscanf(
        acLine, 
        "%d %lf %lf %lf",
        &nDum,
        &padNodeXYZ[dimX + NumDim * nnid],
        &padNodeXYZ[dimY + NumDim * nnid],
        &padNodeXYZ[dimZ + NumDim * nnid]);
      nnid++;
    }

  }

  return 0;
}

//---------------------------
// Wrapper function to read mesh data from file and fill 
// mesh structure.
//---------------------------
clFEMesh * clFEMeshParser::ReadData()
{

  // Time now.
  double dT0 = TimeUtil::TimeDiff(0.0);

  // Open file.
  FILE *pfMeshFile = 0;
  clFEMeshParser::OpenFile( 
    &pfMeshFile,
    "r");

  // Parse file for number of elements and nodes.
  int nNumNodes = 0;
  int nNumElems = 0;
  int nLbE = 0;
  int nLbN = 0;
  int nNumLinesTot = 0;

  nNumLinesTot = clFEMeshParser::ParseGenData(
    pfMeshFile,
    nLbE,
    nLbN,
    nNumElems,
    nNumNodes);
  
  // Elapsed time for general parsing.
  double dT1 = TimeUtil::TimeDiff(dT0);

  clFEMeshParser::SetNumLines(
    nNumLinesTot,
    nLbE,
    nLbN);

  // rewind file to the beginning.
  rewind(pfMeshFile);

  // Create mesh class.
  clFEMesh *pclFEMesh = new clFEMesh(nNumElems, nNumNodes);

  // Time now.
  dT0 = TimeUtil::TimeDiff(0.0);
  // Read mesh info.
  clFEMeshParser::ReadMeshData(
    pfMeshFile,
    pclFEMesh);

  // Elapsed time for Filling FE data.
  double dT2 = TimeUtil::TimeDiff(dT0);

  // close file.
  fclose(pfMeshFile);

  // Print info.
  fprintf(
    OUTPUT_STREAM,
    "\n"
    "General parsing %9.3f (s)  %9.3f(m)\n"
    "Fill FE mesh    %9.3f (s)  %9.3f(m)\n",
    dT1,
    dT1/60.0,
    dT2,
    dT2/60.0);

  return pclFEMesh;
}

//---------------------------
// Function to read mesh data from file and fill 
// mesh structure.
// Node and Element class are allocated.
//---------------------------
 void clFEMeshParser::ReadFEData(clFEMesh &io_clFEMesh)
 {

   // Time now.
   double dT0 = TimeUtil::TimeDiff(0.0);

   // Open file.
   FILE *pfMeshFile = 0;
   clFEMeshParser::OpenFile( 
     &pfMeshFile,
     "r");

   // Parse file for number of elements and nodes.
   int nNumNodes = 0;
   int nNumElems = 0;
   int nLbE = 0;
   int nLbN = 0;
   int nNumLinesTot = 0;

   nNumLinesTot = clFEMeshParser::ParseGenData(
     pfMeshFile,
     nLbE,
     nLbN,
     nNumElems,
     nNumNodes);

   // Elapsed time for general parsing.
   double dT1 = TimeUtil::TimeDiff(dT0);

   clFEMeshParser::SetNumLines(
     nNumLinesTot,
     nLbE,
     nLbN);

   // rewind file to the beginning.
   rewind(pfMeshFile);

   // Allocate mem. for Nodes and Element classes.
   io_clFEMesh.SetClassElemsandNodes(nNumElems, nNumNodes);

   // Time now.
   dT0 = TimeUtil::TimeDiff(0.0);
   // Read mesh info.
   clFEMeshParser::ReadMeshData(
     pfMeshFile,
     &io_clFEMesh);

   // Elapsed time for Filling FE data.
   double dT2 = TimeUtil::TimeDiff(dT0);

   // close file.
   fclose(pfMeshFile);

   // Print info.
   fprintf(
     OUTPUT_STREAM,
     "\n"
     "General parsing %9.3f (s)  %9.3f(m)\n"
     "Fill FE mesh    %9.3f (s)  %9.3f(m)\n",
     dT1,
     dT1/60.0,
     dT2,
     dT2/60.0);

 }