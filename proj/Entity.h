#pragma once

#include <vector>
#include "..\OpenGLBasic\Edge.h"
#include "..\OpenGLBasic\Face.h"
#include "..\OpenGLBasic\Vertex.h"
#include "..\OpenGLBasic\HalfEdge.h"
#include "..\OpenGLBasic\Boundary.h"
#include "..\PoissonShapeRepresentation\Material.h"

class CEntity
{
public:
	CEntity(void);
	~CEntity(void);
protected:
	CMaterial m_FrontMaterial;
	CMaterial m_BackMaterial;
	std::vector<CFace*> m_vecFaces;
	std::vector<CVertex*> m_vecVertices;
	std::vector<CEdge*> m_vecEdges;
	std::vector<CBoundary*> m_vecBoundaries;
	std::vector<CFace*> m_vecStrangeFaces;
public:
	void AddVertex(CVertex* pVertex);
	void AddEdge(CEdge* pE);
	void AddFace(CFace* pF);
	void DeleteEdge(CEdge* pE);
	void DeleteVertex(CVertex* pV);
	void DeleteFace(CFace* pF);

	int GetVertexNum(void);
	CVertex* GetVertexByIndex(int index);
	int GetEdgeNum(void);
	CEdge* GetEdgeByIndex(int index);
	int GetFaceNum(void);
	CFace* GetFaceByIndex(int index);

	virtual void Display(ObjectDisplayMode odm)=0;

	void ClearMemory(void);
	void Merge(CEntity* pEntity);
	CMaterial& GetMaterial(void);
	int GetBoundaryNum(void);
	CBoundary* GetBoundaryByIndex(int index);
	bool RemoveBoundary(CBoundary* pBound);
	bool AddBoundary(CBoundary* pBound);
	void ClearBoundary(void);
	void SetMaterial(CString name);
	void Subtract(CEntity* pEntity);
	bool Find(CVertex* pVertex);
	bool Find(CFace* pFace);
	bool Find(CEdge* pEdge);
	bool CheckTopology(void);
	void RemoveFace(CFace* pFace);
	void AddFace(vector<CVertex*>& vecVertices);
};
