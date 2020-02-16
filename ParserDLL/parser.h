/********************************************************************
Class for parsing the mesh

By: Mehdi Paak

********************************************************************/

#pragma once

#ifdef PARSERDLL_EXPORTS
#define PARSER_LIBRARY_API __declspec(dllexport) 
#else
#define PARSER_LIBRARY_API __declspec(dllimport) 
#endif

//--- File format ---
enum enmFormatType
{
  eDyna,
  eNastran,
  eNumFormats
};

//--- File mode ---
enum FileMode
{
  ASCII = 0,
  BINARY,
};

//--- Forward declaration ---
// FE mesh class.
class clFEMesh;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class FE mesh Parser..
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class PARSER_LIBRARY_API clFEMeshParser
{

public:

  // Constructor.
  explicit clFEMeshParser(const char *i_pacFileName);

  // Destructor.
  ~clFEMeshParser(){}

  //---------------------------
  // Function to read mesh data from file and fill 
  // mesh structure.
  // Node and Element class are allocated.
  //---------------------------
  void ReadFEData(clFEMesh &io_clFEMesh);

  //---------------------------
  // Wrapper function to read mesh data from file and fill 
  // mesh structure.
  //---------------------------
  clFEMesh *ReadData();

  //--- Get info ---
  // Get file name.
  const char * GetFileName() const{return m_pacFileName;}

  // Get number of comment Lines before Element section.
  int GetNumLinsBfrElems() const {return m_nNumLinesBeforeElems;}

  // Get number of comment lines before Node section.
  int GetNumLinsBfrNodes() const {return m_nNumLinesBeforeNodes;}

private:
  //--- Set data ---
  // Set file data.
  void SetNumLines(
    const int i_nNumLinesTot,
    const int i_nNumLinesBeforeElems,
    const int i_nNumLinesBeforeNodes);

  //---------------------------
  // Open File.
  // IN:
  //   io_ppFile: Pointer to Pointer to File.
  //   i_pacMpde: File opening mode ASCII or BIN.
  // RETURN:
  //---------------------------
 void OpenFile(
    FILE **io_ppFile,
    const char *i_pacMode);

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
 int ParseGenData(
    FILE *i_pFile,
    int &o_nNumCommentLineBeforElem,
    int &o_nNumCommentLineBeforeNode,
    int &o_nNumElem,
    int &o_nNumNodes);

 //---------------------------
 // Read mesh data from file and fill mesh structure.
 // IN:
 //   i_pFile:          Pointer to File.
 // INOUT:
 //   io_pclFEMesh:     Pointer to mesh class.
 // RETURN:
 //---------------------------
 int ReadMeshData(
    FILE *i_pFile,
    clFEMesh *io_pclFEMesh);


//private:
  
const char *m_pacFileName;      // File name including path.
int   m_nNumLinesTot;           // Total number of lines in the file (ASCII).
int   m_nNumLinesBeforeElems;   // Number of comment lines before element section.
int   m_nNumLinesBeforeNodes;   // Number of lines before node section.
enmFormatType m_eFormat;        // File format (for now .k DYNA).
};