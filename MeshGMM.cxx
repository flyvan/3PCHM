/*
Copyright 2013 BIT OTISMED
*/
#include "MeshGMM.h"

MeshGMM_Register::MeshGMM_Register()
{
	VTK_NEW(vtkPoints, Points_Source);
	VTK_NEW(vtkPoints, Points_Target);
	VTK_NEW(vtkPolyData, Poly_Source);
	VTK_NEW(vtkPolyData, Poly_TransformedSource);
	VTK_NEW(vtkPolyData, Poly_Target);
	VTK_NEW(vtkPoints, CH_SourceCell);
	VTK_NEW(vtkPoints, CH_TargetCell);
	VTK_NEW(vtkPolyData, ConvexHull_Source);
	VTK_NEW(vtkPolyData, ConvexHull_TransformedSource);
	VTK_NEW(vtkPolyData, ConvexHull_Target);
	VTK_NEW(vtkMatrix4x4, BestTransform);
	VTK_NEW(vtkDelaunay3D, SourceDelaunay);
	VTK_NEW(vtkDelaunay3D, TargetDelaunay);
	VTK_NEW(vtkDataSetSurfaceFilter, SourceSurfaceFilter);
	VTK_NEW(vtkDataSetSurfaceFilter, TargetSurfaceFilter);
//	VTK_NEW(vtkLineSource, lineSource);

	this->BestSimilarity = 0.0;
	this->BestTriSimilarity = 0.0;
}
MeshGMM_Register::~MeshGMM_Register()
{
}
void MeshGMM_Register::SetSource(vtkSmartPointer<vtkPolyData> Source)
{

	this->Points_Source->Initialize();
	this->Poly_Source->Initialize();
/*	this->Poly_Source->SetPoints(Source->GetPoints());
*/
	int step = Source->GetPoints()->GetNumberOfPoints() / 5000 + 1;
	for (int i = 0; i * step < Source->GetPoints()->GetNumberOfPoints(); i ++)
	{
		this->Points_Source->InsertPoint(i, Source->GetPoints()->GetPoint(i * step));
	}
	this->Poly_Source->SetPoints(this->Points_Source);
	
	
}
void MeshGMM_Register::SetTarget(vtkSmartPointer<vtkPolyData> Target)
{

	this->Points_Target->Initialize();
	this->Poly_Target->Initialize();
/*	this->Poly_Target->SetPoints(Target->GetPoints());
*/
	int step = Target->GetPoints()->GetNumberOfPoints() / 5000 + 1;
	for (int i = 0; i * step < Target->GetPoints()->GetNumberOfPoints(); i ++)
	{
		this->Points_Target->InsertPoint(i, Target->GetPoints()->GetPoint(i * step));
	}
	this->Poly_Target->SetPoints(this->Points_Target);

}
void MeshGMM_Register::Cal_CH_Source()
{
	if(NormalCH)
	{
		VTK_CREATE(vtkDelaunay3D, delaunay);
		delaunay->SetInput(this->Poly_Source);
		delaunay->Update();
		delaunay->GetBoundingTriangulation();
		VTK_CREATE(vtkDataSetSurfaceFilter, surfaceFilter);
		surfaceFilter->SetInputConnection(delaunay->GetOutputPort());
		surfaceFilter->Update(); 
		
		VTK_CREATE(vtkPolyData, CH);
		CH->DeepCopy(surfaceFilter->GetOutput());
		
		VTK_CREATE(vtkPoints, NewPoint);
		VTK_CREATE(vtkPolyData, NewData);
		VTK_CREATE(vtkCellArray, vertices);
		vtkIdType pid[1];
		int i, j;
		bool near;
		for (i = 0; i < CH->GetNumberOfPoints(); i++)
		{
			near = false;
			for (j = 0; j < NewPoint->GetNumberOfPoints(); j++)
			{
				if (distance(CH->GetPoint(i), NewPoint->GetPoint(j)) < 0.25)
					near = true;
			}
			if (!near)
			{
				pid[0] = NewPoint->GetNumberOfPoints();
				NewPoint->InsertNextPoint(CH->GetPoint(i));
				vertices->InsertNextCell(1, pid);
			}
		}
		NewData->SetPoints(NewPoint);
		NewData->SetVerts(vertices);

		VTK_CREATE(vtkDelaunay3D, delaunay2nd);
		delaunay2nd->SetInput(NewData);
		delaunay2nd->Update();
		delaunay2nd->GetBoundingTriangulation();
		VTK_CREATE(vtkDataSetSurfaceFilter, surfaceFilter2nd);
		surfaceFilter2nd->SetInputConnection(delaunay2nd->GetOutputPort());
		surfaceFilter2nd->Update(); 
		ConvexHull_Source->DeepCopy(surfaceFilter2nd->GetOutput());
	}
	else
	{
		SourceDelaunay->RemoveAllInputs();
		SourceDelaunay->SetInput(this->Poly_Source);
	//	SourceDelaunay->SetAlpha(inf);
		SourceDelaunay->Update();
		SourceDelaunay->GetBoundingTriangulation();
		SourceSurfaceFilter->SetInputConnection(SourceDelaunay->GetOutputPort());
		SourceSurfaceFilter->Update(); 
		ConvexHull_Source->DeepCopy(SourceSurfaceFilter->GetOutput());

		//vtkSmartPointer<vtkQuadricClustering> decimate =
		//    vtkSmartPointer<vtkQuadricClustering>::New();

		//vtkSmartPointer<vtkQuadricDecimation> decimate =
		//    vtkSmartPointer<vtkQuadricDecimation>::New();
		//	vtkSmartPointer<vtkQuadricDecimation> decimate_1 =
		//    vtkSmartPointer<vtkQuadricDecimation>::New();

		//decimate->SetInput(SourceSurfaceFilter->GetOutput());
		//decimate->Update();

		//decimate_1->SetInput(decimate->GetOutput());
		//decimate_1->Update();
		//ConvexHull_Source->DeepCopy(decimate_1->GetOutput());
	}

}
void MeshGMM_Register::Cal_CH_Target()
{
	if(NormalCH)
	{
		VTK_CREATE(vtkDelaunay3D, delaunay);
		delaunay->SetInput(this->Poly_Target);
		delaunay->Update();
		delaunay->GetBoundingTriangulation();
		VTK_CREATE(vtkDataSetSurfaceFilter, surfaceFilter);
		surfaceFilter->SetInputConnection(delaunay->GetOutputPort());
		surfaceFilter->Update(); 

		VTK_CREATE(vtkPolyData, CH);
		CH->DeepCopy(surfaceFilter->GetOutput());
		
		VTK_CREATE(vtkPoints, NewPoint);
		VTK_CREATE(vtkPolyData, NewData);
		VTK_CREATE(vtkCellArray, vertices);
		vtkIdType pid[1];
		int i, j;
		bool near;
		for (i = 0; i < CH->GetNumberOfPoints(); i++)
		{
			near = false;
			for (j = 0; j < NewPoint->GetNumberOfPoints(); j++)
			{
				if (distance(CH->GetPoint(i), NewPoint->GetPoint(j)) < 0.25)
					near = true;
			}
			if (!near)
			{
				pid[0] = NewPoint->GetNumberOfPoints();
				NewPoint->InsertNextPoint(CH->GetPoint(i));
				vertices->InsertNextCell(1, pid);
			}
		}
		NewData->SetPoints(NewPoint);
		NewData->SetVerts(vertices);

		VTK_CREATE(vtkDelaunay3D, delaunay2nd);
		delaunay2nd->SetInput(NewData);
		delaunay2nd->Update();
		delaunay2nd->GetBoundingTriangulation();
		VTK_CREATE(vtkDataSetSurfaceFilter, surfaceFilter2nd);
		surfaceFilter2nd->SetInputConnection(delaunay2nd->GetOutputPort());
		surfaceFilter2nd->Update(); 
		ConvexHull_Target->DeepCopy(surfaceFilter2nd->GetOutput());
	}
	else
	{
		TargetDelaunay->RemoveAllInputs();
		TargetDelaunay->SetInput(this->Poly_Target);
	//	TargetDelaunay->SetAlpha(inf);
		TargetDelaunay->Update();
		TargetDelaunay->GetBoundingTriangulation();
		TargetSurfaceFilter->SetInputConnection(TargetDelaunay->GetOutputPort());
		TargetSurfaceFilter->Update(); 
		ConvexHull_Target->DeepCopy(TargetSurfaceFilter->GetOutput());

		//vtkSmartPointer<vtkQuadricClustering> decimate =
		//    vtkSmartPointer<vtkQuadricClustering>::New();

		//vtkSmartPointer<vtkQuadricDecimation> decimate =
		//    vtkSmartPointer<vtkQuadricDecimation>::New();

		//decimate->SetInput(surfaceFilter->GetOutput());
		//decimate->Update();
		//ConvexHull_Target->DeepCopy(decimate->GetOutput());
	}
}
void MeshGMM_Register::Register(bool FlagReg)
{
	clock_t start, finish;
	start = clock();

	Init();
	InitSource();
	InitTarget();
	
	CellCenter(ConvexHull_Source, CH_SourceCell, SourceArea, SourceNormal, SourceVectorX, SourceVectorY, 
		SourceAngleMap, SourceNormalAngle, SourceWeight, SourceMap, SourceTri_AngleMap, SourceNTpara);
	CellCenter(ConvexHull_Target, CH_TargetCell, TargetArea, TargetNormal, TargetVectorX, TargetVectorY, 
		TargetAngleMap, TargetNormalAngle, TargetWeight, TargetMap, TargetTri_AngleMap, TargetNTpara);
	TotalArea(ConvexHull_Source, SourceArea, TotalAreaSource);
	TotalArea(ConvexHull_Target, TargetArea, TotalAreaTarget);
	
	cout << *TotalAreaSource << " " << *TotalAreaTarget << endl;
	
	SourceSQRExpectation = 2 * (*TotalAreaSource) / ConvexHull_Source->GetNumberOfCells();
	TargetSQRExpectation = 2 * (*TotalAreaTarget) / ConvexHull_Target->GetNumberOfCells();


	if(SourceSQRExpectation > TargetSQRExpectation)
		TargetSQRExpectation = SourceSQRExpectation;
	else
		SourceSQRExpectation = TargetSQRExpectation;

	Map2ExpMap(SourceMap, SourceExpMap, ConvexHull_Source->GetNumberOfCells(), SourceSQRExpectation);
	Map2ExpMap(TargetMap, TargetExpMap, ConvexHull_Target->GetNumberOfCells(), TargetSQRExpectation);

	Map2HalfMap(SourceMap, SourceHalfMap, ConvexHull_Source->GetNumberOfCells());
	Map2HalfMap(TargetMap, TargetHalfMap, ConvexHull_Target->GetNumberOfCells());

	finish = clock();
	InitTime = finish - start;
	#ifdef _WIN32
	InitTime = InitTime;
	#endif
	#ifdef linux
	InitTime = InitTime / 1000;
	#endif
	//cout << "Initiate: " << InitTime << "ms" << endl;
    CellMatching(FlagReg);
}

