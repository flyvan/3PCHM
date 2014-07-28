#include <SamplingTools.h>

SamplingTools::SamplingTools()
{
	this->Source_Interval = 16;
	this->Target_Interval = 16;
	this->Envelope_Interval = 10;
}

void SamplingTools::Sample(vtkPolyData *PolyData, vtkPolyData *PolyData_Sample, int Interval)
{
	VTK_CREATE(vtkKdTree, AGridkdtree);
	VTK_CREATE(vtkPoints, AGrid);
	VTK_CREATE(vtkPoints, Grid);
	double closestPointDist;
	double GridPoint[3];
	double Bounds[6];
	double UnitSize;
	PolyData->GetBounds(Bounds);
	for(int i = 0;i < 6;i+=2)
	{
		if(Bounds[(i+1)%6]-Bounds[(i)%6] > Bounds[(i+3)%6]-Bounds[(i+2)%6] && Bounds[(i+1)%6]-Bounds[(i)%6] > Bounds[(i+5)%6]-Bounds[(i+4)%6])
		{
			UnitSize = (Bounds[(i+1)%6]-Bounds[(i)%6]) / Interval;
		}
	}
	int intervalX = (Bounds[1]-Bounds[0]) / UnitSize;
	int intervalY = (Bounds[3]-Bounds[2]) / UnitSize;
	int intervalZ = (Bounds[5]-Bounds[4]) / UnitSize;

	for(int i = 0; i <= intervalX; i++)
	{
		for(int j = 0; j <= intervalY; j++)
		{
			for(int k = 0; k <= intervalZ; k++)
			{
				GridPoint[0] = Bounds[0] + i * UnitSize;
				GridPoint[1] = Bounds[2] + j * UnitSize;
				GridPoint[2] = Bounds[4] + k * UnitSize;
				Grid->InsertNextPoint(GridPoint);			
			}
		}
	}
    int *flag;
	flag = new int [Grid->GetNumberOfPoints()];
	for(int i = 0; i < Grid->GetNumberOfPoints(); i++)
		flag[i] = 0;
	AGridkdtree->BuildLocatorFromPoints(Grid);
	for(int i = 0; i< PolyData->GetNumberOfPoints(); i++)
	{
		vtkIdType n = AGridkdtree->FindClosestPoint(PolyData->GetPoint(i),closestPointDist);
		if(flag[n] == 0)
		{
			AGrid->InsertNextPoint(Grid->GetPoint(n));
			flag[n]++;
		}

	}
	delete flag;
	PolyData_Sample->SetPoints(AGrid);
}

void SamplingTools::EnvelopeSample(vtkPolyData *PolyData, vtkPolyData *PolyData_Sample, int Interval)
{

	double Bounds[6];
	PolyData->GetBounds(Bounds);
	double closestPointDist;
	double UnitSize;
	VTK_CREATE(vtkKdTree, Sourcekdtree);
	Sourcekdtree->BuildLocatorFromPoints(PolyData->GetPoints());

	VTK_CREATE(vtkPoints, Grid);

	for(int i = 0;i < 6;i+=2)
	{
		if(Bounds[(i+1)%6]-Bounds[(i)%6] > Bounds[(i+3)%6]-Bounds[(i+2)%6] && Bounds[(i+1)%6]-Bounds[(i)%6] > Bounds[(i+5)%6]-Bounds[(i+4)%6])
		{
			UnitSize = (Bounds[(i+1)%6]-Bounds[(i)%6]) / Interval;
		}
	}

	int intervalX = (Bounds[1]-Bounds[0]) / UnitSize;
	int intervalY = (Bounds[3]-Bounds[2]) / UnitSize;
	int intervalZ = (Bounds[5]-Bounds[4]) / UnitSize;
	double GridPoint[3];
	double SphereUnitSize = 1.414 * UnitSize;
	for(int i = 0; i <= intervalX; i++)
	{
		for(int j = 0; j <= intervalY; j++)
		{
			for(int k = 0; k <= intervalZ; k++)
			{
				GridPoint[0] = Bounds[0] + i * UnitSize;
				GridPoint[1] = Bounds[2] + j * UnitSize;
				GridPoint[2] = Bounds[4] + k * UnitSize;
				Sourcekdtree->FindClosestPoint(GridPoint,closestPointDist);
				if(closestPointDist < SphereUnitSize)
				{
					Grid->InsertNextPoint(GridPoint);
				}
			}
		}
	}
	PolyData_Sample->SetPoints(Grid);
}
