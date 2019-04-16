#pragma once
#include "..\OpenGLBasic\Solid.h"
#include "TransitionPrimitive.h"
#include "ParticleSamplingSys.h"
#include "Stroke.h"
class CFSNFileManager
{
public:
	CFSNFileManager(void);
	~CFSNFileManager(void);
private:
	vector<CSolid*> m_vecComponents;
	vector<CSolid*> m_vecTComponents;
	vector<CStroke*> m_vecSilhouetteStroke;
	vector<CStroke*> m_vecStructureStroke;

	CTransitionPrimitive *m_pTPrimitive;

	CSolid *m_pFusionModel;
	CParticleSamplingSys *m_pParticleSys;

public:
	void Write(string path);
	void Read(string path);

	int GetComponentNum(void);
	CSolid* GetComponentByIndex(int index);
	int GetTComponentNum(void);
	CSolid* GetTComponentByIndex(int index);
	void RegisterComponent(CSolid* pComponent);
	void RegisterTComponent(CSolid* pComponent);
	void RegisterTPrimitive(CTransitionPrimitive* pPrimitive);
	void RegisterFusionModel(CSolid* pFusModel);
	CSolid* GetFusionModel(void);
	void RegisterPS(CParticleSamplingSys* pPS);
	CTransitionPrimitive* GetTPrimitive(void);
	CParticleSamplingSys* GetPS(void);
	void RegisterSilhouette(CStroke* pStroke);
	CStroke* GetSilhouetteByIndex(int index);
	int GetSilhouetteNum(void);
	void RemoveSilhouette(CStroke* pStroke);
	void RegisterStructure(CStroke* pStroke);
	int GetStructureNum(void);
	CStroke* GetStructureByIndex(int index);
	void RemoveStructure(CStroke* pStroke);
};