void MeshGMM_Register::TotalArea(vtkPolyData* A, double * AreaA, double * TotalAreaA)
{
	double TempAreaA = 0;
	for(int i = 0; i < A->GetNumberOfCells(); i++)
	{
		TempAreaA  += AreaA[i];
	}
	(*TotalAreaA) = TempAreaA;
}
void MeshGMM_Register::Output2MatlabCore()
{
	char SaveFileA_Path[100];
	strcpy(SaveFileA_Path,MatlabPath);
	strcat(SaveFileA_Path, "/DistanceMapA.txt");
	ofstream SaveFileA(SaveFileA_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Source->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Source->GetNumberOfCells() - 1; j++)
			SaveFileA << SourceMap[i][j] << " ";
		SaveFileA << SourceMap[i][ConvexHull_Source->GetNumberOfCells() - 1] << "\n";
	}
	SaveFileA.close();

	char SaveFileScaleA_Path[100];
	strcpy(SaveFileScaleA_Path,MatlabPath);
	strcat(SaveFileScaleA_Path, "/ScaleA.txt");
	ofstream SaveFileScaleA(SaveFileScaleA_Path,ios::trunc);
	SaveFileScaleA << SourceSQRExpectation << "\n";
	SaveFileScaleA.close();

	char SaveFileWeightMapAn_Path[100];
	strcpy(SaveFileWeightMapAn_Path,MatlabPath);
	strcat(SaveFileWeightMapAn_Path, "/WeightMapAn.txt");
	ofstream SaveFileWeightMapAn(SaveFileWeightMapAn_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Source->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Source->GetNumberOfCells() - 1; j++)
			SaveFileWeightMapAn << SourceWeight[i][j] * SourceNTpara[i][j] << " ";
		SaveFileWeightMapAn << SourceWeight[i][ConvexHull_Source->GetNumberOfCells() - 1] * SourceNTpara[i][ConvexHull_Source->GetNumberOfCells() - 1]  << "\n";
	}
	SaveFileWeightMapAn.close();

	char SaveFileWeightMapAt_Path[100];
	strcpy(SaveFileWeightMapAt_Path,MatlabPath);
	strcat(SaveFileWeightMapAt_Path, "/WeightMapAt.txt");
	ofstream SaveFileWeightMapAt(SaveFileWeightMapAt_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Source->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Source->GetNumberOfCells() - 1; j++)
			SaveFileWeightMapAt << SourceWeight[i][j] * sqrt(1 - SQR(SourceNTpara[i][j])) << " ";
		SaveFileWeightMapAt << SourceWeight[i][ConvexHull_Source->GetNumberOfCells() - 1] * sqrt(1 - SQR(SourceNTpara[i][ConvexHull_Source->GetNumberOfCells() - 1])) << "\n";
	}
	SaveFileWeightMapAt.close();

	char SaveFileVectorXMapA_Path[100];
	strcpy(SaveFileVectorXMapA_Path,MatlabPath);
	strcat(SaveFileVectorXMapA_Path, "/VectorXMapA.txt");
	ofstream SaveFileVectorXMapA(SaveFileVectorXMapA_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Source->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Source->GetNumberOfCells() - 1; j++)
			SaveFileVectorXMapA << SourceAngleMap[0][0][i][j] / sqrt(1 - SQR(SourceNTpara[i][j])) << " ";
		SaveFileVectorXMapA << SourceAngleMap[0][0][i][ConvexHull_Source->GetNumberOfCells() - 1] / sqrt(1 - SQR(SourceNTpara[i][ConvexHull_Source->GetNumberOfCells() - 1])) << "\n";
	}
	SaveFileVectorXMapA.close();
	
	char SaveFileVectorYMapA_Path[100];
	strcpy(SaveFileVectorYMapA_Path,MatlabPath);
	strcat(SaveFileVectorYMapA_Path, "/VectorYMapA.txt");
	ofstream SaveFileVectorYMapA(SaveFileVectorYMapA_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Source->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Source->GetNumberOfCells() - 1; j++)
			SaveFileVectorYMapA << SourceAngleMap[0][1][i][j] / sqrt(1 - SQR(SourceNTpara[i][j])) << " ";
		SaveFileVectorYMapA << SourceAngleMap[0][1][i][ConvexHull_Source->GetNumberOfCells() - 1] / sqrt(1 - SQR(SourceNTpara[i][ConvexHull_Source->GetNumberOfCells() - 1])) << "\n";
	}
	SaveFileVectorYMapA.close();

}
void MeshGMM_Register::Output2Matlab(int axis, int p, int q)
{
	cout << "Output2Matlab..." << endl;
	clock_t IO_start, IO_finish;
	//double IO_totalTime;
	IO_start = clock();

	Output2MatlabCore();

	char SaveFileB_Path[100];
	strcpy(SaveFileB_Path,MatlabPath);
	strcat(SaveFileB_Path, "/DistanceMapB.txt");
	ofstream SaveFileB(SaveFileB_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Target->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Target->GetNumberOfCells() - 1; j++)
			SaveFileB << TargetMap[i][j] << " ";
		SaveFileB << TargetMap[i][ConvexHull_Target->GetNumberOfCells() - 1] << "\n";
	}
	SaveFileB.close();

	char SaveFileScaleB_Path[100];
	strcpy(SaveFileScaleB_Path,MatlabPath);
	strcat(SaveFileScaleB_Path, "/ScaleB.txt");
	ofstream SaveFileScaleB(SaveFileScaleB_Path,ios::trunc);
	SaveFileScaleB << TargetSQRExpectation << "\n";
	SaveFileScaleB.close();

	char SaveFileWeightMapBn_Path[100];
	strcpy(SaveFileWeightMapBn_Path,MatlabPath);
	strcat(SaveFileWeightMapBn_Path, "/WeightMapBn.txt");
	ofstream SaveFileWeightMapBn(SaveFileWeightMapBn_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Target->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Target->GetNumberOfCells() - 1; j++)
			SaveFileWeightMapBn << TargetWeight[i][j] * TargetNTpara[i][j] << " ";
		SaveFileWeightMapBn << TargetWeight[i][ConvexHull_Target->GetNumberOfCells() - 1] * TargetNTpara[i][ConvexHull_Target->GetNumberOfCells() - 1] << "\n";
	}
	SaveFileWeightMapBn.close();

	char SaveFileWeightMapBt_Path[100];
	strcpy(SaveFileWeightMapBt_Path,MatlabPath);
	strcat(SaveFileWeightMapBt_Path, "/WeightMapBt.txt");
	ofstream SaveFileWeightMapBt(SaveFileWeightMapBt_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Target->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Target->GetNumberOfCells() - 1; j++)
			SaveFileWeightMapBt << TargetWeight[i][j] * sqrt(1 - SQR(TargetNTpara[i][j])) << " ";
		SaveFileWeightMapBt << TargetWeight[i][ConvexHull_Target->GetNumberOfCells() - 1] * sqrt(1 - SQR(TargetNTpara[i][ConvexHull_Target->GetNumberOfCells() - 1])) << "\n";
	}
	SaveFileWeightMapBt.close();

	char SaveFileVectorXMapB_Path[100];
	strcpy(SaveFileVectorXMapB_Path,MatlabPath);
	strcat(SaveFileVectorXMapB_Path, "/VectorXMapB.txt");
	ofstream SaveFileVectorXMapB(SaveFileVectorXMapB_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Target->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Target->GetNumberOfCells() - 1; j++)
			SaveFileVectorXMapB << TargetAngleMap[axis][0][i][j] / sqrt(1 - SQR(TargetNTpara[i][j])) << " ";
		SaveFileVectorXMapB << TargetAngleMap[axis][0][i][ConvexHull_Target->GetNumberOfCells() - 1] / sqrt(1 - SQR(TargetNTpara[i][ConvexHull_Target->GetNumberOfCells() - 1])) << "\n";
	}
	SaveFileVectorXMapB.close();

    char SaveFileVectorYMapB_Path[100];
	strcpy(SaveFileVectorYMapB_Path,MatlabPath);
	strcat(SaveFileVectorYMapB_Path, "/VectorYMapB.txt");
	ofstream SaveFileVectorYMapB(SaveFileVectorYMapB_Path,ios::trunc);
	for(int i = 0; i < ConvexHull_Target->GetNumberOfCells(); i++)
	{
		for(int j = 0; j < ConvexHull_Target->GetNumberOfCells() - 1; j++)
			SaveFileVectorYMapB << TargetAngleMap[axis][1][i][j] / sqrt(1 - SQR(TargetNTpara[i][j])) << " ";
		SaveFileVectorYMapB << TargetAngleMap[axis][1][i][ConvexHull_Target->GetNumberOfCells() - 1] / sqrt(1 - SQR(TargetNTpara[i][ConvexHull_Target->GetNumberOfCells() - 1])) << "\n";
	}
	SaveFileVectorYMapB.close();


	char SaveFileMatchingA_Path[100];
	strcpy(SaveFileMatchingA_Path,MatlabPath);
	strcat(SaveFileMatchingA_Path, "/MatchingA.txt");
	ofstream SaveFileMatchingA(SaveFileMatchingA_Path,ios::trunc);
	SaveFileMatchingA<< p << "\n";
	SaveFileMatchingA.close();

	char SaveFileMatchingB_Path[100];
	strcpy(SaveFileMatchingB_Path,MatlabPath);
	strcat(SaveFileMatchingB_Path, "/MatchingB.txt");
	ofstream SaveFileMatchingB(SaveFileMatchingB_Path,ios::trunc);
	SaveFileMatchingB<< q << "\n";
	SaveFileMatchingB.close();

	IO_finish = clock();
	//IO_totalTime = IO_finish - IO_start;
	Output2MatlabTime = IO_finish - IO_start;
	#ifdef _WIN32
	Output2MatlabTime = Output2MatlabTime;
	#endif
	#ifdef linux
	Output2MatlabTime = Output2MatlabTime / 1000;
	#endif
	//cout << "Elapsed Time: " << Output2MatlabTime << "ms" << endl;
}

