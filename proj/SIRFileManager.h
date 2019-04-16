#pragma once
#include "..\OpenGLBasic\Solid.h"
class CSIRFileManager
{
public:
	CSIRFileManager(void);
	~CSIRFileManager(void);

	void RegisterModel(CSolid* pModel);
private:
	CSolid* m_pModel;
public:
	void Write(string path);
	void Read(string path);
	CSolid* GetModel(void);
};
