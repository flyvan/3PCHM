/*=========================================================================
    Copyright: Dec.5 2013
    BIT OTISLAB
    Medical Group
    Jingfan Fan
=========================================================================*/

#include "CHM.h"

CHM_Register::CHM_Register()
{
    VTK_NEW(vtkPoints, Points_Source);
    VTK_NEW(vtkPoints, Points_Target);
    VTK_NEW(vtkPolyData, Poly_Source);
    VTK_NEW(vtkPolyData, Poly_Target);
    VTK_NEW(vtkPolyData, ConvexHull_Source);
    VTK_NEW(vtkPolyData, ConvexHull_Target);
    VTK_NEW(vtkMatrix4x4, BestTransform);
}

CHM_Register::~CHM_Register()
{
}

/* Set Source Data in this Class */
void CHM_Register::SetSource(vtkSmartPointer<vtkPolyData> Source, int sd)
{
    int step = Source->GetPoints()->GetNumberOfPoints() / sd + 1;
    Points_Source->Reset();
    for (int i = 0; i * step < Source->GetPoints()->GetNumberOfPoints(); i ++)
        Points_Source->InsertNextPoint(Source->GetPoints()->GetPoint(i * step));
    Points2PolyData(Points_Source, Poly_Source);
}

/* Set Target Data in this Class */
void CHM_Register::SetTarget(vtkSmartPointer<vtkPolyData> Target, int sd)
{
    int step = Target->GetPoints()->GetNumberOfPoints() / sd + 1;
    Points_Target->Reset();
    for (int i = 0; i * step < Target->GetPoints()->GetNumberOfPoints(); i ++)
        Points_Target->InsertNextPoint(Target->GetPoints()->GetPoint(i * step));
    Points2PolyData(Points_Target, Poly_Target);
}

/* Calculate 3D Convex Hull by vtkDelaunay3D */
void CHM_Register::Cal_CH_Source(bool Norm)
{
    VTK_CREATE(vtkDelaunay3D, delaunay);
    delaunay->SetInput(Poly_Source);
    delaunay->Update();
    delaunay->GetBoundingTriangulation();

    VTK_CREATE(vtkDataSetSurfaceFilter, surfaceFilter);
    surfaceFilter->SetInputConnection(delaunay->GetOutputPort());
    surfaceFilter->Update();

    VTK_CREATE(vtkPolyData, CH);
    CH->DeepCopy(surfaceFilter->GetOutput());

    VTK_CREATE(vtkPoints, NewPoint);
    VTK_CREATE(vtkPolyData, NewData);

    int i, j;
    bool near;

    for (i = 0; i < CH->GetNumberOfPoints(); i++)
    {
        near = false;
        for (j = 0; j < NewPoint->GetNumberOfPoints(); j++)
        {
            if (Euc_Dist(CH->GetPoint(i), NewPoint->GetPoint(j)) < Max_Bound(Poly_Source)/10)
                near = true;
        }
        if (!near)
            NewPoint->InsertNextPoint(CH->GetPoint(i));
    }
    Points2PolyData(NewPoint, NewData);

    VTK_CREATE(vtkDelaunay3D, delaunay2nd);
    delaunay2nd->SetInput(NewData);
    delaunay2nd->Update();
    delaunay2nd->GetBoundingTriangulation();
    VTK_CREATE(vtkDataSetSurfaceFilter, surfaceFilter2nd);
    surfaceFilter2nd->SetInputConnection(delaunay2nd->GetOutputPort());
    surfaceFilter2nd->Update();
    if (Norm)
        ConvexHull_Source->DeepCopy(surfaceFilter2nd->GetOutput());
    else
        ConvexHull_Source->DeepCopy(surfaceFilter->GetOutput());
}

/* Calculate 3D Convex Hull by vtkDelaunay3D */
void CHM_Register::Cal_CH_Target(bool Norm)
{
    VTK_CREATE(vtkDelaunay3D, delaunay);
    delaunay->SetInput(Poly_Target);
    delaunay->Update();
    delaunay->GetBoundingTriangulation();
    VTK_CREATE(vtkDataSetSurfaceFilter, surfaceFilter);
    surfaceFilter->SetInputConnection(delaunay->GetOutputPort());
    surfaceFilter->Update();

    VTK_CREATE(vtkPolyData, CH);
    CH->DeepCopy(surfaceFilter->GetOutput());

    VTK_CREATE(vtkPoints, NewPoint);
    VTK_CREATE(vtkPolyData, NewData);

    int i, j;
    bool near;

    for (i = 0; i < CH->GetNumberOfPoints(); i++)
    {
        near = false;
        for (j = 0; j < NewPoint->GetNumberOfPoints(); j++)
        {
            if (Euc_Dist(CH->GetPoint(i), NewPoint->GetPoint(j)) < Max_Bound(Poly_Target)/10)
                near = true;
        }
        if (!near)
            NewPoint->InsertNextPoint(CH->GetPoint(i));
    }
    Points2PolyData(NewPoint, NewData);

    VTK_CREATE(vtkDelaunay3D, delaunay2nd);
    delaunay2nd->SetInput(NewData);
    delaunay2nd->Update();
    delaunay2nd->GetBoundingTriangulation();
    VTK_CREATE(vtkDataSetSurfaceFilter, surfaceFilter2nd);
    surfaceFilter2nd->SetInputConnection(delaunay2nd->GetOutputPort());
    surfaceFilter2nd->Update();
    if (Norm)
        ConvexHull_Target->DeepCopy(surfaceFilter2nd->GetOutput());
    else
        ConvexHull_Target->DeepCopy(surfaceFilter->GetOutput());
}

