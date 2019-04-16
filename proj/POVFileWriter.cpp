#include "StdAfx.h"
#include ".\povfilewriter.h"
#include "nr.h"

CPOVFileWriter::CPOVFileWriter(void)
{
	m_pFileHead=NULL;
	SetupMaterialLibrary();
}

CPOVFileWriter::~CPOVFileWriter(void)
{
}

bool CPOVFileWriter::InitFile(CString FilePath)
{
	m_pFileHead = fopen((LPCTSTR)FilePath,"w");
	if (m_pFileHead == NULL) 
		return(false);

	fprintf(m_pFileHead,"// POV Image File Generated from Geometry Studio\n");
	fprintf(m_pFileHead,"background {\n");
	fprintf(m_pFileHead,"color rgb<1.0, 1.0, 1.0>");
	fprintf(m_pFileHead,"}\n");
	return true;

}

void CPOVFileWriter::SetupEnvironment(CEasyOpenGL& eogl,int Scene)
{

	switch(Scene)
	{
	case 0:
		{
			fprintf(m_pFileHead,"global_settings{\n");
			fprintf(m_pFileHead,"  adc_bailout 0.0039216\n");
			fprintf(m_pFileHead,"  max_trace_level 10\n");
			fprintf(m_pFileHead,"  noise_generator 2\n");
			fprintf(m_pFileHead,"  radiosity{\n");
			fprintf(m_pFileHead,"    adc_bailout 0.005\n");
			fprintf(m_pFileHead,"    count 200\n");
			fprintf(m_pFileHead,"    error_bound 0.5\n");
			fprintf(m_pFileHead,"    nearest_count 10\n");
			fprintf(m_pFileHead,"    pretrace_end 0.01\n");
			fprintf(m_pFileHead,"    recursion_limit 3\n");
			fprintf(m_pFileHead,"    always_sample off\n");
			fprintf(m_pFileHead,"    load_file \"radio.rad\"\n");
			fprintf(m_pFileHead,"    load_file \"radio.rad\"\n");
			fprintf(m_pFileHead,"  }\n");
			fprintf(m_pFileHead,"}\n");

			fprintf(m_pFileHead,"light_source{\n");
			fprintf(m_pFileHead,"  0*x\n");
			fprintf(m_pFileHead,"  color rgb<1.7,1.6,1.3>\n");
			fprintf(m_pFileHead,"  area_light\n");
			fprintf(m_pFileHead,"  <3,0,0><0,0,3>\n");
			fprintf(m_pFileHead,"  10,10\n");
			fprintf(m_pFileHead,"  adaptive 0\n");
			fprintf(m_pFileHead,"  jitter\n");
			fprintf(m_pFileHead,"  circular\n");
			fprintf(m_pFileHead,"  orient\n");
			fprintf(m_pFileHead,"  translate<20,20,-20>\n");
			fprintf(m_pFileHead,"  fade_distance 40\n");
			fprintf(m_pFileHead,"  fade_power 2\n");
			fprintf(m_pFileHead,"}\n");

			fprintf(m_pFileHead,"light_source{\n");
			fprintf(m_pFileHead,"  0*x\n");
			fprintf(m_pFileHead,"  color rgb<1.7,1.6,1.3>*5\n");
			fprintf(m_pFileHead,"  area_light\n");
			fprintf(m_pFileHead,"  <3,0,0><0,0,3>\n");
			fprintf(m_pFileHead,"  10,10\n");
			fprintf(m_pFileHead,"  adaptive 0\n");
			fprintf(m_pFileHead,"  jitter\n");
			fprintf(m_pFileHead,"  circular\n");
			fprintf(m_pFileHead,"  orient\n");
			fprintf(m_pFileHead,"  translate<20,20,-20>\n");
			fprintf(m_pFileHead,"  projected_through{object{box{<6,-3,-4><10,8,-1>}}}\n");
			fprintf(m_pFileHead,"  fade_distance 40\n");
			fprintf(m_pFileHead,"  fade_power 2\n");
			fprintf(m_pFileHead,"}\n");

		}
		break;
	case 1:
		{
			fprintf(m_pFileHead,"global_settings{\n");
			fprintf(m_pFileHead,"  assumed_gamma 1\n");
			fprintf(m_pFileHead,"  radiosity{\n");
			fprintf(m_pFileHead,"    pretrace_start 0.08\n");
			fprintf(m_pFileHead,"    pretrace_end 0.04\n");
			fprintf(m_pFileHead,"    count 35\n");
			fprintf(m_pFileHead,"    nearest_count 5\n");
			fprintf(m_pFileHead,"    error_bound 0.2\n");
			fprintf(m_pFileHead,"    recursion_limit 1\n");
			fprintf(m_pFileHead,"    low_error_factor 0.5\n");
			fprintf(m_pFileHead,"    gray_threshold 0.0\n");
			fprintf(m_pFileHead,"    minimum_reuse 0.015\n");
			fprintf(m_pFileHead,"    brightness 1\n");
			fprintf(m_pFileHead,"    adc_bailout 0.01\n");
			fprintf(m_pFileHead,"  }\n");
			fprintf(m_pFileHead,"}\n");


			fprintf(m_pFileHead,"light_source{\n");
			fprintf(m_pFileHead,"  <-1.5,1,-0.5>*10000\n");
			fprintf(m_pFileHead,"  color rgb<1.0,0.92,0.77>\n");
			fprintf(m_pFileHead,"}\n");
		}
	}

	int i,j;
	CCamera cam=eogl.GetCamera();
	GLdouble *mxMV=eogl.GetModelViewMatrix();

	CVector3d vpos=cam.GetPosition();

	float **a,**ident;
	a=matrix(1,4,1,4);
	ident=matrix(1,4,1,4);

	for (i=1;i<=4;i++)
	{
		for (j=1;j<=4;j++)
		{
			a[i][j]=mxMV[(i-1)*4+(j-1)];
		}
	}

	gauss_jordan(a,4,NULL,0);

	for (i=1;i<=4;i++)
	{
		for (j=1;j<=4;j++)
		{
			ident[i][j]=0;
			for (int k=1;k<=4;k++)
			{
				ident[i][j]+=mxMV[(i-1)*4+(k-1)]*a[k][j];
			}
		}
	}

	double pos[3],sky[3];


	for (i=0;i<3;i++)
	{
		pos[i]=a[i+1][1]*vpos[0]+a[i+1][2]*vpos[1]+a[i+1][3]*vpos[2]+a[i+1][4];
	}

	free_matrix(a,1,4,1,4);
	free_matrix(ident,1,4,1,4);

	fprintf(m_pFileHead,"camera {\n");                 
	fprintf(m_pFileHead,"perspective\n");                 
	fprintf(m_pFileHead,"  location <%.6lf, %.6lf, %.6lf>\n",pos[0],pos[1],pos[2]); 
	fprintf(m_pFileHead,"  up y\n");
	//fprintf(m_pFileHead,"  look_at <%.6lf, %.6lf, %.6lf>\n",xo,yo,zo);     
	fprintf(m_pFileHead,"  look_at <0, 0, 0>\n");     
	fprintf(m_pFileHead,"  angle %f\n",eogl.GetCamera().GetHeightAngle());     
	fprintf(m_pFileHead,"}\n");    


	//eogl.BeginContext();
	//for (i=0;i<8;i++)
	//{
	//	CLight lit=eogl.GetLight(i);
	//	if(lit.IsEnabled())
	//	{
	//		GLfloat * pLPos=lit.GetPosition();
	//		GLfloat * pLDif=lit.GetDiffuse();
	//		fprintf(m_pFileHead,"light_source { <%.6lf, %.6lf, %.6lf> rgb <%.6lf, %.6lf, %.6lf> }\n",pLPos[0],pLPos[1],pLPos[2],pLDif[0],pLDif[1],pLDif[2]);
	//	}
	//}
	//eogl.EndContext();

}

