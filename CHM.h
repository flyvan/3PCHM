/*=========================================================================
	Copyright: Dec.5 2013
	BIT OTISLAB
	Medical Group
	Jingfan Fan
=========================================================================*/

#ifndef CHM_H
#define CHM_H

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
#include <vtkIterativeClosestPointTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkMath.h>
#include <vtkKdTree.h>
#include <vtkMatrix4x4.h>

#define VTK_NEW(type, name) name = vtkSmartPointer<type>::New()
#define VTK_CREATE(type, name) vtkSmartPointer<type>name = vtkSmartPointer<type>::New()
#define minieps 1e-6

class CHM_Register
{
public:
	CHM_Register();
	~CHM_Register();
	
	vtkSmartPointer<vtkPoints> Points_Source;
	vtkSmartPointer<vtkPoints> Points_Target;
	vtkSmartPointer<vtkPolyData> Poly_Source;
	vtkSmartPointer<vtkPolyData> Poly_Target;
	vtkSmartPointer<vtkPolyData> ConvexHull_Source;
	vtkSmartPointer<vtkPolyData> ConvexHull_Target;
	vtkSmartPointer<vtkMatrix4x4> BestTransform;

	void SetSource(vtkSmartPointer<vtkPolyData> Source, int sd);
	void SetTarget(vtkSmartPointer<vtkPolyData> Target, int sd);
    void Cal_CH_Source(bool Norm);
    void Cal_CH_Target(bool Norm);
	void Register();

private:	
	bool isSimilar(vtkPoints *a, vtkPoints *b);
};

void Points2PolyData(vtkPoints *Points, vtkPolyData *PolyData);
double Euc_Dist(double *pt1, double *pt2);
double Max_Bound(vtkPolyData *PolyData);
double xmult(double x1,double y1,double x2,double y2,double x0,double y0);
double NoiseGen(double p);

#endif