void MeshGMM_Register::CellMatching(bool FlagReg)
{
	cout << "Cell Matching..." << endl;
	clock_t start, finish;
	start = clock();

	int SourceSize = ConvexHull_Source->GetNumberOfCells();
	int TargetSize = ConvexHull_Target->GetNumberOfCells();
	double GMML2, GMML2Scale = (SourceSQRExpectation + TargetSQRExpectation) / 2;
	double GMMSimilarity = 0, TriSimilarity = 0, Similarity = 0;
	BestSimilarity = 0;
	int bp = 0, bq = 0, bt = 0;
	int max_iter = SourceSize * TargetSize;

	int *mp = new int [max_iter];
	int *mq = new int [max_iter];
	int *mt = new int [max_iter];
	double *ms = new double [max_iter];
	for(int i = 0; i < max_iter; i++)
		ms[i] = -1.0;

	int count = 0, percent, pre_percent = 0;
	for (int p = 0; p < SourceSize && count < max_iter; p++)
	{
		percent = PrintPercent(p, SourceSize, count, max_iter);
		if(percent != pre_percent)
		{
            cout << "Processing: " << percent << " %" << endl;
			pre_percent = percent;
		}

        if(SourceArea[p] < SourceSQRExpectation / 2 && FlagReg)
			continue;
		GMML2_0[p] = GaussTransform_Self(p, SourceSize, SourceWeight, SourceExpMap);
        CHGMML2[p] = Max_Bound(ConvexHull_Source);
		for(int q = 0; q < TargetSize; q++)
		{
            if(TargetArea[q] < TargetSQRExpectation / 2 && FlagReg)
                continue;

			for(int t = 0; t < 3; t++)
			{
				TriSimilarity = TriangleSimilarity(SourceTri_AngleMap[p], TargetTri_AngleMap[q], SourceArea[p], TargetArea[q], t);
                if ((TriSimilarity < 0.88 || TriSimilarity < BestSimilarity) && FlagReg)
                    continue;
                if (TriSimilarity < 0.85 && (!FlagReg))
                    continue;
				
				if(GMML2_1[q] < minieps)
					GMML2_1[q] = GaussTransform_Self(q, TargetSize, TargetWeight, TargetExpMap);
				
				GMML2 = GMML2_1[q] + GMML2_0[p]
					- 2 * GaussTransform(p, q, t, GMML2Scale, SourceSize, TargetSize);

                CHGMML2[p] = std::min(CHGMML2[p], GMML2);

				GMMSimilarity = exp(-(GMML2) * Beta);

				Similarity = TriSimilarity * GMMSimilarity;

				if(Similarity > 0.85 * BestSimilarity)
				{
					//consider usint sorting here
					mp[count] = p;
					mq[count] = q;
					mt[count] = t;
					ms[count] = Similarity;
					count++;
				}
				if(Similarity > BestSimilarity)
				{
					BestSimilarity = Similarity;
					bp = p;
					bq = q;
					bt = t;
				}
			}
		}        
	}

	VTK_CREATE(vtkPoints, m_Source);
	VTK_CREATE(vtkPoints, m_Target);
	m_Source->SetNumberOfPoints(max_iter);
	m_Target->SetNumberOfPoints(max_iter);
	int m_num = 0;
	for(int i = 0; i < max_iter; i++)
	{
		if(ms[i] > 0.95 * BestSimilarity)
		{
			m_Source->SetPoint(m_num, CH_SourceCell->GetPoint(mp[i]));
			m_Target->SetPoint(m_num, CH_TargetCell->GetPoint(mq[i]));
			m_num++;
		}
	}
	if(m_num < 3 || BestSimilarity < 0.5)
	{
		m_Source->SetNumberOfPoints(3);
		m_Source->SetPoint(0, ConvexHull_Source->GetPoint(ConvexHull_Source->GetCell(bp)->GetPointId(0)));
		m_Source->SetPoint(1, ConvexHull_Source->GetPoint(ConvexHull_Source->GetCell(bp)->GetPointId(1)));
		m_Source->SetPoint(2, ConvexHull_Source->GetPoint(ConvexHull_Source->GetCell(bp)->GetPointId(2)));

		m_Target->SetNumberOfPoints(3);
		m_Target->SetPoint(0, ConvexHull_Target->GetPoint(ConvexHull_Target->GetCell(bq)->GetPointId(bt)));
		m_Target->SetPoint(1, ConvexHull_Target->GetPoint(ConvexHull_Target->GetCell(bq)->GetPointId((bt+1)%3)));
		m_Target->SetPoint(2, ConvexHull_Target->GetPoint(ConvexHull_Target->GetCell(bq)->GetPointId((bt+2)%3)));
		cout << "Based on the Best Matching Cell" << endl;
	}
	else
	{
		m_Source->SetNumberOfPoints(m_num);
		m_Target->SetNumberOfPoints(m_num);
		cout << "Based on " << m_num <<" Matching Cells" << endl;
	}

	VTK_CREATE(vtkLandmarkTransform, landmarkTransform);
	landmarkTransform->SetSourceLandmarks(m_Source);
	landmarkTransform->SetTargetLandmarks(m_Target);
	landmarkTransform->SetModeToRigidBody();
	landmarkTransform->Update();
	BestTransform->DeepCopy(landmarkTransform->GetMatrix());
	cout << *landmarkTransform->GetMatrix();

	finish = clock();
	CellMatchingTime = finish - start;
	#ifdef _WIN32
	CellMatchingTime = CellMatchingTime;
	#endif
	#ifdef linux
	CellMatchingTime = CellMatchingTime / 1000;
	#endif

    free(mp);
    free(mq);
    free(mt);
    free(ms);
	if(isOutput2Matlab)
		Output2Matlab(bt, bp ,bq);	
}