void CPOVFileWriter::WriteEntity(CEntity* pEntity,CString material)
{
	fprintf(m_pFileHead, "mesh { \n");   
	CFace* pFace;
	CVertex* pV,*pV1,*pV2,*pV3;
	CVector3d coord,norm;
	CHalfEdge* pHe;
	int numTri = pEntity->GetFaceNum();
	CString pigment,finish,interior;
	InterpretMaterial(material,pigment,finish,interior);
	for (int i = 0; i < numTri; i++)
	{
		fprintf(m_pFileHead, " smooth_triangle { ");  //  begin triangle
		pFace=pEntity->GetFaceByIndex(i);

		pHe = pFace->GetFirstEdge ();
		pV=pHe->GetVertex();
		coord=pV->GetCoordinate();
		fprintf(m_pFileHead, " <%f, %f, %f>", coord[0], coord[1],coord[2]);
		fprintf(m_pFileHead, ", ");
		norm=pV->GetNormal();
		fprintf(m_pFileHead, " <%f, %f, %f>", norm[0], norm[1],norm[2]);
		fprintf(m_pFileHead, ", ");

		pHe =pFace->GetNextEdge(pHe);
		pV=pHe->GetVertex();
		coord=pV->GetCoordinate();
		fprintf(m_pFileHead, " <%f, %f, %f>", coord[0], coord[1],coord[2]);
		fprintf(m_pFileHead, ", ");
		norm=pV->GetNormal();
		fprintf(m_pFileHead, " <%f, %f, %f>", norm[0], norm[1],norm[2]);
		fprintf(m_pFileHead, ", ");

		pHe =pFace->GetNextEdge(pHe);
		pV=pHe->GetVertex();
		coord=pV->GetCoordinate();
		fprintf(m_pFileHead, " <%f, %f, %f>", coord[0], coord[1],coord[2]);
		fprintf(m_pFileHead, ", ");
		norm=pV->GetNormal();
		fprintf(m_pFileHead, " <%f, %f, %f>", norm[0], norm[1],norm[2]);
		fprintf(m_pFileHead, "}\n");           // end triangle
	}
	fprintf(m_pFileHead, "texture {%s %s} %s\n",m_pigmentLibrary[pigment],m_finishLibrary[finish],m_interiorLibrary[interior]);


	fprintf(m_pFileHead, "} \n\n");

}

