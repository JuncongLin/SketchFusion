#include "StdAfx.h"
#include ".\spatialsubdivision.h"

CSpatialSubdivision::CSpatialSubdivision(void)
{
	m_iSpatDim=15;
	m_iMaxPrimitives=10000;
	m_iMaxSpatPerBlock=30;
	m_dSpatMax=5;
	m_dSpatMin=-5;
	m_dSpatDelta=(m_dSpatMax-m_dSpatMin)/m_iSpatDim;
}

CSpatialSubdivision::~CSpatialSubdivision(void)
{
}

void CSpatialSubdivision::Insert(int id, CVector3d v)
{
	int sector_x,sector_y,sector_z,block_no,block_id;
	CSpatialSubdivisionEntry *entry;
	CSpatialSubdivisionBlockList *sbl;

	// Determine in which sector we are located

	sector_x = (int)((v.x()-m_dSpatMin)/(m_dSpatDelta));
	sector_y = (int)((v.y()-m_dSpatMin)/(m_dSpatDelta));
	sector_z = (int)((v.z()-m_dSpatMin)/(m_dSpatDelta));

	// Clamp to avoid 'em dumps

	(void) ClampInt(&sector_x,m_iSpatDim);
	(void) ClampInt(&sector_y,m_iSpatDim);
	(void) ClampInt(&sector_z,m_iSpatDim);

	entry = table[sector_x][sector_y][sector_z];

	// If the entry is emptry: allocate memory for it 

	if (entry == NULL) {
		table[sector_x][sector_y][sector_z] = new CSpatialSubdivisionEntry();
		entry = table[sector_x][sector_y][sector_z];
	}

	// Determine in which block the entry is located

	block_no = (entry->size/m_iMaxSpatPerBlock);
	block_id = entry->size-block_no*m_iMaxSpatPerBlock;

	if (!block_id)   // Allocate a new block
	{
		CSpatialSubdivisionBlockList **sbl_tmp = &(entry->blocks);

		while (*sbl_tmp != NULL)
			sbl_tmp = &((*sbl_tmp)->next_block);
		*sbl_tmp = new CSpatialSubdivisionBlockList(m_iMaxSpatPerBlock);
	}

	// Forward block list to correct block

	sbl = entry->blocks;  
	if (block_no != 0)
	{
		int block_counter = 0;
		while (block_counter < block_no) {
			block_counter++;
			sbl = sbl->next_block;
		}
	}

	// OK, now we are happy and can insert our new friend in block

	sbl->data[block_id] = id;
	spat_x[id] = sector_x;
	spat_y[id] = sector_y;
	spat_z[id] = sector_z;
	spat_index[id] = entry->size;
	entry->size++;

}

void CSpatialSubdivision::Initialize(void)
{
	int i,j,k;
	table=new  CSpatialSubdivisionEntry***[m_iSpatDim];
	for (i=0;i<m_iSpatDim;i++)
	{
        table[i]=new CSpatialSubdivisionEntry** [m_iSpatDim];
		for(j=0;j<m_iSpatDim;j++)
		{
			table[i][j]=new CSpatialSubdivisionEntry* [m_iSpatDim];
			for (k=0;k<m_iSpatDim;k++)
			{
				table[i][j][k]=NULL;
			}
		}
	}

	spat_x=new int [m_iMaxPrimitives];
	spat_y=new int [m_iMaxPrimitives];
	spat_z=new int [m_iMaxPrimitives];
	spat_index=new int [m_iMaxPrimitives];

}

void CSpatialSubdivision::FindNeighborhood(int id,vector<int>* neighbors)
{
	int a,b,c,sector_x,sector_y,sector_z,total_checked;

	int count,block_count,lowx,highx,lowy,highy,lowz,highz;
	sector_x = spat_x[id];
	sector_y = spat_y[id];
	sector_z = spat_z[id];

	CSpatialSubdivisionEntry *entry;
	CSpatialSubdivisionBlockList *sbl;

	total_checked = 0;  // This guy keeps track on how many
	// neighbors have been found so far 
	// (max CL_CACHE_SZ)

	lowx = MAX(0,sector_x-1);
	highx = MIN(m_iSpatDim-1,sector_x+1);
	lowy = MAX(0,sector_y-1);
	highy = MIN(m_iSpatDim-1,sector_y+1);
	lowz = MAX(0,sector_z-1);
	highz = MIN(m_iSpatDim-1,sector_z+1);
	for ( a = lowx; a <= highx; a++)
		for ( b = lowy; b <= highy; b++)
			for ( c = lowz; c <= highz; c++)
			{
				entry = table[a][b][c];
				if (entry != NULL) 
					if (entry->size != 0) {
						sbl = entry->blocks;
						count = block_count = 0;
						while (count < entry->size) {
							if (sbl->data[block_count] != id)  
								neighbors->push_back(sbl->data[block_count]);
							count++; 
							block_count++;

							if (block_count == m_iMaxSpatPerBlock) {
								block_count = 0;
								sbl = sbl->next_block;
							}
						}//while
					}
			}//for
}

