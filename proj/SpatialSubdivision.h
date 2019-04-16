#pragma once
// =====================================================
// Class SpatialSubdivisionBlockList
// ------------------------------------
// Lists of blocks of entries (more efficient than
// lists of single entries).
// ===================================================== 
#include "..\OpenGLBasic\\Arcball\Vector3d.h"
#include "global.h"
#include <vector>
using namespace std;
class CSpatialSubdivision
{
	class CSpatialSubdivisionBlockList {
	public:

		// ----- Members: ----------------

		int *data;
		CSpatialSubdivisionBlockList *next_block;

		// ----- Methods: ----------------

		CSpatialSubdivisionBlockList(int SpatPerBlock){next_block=NULL;data=new int[SpatPerBlock];};
		~CSpatialSubdivisionBlockList(){if (next_block != NULL)delete next_block;if(data!=NULL)delete data;};
	};

	// =====================================================
	// Class SpatialSubdivisionEntry
	// -------------------------------
	// One of these exists for each particle
	// =====================================================

	class CSpatialSubdivisionEntry {
	public:

		// --------------- Members -------------------------------

		int size;                         // Number of particles in this block
		CSpatialSubdivisionBlockList *blocks;
		// Pointer to next block

		// --------------- Constructor/destructor ----------------

		CSpatialSubdivisionEntry() { size = 0; blocks = NULL; }
		~CSpatialSubdivisionEntry() { if (blocks != NULL) delete blocks; }
	};

public:
	CSpatialSubdivision(void);
	~CSpatialSubdivision(void);
private:
	int m_iSpatDim;
	int m_iMaxSpatPerBlock;
	int m_iMaxPrimitives;
	double m_dSpatMax;
	double m_dSpatMin;
	double m_dSpatDelta;

	CSpatialSubdivisionEntry ****table;
	int *spat_x;
	int *spat_y;
	int *spat_z;
	int *spat_index;

public:

	void Insert(int id, CVector3d v);
	void Initialize(void);
	void FindNeighborhood(int id,vector<int>* neighbours);
	void Update(int id, CVector3d v);
	void Delete(int id);
	void Insert(int id, int sector_x, int sector_y, int sector_z);
};