void CPOVFileWriter::WriteVector(CVector3d pos, CVector3d dir)
{
	CVector3d end1,end2;
	end1=pos+dir*0.15;
	end2=pos+dir*0.25;
	fprintf(m_pFileHead,"sphere {\n");
	fprintf(m_pFileHead,"<%f, %f, %f>\n",pos[0],pos[1],pos[2]);
	fprintf(m_pFileHead,"0.018\n");
	fprintf(m_pFileHead, "texture {%s}\n",m_pigmentLibrary["GlassVicks"]);
	fprintf(m_pFileHead,"}\n");
    
	fprintf(m_pFileHead,"cylinder {\n");
	fprintf(m_pFileHead,"<%f, %f, %f>,\n",pos[0],pos[1],pos[2]);
	fprintf(m_pFileHead,"<%f, %f, %f>,\n",end1[0],end1[1],end1[2]);
	fprintf(m_pFileHead,"0.01\n");
	fprintf(m_pFileHead, "texture {%s}\n",m_pigmentLibrary["GlassGreen"]);
	fprintf(m_pFileHead,"}\n");

	fprintf(m_pFileHead,"cone {\n");
	fprintf(m_pFileHead,"<%f, %f, %f>,0.018,\n",end1[0],end1[1],end1[2]);
	fprintf(m_pFileHead,"<%f, %f, %f>,0.0\n",end2[0],end2[1],end2[2]);
	fprintf(m_pFileHead, "texture {%s}\n",m_pigmentLibrary["GlassGreen"]);
	fprintf(m_pFileHead,"}\n");

}

void CPOVFileWriter::CloseFile(void)
{
	fclose(m_pFileHead);
}