void CSpatialSubdivision::Update(int id, CVector3d v)
{
	int sector_x,sector_y,sector_z;

	sector_x = (int)((v.x()-m_dSpatMin)/(m_dSpatDelta));
	sector_y = (int)((v.y()-m_dSpatMin)/(m_dSpatDelta));
	sector_z = (int)((v.z()-m_dSpatMin)/(m_dSpatDelta));


	(void) ClampInt(&sector_x,m_iSpatDim);
	(void) ClampInt(&sector_y,m_iSpatDim);
	(void) ClampInt(&sector_z,m_iSpatDim);

	if ((sector_x != spat_x[id]) ||
		(sector_y != spat_y[id]) ||
		(sector_z != spat_z[id])) {
			Delete(id);
			Insert(id,sector_x,sector_y,sector_z);
		}

}

void CSpatialSubdivision::Delete(int id)
{
	int sector_x,sector_y,sector_z,sector_id,block_no,block_id,last_block_id;
	CSpatialSubdivisionEntry *entry;
	CSpatialSubdivisionBlockList *sbl,*sbl_last;

	// Determine in which sector we are located

	sector_x = spat_x[id];
	sector_y = spat_y[id];
	sector_z = spat_z[id];
	sector_id = spat_index[id];

	entry = table[sector_x][sector_y][sector_z];

	// Determine in which block of the sector the entry is located 

	block_no = (sector_id/m_iMaxSpatPerBlock);
	block_id = sector_id-block_no*m_iMaxSpatPerBlock;

	// Forward block list to correct block

	sbl = entry->blocks;
	if (block_no != 0)
	{
		int block_counter = 0;
		while (block_counter < block_no) {
			block_counter++;
			sbl = sbl->next_block;
		}
	}

	// Set sbl_last to last block

	sbl_last = sbl;
	while (sbl_last->next_block != NULL) sbl_last = sbl_last->next_block;
	last_block_id = (entry->size-1)%m_iMaxSpatPerBlock;

	// OK, now we are happy again and can remove our friend from block

	sbl->data[block_id] = sbl_last->data[last_block_id];
	spat_index[sbl_last->data[last_block_id]] = sector_id;

	if (last_block_id == 0) { // Delete block if empty and 
		// set pointer to NULL
		CSpatialSubdivisionBlockList **sbl_ptr = &(entry->blocks);
		while ((*sbl_ptr)->next_block != NULL)
			sbl_ptr = &((*sbl_ptr)->next_block);
		delete (*sbl_ptr);
		*sbl_ptr = NULL;
	}

	entry->size--;

}

void CSpatialSubdivision::Insert(int id, int sector_x, int sector_y, int sector_z)
{
	int block_no,block_id;
	CSpatialSubdivisionEntry *entry;
	CSpatialSubdivisionBlockList *sbl;

	entry = table[sector_x][sector_y][sector_z];

	// If the entry is emptry: allocate memory for it 

	if (entry == NULL) {
		table[sector_x][sector_y][sector_z] = new CSpatialSubdivisionEntry();
		entry = table[sector_x][sector_y][sector_z];
	}

	// Determine in which block the entry is located

	block_no = entry->size/m_iMaxSpatPerBlock;
	block_id = entry->size-block_no*m_iMaxSpatPerBlock;

	if (!block_id)   // Allocate a new block
	{
		CSpatialSubdivisionBlockList **sbl_tmp = &(entry->blocks);

		while (*sbl_tmp != NULL)
			sbl_tmp = &((*sbl_tmp)->next_block);
		*sbl_tmp = new CSpatialSubdivisionBlockList(m_iMaxSpatPerBlock);
	}

	// Forward block list to correct block

	sbl = entry->blocks;
	if (block_no != 0)
	{
		int block_counter = 0;
		while (block_counter < block_no) {
			block_counter++;
			sbl = sbl->next_block;
		}
	}

	// OK, now we are happy again and can insert our new friend in block

	sbl->data[block_id] = id;
	spat_x[id] = sector_x;
	spat_y[id] = sector_y;
	spat_z[id] = sector_z;
	spat_index[id] = entry->size;
	entry->size++;

}
