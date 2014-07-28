/*
Copyright 2013 BIT OTISMED
*/

#ifndef MeshGMM_H
#define MeshGMM_H

/* Include VTK header files*/
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkDelaunay3D.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkTriangle.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkMath.h>
#include <vtkKdTree.h>
#include <vtkMatrix4x4.h>
#include <vtkLineSource.h>
#include <vtkQuadricClustering.h>
#include <vtkQuadricDecimation.h>
#include <vtkUnsignedCharArray.h>
#include <cmath>
#include <time.h>
#include <cstdlib>
#include <SamplingTools.h>
#include <iostream>

#include <vtkTextureMapToPlane.h>
#include <vtkPlaneSource.h>
#include <vtkTexture.h>
#include <vtkImageData.h>
#include <vtkJPEGReader.h>
#include <CHM.h>

#define VTK_NEW(type, name) name = vtkSmartPointer<type>::New()
#define VTK_CREATE(type, name) vtkSmartPointer<type>name = vtkSmartPointer<type>::New()
#define minieps 1e-6
#define inf 1000000.00
#define inf_s 10000.00
#define SQR(X)  ((X)*(X))

class MeshGMM_Register
{
public:
	MeshGMM_Register();
	~MeshGMM_Register();

	vtkSmartPointer<vtkPoints> Points_Source;
	vtkSmartPointer<vtkPoints> Points_Target;
	vtkSmartPointer<vtkPolyData> Poly_Source;
	vtkSmartPointer<vtkPolyData> Poly_TransformedSource;
	vtkSmartPointer<vtkPolyData> Poly_Target;
	vtkSmartPointer<vtkPoints> CH_SourceCell;
	vtkSmartPointer<vtkPoints> CH_TargetCell;
	vtkSmartPointer<vtkPolyData> ConvexHull_Source;
	vtkSmartPointer<vtkPolyData> ConvexHull_TransformedSource;
	vtkSmartPointer<vtkPolyData> ConvexHull_Target;
	vtkSmartPointer<vtkMatrix4x4> BestTransform;
	vtkSmartPointer<vtkDelaunay3D> SourceDelaunay;
	vtkSmartPointer<vtkDelaunay3D> TargetDelaunay;
	vtkSmartPointer<vtkDataSetSurfaceFilter> SourceSurfaceFilter;
	vtkSmartPointer<vtkDataSetSurfaceFilter> TargetSurfaceFilter;


	void SetSource(vtkSmartPointer<vtkPolyData> Source);
	void SetTarget(vtkSmartPointer<vtkPolyData> Target);
	void Cal_CH_Source();
	void Cal_CH_Target();
    void Register(bool FlagReg);
	void SourceOutput(int p);
	void TargetOutput(int p);
	const char *MatlabPath;
	std::string Matlabstr;
	/* parameters */
    double BestTriSimilarity, BestSimilarity, *CHGMML2;
	double InitTime, Output2MatlabTime, CellMatchingTime;
	bool isOutput2Matlab;
	double Alpha, Beta;
	double SourceSQRExpectation, TargetSQRExpectation;
	bool NormalCH;
	bool isTexture;
private:
	double DotProduct(double *A, double *B);
	double distance(double *pt1, double *pt2);
	double SQRdistance(double *pt1, double *pt2);
	double TriangleSimilarity(double *Tri_AngleA, double *Tri_AngleB, double AreaA, double AreaB, int Order);
	void CrossProduct(double *A, double *B, double *Product);
	void Output2Matlab(int axis, int p, int q);
	void Output2MatlabCore();
	void InitSource();
	void InitTarget();
    void CellMatching(bool FlagReg);
	void TotalArea(vtkPolyData* A, double * AreaA, double * TotalAreaA);
	void CellCenter(vtkPolyData* A, vtkPoints* B, double * Area, double ** Normal, double *** VectorX, double *** VectorY, 
		double **** AngleMap, double **NormalAngle, double **Weight,double **Map, double **Tri_AngleMap, double **NTpara);
	double GaussTransform(int p, int q, int t, double Scale, double SizeA, double SizeB);
	double GaussTransform_Self(int p, double Size, double **Weight,double **ExpMap);
	void Map2ExpMap(double **Map, double **ExpMap, double Size, double Scale);
	void Map2HalfMap(double **Map, double **HalfMap, double Size);
	void Init();
	int PrintPercent(int p, int size, int count, int max_iter);
	double **SourceNTpara, **TargetNTpara;
	double *GMML2_0, *GMML2_1;
	double **SourceMap, **TargetMap;
	double **SourceHalfMap, **TargetHalfMap;
	double **SourceExpMap, **TargetExpMap;
	double **SourceNormalAngle, **TargetNormalAngle;
	double **SourceWeight, **TargetWeight;
	double ****SourceAngleMap, ****TargetAngleMap; //AngleMap[No.][X/Y][i][j]
	//double *SourceExpectation, *TargetExpectation;

	//double *SourceScale, *TargetScale;
	double *SourceArea, *TargetArea;
	double **SourceNormal, **TargetNormal;
	double ***SourceVectorX, ***TargetVectorX;
	double ***SourceVectorY, ***TargetVectorY;
	double **SourceTri_AngleMap, **TargetTri_AngleMap;
	double *TotalAreaSource, *TotalAreaTarget;
};

#endif