void CPOVFileWriter::WriteTransitionCylinder(CTransitionCylinder* pTCylinder,CString material)
{
	CString pigment,finish,interior;
	InterpretMaterial(material,pigment,finish,interior);
	CVector3d end1,end2;
	end1=pTCylinder->GetCenter();
	end2=end1+pTCylinder->GetDirection()*pTCylinder->GetHeight();
	fprintf(m_pFileHead,"cone {\n");
	fprintf(m_pFileHead,"<%f, %f, %f>, %f\n",end1[0],end1[1],end1[2],pTCylinder->GetBottomRadius());
	fprintf(m_pFileHead,"<%f, %f, %f>, %f\n",end2[0],end2[1],end2[2],pTCylinder->GetTopRadius());
	fprintf(m_pFileHead, "texture {%s %s} %s\n",m_pigmentLibrary[pigment],m_finishLibrary[finish],m_interiorLibrary[interior]);
	fprintf(m_pFileHead,"}\n");
}

void CPOVFileWriter::WriteTransitionSphere(CTransitionSphere* pTSphere,CString material)
{
	CString pigment,finish,interior;
	InterpretMaterial(material,pigment,finish,interior);

	CVector3d pos=pTSphere->GetCenter();
	fprintf(m_pFileHead,"sphere {\n");
	fprintf(m_pFileHead,"<%f, %f, %f>\n",pos[0],pos[1],pos[2]);
	fprintf(m_pFileHead,"%f\n",pTSphere->GetRadius());
	fprintf(m_pFileHead, "texture {%s %s} %s\n",m_pigmentLibrary[pigment],m_finishLibrary[finish],m_interiorLibrary[interior]);
	fprintf(m_pFileHead,"}\n");

}