int MeshGMM_Register::PrintPercent(int p, int size, int count, int max_iter)
{
	int percent;
	if((double)count / (double)max_iter > (double)p / (double)size)
		percent = (int)(100 * (double)count / (double)max_iter);
	else
		percent = (int)(100 * (double)p / (double)size);
	return percent;
}
double MeshGMM_Register::distance(double *pt1, double *pt2)
{
	return sqrt((pt1[0]-pt2[0]) * (pt1[0]-pt2[0]) + (pt1[1]-pt2[1]) * (pt1[1]-pt2[1]) + (pt1[2]-pt2[2]) * (pt1[2]-pt2[2]));
}
double MeshGMM_Register::SQRdistance(double *pt1, double *pt2)
{
	return ((pt1[0]-pt2[0]) * (pt1[0]-pt2[0]) + (pt1[1]-pt2[1]) * (pt1[1]-pt2[1]) + (pt1[2]-pt2[2]) * (pt1[2]-pt2[2]));
}
double MeshGMM_Register::TriangleSimilarity(double *Tri_AngleA, double *Tri_AngleB, double AreaA, double AreaB, int Order)
{
	for(int i = 0; i < 3; i++)
		if(Tri_AngleA[i] < 0.26 || Tri_AngleB[i] < 0.26)
			return 0.5;
	double para = Alpha;
    double error = exp(-para*(SQR(Tri_AngleA[0]-Tri_AngleB[Order]) + SQR(Tri_AngleA[1]-Tri_AngleB[(Order+1)%3]) +
		SQR(Tri_AngleA[2]-Tri_AngleB[(Order+2)%3])));
    error = error * (1 - fabs(AreaA - AreaB) / (AreaA + AreaB));
	return error;
}
void MeshGMM_Register::CellCenter(vtkPolyData* A, vtkPoints* B, double * Area, double ** Normal, double *** VectorX, double *** VectorY, 
	double **** AngleMap, double **NormalAngle, double **Weight,double **Map, double **Tri_AngleMap, double **NTpara)
{
	clock_t start, final;
	double totaltime;
	start = clock();

	double cent[3];
	double p[3][3];
	double Vector[3], EdgeLens[3];
	for(int i = 0; i < A->GetNumberOfCells(); i++)
	{
		vtkCell* cell = A->GetCell(i);
		vtkTriangle* triangle = dynamic_cast<vtkTriangle*>(cell);
		triangle->GetPoints()->GetPoint(0, p[0]);
		triangle->GetPoints()->GetPoint(1, p[1]);
		triangle->GetPoints()->GetPoint(2, p[2]);
		Area[i] = vtkTriangle::TriangleArea(p[0], p[1], p[2]);
		vtkTriangle::TriangleCenter(p[0], p[1], p[2], cent);
		B->InsertPoint(i, cent);
		vtkTriangle::ComputeNormal(p[0], p[1], p[2], Normal[i]);

		for(int m = 0; m < 3; m++)
		{
			EdgeLens[m] = SQRdistance(p[(m+1)%3], p[m]);
			for(int j = 0; j < 3; j++)
				VectorX[m][i][j] = (p[(m+1)%3][j] - p[m][j]) / sqrt(EdgeLens[m]);
			CrossProduct(Normal[i], VectorX[m][i], VectorY[m][i]);
		}

		for(int m = 0; m < 3; m++)
		{
			Tri_AngleMap[i][m] = - DotProduct(VectorX[m][i], VectorX[(m+2)%3][i]);
			Tri_AngleMap[i][m] = acosf(Tri_AngleMap[i][m]);
		}
	}

	for(int i = 0; i < A->GetNumberOfCells(); i++)
	{
		B->GetPoint(i, p[0]);
		Map[i][i] = inf_s;
		NormalAngle[i][i] = 0;
		Weight[i][i] = 0;
		for(int m = 0; m < 3; m++)
			for(int n = 0; n < 2; n++)
				AngleMap[m][n][i][i] = 0;
		
		for(int j = i + 1; j < A->GetNumberOfCells(); j++)
		{
			B->GetPoint(j, p[1]);
			Map[i][j] = SQRdistance(p[0], p[1]);
			Map[j][i] = Map[i][j];

			Weight[i][j] = Area[j] / (Map[i][j]);
			Weight[j][i] = Area[i] / (Map[i][j]);

			for(int k = 0; k < 3; k++)
			{
				Vector[k] = (p[1][k] - p[0][k]);
			}
			for(int m = 0; m < 3; m++)//different x,y axis
			{
				AngleMap[m][0][i][j] = DotProduct(VectorX[m][i], Vector);//x-axis
				AngleMap[m][1][i][j] = DotProduct(VectorY[m][i], Vector);//y-axis

				AngleMap[m][0][j][i] = -DotProduct(VectorX[m][j], Vector);
				AngleMap[m][1][j][i] = -DotProduct(VectorY[m][j], Vector);
			}
			NormalAngle[i][j] = - DotProduct(Normal[i], Vector);//z-axis
			NormalAngle[j][i] = DotProduct(Normal[j], Vector);

			NTpara[i][j] = NormalAngle[i][j] / sqrt(Map[i][j]);
			NTpara[j][i] = NormalAngle[j][i] / sqrt(Map[i][j]);
		}
	}

	final = clock();
	totaltime = final - start;
	cout << "Cell: " << totaltime << "ms" << endl;
}
void MeshGMM_Register::CrossProduct(double *A, double *B, double *Product)
{
	Product[0] = A[1] * B[2] - A[2] * B[1];
	Product[1] = A[2] * B[0] - A[0] * B[2];
	Product[2] = A[0] * B[1] - A[1] * B[0];
}