/* Iterate the optimal transformation */
void CHM_Register::Register()
{
    int i, j, k, l, iterid = 0, si = 10, NumofInlier, MaxInlier = 0;
    double sumerror, closestPointDist, InlierDist;

    InlierDist = Max_Bound(Poly_Target) / 200;

    VTK_CREATE(vtkPoints, Tri_Source);
    VTK_CREATE(vtkPoints, Tri_Target);
    VTK_CREATE(vtkLandmarkTransform, landmarkTransform);
    VTK_CREATE(vtkTransform, Transform);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter);
    VTK_CREATE(vtkKdTree, kdtree);

    landmarkTransform->SetModeToRigidBody();
    //landmarkTransform->SetModeToSimilarity();

    kdtree->BuildLocatorFromPoints(Poly_Target->GetPoints());
    cout<<"KDTree Build Succes"<<endl;
    for (i = 0; i < ConvexHull_Source->GetNumberOfCells(); i++)
    {
        Tri_Source->DeepCopy(ConvexHull_Source->GetCell(i)->GetPoints());
        landmarkTransform->SetSourceLandmarks(Tri_Source);

        for (j = 0; j < ConvexHull_Target->GetNumberOfCells(); j++)
        {
            for (k = 0; k < 3; k++)
            {
                Tri_Target->SetNumberOfPoints(3);
                Tri_Target->SetPoint(0, ConvexHull_Target->GetCell(j)->GetPoints()->GetPoint(k%3));
                Tri_Target->SetPoint(1, ConvexHull_Target->GetCell(j)->GetPoints()->GetPoint((k+1)%3));
                Tri_Target->SetPoint(2, ConvexHull_Target->GetCell(j)->GetPoints()->GetPoint((k+2)%3));
                if (!isSimilar(Tri_Source, Tri_Target))
                    continue;
                landmarkTransform->SetTargetLandmarks(Tri_Target);
                landmarkTransform->Update();

                /* Translate the Source Model */
                Transform->SetMatrix(landmarkTransform->GetMatrix());
                TransformFilter->SetTransform(Transform);
                TransformFilter->SetInput(Poly_Source);
                TransformFilter->Update();

                sumerror = 0;
                NumofInlier = 0;
                for (l = 0; l < TransformFilter->GetOutput()->GetNumberOfPoints(); l+=si)
                {
                    kdtree->FindClosestPoint(TransformFilter->GetOutput()->GetPoint(l), closestPointDist);
                    sumerror += closestPointDist;
                    if (closestPointDist < InlierDist)
                        NumofInlier ++;
                }

                if (NumofInlier > MaxInlier)
                {
                    MaxInlier = NumofInlier;
                    BestTransform->DeepCopy(landmarkTransform->GetMatrix());
                    cout<<"The "<< ++iterid <<"th iter error is: "<< sumerror * si / Poly_Source->GetNumberOfPoints() <<endl;
                    cout<<"The Max count of inliers is: "<< MaxInlier * si << endl;
                    cout<<*BestTransform;
                }
            }
        }
    }
}

/*Judge the congruence of two triangles */
bool CHM_Register::isSimilar(vtkPoints *a, vtkPoints *b)
{
    double alens[3], blens[3];
    double x[3], y[3];

    for (int i = 0; i < 3; i++)
    {
        /* Why alens[i] = Euc_Dist(a->GetPoint(i%3), a->GetPoint((i+1)%3); is error??? */
        a->GetPoint(i%3, x);
        a->GetPoint((i+1)%3, y);
        alens[i] = Euc_Dist(x, y);

        if (alens[i] < minieps)
            return false;

        b->GetPoint(i%3, x);
        b->GetPoint((i+1)%3, y);
        blens[i] = Euc_Dist(x, y);

        if (blens[i] < minieps)
            return false;

        if (alens[i] / blens[i] > 1.1 || blens[i] / alens[i] > 1.1)
            return false;
    }
    return true;
}

double Euc_Dist(double *pt1, double *pt2)
{
    return sqrt((pt1[0]-pt2[0]) * (pt1[0]-pt2[0]) + (pt1[1]-pt2[1]) * (pt1[1]-pt2[1]) + (pt1[2]-pt2[2]) * (pt1[2]-pt2[2]));
}

void Points2PolyData(vtkPoints *Points, vtkPolyData *PolyData)
{
    VTK_CREATE(vtkCellArray, Vertices);
    vtkIdType pid[1];
    int i;
    for (i = 0; i < Points->GetNumberOfPoints(); i++)
    {
        pid[0] = i;
        Vertices->InsertNextCell(1, pid);
    }
    PolyData->SetPoints(Points);
    PolyData->SetVerts(Vertices);
}

double Max_Bound(vtkPolyData *PolyData)
{
    double bounds[6], maxbound;

    PolyData->GetBounds(bounds);
    for (int i = 0; i < 3; i++)
        maxbound = std::max(maxbound, fabs(bounds[i * 2] - bounds[i * 2 + 1]));
    return maxbound;
}

double xmult(double x1,double y1,double x2,double y2,double x0,double y0){
    return (x1-x0)*(y2-y0)-(x2-x0)*(y1-y0);
}

/* Gassian Noise Generator */
double NoiseGen(double p) {
    double fx[7] = {0, 2.2, 13.7, 50, 84.2, 97.8, 100};
    double ns[7] = {-3, -2, -1, 0, 1, 2, 3};
    int i;
    for (i = 1; i < 7; i++)
        if (p < fx[i])
            break;
    return ns[i - 1] + (ns[i] - ns[i - 1])*(p - fx[i - 1]) / (fx[i] - fx[i - 1]);
}