void CPOVFileWriter::SetupMaterialLibrary(void)
{
	m_pigmentLibrary["None"]="";
	m_pigmentLibrary["GlassRed"]="pigment{color rgbf <0.95, 0.0, 0.0, 0.95>}";
	m_pigmentLibrary["GlassGreen"]="pigment{color rgbf <0.0, 0.95, 0.0, 0.95>}";
	m_pigmentLibrary["GlassBlue"]="pigment{color rgbf <0.0, 0.0, 0.95, 0.95>}";
	m_pigmentLibrary["GlassSkyBlue"]="pigment{color rgbf <0.0, 1.0, 1.0, 0.95>}";
	m_pigmentLibrary["GlassRuby"]="pigment{color rgbf <0.9, 0.1, 0.2, 0.8>}";
	m_pigmentLibrary["GlassBeer"]="pigment{color rgbf <0.7, 0.5, 0.1, 0.6>}";
	m_pigmentLibrary["GlassOrange"]="pigment{color rgbf <1.0, 0.5, 0.0, 0.95>}";
	m_pigmentLibrary["GlassVicks"]="pigment{color rgbf <0.1, 0.15, 0.5, 0.9>}";
	m_pigmentLibrary["GlassYellow"]="pigment{color rgbf <0.8, 0.8, 0.2, 0.8>}";
	m_pigmentLibrary["GlassGeneral"]="pigment{color rgbf <0.97, 0.99, 0.98, 0.9>}";
	m_pigmentLibrary["MineralAmethyst"]="pigment{color rgb<0.4392, 0.1765, 0.3137>}";
	m_pigmentLibrary["MineralApatite"]="pigment{color rgb<0.0549, 0.3451, 0.4314>}";
	m_pigmentLibrary["MineralAquamarine"]="pigment{color rgb<0.5333, 0.6157, 0.6196>}";
	m_pigmentLibrary["MineralAzurite"]="pigment{color rgb<0.2863, 0.3569, 0.7216>}";
	m_pigmentLibrary["MineralCitrine"]="pigment{color rgb<0.3020, 0.2314, 0.1529>}";
	m_pigmentLibrary["MineralEmerald"]="pigment{color rgb<0.0471, 0.4000, 0.2549>}";
	m_pigmentLibrary["MineralSapphire"]="pigment{color rgb<0.2118, 0.3020, 0.4980>}";
	m_pigmentLibrary["MineralTopaz"]="pigment{color rgb<0.7333, 0.7451, 0.6980>}";
	m_pigmentLibrary["MineralRuby"]="pigment{color rgb<0.8039, 0.2039, 0.5569>}";
	m_pigmentLibrary["MineralGreen"]="pigment{color rgb<0.0, 0.95, 0.0>}";
	m_pigmentLibrary["MetalBrass"]="pigment{color rgb<0.70, 0.55, 0.40>}";
	m_pigmentLibrary["MetalCopper"]="pigment{color rgb<0.70, 0.25, 0.15>}";
	m_pigmentLibrary["MetalChrome"]="pigment{color rgb<0.95, 0.95, 0.95>}";
	m_pigmentLibrary["MetalSilver"]="pigment{color rgb<0.94, 0.93, 0.80>}";
	m_pigmentLibrary["MetalRed"]="pigment{color rgb<0.95, 0.0, 0.0>}";
	m_pigmentLibrary["MetalGreen"]="pigment{color rgb<0.0, 0.95, 0.0>}";
	m_pigmentLibrary["MetalBlue"]="pigment{color rgb<0.0, 0.0, 0.95>}";
	m_pigmentLibrary["MetalOrange"]="pigment{color rgb<1.0, 0.5, 0.0>}";
	m_pigmentLibrary["MetalYellow"]="pigment{color rgb<1.0, 1.0, 0.0>}";

	m_finishLibrary["None"]="";
	m_finishLibrary["FGlass1"]="finish{specular 0.9 roughness 0.001 ambient 0 diffuse 0 reflection{0.0, 1.0 fresnel on}conserve_energy}";
	m_finishLibrary["FGlass2"]="finish{specular 0.7 roughness 0.001 ambient 0 diffuse 0 reflection{0.2, 1.0 fresnel on}conserve_energy}";
	m_finishLibrary["FGlass3"]="finish{specular 0.6 roughness 0.002 ambient 0 diffuse 0.1 brilliance 5 reflection{0.1, 1.0 fresnel on}conserve_energy}";
	m_finishLibrary["FGlass4"]="finish{specular 0.6 roughness 0.005 ambient 0 diffuse 0.15 brilliance 4 reflection{0.2, 1.0 fresnel on}conserve_energy}";
	m_finishLibrary["FGlass5"]="finish{specular 0.8 roughness 0.001 ambient 0 diffuse 0 reflection{0.05, 1.0}conserve_energy}";
	m_finishLibrary["FMelta1"]="finish{ambient 0.35 brilliance 2 diffuse 0.3 metallic specular 0.8 roughness 1/20 reflection 0.1}";
	m_finishLibrary["FMelta2"]="finish{ambient 0.3 brilliance 3 diffuse 0.4 metallic specular 0.7 roughness 1/60 reflection 0.25}";
	m_finishLibrary["FMelta3"]="finish{ambient 0.25 brilliance 4 diffuse 0.5 metallic specular 0.8 roughness 1/80 reflection 0.5}";
	m_finishLibrary["FMelta4"]="finish{ambient 0.15 brilliance 5 diffuse 0.6 metallic specular 0.8 roughness 1/100 reflection 0.65}";
	m_finishLibrary["FMelta5"]="finish{ambient 0.1 brilliance 6 diffuse 0.7 metallic specular 0.8 roughness 1/120 reflection 0.8}";



	m_interiorLibrary["None"]="";
	m_interiorLibrary["IGlass1"]="interior{ior 1.5 fade_distance 1.0 fade_power 2}";
	m_interiorLibrary["IGlass2"]="interior{ior 1.5 fade_distance 0.5 fade_power 1001}";
	m_interiorLibrary["IGlass3"]="interior{ior 1.5 fade_distance 1.0 fade_power 1001}";
	m_interiorLibrary["IGlass4"]="interior{ior 1.5 fade_distance 2.0 fade_power 1001}";
	m_interiorLibrary["IDGlass1"]="interior{ior 1.5 dispersion 1.02 fade_distance 1 fade_power 1001}";
	m_interiorLibrary["IDGlass2"]="interior{ior 1.5 dispersion 1.15 fade_distance 1 fade_power 1001}";
	m_interiorLibrary["ICGlass1"]="interior{ior 1.5 caustics 0.5 fade_distance 1 fade_power 1001}";
	m_interiorLibrary["ICGlass2"]="interior{ior 1.5 caustics 1.0 fade_distance 1 fade_power 1001}";


}

