Felez
---
Felez is a command line program to parse finite element surface mesh
in Nastran or Dyna format. This program is written in C++ and built using
Visual  Studio Community. Codes are platform independent and can be compiled
and linked for other OS's other than Windows.

This is a test program, developed in a modular form as independent libraries. 
It can be used to create the mesh data structure which can be used to calculate stiffness matrix
and solve for the unkowns.
The companion  [MD matrix reordering program](https://github.com/matti-logiko/PyPlate) can be used to study the structure of the
resulting stiffness matrix  and choose/design a high performance direct or iterative solver.

## Class structures
This program contains:
* __FelezMain__: entry point for the console application, driver program.
* BasicDLL: exception definition and general constants
* UtilityDLL: utility functions for measuring performance
* MeshDLL: mesh data, Node class and Element class
	* Node object:
		 * double *m_padNodeXYZ;  -->    Pointer to nodes XYZ.
		 * double *m_padNodeNormal;  -->   Pointer to nodes normal.
		 * int m_nNumNodes;           --> Number of nodes.
	* Element Object:
		* sElement *m_pasElements;  --> Pointer to the array of structure for elements.
		* int m_nNumElems;           --> Total number of nodes.
		* int m_nElemMaxNodes;       --> Maximum nodes per element.	
	* sElement struct: 
		* int anElemConnectvity[MAX_NODE_PER_ELEM]; -->Concectivity TODO:  Redesign!
		* int nMatID;								  --> Material ID
		* int nNumElemNodes;                       --> number of nodes
		* enmElemTopol eElemTopol;               --> Element shape, Tri, Quad
* ParserDLL: parser functions which read mesh file in ascii format and fill Mesh data structure
*

## How to use
Open VS solution file FelezMain.sln in Visual studio and build.
The executable is placed in BIN/. There are sample mesh files in Felez/TestData.
```
$ ./Felez.exe MeshFileName
```

## Mesh files
- input file: Dyna or Nastran mesh file in ascii format
-  Mesh can contain Triangular and/or Quad shell/surface element
-  Mesh can be generated programatically or using CAD software, e.g. free LS Pre-Post can be used
for visualization, and mesh generation 
- Dyna mesh file example:
```
$# LS-DYNA Keyword file created by LS-PrePost(R) V4.3 (Beta) - 20Dec2015(08:00)
$# Created on May-17-2019 (8:07:19)
*KEYWORD  
*TITLE
$#                                                                         title
LS-DYNA keyword deck by LS-PrePost
*ELEMENT_SHELL
$#   eid     pid      n1      n2      n3      n4      n5      n6      n7      n8
  0	 100 3	0	4
  1  100 0	1	4
  2	 100 4	1	2
  3	 100 4	2	5
*NODE
$#   nid               x               y               z      tc      rc 
0	0.120	-0.050	0.050
1	0.120	0.0	0.050
2 	0.120	0.050	0.050
3	0.060	-0.050	0.040
4	0.060	0.0	0.040
5	0.060	0.050	0.040
*END
```
- A few Dyna mesh files are in "Felez/TestData/ *.k"
- Dyna format mesh2.k :

## Authors
Mehdi Paak (matti.logiko@gmail.com)

## License
This project is licensed under the MIT License - see the [LICENSE](./LICENSE)  file for details.
For pedagogical purposes, please, do not submit this code as a solution for course assignments,exams and interview case studies.

## TODOs
This is a test program, developed in a modular form as independent libraries. 
It can be used to create the mesh data structure which can be used to calculate stiffness matrix
and solve for the unkowns.
The companion  [MD matrix reordering program](https://github.com/matti-logiko/PyPlate) can be used to study the structure of the
resulting stiffness matrix  and choose/design a high performance direct or iterative solver.
