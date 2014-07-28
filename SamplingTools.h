/*
Copyright 2013 BIT OTISMED
*/

#ifndef SamplingTools_H
#define SamplingTools_H

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
#include <cmath>

#define VTK_NEW(type, name) name = vtkSmartPointer<type>::New()
#define VTK_CREATE(type, name) vtkSmartPointer<type>name = vtkSmartPointer<type>::New()

class SamplingTools
{
public:
	SamplingTools();
	//~SamplingTools();
	
	void Sample(vtkPolyData *PolyData, vtkPolyData *PolyData_Sample, int Interval);
	void EnvelopeSample(vtkPolyData *PolyData, vtkPolyData *PolyData_Sample, int Interval);
	int Source_Interval, Target_Interval, Envelope_Interval;	
};

#endif