void CPOVFileWriter::WriteBoundary(CBoundary* pBound)
{
	int i;
	CVector3d coord1,coord2;
	for (i=0;i<pBound->GetVertexNum()-1;i++)
	{
		coord1=pBound->GetVertexByIndex(i)->GetCoordinate();
		coord2=pBound->GetVertexByIndex(i+1)->GetCoordinate();

		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["MetalRed"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");
	}
	coord1=pBound->GetVertexByIndex(i)->GetCoordinate();
	coord2=pBound->GetVertexByIndex(0)->GetCoordinate();

	fprintf(m_pFileHead,"cylinder {\n");
	fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
	fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
	fprintf(m_pFileHead,"0.01\n");
	fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["MetalRed"],m_finishLibrary["FMetal3"]);
	fprintf(m_pFileHead,"}\n");

	for (i=0;i<pBound->GetVertexNum();i++)
	{
		coord1=pBound->GetVertexByIndex(i)->GetCoordinate();
		fprintf(m_pFileHead,"sphere {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"%f\n",0.01);
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["MetalRed"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");
	}


}

void CPOVFileWriter::InterpretMaterial(CString mat,CString& pigment,CString& finish, CString& interior)
{
	pigment=mat.SpanExcluding("|");
	mat.Delete(0,pigment.GetLength()+1);
	finish=mat.SpanExcluding("|");
	mat.Delete(0,finish.GetLength()+1);
	interior=mat;
}

void CPOVFileWriter::WriteBox(CBox3& box,CString material,bool wire)
{

	CString pigment,finish,interior;
	InterpretMaterial(material,pigment,finish,interior);

	if(wire)
	{
		CVector3d coord1,coord2;
		for (int i=0;i<8;i++)
		{
			coord1=box.Vertex(i);
			fprintf(m_pFileHead,"sphere {\n");
			fprintf(m_pFileHead,"<%f, %f, %f>\n",coord1[0],coord1[1],coord1[2]);
			fprintf(m_pFileHead,"%f\n",0.01);
			fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
			fprintf(m_pFileHead,"}\n");
		}

		coord1=box.Vertex(0);

		coord2=box.Vertex(1);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");

		coord2=box.Vertex(2);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");

		coord2=box.Vertex(4);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");


		coord1=box.Vertex(7);

		coord2=box.Vertex(3);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");

		coord2=box.Vertex(5);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");

		coord2=box.Vertex(6);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");

		coord1=box.Vertex(2);
		coord2=box.Vertex(3);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");
		coord2=box.Vertex(6);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");

		coord1=box.Vertex(5);
		coord2=box.Vertex(1);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");
		coord2=box.Vertex(4);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");


		coord1=box.Vertex(3);
		coord2=box.Vertex(1);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");

		coord1=box.Vertex(4);
		coord2=box.Vertex(6);
		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["GlassOrange"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");

	}
	else
	{
		CVector3d coord1,coord2;
		coord1=box.GetMinCorner();
		coord2=box.GetMaxCorner();
	
		fprintf(m_pFileHead,"box {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead, "texture {%s %s} %s\n",m_pigmentLibrary[pigment],m_finishLibrary[finish],m_interiorLibrary[interior]);
		fprintf(m_pFileHead,"}\n");

	}

}

void CPOVFileWriter::WriteStroke(CStroke* pStroke)
{
	int i;
	CVector3d coord1,coord2;
	for (i=0;i<pStroke->GetPointsNum()-1;i++)
	{
		coord1=pStroke->GetPoint(i);
		coord2=pStroke->GetPoint(i+1);

		fprintf(m_pFileHead,"cylinder {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"<%f, %f, %f>,\n",coord2[0],coord2[1],coord2[2]);
		fprintf(m_pFileHead,"0.01\n");
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["MetalGreen"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");
	}

	for (i=0;i<pStroke->GetPointsNum();i++)
	{
		coord1=pStroke->GetPoint(i);
		fprintf(m_pFileHead,"sphere {\n");
		fprintf(m_pFileHead,"<%f, %f, %f>\n",coord1[0],coord1[1],coord1[2]);
		fprintf(m_pFileHead,"%f\n",0.01);
		fprintf(m_pFileHead, "texture {%s %s}\n",m_pigmentLibrary["MetalGreen"],m_finishLibrary["FMetal3"]);
		fprintf(m_pFileHead,"}\n");
	}
}