double MeshGMM_Register::DotProduct(double *A, double *B)
{
	double Product = A[0] * B[0] + A[1] * B[1] + A[2] * B[2];
	return Product;
}

void MeshGMM_Register::Init()
{
	int SourceSize = ConvexHull_Source->GetNumberOfCells();
	int TargetSize = ConvexHull_Target->GetNumberOfCells();

	GMML2_0 = new double[SourceSize];
	//for(int i = 0; i < SourceSize; i++)
	//	GMML2_0[i] = 0;
	GMML2_1 = new double[TargetSize];
	for(int i = 0; i < TargetSize; i++)
		GMML2_1[i] = 0;

}
void MeshGMM_Register::InitSource()
{
	int SourceSize = ConvexHull_Source->GetNumberOfCells();
    CHGMML2 = new double [SourceSize];

	SourceNTpara = new double *[SourceSize];
	for(int i = 0; i < SourceSize; i++)
		SourceNTpara[i] = new double[SourceSize];

	SourceTri_AngleMap = new double *[SourceSize];
	for(int i = 0; i < SourceSize; i++)
		SourceTri_AngleMap[i] = new double[3];

	SourceMap = new double *[SourceSize];
	for(int i = 0; i < SourceSize; i++)
		SourceMap[i] = new double[SourceSize];

	SourceHalfMap = new double *[SourceSize];
	for(int i = 0; i < SourceSize; i++)
		SourceHalfMap[i] = new double[SourceSize];

	SourceExpMap = new double *[SourceSize];
	for(int i = 0; i < SourceSize; i++)
		SourceExpMap[i] = new double[SourceSize];

	SourceNormalAngle = new double *[SourceSize];
	for(int i = 0; i < SourceSize; i++)
		SourceNormalAngle[i] = new double[SourceSize];

	SourceWeight = new double *[SourceSize];
	for(int i = 0; i < SourceSize; i++)
		SourceWeight[i] = new double[SourceSize];

	SourceAngleMap = new double ***[3];
	for(int k = 0; k < 3; k++)
	{
		SourceAngleMap[k] = new double **[2];
		for(int i = 0; i < 2; i++)
		{
			SourceAngleMap[k][i] = new double *[SourceSize];
			for(int j = 0; j < SourceSize; j++)
				SourceAngleMap[k][i][j] = new double [SourceSize];
		}
	}

	TotalAreaSource = new double;
	SourceArea = new double[SourceSize];

	SourceNormal = new double *[SourceSize];
	for(int i =0; i < SourceSize; i++)
		SourceNormal[i] = new double[3];

	SourceVectorX = new double **[3];
	for(int j = 0; j < 3; j++)
	{
		SourceVectorX[j] = new double *[SourceSize];
		for(int i =0; i < SourceSize; i++)
			SourceVectorX[j][i] = new double[3];
	}

	SourceVectorY = new double **[3];
	for(int j = 0; j < 3; j++)
	{
		SourceVectorY[j] = new double *[SourceSize];
		for(int i =0; i < SourceSize; i++)
			SourceVectorY[j][i] = new double[3];
	}
}
void MeshGMM_Register::InitTarget()
{
	int TargetSize = ConvexHull_Target->GetNumberOfCells();

	TargetNTpara = new double *[TargetSize];
	for(int i = 0; i < TargetSize; i++)
		TargetNTpara[i] = new double[TargetSize];

	TargetTri_AngleMap = new double *[TargetSize];
	for(int i = 0; i < TargetSize; i++)
		TargetTri_AngleMap[i] = new double[3];

	TargetMap = new double *[TargetSize];
	for(int i = 0; i < TargetSize; i++)
		TargetMap[i] = new double[TargetSize];

	TargetHalfMap = new double *[TargetSize];
	for(int i = 0; i < TargetSize; i++)
		TargetHalfMap[i] = new double[TargetSize];

	TargetExpMap = new double *[TargetSize];
	for(int i = 0; i < TargetSize; i++)
		TargetExpMap[i] = new double[TargetSize];

	TargetNormalAngle = new double *[TargetSize];
	for(int i = 0; i < TargetSize; i++)
		TargetNormalAngle[i] = new double[TargetSize];

	TargetWeight = new double *[TargetSize];
	for(int i = 0; i < TargetSize; i++)
		TargetWeight[i] = new double[TargetSize];

	TargetAngleMap = new double ***[3];
	for(int k = 0; k < 3; k++)
	{
		TargetAngleMap[k] = new double **[2];
		for(int i = 0; i < 2; i++)
		{
			TargetAngleMap[k][i] = new double *[TargetSize];
			for(int j = 0; j < TargetSize; j++)
				TargetAngleMap[k][i][j] = new double [TargetSize];
		}
	}

	TotalAreaTarget = new double;
	TargetArea = new double[TargetSize];

	TargetNormal = new double *[TargetSize];
	for(int i =0; i < TargetSize; i++)
		TargetNormal[i] = new double[3];

	TargetVectorX = new double **[3];
	for(int j = 0; j < 3; j++)
	{
		TargetVectorX[j] = new double *[TargetSize];
		for(int i =0; i < TargetSize; i++)
			TargetVectorX[j][i] = new double[3];
	}

	TargetVectorY = new double **[3];
	for(int j = 0; j < 3; j++)
	{
		TargetVectorY[j] = new double *[TargetSize];
		for(int i =0; i < TargetSize; i++)
			TargetVectorY[j][i] = new double[3];
	}
}

