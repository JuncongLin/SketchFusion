#pragma once
#include "..\OpenGLBasic\EasyOpenGL.h"
#include "Entity.h"
#include "TransitionCylinder.h"
#include "TransitionSphere.h"
#include "Box3.h"
#include "Stroke.h"
#include <map>
class CPOVFileWriter
{
public:
	CPOVFileWriter(void);
	~CPOVFileWriter(void);
	bool InitFile(CString FilePath);
private:
	FILE* m_pFileHead;
	std::map<CString,CString> m_pigmentLibrary;
	std::map<CString,CString> m_finishLibrary;
	std::map<CString,CString> m_interiorLibrary;
public:
	void SetupEnvironment(CEasyOpenGL& eogl,int Scene);
	void WriteEntity(CEntity* pEntity,CString material);
	void WriteVector(CVector3d pos, CVector3d dir);
	void CloseFile(void);
	void WriteTransitionCylinder(CTransitionCylinder* pTCylinder,CString material="GlassVicks|None|None");
	void WriteTransitionSphere(CTransitionSphere* pTSphere,CString material="GlassVicks|None|None");
	void SetupMaterialLibrary(void);
	void WriteBoundary(CBoundary* pBound);
	void InterpretMaterial(CString mat,CString& pigment,CString& finish, CString& interior);
	void WriteBox(CBox3& box,CString material="GlassRed|None|None",bool wire=true);
	void WriteStroke(CStroke* pStroke);
};
