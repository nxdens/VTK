/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkHyperTreeGridEntry.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkHyperTreeGridEntry
 * @brief   Entries are cache data for cursors
 *
 * Entries are relevant for cursor/supercursor developpers. Filters
 * developpers should have a look at cursors/supercursors documentation.
 * (cf. vtkHyperTreeGridNonOrientedCursor). When writing a new cursor or
 * supercursor the choice of the entry is very important: it will drive
 * the performance and memory cost. This is even more important for
 * supercursors which have several neighbors: 6x for VonNeuman and 26x for
 * Moore.
 *
 * Several types of Entries exist:
 * 1. vtkHyperTreeGridEntry
 * This cache only memorizes the current cell index in one HyperTree.
 * Using the index, this entry provides several services such as:
 * is the cell coarse or leaf, get or set global index (to access
 * field value, cf. vtkHyperTree), descend into selected child,
 * subdivise the cell. Equivalent services are available for all entries.
 *
 * 2. vtkHyperTreeGridGeometryEntry
 * This cache adds the origin coordinates of the cell atop
 * vtkHyperTreeGridEntry. Getter is provided, as well as services related
 * to the bounding box and cell center.
 *
 * 3. vtkHyperTreeGridLevelEntry
 * This cache adds the following information with their getters atop
 * vtkHyperTreeGridEntry: pointer to the HyperTree, level of the current
 * cell.
 *
 * 4. vtkHyperTreeGridGeometryLevelEntry
 * This cache is a combination of vtkHyperTreeGridLevelEntry and
 * vtkHyperTreeGridLevelEntry: it provides all combined services.
 *
 * @sa
 * vtkHyperTreeGridEntry
 * vtkHyperTreeGridLevelEntry
 * vtkHyperTreeGridGeometryEntry
 * vtkHyperTreeGridGeometryLevelEntry
 * vtkHyperTreeGridOrientedCursor
 * vtkHyperTreeGridNonOrientedCursor
 *
 * @par Thanks:
 * This class was written by Jacques-Bernard Lekien, Jerome Dubois and
 * Guenole Harel, CEA 2018.
 * This work was supported by Commissariat a l'Energie Atomique
 * CEA, DAM, DIF, F-91297 Arpajon, France.
 */

#ifndef vtkHyperTreeGridEntry_h
#define vtkHyperTreeGridEntry_h

#ifndef __VTK_WRAP__

#include "vtkObject.h"

class vtkHyperTree;
class vtkHyperTreeGrid;

class vtkHyperTreeGridEntry
{
public:
  /**
   * Display info about the entry
   */
  void PrintSelf( ostream& os, vtkIndent indent );

  /**
   * Dump information
   */
  void Dump( ostream& os );

  /**
   * Constructor
   */
  vtkHyperTreeGridEntry() {
    this->Index = 0;
  }

  /**
   * Constructor
   */
  vtkHyperTreeGridEntry( vtkIdType index ) {
    this->Index = index;
  }

  /**
   * Destructor
   */
  ~vtkHyperTreeGridEntry() = default;

  /**
   * Initialize cursor at root of given tree index in grid.
   */
  vtkHyperTree* Initialize(
    vtkHyperTreeGrid* grid,
    vtkIdType treeIndex,
    bool create = false
  );

  /**
   * Initialize cursor at root of given tree index in grid.
   */
  void Initialize( vtkIdType index ) {
    this->Index = index;
  }

  /**
   * Copy function
   */
  void Copy( const vtkHyperTreeGridEntry* entry) {
    this->Index = entry->Index;
  }

  /**
   * Return the index of the current vertex in the tree.
   */
  vtkIdType GetVertexId() const {
    return this->Index;
  }

  /**
   * Return the global index for the current cell (cf. vtkHyperTree).
   */
  vtkIdType GetGlobalNodeIndex( const vtkHyperTree* tree ) const;

  /**
   * Set the global index for the root cell of the HyperTree.
   */
  void SetGlobalIndexStart( vtkHyperTree* tree, vtkIdType index );

  /**
   * Set the global index for the current cell of the HyperTree.
   */
  void SetGlobalIndexFromLocal( vtkHyperTree* tree, vtkIdType index );

  /**
   * Is the cursor pointing to a leaf?
   */
  bool IsLeaf( const vtkHyperTree* tree ) const;

  /**
   * Change the current cell's status: if leaf then becomes coarse and
   * all its children are created, cf. HyperTree.
   */
  void SubdivideLeaf( vtkHyperTree* tree, unsigned int level );

  /**
   * Is the cursor pointing to a coarse with all childrens being leaves?
   */
  bool IsTerminalNode( const vtkHyperTree* tree ) const;

  /**
   * Is the cursor at HyperTree root?
   */
  bool IsRoot() const {
    return ( this->Index == 0 );
  }

  /**
   * Move the cursor to i-th child of the current cell.
   * \pre not_leaf: !IsLeaf()
   * \pre valid_child: ichild>=0 && ichild<this->GetNumberOfChildren()
   */
  void ToChild( const vtkHyperTree* tree, unsigned char ichild );

protected:

  /**
   * index of the current cell in the HyperTree.
   */
  vtkIdType Index;
};

#endif// __VTK_WRAP__

#endif// vtkHyperTreeGridEntry_h
// VTK-HeaderTest-Exclude: vtkHyperTreeGridEntry.h