double MeshGMM_Register::GaussTransform(int p, int q, int t, double Scale, double SizeA, double SizeB)
{
	//clock_t start, finish;
	//start = clock();
	double cross_term = 0;

	for (int i = 0; i < SizeA; ++i)
	{
		double term_B = 0;
		for (int j = 0; j < SizeB; ++j)
		{
			//double cost_ij = SourceWeight[p][i] * TargetWeight[q][j] * 
			//	exp(- (SQR(SourceNormalAngle[p][i] - TargetNormalAngle[q][j]) + SQR(SourceAngleMap[0][0][p][i] - TargetAngleMap[t][0][q][j]) + 
			//	SQR(SourceAngleMap[0][1][p][i] - TargetAngleMap[t][1][q][j])) / Scale);
			double cost_ij = TargetWeight[q][j] * 
				exp(((SourceNormalAngle[p][i] * TargetNormalAngle[q][j] + SourceAngleMap[0][0][p][i] * TargetAngleMap[t][0][q][j] + SourceAngleMap[0][1][p][i] * TargetAngleMap[t][1][q][j]) - SourceHalfMap[p][i] - TargetHalfMap[q][j]) / Scale);
			term_B += cost_ij;
		}
		term_B *= SourceWeight[p][i];
		cross_term += term_B;
	}
	//finish = clock();
	//double totaltime = finish - start;
	//cout << "time = " << totaltime << endl;

	//return cross_term / (SizeA * SizeB);
	return cross_term;
}
double MeshGMM_Register::GaussTransform_Self(int p, double Size, double **Weight,double **ExpMap)
{
	//clock_t start, finish;
	//start = clock();
	double cross_term = 0;

	for (int i = 0; i < Size; ++i)
	{
		cross_term += Weight[p][i] * Weight[p][i];
		double term_B = 0;
		for (int j = i + 1; j < Size; ++j)
		{
			double cost_ij = Weight[p][j] * ExpMap[i][j];
			term_B += 2 * cost_ij;
		}
		term_B *= Weight[p][i];
		cross_term += term_B;
	}
	//finish = clock();
	//double totaltime = finish - start;
	//cout << "time_self = " << totaltime << endl;

	//return cross_term / (Size * Size);
	return cross_term;
}
void MeshGMM_Register::Map2ExpMap(double **Map, double **ExpMap, double Size, double Scale)
{
	Scale = 2 * Scale;
	for (int i = 0; i < Size; ++i)
	{
		for (int j = i + 1; j < Size; ++j)
		{
			ExpMap[i][j] = exp(-(Map[i][j]) / Scale);
		}
	}
}
void MeshGMM_Register::Map2HalfMap(double **Map, double **HalfMap, double Size)
{
	for (int i = 0; i < Size; ++i)
	{
		HalfMap[i][i] = 0;
		for (int j = i + 1; j < Size; ++j)
		{
			HalfMap[i][j] = Map[i][j] / 2;
			HalfMap[j][i] = HalfMap[i][j];
		}
	}
}
void MeshGMM_Register::SourceOutput(int p)
{
	InitSource();
	
	CellCenter(ConvexHull_Source, CH_SourceCell, SourceArea, SourceNormal, SourceVectorX, SourceVectorY, 
		SourceAngleMap, SourceNormalAngle, SourceWeight, SourceMap, SourceTri_AngleMap, SourceNTpara);

	TotalArea(ConvexHull_Source, SourceArea, TotalAreaSource);
	SourceSQRExpectation = 2 * (*TotalAreaSource) / ConvexHull_Source->GetNumberOfCells();
	Output2MatlabCore();

	int SourceSize = ConvexHull_Source->GetNumberOfCells();
	vtkSmartPointer<vtkUnsignedCharArray> SourceColors =
	    vtkSmartPointer<vtkUnsignedCharArray>::New();
	SourceColors->SetNumberOfComponents(3);
	SourceColors->SetName("SourceColors");

	unsigned char MatchingColor[3] = {255, 255, 255};
	unsigned char UnMatchingColor[3] = {170, 130, 66};

	for(int i = 0; i < SourceSize; i++)
	{
		if(i == p)
		//if(i == 8 || i == 10 || i == 12 || i == 19)
		{
			SourceColors->InsertNextTupleValue(MatchingColor);
		}
		else
		{
			SourceColors->InsertNextTupleValue(UnMatchingColor);
		}
	}
	ConvexHull_Source->GetCellData()->SetScalars(SourceColors);

	char SaveFileMatchingA_Path[100];
	strcpy(SaveFileMatchingA_Path,MatlabPath);
	strcat(SaveFileMatchingA_Path, "/MatchingA.txt");
	ofstream SaveFileMatchingA(SaveFileMatchingA_Path,ios::trunc);
	SaveFileMatchingA<< p << "\n";
	SaveFileMatchingA.close();
}

void MeshGMM_Register::TargetOutput(int p)
{
	InitTarget();
	
	CellCenter(ConvexHull_Target, CH_TargetCell, TargetArea, TargetNormal, TargetVectorX, TargetVectorY, 
		TargetAngleMap, TargetNormalAngle, TargetWeight, TargetMap, TargetTri_AngleMap, TargetNTpara);

	TotalArea(ConvexHull_Target, TargetArea, TotalAreaTarget);
	TargetSQRExpectation = 2 * (*TotalAreaTarget) / ConvexHull_Target->GetNumberOfCells();
	//Output2MatlabCore();

	int TargetSize = ConvexHull_Target->GetNumberOfCells();
	vtkSmartPointer<vtkUnsignedCharArray> TargetColors =
	    vtkSmartPointer<vtkUnsignedCharArray>::New();
	TargetColors->SetNumberOfComponents(3);
	TargetColors->SetName("TargetColors");

	unsigned char MatchingColor[3] = {255, 255, 255};
	unsigned char UnMatchingColor[3] = {170, 130, 66};

	for(int i = 0; i < TargetSize; i++)
	{
		if(i == p)
		//if(i == 8 || i == 10 || i == 12 || i == 19)
		{
			TargetColors->InsertNextTupleValue(MatchingColor);
		}
		else
		{
			TargetColors->InsertNextTupleValue(UnMatchingColor);
		}
	}
	ConvexHull_Target->GetCellData()->SetScalars(TargetColors);

	char SaveFileMatchingB_Path[100];
	strcpy(SaveFileMatchingB_Path,MatlabPath);
	strcat(SaveFileMatchingB_Path, "/MatchingA.txt");
	ofstream SaveFileMatchingB(SaveFileMatchingB_Path,ios::trunc);
	SaveFileMatchingB<< p << "\n";
	SaveFileMatchingB.close();
}
