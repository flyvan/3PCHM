/*=========================================================================
Copyright: Dec.5 2013
BIT OTISLAB
Medical Group
Jingfan Fan
=========================================================================*/

#include "SimpleView.h"

/* Constructor of SimpleView*/
SimpleView::SimpleView()
{
    VTKVariablesInit();
    QTVariablesInit();
    Flag_Source = false;
    Flag_Target = false;
    FlagReset();
    srand((unsigned)time(NULL));
    slotRender();
}

SimpleView::~SimpleView()
{
    //The smart pointers should clean up for up
}

void SimpleView::VTKVariablesInit()
{
    /* Create VTK objects */
    VTK_NEW(vtkQtTableView, TableView);
    VTK_NEW(vtkPolyData, PolyData_Source);
    VTK_NEW(vtkPolyData, PolyData_Target);

    VTK_NEW(vtkPolyData, SparsePoints_Source);
    VTK_NEW(vtkPolyData, SparsePoints_Target);

    VTK_NEW(vtkPolyDataMapper, PolyDataMapper_Source);
    VTK_NEW(vtkPolyDataMapper, PolyDataMapper_Target);

    VTK_NEW(vtkPolyDataMapper, SparsePointsMapper_Source);
    VTK_NEW(vtkPolyDataMapper, SparsePointsMapper_Target);

    VTK_NEW(vtkPolyDataMapper, PolyDataMapper_ConvexHullSource);
    VTK_NEW(vtkPolyDataMapper, PolyDataMapper_ConvexHullTarget);


    VTK_NEW(vtkActor, Actor_Source);
    VTK_NEW(vtkActor, Actor_Target);

    VTK_NEW(vtkActor, Actor_SparsePointsSource);
    VTK_NEW(vtkActor, Actor_SparsePointsTarget);

    VTK_NEW(vtkActor, Actor_ConvexHullSource);
    VTK_NEW(vtkActor, Actor_ConvexHullTarget);

    VTK_NEW(vtkAxesActor, Actor_axes);

    VTK_NEW(vtkRenderer, ren);

    VTK_NEW(vtkPoints, GMPoints_Source);
    VTK_NEW(vtkPoints, GMPoints_Target);

    VTK_NEW(vtkPolyDataMapper, DistanceMapper);
    VTK_NEW(vtkLookupTable, lut);
    VTK_NEW(vtkDistancePolyDataFilter, DistanceFilter);
    VTK_NEW(vtkActor, Actor_DistanceMap);
    VTK_NEW(vtkScalarBarActor, Actor_scalarBar);
}

void SimpleView::QTVariablesInit()
{
    ui = new Ui_SimpleView;
    ui->setupUi(this);
    /* Set up action signals and slots */
    connect(ui->Action_OpenSource, SIGNAL(triggered()), this, SLOT(slotOpenSource()));
    connect(ui->Action_OpenTarget, SIGNAL(triggered()), this, SLOT(slotOpenTarget()));
    connect(ui->Action_SaveSource, SIGNAL(triggered()), this, SLOT(slotSaveSource()));
    connect(ui->Action_SaveTarget, SIGNAL(triggered()), this, SLOT(slotSaveTarget()));
    connect(ui->Action_SaveMixed, SIGNAL(triggered()), this, SLOT(slotSaveMixed()));
    connect(ui->Action_Exit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(ui->PushButton_Path1, SIGNAL(clicked()), this, SLOT(slotSetSourcePath()));
    connect(ui->PushButton_Path2, SIGNAL(clicked()), this, SLOT(slotSetTargetPath()));
    connect(ui->PushButton_Transform, SIGNAL(clicked()), this, SLOT(slotTransform()));
    connect(ui->PushButton_LMICP, SIGNAL(clicked()), this, SLOT(slotICP()));
    connect(ui->PushButton_ConvexHull, SIGNAL(clicked()), this, SLOT(slotConvexHull()));
    connect(ui->PushButton_3PCHM, SIGNAL(clicked()), this, SLOT(slotCHM()));
    connect(ui->PushButton_CHGMM, SIGNAL(clicked()), this, SLOT(slotCHGMM()));
    connect(ui->PushButton_CHGMMNonRigid, SIGNAL(clicked()), this, SLOT(slotCHGMMNonRigid()));
    connect(ui->PushButton_GMMRigid, SIGNAL(clicked()), this, SLOT(slotGMMRigid()));
    connect(ui->PushButton_GMMNonRigid, SIGNAL(clicked()), this, SLOT(slotGMMNonRigid()));
    connect(ui->PushButton_CPD, SIGNAL(clicked()), this, SLOT(slotCPD()));
    connect(ui->PushButton_Normalization, SIGNAL(clicked()), this, SLOT(slotNormalize()));
    connect(ui->PushButton_Proj, SIGNAL(clicked()), this, SLOT(slotProj()));
    connect(ui->PushButton_ProjOneImg, SIGNAL(clicked()), this, SLOT(slotProjOneImg()));
    connect(ui->PushButton_RandomProj, SIGNAL(clicked()), this, SLOT(slotRandomProj()));
    connect(ui->PushButton_ProjMatch, SIGNAL(clicked()), this, SLOT(slotProjMatch()));
    connect(ui->PushButton_Show3DMatches, SIGNAL(clicked()), this, SLOT(slotShow3DMatches()));
    connect(ui->PushButton_ProjRegister, SIGNAL(clicked()), this, SLOT(slotProjRegister()));
    connect(ui->PushButton_RandomT, SIGNAL(clicked()), this, SLOT(slotRandomT()));
    connect(ui->PushButton_Noising, SIGNAL(clicked()), this, SLOT(slotNoising()));
    connect(ui->PushButton_TPSNoising, SIGNAL(clicked()), this, SLOT(slotTPSNoising()));
    connect(ui->PushButton_Render, SIGNAL(clicked()), this, SLOT(slotRender()));
    connect(ui->PushButton_StatUpdate, SIGNAL(clicked()), this, SLOT(slotStatUpdate()));
    connect(ui->PushButton_Test, SIGNAL(clicked()), this, SLOT(slotTest()));
    connect(ui->PushButton_TestPath, SIGNAL(clicked()), this, SLOT(slotTestPath()));
    connect(ui->PushButton_MeanModel, SIGNAL(clicked()), this, SLOT(slotMeanModel()));

    /* Initialization */
    /* Page of Initial */
    ui->LineEdit_RotationX->setText("0");
    ui->LineEdit_RotationY->setText("0");
    ui->LineEdit_RotationZ->setText("0");
    ui->LineEdit_TranslationX->setText("2");
    ui->LineEdit_TranslationY->setText("0");
    ui->LineEdit_TranslationZ->setText("0");
    ui->LineEdit_Scale->setText("1");
    ui->LineEdit_Bounding->setText("1.0");
    ui->LineEdit_Sigma->setText("2.0");
    ui->LineEdit_SourcePts->setText("N/A");
    ui->LineEdit_TargetPts->setText("N/A");
    ui->LineEdit_SourceVol->setText("N/A");
    ui->LineEdit_TargetVol->setText("N/A");
    ui->RadioButton_Source->setOn(true);

    /* Page of ICP */
    ui->LineEdit_ICPIter->setText("30");

    /* Page of CHM */
    ui->LineEdit_SourceSD->setText("5000");
    ui->LineEdit_TargetSD->setText("5000");
    ui->CheckBox_ConvexHullNormalizing->setOn(false);
    ui->RadioButton_ADMMeasure->setOn(true);

    /* Page of CHGMM */
    ui->LineEdit_CHGMMAlpha->setText("3");
    ui->LineEdit_CHGMMBeta->setText("1");

    /* Page of IIOCH */
    ui->LineEdit_Path1->setText("../Data/T1");
    ui->LineEdit_Path2->setText("../Data/T2");
    ui->LineEdit_ImgSize->setText("100");
    ui->RadioButton_RenderNothing->setOn(true);

    /* Page of GMMREG */
    ui->LineEdit_GMMLevel->setText("1");
    ui->LineEdit_GMMBeta->setText("1");
    ui->LineEdit_GMMLambda->setText("0");
    ui->LineEdit_GMMMaxEvals->setText("30");

    /* Page of CPD */
    ui->LineEdit_CPDEmtol->setText("1e-15");
    ui->LineEdit_CPDAnneal->setText("0.97");
    ui->LineEdit_CPDBeta->setText("1");
    ui->LineEdit_CPDLambda->setText("1");
    ui->LineEdit_CPDOutliers->setText("1");
    ui->LineEdit_CPDSigma->setText("0.5");
    ui->LineEdit_CPDTol->setText("1e-18");
    ui->LineEdit_CPDMaxIter->setText("30");
    ui->LineEdit_CPDMaxEMIter->setText("10");

    /* Page of Statistic */
    ui->LineEdit_StatClosetPointDistance->setText("N/A");
    ui->LineEdit_StatConvexHullDistance->setText("N/A");
    ui->LineEdit_StatGMML2->setText("N/A");
    ui->LineEdit_StatCHGMML2->setText("N/A");
    ui->LineEdit_TestNum->setText("5");

    /* Page of Time */
    ui->LineEdit_ICPTime->setText("N/A");
    ui->LineEdit_CHMTime->setText("N/A");
    ui->LineEdit_IIOCHTime->setText("N/A");
    ui->LineEdit_CHGMMTime->setText("N/A");
    ui->LineEdit_GMMTime->setText("N/A");
    ui->LineEdit_CPDTime->setText("N/A");

    /* Page of View */
    ui->ViewCheck_Axis->setOn(true);
    ui->ViewCheck_SourceConvexHull->setOn(false);
    ui->ViewCheck_TargetConvexHull->setOn(false);
    ui->ViewCheck_SourceMesh->setOn(true);
    ui->ViewCheck_TargetMesh->setOn(true);
    ui->ViewCheck_SourcePoint->setOn(false);
    ui->ViewCheck_TargetPoint->setOn(false);

    ui->LineEdit_SourceMeshOpacity->setText("1.0");
    ui->LineEdit_TargetMeshOpacity->setText("1.0");
    ui->LineEdit_SourceConvexHullOpacity->setText("1.0");
    ui->LineEdit_TargetConvexHullOpacity->setText("1.0");

    ui->LineEdit_BackGroundR->setText("0.3");
    ui->LineEdit_BackGroundG->setText("0.4");
    ui->LineEdit_BackGroundB->setText("0.5");

    ui->LineEdit_SourcePointR->setText("0.0");
    ui->LineEdit_SourcePointG->setText("0.0");
    ui->LineEdit_SourcePointB->setText("1.0");
    ui->LineEdit_TargetPointR->setText("1.0");
    ui->LineEdit_TargetPointG->setText("0.0");
    ui->LineEdit_TargetPointB->setText("0.0");

    ui->LineEdit_SourceMeshR->setText("0.0");
    ui->LineEdit_SourceMeshG->setText("0.0");
    ui->LineEdit_SourceMeshB->setText("1.0");
    ui->LineEdit_TargetMeshR->setText("1.0");
    ui->LineEdit_TargetMeshG->setText("0.0");
    ui->LineEdit_TargetMeshB->setText("0.0");

    ui->LineEdit_SourceConvexHullR->setText("0.8");
    ui->LineEdit_SourceConvexHullG->setText("0.7");
    ui->LineEdit_SourceConvexHullB->setText("0.5");
    ui->LineEdit_TargetConvexHullR->setText("0.8");
    ui->LineEdit_TargetConvexHullG->setText("0.7");
    ui->LineEdit_TargetConvexHullB->setText("0.5");
    ui->LineEdit_Range->setText("0.03");

    nofmean = 0;
}

void SimpleView::FlagReset()
{
    Flag_ConvexHull = false;
    Flag_Proj = false;
    Flag_ProjMatch = false;
}

void SimpleView::slotRender()
{
    double r, g, b, o;

    if (!Flag_ConvexHull)
        slotConvexHull();

    r = ui->LineEdit_SourceMeshR->text().toDouble();
    g = ui->LineEdit_SourceMeshG->text().toDouble();
    b = ui->LineEdit_SourceMeshB->text().toDouble();
    o = ui->LineEdit_SourceMeshOpacity->text().toDouble();
    PolyDataMapper_Source->SetInput(PolyData_Source);
    Actor_Source->SetMapper(PolyDataMapper_Source);
    Actor_Source->GetProperty()->SetColor(r, g, b);
    Actor_Source->GetProperty()->SetEdgeVisibility(1);
    r = ui->LineEdit_SourcePointR->text().toDouble();
    g = ui->LineEdit_SourcePointG->text().toDouble();
    b = ui->LineEdit_SourcePointB->text().toDouble();
    Actor_Source->GetProperty()->SetEdgeColor(r, g, b);
    Actor_Source->GetProperty()->SetPointSize(0);
    Actor_Source->GetProperty()->SetOpacity(o);

    r = ui->LineEdit_SourcePointR->text().toDouble();
    g = ui->LineEdit_SourcePointG->text().toDouble();
    b = ui->LineEdit_SourcePointB->text().toDouble();
    SparsePointsMapper_Source->SetInput(SparsePoints_Source);
    Actor_SparsePointsSource->SetMapper(SparsePointsMapper_Source);
    Actor_SparsePointsSource->GetProperty()->SetColor(r, g, b);
    Actor_SparsePointsSource->GetProperty()->SetPointSize(2);

    r = ui->LineEdit_SourceConvexHullR->text().toDouble();
    g = ui->LineEdit_SourceConvexHullG->text().toDouble();
    b = ui->LineEdit_SourceConvexHullB->text().toDouble();
    o = ui->LineEdit_SourceConvexHullOpacity->text().toDouble();
    PolyDataMapper_ConvexHullSource->SetInput(CHM.ConvexHull_Source);
    Actor_ConvexHullSource->SetMapper(PolyDataMapper_ConvexHullSource);
    Actor_ConvexHullSource->GetProperty()->SetColor(r, g, b);
    Actor_ConvexHullSource->GetProperty()->SetEdgeVisibility(1);
    r = ui->LineEdit_SourcePointR->text().toDouble();
    g = ui->LineEdit_SourcePointG->text().toDouble();
    b = ui->LineEdit_SourcePointB->text().toDouble();
    Actor_ConvexHullSource->GetProperty()->SetEdgeColor(r, g, b);
    Actor_ConvexHullSource->GetProperty()->SetOpacity(o);
    Actor_ConvexHullSource->GetProperty()->SetPointSize(0);


    r = ui->LineEdit_TargetMeshR->text().toDouble();
    g = ui->LineEdit_TargetMeshG->text().toDouble();
    b = ui->LineEdit_TargetMeshB->text().toDouble();
    o = ui->LineEdit_TargetMeshOpacity->text().toDouble();
    PolyDataMapper_Target->SetInput(PolyData_Target);
    Actor_Target->SetMapper(PolyDataMapper_Target);
    Actor_Target->GetProperty()->SetColor(r, g, b);
    Actor_Target->GetProperty()->SetEdgeVisibility(1);
    r = ui->LineEdit_TargetPointR->text().toDouble();
    g = ui->LineEdit_TargetPointG->text().toDouble();
    b = ui->LineEdit_TargetPointB->text().toDouble();
    Actor_Target->GetProperty()->SetEdgeColor(r, g, b);
    Actor_Target->GetProperty()->SetPointSize(0);
    Actor_Target->GetProperty()->SetOpacity(o);

    r = ui->LineEdit_TargetPointR->text().toDouble();
    g = ui->LineEdit_TargetPointG->text().toDouble();
    b = ui->LineEdit_TargetPointB->text().toDouble();
    SparsePointsMapper_Target->SetInput(SparsePoints_Target);
    Actor_SparsePointsTarget->SetMapper(SparsePointsMapper_Target);
    Actor_SparsePointsTarget->GetProperty()->SetColor(r, g, b);
    Actor_SparsePointsTarget->GetProperty()->SetPointSize(2);

    r = ui->LineEdit_TargetConvexHullR->text().toDouble();
    g = ui->LineEdit_TargetConvexHullG->text().toDouble();
    b = ui->LineEdit_TargetConvexHullB->text().toDouble();
    o = ui->LineEdit_TargetConvexHullOpacity->text().toDouble();
    PolyDataMapper_ConvexHullTarget->SetInput(CHM.ConvexHull_Target);
    Actor_ConvexHullTarget->SetMapper(PolyDataMapper_ConvexHullTarget);
    Actor_ConvexHullTarget->GetProperty()->SetColor(r, g, b);
    Actor_ConvexHullTarget->GetProperty()->SetEdgeVisibility(1);
    r = ui->LineEdit_TargetPointR->text().toDouble();
    g = ui->LineEdit_TargetPointG->text().toDouble();
    b = ui->LineEdit_TargetPointB->text().toDouble();
    Actor_ConvexHullTarget->GetProperty()->SetEdgeColor(r, g, b);
    Actor_ConvexHullTarget->GetProperty()->SetOpacity(o);
    Actor_ConvexHullTarget->GetProperty()->SetPointSize(0);

    r = ui->LineEdit_BackGroundR->text().toDouble();
    g = ui->LineEdit_BackGroundG->text().toDouble();
    b = ui->LineEdit_BackGroundB->text().toDouble();
    ren->SetBackground(r, g, b);
    ren->RemoveAllViewProps();

    if (ui->ViewCheck_Axis->isOn())
        ren->AddActor(Actor_axes);

    if (ui->ViewCheck_SourceMesh->isOn())
        ren->AddActor(Actor_Source);
    if (ui->ViewCheck_TargetMesh->isOn())
        ren->AddActor(Actor_Target);

    if (ui->ViewCheck_SourcePoint->isOn())
        ren->AddActor(Actor_SparsePointsSource);
    if (ui->ViewCheck_SourceConvexHull->isOn())
        ren->AddActor(Actor_ConvexHullSource);
    if (ui->ViewCheck_TargetPoint->isOn())
        ren->AddActor(Actor_SparsePointsTarget);
    if (ui->ViewCheck_TargetConvexHull->isOn())
        ren->AddActor(Actor_ConvexHullTarget);

    if (ui->ViewCheck_DistanceMap->isOn())
    {
        DistanceMap();
        o = ui->LineEdit_Range->text().toDouble() * Max_Bound(PolyData_Target);
        DistanceMapper->SetInputConnection(DistanceFilter->GetOutputPort());
        DistanceMapper->SetScalarRange(0, o);
        Actor_DistanceMap->SetMapper(DistanceMapper);
        Actor_DistanceMap->GetProperty()->SetAmbient(0.5);
        Actor_DistanceMap->GetProperty()->SetDiffuse(0.2);
        DistanceMapper->SetLookupTable(lut);
        lut->SetNumberOfTableValues(42);
        lut->Build();
        for (int i = 0; i < 21; i++)
            lut->SetTableValue(i, i * 0.05, 1, 0);
        for (int i = 0; i < 21; i++)
            lut->SetTableValue(21 + i, 1, 1 - i * 0.05, 0);
        lut->SetRange(0, o);
        Actor_scalarBar->SetLookupTable(lut);
        Actor_scalarBar->SetTitle("Distance");
        Actor_scalarBar->SetNumberOfLabels(4);
        Actor_scalarBar->SetWidth(0.1);
        ren->AddActor(Actor_DistanceMap);
        ren->AddActor2D(Actor_scalarBar);
    }

    ui->VTKWidget_Render->GetRenderWindow()->AddRenderer(ren);
    ui->VTKWidget_Render->GetRenderWindow()->Render();
}

/* Action to Open the Source Data File */
void SimpleView::slotOpenSource()
{
    QString fileName = QFileDialog::getOpenFileName("../Data","PLY FILES(*.ply)",0,"","Open Source File");
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    LoadModel(PolyData_Source, fileName);

    char s[10];
    sprintf(s, "%d", PolyData_Source->GetNumberOfPoints());
    this->ui->LineEdit_SourcePts->setText(s);

    setWindowModified(false);
    setWindowFilePath(fileName);

    statusBar()->showMessage(tr("File loaded"), 2000);
    Flag_Source = true;
    FlagReset();
    slotConvexHull();
}

/* Action to Open the Target Data File */
void SimpleView::slotOpenTarget()
{
    QString fileName = QFileDialog::getOpenFileName("../Data","PLY FILES(*.ply)",0,"","Open Target File");
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    LoadModel(PolyData_Target, fileName);

    char s[10];
    sprintf(s, "%d", PolyData_Target->GetNumberOfPoints());
    this->ui->LineEdit_TargetPts->setText(s);

    setWindowModified(false);
    setWindowFilePath(fileName);

    statusBar()->showMessage(tr("File loaded"), 2000);
    Flag_Target = true;
    FlagReset();
    slotConvexHull();
}

void SimpleView::LoadModel(vtkPolyData *PolyData, const char *path)
{
    VTK_CREATE(vtkPLYReader, PLYReader);
    PLYReader->SetFileName(path);
    PLYReader->Update();
    PolyData->DeepCopy(PLYReader->GetOutput());

    if (PolyData->GetNumberOfCells() == 0)
        Points2PolyData(PolyData->GetPoints(), PolyData);
    /* Only Points Data */
}

void SimpleView::SaveModel(vtkPolyData *PolyData, const char *path)
{
    VTK_CREATE(vtkPLYWriter, plyWriter);
    plyWriter->SetFileName(path);
    plyWriter->SetInput(PolyData);
    plyWriter->SetFileTypeToASCII();
    plyWriter->Write();
}

/* Action to Save the Source Data File */
void SimpleView::slotSaveSource()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "../Data/new.ply", tr("PLY (*.ply)"));
    if (fileName == "")
        return;

    SaveModel(PolyData_Source, fileName);
    //SaveModel(CHM.ConvexHull_Source, fileName);
}

/* Action to Save the Target Data File */
void SimpleView::slotSaveTarget()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "../Data/new.ply", tr("PLY (*.ply)"));
    if (fileName == "")
        return;

    SaveModel(PolyData_Target, fileName);
}

/* Action to Save the Mixed Data File */
void SimpleView::slotSaveMixed()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "../Data/new.ply", tr("PLY (*.ply)"));
    if (fileName == "")
        return;

    //Append the two meshes
    VTK_CREATE(vtkAppendPolyData, appendFilter);
#if VTK_MAJOR_VERSION <= 5
    appendFilter->AddInputConnection(PolyData_Source->GetProducerPort());
    appendFilter->AddInputConnection(PolyData_Target->GetProducerPort());
#else
    appendFilter->AddInputData(PolyData_Source);
    appendFilter->AddInputData(PolyData_Target);
#endif
    appendFilter->Update();

    // Remove any duplicate points.
    VTK_CREATE(vtkCleanPolyData, cleanFilter);
    cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
    cleanFilter->Update();

    VTK_CREATE(vtkPLYWriter, plyWriter);
    plyWriter->SetFileName(fileName);
    plyWriter->SetInputConnection(cleanFilter->GetOutputPort());
    plyWriter->Write();
}

/* Function of Set the Path of the Source Projection Images */
void SimpleView::slotSetSourcePath()
{
    QString fileName = QFileDialog::getExistingDirectory("../Data");
    ui->LineEdit_Path1->setText(fileName);
}

/* Function of Set the Path of the Target Projection Images */
void SimpleView::slotSetTargetPath()
{
    QString fileName = QFileDialog::getExistingDirectory("../Data");
    ui->LineEdit_Path2->setText(fileName);
}

void SimpleView::slotTestPath()
{
    QString fileName = QFileDialog::getExistingDirectory("../Data");
    ui->LineEdit_TestPath->setText(fileName);
}

void SimpleView::slotExit() 
{
    qApp->exit();
}

/* Function of ICP Registration */
void SimpleView::slotICP()
{
    clock_t start, finish;
    double TotalTime;
    start = clock();

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }


    // Setup ICP transform
    VTK_CREATE(vtkIterativeClosestPointTransform, icp);
    icp->SetSource(PolyData_Source);
    icp->SetTarget(PolyData_Target);
    icp->GetLandmarkTransform()->SetModeToRigidBody();
    icp->SetMaximumNumberOfIterations(ui->LineEdit_ICPIter->text().toInt());
    icp->StartByMatchingCentroidsOn();
    icp->Modified();
    icp->Update();

    // Get the resulting transformation matrix (this matrix takes the source points to the target points)
    vtkSmartPointer<vtkMatrix4x4> m = icp->GetMatrix();
    // std::cout << "The resulting matrix is: " << *m << std::endl;
    // Transform the source points by the ICP solution

    VTK_CREATE(vtkTransformPolyDataFilter, icpTransformFilter);

#if VTK_MAJOR_VERSION <= 5
    icpTransformFilter->SetInput(PolyData_Source);
#else
    icpTransformFilter->SetInputData(PolyData_Source);
#endif

    icpTransformFilter->SetTransform(icp);
    icpTransformFilter->Update();

    PolyData_Source->DeepCopy(icpTransformFilter->GetOutput());

    finish = clock();
    TotalTime = (double)(finish - start);
    char TotalTime_s[15];
#ifdef _WIN32
    sprintf(TotalTime_s, "%10.1lfms", TotalTime);
#endif
#ifdef linux
    sprintf(TotalTime_s, "%10.1lfms", TotalTime / 1000);
#endif
    this->ui->LineEdit_ICPTime->setText(TotalTime_s);
    statusBar()->showMessage("ICP Done", 2000);

    FlagReset();
    /* Visualization */
    slotRender();
}

/* Function of Transforming one Model */
void SimpleView::slotTransform()
{
    double a, b, r, x, y, z, scale;
    double s[3], c[3];
    if (!((Flag_Source && ui->RadioButton_Source->isOn())||(Flag_Target && ui->RadioButton_Target->isOn())))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }

    /* Build the Transformation Objects */
    VTK_CREATE(vtkTransform, Transform_Pre);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter_Pre);
    VTK_CREATE(vtkTransform, Transform_Real);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter_Real);
    VTK_CREATE(vtkTransform, Transform_Drawback);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter_Drawback);

    /* Get the Transformation Parameters */
    a = ui->LineEdit_RotationX->text().toDouble();
    b = ui->LineEdit_RotationY->text().toDouble();
    r = ui->LineEdit_RotationZ->text().toDouble();
    x = ui->LineEdit_TranslationX->text().toDouble();
    y = ui->LineEdit_TranslationY->text().toDouble();
    z = ui->LineEdit_TranslationZ->text().toDouble();
    s[0]=s[1]=s[2] = ui->LineEdit_Scale->text().toDouble();

    /* Choose the object */
    if (ui->RadioButton_Target->isOn())
    {
        TransformFilter_Pre->SetInput(PolyData_Target);
        PolyData_Target->GetCenter(c);
    }
    else if (ui->RadioButton_Source->isOn())
    {
        TransformFilter_Pre->SetInput(PolyData_Source);
        PolyData_Source->GetCenter(c);
    }

    /* Translate the Model center to 0,0,0 */
    Transform_Pre->Translate(-c[0], -c[1], -c[2]);
    Transform_Pre->RotateX(0);
    Transform_Pre->RotateY(0);
    Transform_Pre->RotateZ(0);
    /* Transform the Model with Transform Parameters */
    Transform_Real->RotateX(a);
    Transform_Real->RotateY(b);
    Transform_Real->RotateZ(r);
    Transform_Real->Scale(s);
    Transform_Real->Translate(x, y, z);
    /* Translate the Model center back to the source */
    Transform_Drawback->Translate(c[0], c[1], c[2]);
    Transform_Drawback->RotateX(0);
    Transform_Drawback->RotateY(0);
    Transform_Drawback->RotateZ(0);
    //左右，右为正,上下，上为正，前后，前为正

    TransformFilter_Pre->SetTransform(Transform_Pre);
    TransformFilter_Pre->Update();

    TransformFilter_Real->SetInput(TransformFilter_Pre->GetOutput());
    TransformFilter_Real->SetTransform(Transform_Real);
    TransformFilter_Real->Update();

    TransformFilter_Drawback->SetInput(TransformFilter_Real->GetOutput());
    TransformFilter_Drawback->SetTransform(Transform_Drawback);
    TransformFilter_Drawback->Update();

    if (ui->RadioButton_Target->isOn())
        PolyData_Target->DeepCopy(TransformFilter_Drawback->GetOutput());
    else if (ui->RadioButton_Source->isOn())
        PolyData_Source->DeepCopy(TransformFilter_Drawback->GetOutput());

    FlagReset();
    /* Visualization */
    slotRender();
}

/* Function of Normalizing one Model */
void SimpleView::slotNormalize()
{
    double cent[3];
    double s;

    if (!((Flag_Source && ui->RadioButton_Source->isOn())||(Flag_Target && ui->RadioButton_Target->isOn())))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    double Bounding = ui->LineEdit_Bounding->text().toDouble();

    /* Build the Transformation Objects */
    VTK_CREATE(vtkTransform, Transform);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter);

    if (ui->RadioButton_Target->isOn())
    {
        TransformFilter->SetInput(PolyData_Target);
        PolyData_Target->GetCenter(cent);
        s = Bounding / Max_Bound(PolyData_Target);
    }
    else if (ui->RadioButton_Source->isOn())
    {
        TransformFilter->SetInput(PolyData_Source);
        PolyData_Source->GetCenter(cent);
        s = Bounding / Max_Bound(PolyData_Source);
    }

    Transform->Scale(s,s,s);
    Transform->Translate(-cent[0], -cent[1], -cent[2]);
    TransformFilter->SetTransform(Transform);
    TransformFilter->Update();

    if (ui->RadioButton_Target->isOn())
        PolyData_Target->DeepCopy(TransformFilter->GetOutput());
    else if (ui->RadioButton_Source->isOn())
        PolyData_Source->DeepCopy(TransformFilter->GetOutput());

    FlagReset();
    /* Visualization */
    slotRender();
}

/* Function of 3PCHM*/
void SimpleView::slotCHM()
{
    clock_t start, finish;
    double TotalTime;
    start = clock();

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    if (!Flag_ConvexHull)
        slotConvexHull();

    CHM.Register();

    VTK_CREATE(vtkTransform, Transform);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter);
    Transform->SetMatrix(CHM.BestTransform);
    TransformFilter->SetInput(PolyData_Source);
    TransformFilter->SetTransform(Transform);
    TransformFilter->Update();
    PolyData_Source->DeepCopy(TransformFilter->GetOutput());

    TransformFilter->SetInput(SparsePoints_Source);
    TransformFilter->Update();
    SparsePoints_Source->DeepCopy(TransformFilter->GetOutput());

    finish = clock();
    TotalTime = (double)(finish - start);
    char TotalTime_s[15];
#ifdef _WIN32
    sprintf(TotalTime_s, "%10.1lfms", TotalTime);
#endif
#ifdef linux
    sprintf(TotalTime_s, "%10.1lfms", TotalTime / 1000);
#endif
    this->ui->LineEdit_CHMTime->setText(TotalTime_s);
    statusBar()->showMessage("CHM Done", 2000);

    /* ICP Refine if need */
    if (ui->RadioButton_ADMMeasure->isOn())
        slotICP();

    FlagReset();
    /* Visualization */
    slotRender();
}

/* Function of Extraction for the Convex Hull */
void SimpleView::slotConvexHull()
{	
    VTK_CREATE(vtkMassProperties, massProperty);
    char s[10];

    if (Flag_Source)
    {
        CHM.SetSource(PolyData_Source, ui->LineEdit_SourceSD->text().toInt());
        CHM.Cal_CH_Source(ui->CheckBox_ConvexHullNormalizing->isOn());
        Points2PolyData(CHM.Points_Source, SparsePoints_Source);
        massProperty->SetInput(CHM.ConvexHull_Source);
        massProperty->Update();
        sprintf(s, "%.1lf", massProperty->GetVolume());
        this->ui->LineEdit_SourceVol->setText(s);
    }
    if (Flag_Target)
    {
        CHM.SetTarget(PolyData_Target, ui->LineEdit_TargetSD->text().toInt());
        CHM.Cal_CH_Target(ui->CheckBox_ConvexHullNormalizing->isOn());
        Points2PolyData(CHM.Points_Target, SparsePoints_Target);
        massProperty->SetInput(CHM.ConvexHull_Target);
        massProperty->Update();
        sprintf(s, "%.1lf", massProperty->GetVolume());
        this->ui->LineEdit_TargetVol->setText(s);
    }

    Flag_ConvexHull = true;
    slotRender();
}

/* Random Shuffle one Model */
void SimpleView::slotRandomT()
{    
    char s[100];
    double x;
    x = rand()%180;
    sprintf(s, "%.0lf", x);
    ui->LineEdit_RotationX->setText(s);
    x = rand()%180;
    sprintf(s, "%.0lf", x);
    ui->LineEdit_RotationY->setText(s);
    x = rand()%180;
    sprintf(s, "%.0lf", x);
    ui->LineEdit_RotationZ->setText(s);
    x = rand()%200/100.0 - 1;
    sprintf(s, "%.2lf", x);
    ui->LineEdit_TranslationX->setText(s);
    x = rand()%200/100.0 - 1;
    sprintf(s, "%.2lf", x);
    ui->LineEdit_TranslationY->setText(s);
    x = rand()%200/100.0 - 1;
    sprintf(s, "%.2lf", x);
    ui->LineEdit_TranslationZ->setText(s);
}


/* Noising one Model */
void SimpleView::slotNoising()
{
    if (!((Flag_Source && ui->RadioButton_Source->isOn())||(Flag_Target && ui->RadioButton_Target->isOn())))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }

    double sig = ui->LineEdit_Sigma->text().toDouble() / 100;
    double x[3];

    if (ui->RadioButton_Target->isOn())
    {
        for (int i = 0; i < PolyData_Target->GetNumberOfPoints(); i++)
        {
            PolyData_Target->GetPoint(i, x);
            x[0] = x[0] + NoiseGen(rand() % 100) * sig;
            x[1] = x[1] + NoiseGen(rand() % 100) * sig;
            x[2] = x[2] + NoiseGen(rand() % 100) * sig;
            PolyData_Target->GetPoints()->SetPoint(i, x);
        }
    }
    else if (ui->RadioButton_Source->isOn())
    {
        for (int i = 0; i < PolyData_Source->GetNumberOfPoints(); i++)
        {
            PolyData_Source->GetPoint(i, x);
            x[0] = x[0] + NoiseGen(rand() % 100) * sig;
            x[1] = x[1] + NoiseGen(rand() % 100) * sig;
            x[2] = x[2] + NoiseGen(rand() % 100) * sig;
            PolyData_Source->GetPoints()->SetPoint(i, x);
        }
    }

    FlagReset();
    /* Visualization */
    slotRender();
}

/* Noising one Model */
void SimpleView::slotTPSNoising()
{
    if (!((Flag_Source && ui->RadioButton_Source->isOn())||(Flag_Target && ui->RadioButton_Target->isOn())))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }

    if (!Flag_ConvexHull)
        slotConvexHull();

    double sig = ui->LineEdit_Sigma->text().toDouble() / 100;
    double x[3];

    if (ui->RadioButton_Target->isOn())
    {

        vtkPolyData2vnl_matrix(PolyData_Target, GMMTPS.model);
        vtkPolyData2vnl_matrix(CHM.ConvexHull_Target, GMMTPS.ctrl_pts);
        GMMTPS.initializeTPS();
        int d = 3;
        int rows_x = CHM.ConvexHull_Target->GetNumberOfPoints();
        int n = rows_x;
        vnl_matrix<double> param_tps;
        param_tps.set_size(n - d - 1, d);
        param_tps.fill(0);
        //param_all.update(param_affine);
        for (int i = 0; i < rows_x - d - 1; i++)
        {
            for (int j = 0; j < d; j++)
            {
                param_tps(i, j) = param_tps(i, j) + vtkMath::Gaussian(0.0,1.0) * sig;
            }
        }
        GMMTPS.param_all.update(param_tps, d + 1);
        GMMTPS.transformed_model = GMMTPS.basis * GMMTPS.param_all;
        GMMTPS.denormalize_transformed_model();
        //get result
        vnl_matrix2vtkPolyData(PolyData_Target, GMMTPS.transformed_model);
    }
    else if (ui->RadioButton_Source->isOn())
    {
        vtkPolyData2vnl_matrix(PolyData_Source, GMMTPS.model);
        vtkPolyData2vnl_matrix(CHM.ConvexHull_Source, GMMTPS.ctrl_pts);
        GMMTPS.initializeTPS();
        int d = 3;
        int rows_x = CHM.ConvexHull_Source->GetNumberOfPoints();
        int n = rows_x;
        vnl_matrix<double> param_tps;
        param_tps.set_size(n - d - 1, d);
        param_tps.fill(0);
        //param_all.update(param_affine);
        for (int i = 0; i < rows_x - d - 1; i++)
        {
            for (int j = 0; j < d; j++)
            {
                param_tps(i, j) = param_tps(i, j) + vtkMath::Gaussian(0.0,1.0) * sig;
            }
        }
        GMMTPS.param_all.update(param_tps, d + 1);
        GMMTPS.transformed_model = GMMTPS.basis * GMMTPS.param_all;
        GMMTPS.denormalize_transformed_model();
        //get result
        vnl_matrix2vtkPolyData(PolyData_Source, GMMTPS.transformed_model);
    }

    FlagReset();
    /* Visualization */
    slotRender();
}

/* Project one Image of Source Data */
void SimpleView::slotProjOneImg()
{
    if (!Flag_Source)
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    ProjOneFace(PolyData_Source, CHM.ConvexHull_Source, ui->LineEdit_ProjOneId->text().toInt(), ui->LineEdit_Path1->text().toAscii().data());
}

/* Random Project one Image of Source Data */
void SimpleView::slotRandomProj()
{
    if (!Flag_Source)
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    ProjOneFace(PolyData_Source, CHM.ConvexHull_Source, rand()%50, ui->LineEdit_Path1->text().toAscii().data());
}

void SimpleView::slotProj()
{
    char String_Message[100];

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    if (!Flag_ConvexHull)
        slotConvexHull();

    int i;
    for (i = 0; i < CHM.ConvexHull_Source->GetNumberOfCells(); i++)
    {
        ProjOneFace(PolyData_Source, CHM.ConvexHull_Source, i, ui->LineEdit_Path1->text().toAscii().data());
        sprintf(String_Message, "Projection Images for Source: %d%% finished", i*100 / CHM.ConvexHull_Source->GetNumberOfCells());
        statusBar()->showMessage(String_Message, 2000);
    }
    for (i = 0; i < CHM.ConvexHull_Target->GetNumberOfCells(); i++)
    {
        ProjOneFace(PolyData_Target, CHM.ConvexHull_Target, i, ui->LineEdit_Path2->text().toAscii().data());
        sprintf(String_Message, "Projection Images for Target: %d%% finished", i*100 / CHM.ConvexHull_Target->GetNumberOfCells());
        statusBar()->showMessage(String_Message, 2000);
    }

    Flag_Proj = true;
    statusBar()->showMessage("Done", 2000);
}

void SimpleView::slotProjMatch()
{
    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    if (!Flag_Proj)
        slotProj();

    double x[3];
    int i, j, tot2d = 0;
    char String_Message[100];
    vector<bool> ps_in, pt_in;

    VTK_CREATE(vtkPoints, Points_2d_source);
    VTK_CREATE(vtkPolyData, PolyData_2d_source);
    VTK_CREATE(vtkPolyData, PolyData_3d_source);

    VTK_CREATE(vtkPoints, Points_2d_target);
    VTK_CREATE(vtkPolyData, PolyData_2d_target);
    VTK_CREATE(vtkPolyData, PolyData_3d_target);

    VTK_CREATE(vtkPlaneSource, planesource);

    PM.SetPath(ui->LineEdit_Path1->text().toAscii().data(), ui->LineEdit_Path2->text().toAscii().data());
    PM.SetSize(CHM.ConvexHull_Source->GetNumberOfCells(), CHM.ConvexHull_Target->GetNumberOfCells());
    PM.KeypointsExtract();

    GMPoints_Source->Reset();
    GMPoints_Target->Reset();
    for (i = 0; i < CHM.ConvexHull_Source->GetNumberOfCells(); i++)
    {
        PM.Update(i);
        sprintf(String_Message, "Matching State: %d%% finished", i*100 / CHM.ConvexHull_Source->GetNumberOfCells());
        statusBar()->showMessage(String_Message, 2000);

        tot2d += PM.GoodMatches[i].size();

        if (ui->ViewCheck_Show2DMatches->isOn())
            PM.ShowMatch(i, PM.BestPair[i]);

        Points_2d_source->Reset();
        ps_in.clear();
        Points_2d_target->Reset();
        pt_in.clear();

        for (j = 0; j < PM.GoodMatches[i].size(); j++)
        {
            // modify
            x[0] = (double) (PM.keypoints_1[i][PM.GoodMatches[i][j].queryIdx].pt.y) / (sizeofimg/4) - 2;
            x[1] = (double) (PM.keypoints_1[i][PM.GoodMatches[i][j].queryIdx].pt.x) / (sizeofimg/4) - 2;
            x[2] = 0;
            Points_2d_source->InsertNextPoint(x);
        }
        Points2PolyData(Points_2d_source, PolyData_2d_source);
        planesource->SetOrigin(CHM.ConvexHull_Source->GetCell(i)->GetPoints()->GetPoint(0));
        planesource->SetPoint1(CHM.ConvexHull_Source->GetCell(i)->GetPoints()->GetPoint(1));
        planesource->SetPoint2(CHM.ConvexHull_Source->GetCell(i)->GetPoints()->GetPoint(2));
        planesource->Update();
        PolyDataBackProject(PolyData_2d_source, PolyData_3d_source, planesource, ps_in);

        for (j = 0; j < PM.GoodMatches[i].size(); j++)
        {
            // modify
            x[0] = (double) (PM.keypoints_2[PM.BestPair[i]][PM.GoodMatches[i][j].trainIdx].pt.y) / (sizeofimg/4) - 2;
            x[1] = (double) (PM.keypoints_2[PM.BestPair[i]][PM.GoodMatches[i][j].trainIdx].pt.x) / (sizeofimg/4) - 2;
            x[2] = 0;
            Points_2d_target->InsertNextPoint(x);
        }
        Points2PolyData(Points_2d_target, PolyData_2d_target);
        planesource->SetOrigin(CHM.ConvexHull_Target->GetCell(PM.BestPair[i])->GetPoints()->GetPoint(0));
        planesource->SetPoint1(CHM.ConvexHull_Target->GetCell(PM.BestPair[i])->GetPoints()->GetPoint(1));
        planesource->SetPoint2(CHM.ConvexHull_Target->GetCell(PM.BestPair[i])->GetPoints()->GetPoint(2));
        planesource->Update();
        PolyDataBackProject(PolyData_2d_target, PolyData_3d_target, planesource, pt_in);

        for (j = 0; j < PM.GoodMatches[i].size(); j++)
        {
            if(ps_in[j] && pt_in[j])
            {
                GMPoints_Source->InsertNextPoint(PolyData_3d_source->GetPoint(j));
                GMPoints_Target->InsertNextPoint(PolyData_3d_target->GetPoint(j));
            }
        }

        /* Time-lapse */
        QElapsedTimer t;
        t.start();
        while(t.elapsed()< 5)
            QCoreApplication::processEvents();
    }

    cout<<"Numbers of 2d Matches = "<<tot2d<<endl;
    statusBar()->showMessage("Done", 2000);
    Flag_ProjMatch = true;
}

void SimpleView::slotShow3DMatches()
{
    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    if (!Flag_ProjMatch)
        slotProjMatch();

    VTK_CREATE(vtkPoints, Points_Mat);
    VTK_CREATE(vtkPolyData, Mat_3D);
    VTK_CREATE(vtkLine, line);
    VTK_CREATE(vtkCellArray, lines);
    VTK_CREATE(vtkPolyDataMapper, Mat_map);
    VTK_CREATE(vtkActor, Mat_act);
    int i;

    for (i = 0; i < GMPoints_Source->GetNumberOfPoints(); i++)
    {
        line->GetPointIds()->SetId(0, Points_Mat->InsertNextPoint(GMPoints_Source->GetPoint(i)));
        line->GetPointIds()->SetId(1, Points_Mat->InsertNextPoint(GMPoints_Target->GetPoint(i)));
        lines->InsertNextCell(line);
    }

    Mat_3D->SetPoints(Points_Mat);
    Mat_3D->SetLines(lines);

    Mat_map->SetInput(Mat_3D);

    Mat_act->SetMapper(Mat_map);
    Mat_act->GetProperty()->SetColor(0,1,0);
    Mat_act->GetProperty()->SetPointSize(10);

    ren->AddActor(Mat_act);
    ui->VTKWidget_Render->GetRenderWindow()->Render();

    statusBar()->showMessage("Done", 2000);
}

void SimpleView::slotProjRegister()
{
    clock_t start, finish;
    double TotalTime;
    start = clock();

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    if (!Flag_ProjMatch)
        slotProjMatch();

    VTK_CREATE(vtkLandmarkTransform, landmarkTransform);
    VTK_CREATE(vtkPolyData, polydata);
    int i;

    cout<<"Number of 3d Matches = "<<GMPoints_Source->GetNumberOfPoints()<<endl;
    landmarkTransform->SetSourceLandmarks(GMPoints_Source);
    landmarkTransform->SetTargetLandmarks(GMPoints_Target);
    landmarkTransform->Update();

    cout << *landmarkTransform->GetMatrix();

    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter);
    TransformFilter->SetInput(PolyData_Source);
    TransformFilter->SetTransform(landmarkTransform);
    TransformFilter->Update();

    PolyData_Source->DeepCopy(TransformFilter->GetOutput());

    Points2PolyData(GMPoints_Source, polydata);
    TransformFilter->SetInput(polydata);
    TransformFilter->Update();

    FILE *fp;
    fp = fopen("error_of_3d_Matches.txt", "w");
    for (i = 0; i < GMPoints_Source->GetNumberOfPoints(); i++)
        fprintf(fp, "%lf\n", 100 * Euc_Dist(TransformFilter->GetOutput()->GetPoint(i), GMPoints_Target->GetPoint(i)));
    fclose(fp);

    finish = clock();
    TotalTime = (double)(finish - start);
    char TotalTime_s[15];
#ifdef _WIN32
    sprintf(TotalTime_s, "%10.1lfms", TotalTime);
#endif
#ifdef linux
    sprintf(TotalTime_s, "%10.1lfms", TotalTime / 1000);
#endif
    this->ui->LineEdit_IIOCHTime->setText(TotalTime_s);
    statusBar()->showMessage("IIOCH Registration Done", 2000);

    FlagReset();
    /* Visualization */
    slotRender();
}

void SimpleView::PolyDataProject(vtkPolyData *PolyData, vtkPolyData *ProjectedPolyData, vtkPlaneSource *sourceplane)
{
    VTK_CREATE(vtkPlane, plane);
    VTK_CREATE(vtkPoints, projpoints);
    VTK_CREATE(vtkPolyData, projpoly);
    VTK_CREATE(vtkLandmarkTransform, landmarkTransform);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter);

    VTK_CREATE(vtkPoints, srcpoints);
    VTK_CREATE(vtkPoints, tagpoints);
    int i;
    double x[3];

    plane->SetNormal(sourceplane->GetNormal());
    plane->SetOrigin(sourceplane->GetOrigin());

    for (i = 0; i < PolyData->GetNumberOfPoints(); i++)
    {
        plane->ProjectPoint(PolyData->GetPoint(i), x);
        projpoints->InsertNextPoint(x);
    }
    Points2PolyData(projpoints, projpoly);

    srcpoints->InsertNextPoint(sourceplane->GetOrigin());
    srcpoints->InsertNextPoint(sourceplane->GetOrigin()[0]+sourceplane->GetNormal()[0], sourceplane->GetOrigin()[1]+sourceplane->GetNormal()[1], sourceplane->GetOrigin()[2]+sourceplane->GetNormal()[2]);
    tagpoints->InsertNextPoint(0,0,0);
    tagpoints->InsertNextPoint(0,0,1);
    landmarkTransform->SetSourceLandmarks(srcpoints);
    landmarkTransform->SetTargetLandmarks(tagpoints);
    landmarkTransform->Update();

    TransformFilter->SetInput(projpoly);
    TransformFilter->SetTransform(landmarkTransform);
    TransformFilter->Update();

    ProjectedPolyData->DeepCopy(TransformFilter->GetOutput());
}

void SimpleView::PolyDataBackProject(vtkPolyData *ProjectedPolyData, vtkPolyData *PolyData, vtkPlaneSource *sourceplane, vector<bool> &p_in)
{
    VTK_CREATE(vtkPlane, plane);
    VTK_CREATE(vtkLandmarkTransform, landmarkTransform);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter_tri);
    VTK_CREATE(vtkLandmarkTransform, landmarkTransform_back);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter_back);

    VTK_CREATE(vtkPoints, srcpoints);
    VTK_CREATE(vtkPoints, tagpoints);

    VTK_CREATE(vtkPoints, points_tri);
    VTK_CREATE(vtkPolyData, poly_tri);

    int i;
    double x[3];

    plane->SetNormal(sourceplane->GetNormal());
    plane->SetOrigin(sourceplane->GetOrigin());

    srcpoints->InsertNextPoint(sourceplane->GetOrigin());
    srcpoints->InsertNextPoint(sourceplane->GetOrigin()[0]+sourceplane->GetNormal()[0], sourceplane->GetOrigin()[1]+sourceplane->GetNormal()[1], sourceplane->GetOrigin()[2]+sourceplane->GetNormal()[2]);
    tagpoints->InsertNextPoint(0,0,0);
    tagpoints->InsertNextPoint(0,0,1);
    landmarkTransform->SetSourceLandmarks(srcpoints);
    landmarkTransform->SetTargetLandmarks(tagpoints);
    landmarkTransform->Update();

    points_tri->InsertNextPoint(sourceplane->GetOrigin());
    points_tri->InsertNextPoint(sourceplane->GetPoint1());
    points_tri->InsertNextPoint(sourceplane->GetPoint2());
    Points2PolyData(points_tri, poly_tri);

    TransformFilter_tri->SetInput(poly_tri);
    TransformFilter_tri->SetTransform(landmarkTransform);
    TransformFilter_tri->Update();

    landmarkTransform_back->SetSourceLandmarks(tagpoints);
    landmarkTransform_back->SetTargetLandmarks(srcpoints);
    landmarkTransform_back->Update();

    TransformFilter_back->SetTransform(landmarkTransform_back);

    double ax, ay, bx, by, cx, cy;
    ax = TransformFilter_tri->GetOutput()->GetPoint(0)[0];
    ay = TransformFilter_tri->GetOutput()->GetPoint(0)[1];
    bx = TransformFilter_tri->GetOutput()->GetPoint(1)[0];
    by = TransformFilter_tri->GetOutput()->GetPoint(1)[1];
    cx = TransformFilter_tri->GetOutput()->GetPoint(2)[0];
    cy = TransformFilter_tri->GetOutput()->GetPoint(2)[1];

    // Judge whether point is in the triangle, true means in, true means out
    for (i = 0; i < ProjectedPolyData->GetNumberOfPoints(); i++)
    {
        ProjectedPolyData->GetPoint(i, x);
        if (xmult(x[0],x[1],bx,by,ax,ay)*xmult(x[0],x[1],cx,cy,ax,ay)<0 && xmult(x[0],x[1],bx,by,cx,cy)*xmult(x[0],x[1],ax,ay,cx,cy)<0)
            p_in.push_back(true);
        else
            p_in.push_back(false);;
    }

    TransformFilter_back->SetInput(ProjectedPolyData);
    TransformFilter_back->Update();
    PolyData->DeepCopy(TransformFilter_back->GetOutput());
}

void SimpleView::ProjOneFace(vtkPolyData *PolyData, vtkPolyData *PolyConvexHull, int Pid, char path[])
{
    double x[3];
    int i, j, maxval = 0, hist[256], sum[256];
    cv::Mat projfre, projimg, equahistimg, T;
    vector<bool> p_in;

    VTK_CREATE(vtkPolyData, ProjectedPolyData);
    VTK_CREATE(vtkPlaneSource, planesource);

    //------------------------------------
    VTK_CREATE(vtkPoints, Points_back);
    VTK_CREATE(vtkPolyData, Poly_back);
    VTK_CREATE(vtkDoubleArray, weights_back);
    VTK_CREATE(vtkPolyData, Poly_backed);
    VTK_CREATE(vtkPoints, Points_backtri);
    VTK_CREATE(vtkPolyData, Poly_backtri);
    VTK_CREATE(vtkDoubleArray, weights_backtri);
    VTK_CREATE(vtkPolyDataMapper, mapper_back);
    VTK_CREATE(vtkActor, actor_back);
    //------------------------------------

    PolyConvexHull->GetCell(Pid)->GetPoints();
    planesource->SetOrigin(PolyConvexHull->GetCell(Pid)->GetPoints()->GetPoint(0));
    planesource->SetPoint1(PolyConvexHull->GetCell(Pid)->GetPoints()->GetPoint(1));
    planesource->SetPoint2(PolyConvexHull->GetCell(Pid)->GetPoints()->GetPoint(2));
    planesource->Update();

    PolyDataProject(PolyData, ProjectedPolyData, planesource);

    sizeofimg = ui->LineEdit_ImgSize->text().toInt();
    projfre = Mat::zeros(sizeofimg, sizeofimg, CV_32S);
    for (i = 0; i < ProjectedPolyData->GetNumberOfPoints(); i++)
    {
        ProjectedPolyData->GetPoint(i, x);
        if (x[0] < -2 || x[0] >= 2 || x[1] < - 2 || x[1] >= 2)
            continue;
        maxval = std::max (maxval, ++projfre.at<int>((int)((x[0]+2)*sizeofimg/4), (int)((x[1]+2)*sizeofimg/4)));
    }

    projimg = Mat::zeros(sizeofimg, sizeofimg, CV_8U);
    equahistimg = Mat::zeros(sizeofimg, sizeofimg, CV_8U);
    T = Mat::zeros(1, 256, CV_8U);
    memset(hist, 0, sizeof(hist));
    for (i = 0; i < sizeofimg; i++)
    {
        for (j = 0; j < sizeofimg; j++)
        {
            projimg.at<uchar>(i, j) = 255 - projfre.at<int>(i, j) * 255 / maxval;
            hist[projimg.at<uchar>(i, j)]++;
        }
    }
    // Ignore 255 pixel as background
    hist[255] = 0;
    sum[0] = hist[0];
    for ( i = 1; i < 256; i++)
        sum[i] = sum[i-1] + hist[i];

    // Compute intensity transformation
    for(i = 0; i < 256; i++ )
        T.at<uchar> (0,i) = (uchar) (255 * sum[i] / sum[255]); // range is [0,255]

    // directly use look-up-table function
    LUT(projimg, T, equahistimg);
    char filepath[100];
    sprintf(filepath, "%s/%d.bmp", path, Pid);
    imwrite(filepath, equahistimg);


    /* Back-Projection */
    if (!ui->RadioButton_RenderNothing->isOn())
    {

        for (i = 0; i < sizeofimg; i++)
        {
            for (j = 0; j < sizeofimg; j++)
            {
                if (equahistimg.at<uchar>(i, j) < 255)
                {
                    x[0] = (double)i / (sizeofimg/4) - 2;
                    x[1] = (double)j / (sizeofimg/4) - 2;
                    x[2] = 0;
                    weights_back->InsertNextValue((double) equahistimg.at<uchar>(i,j));
                    Points_back->InsertNextPoint(x);
                }
            }
        }

        Points2PolyData(Points_back, Poly_back);
        PolyDataBackProject(Poly_back, Poly_backed, planesource, p_in);

        if (ui->RadioButton_RenderWhole->isOn())
        {
            Poly_backed->GetPointData()->SetScalars(weights_back);
            mapper_back->SetInput(Poly_backed);
        }
        else if (ui->RadioButton_RenderTriOnly->isOn())
        {
            for (i = 0; i < Poly_backed->GetNumberOfPoints(); i++)
            {
                if (p_in[i])
                {
                    Points_backtri->InsertNextPoint(Poly_backed->GetPoint(i));
                    weights_backtri->InsertNextValue(weights_back->GetValue(i));
                }
            }
            Points2PolyData(Points_backtri, Poly_backtri);
            Poly_backtri->GetPointData()->SetScalars(weights_backtri);
            mapper_back->SetInput(Poly_backtri);
        }

        mapper_back->SetScalarModeToUsePointData();
        mapper_back->ScalarVisibilityOn();
        mapper_back->SetScalarRange(0,255);

        VTK_CREATE(vtkLookupTable, lut);
        lut->SetTableRange (0, 255);
        lut->SetSaturationRange (0, 0);
        lut->SetHueRange (0, 0);
        lut->SetValueRange (0, 1);
        lut->Build(); //effective built

        mapper_back->SetLookupTable(lut);

        actor_back->SetMapper(mapper_back);
        actor_back->GetProperty()->SetPointSize(8);
        ren->AddActor(actor_back);
        ui->VTKWidget_Render->GetRenderWindow()->Render();
    }

    /* Texture */
    /*
    vtkSmartPointer<vtkTexture> atext = vtkSmartPointer<vtkTexture>::New();
    atext->SetInputConnection(imageSource->GetOutputPort());
    atext->InterpolateOff();
    atext->RepeatOff();

    //计算多边形的发向量。
    vtkSmartPointer<vtkPolyDataNormals> normal = vtkSmartPointer<vtkPolyDataNormals>::New();
    normal->SetInput(PolyConvexHull);

    //设置多边形的纹理映射模式
    //设置为plane模式
    vtkSmartPointer<vtkTextureMapToPlane> tmapper = vtkSmartPointer<vtkTextureMapToPlane>::New();
    tmapper->SetInputConnection(normal->GetOutputPort());

    tmapper->SetOrigin(PolyConvexHull->GetCell(Pid)->GetPoints()->GetPoint(0));
    tmapper->SetPoint1(PolyConvexHull->GetCell(Pid)->GetPoints()->GetPoint(1));
    tmapper->SetPoint2(PolyConvexHull->GetCell(Pid)->GetPoints()->GetPoint(2));


    //PolyDataMapper_ConvexHullSource->SetInputConnection(tmapper->GetOutputPort());
    Actor_ConvexHullSource->SetTexture(atext);
    ui->VTKWidget_Render->GetRenderWindow()->Render();
    */
    /* Texture */

    projfre.~Mat();
    projimg.~Mat();
    equahistimg.~Mat();
    T.~Mat();
    /* Time-lapse */
    QElapsedTimer t;
    t.start();
    while(t.elapsed()< 5)
        QCoreApplication::processEvents();
}

void SimpleView::slotCHGMM()
{
    clock_t start, finish;
    double TotalTime;
    start = clock();

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }

    MeshGMM.Alpha = this->ui->LineEdit_CHGMMAlpha->text().toDouble();
    MeshGMM.Beta = this->ui->LineEdit_CHGMMBeta->text().toDouble();


    if(!this->Flag_ConvexHull)
        this->slotConvexHull();
    MeshGMM.Points_Source->DeepCopy(CHM.Points_Source);
    MeshGMM.Points_Target->DeepCopy(CHM.Points_Target);
    MeshGMM.Poly_Source->DeepCopy(CHM.Poly_Source);
    MeshGMM.Poly_Target->DeepCopy(CHM.Poly_Target);
    MeshGMM.ConvexHull_Source->DeepCopy(CHM.ConvexHull_Source);
    MeshGMM.ConvexHull_Target->DeepCopy(CHM.ConvexHull_Target);
    MeshGMM.Register(true);

    VTK_CREATE(vtkTransform, MeshGMMTransform);
    VTK_CREATE(vtkTransformPolyDataFilter, MeshGMMTransformFilter);
    MeshGMMTransform->SetMatrix(MeshGMM.BestTransform);
    MeshGMMTransformFilter->SetInput(PolyData_Source);
    MeshGMMTransformFilter->SetTransform(MeshGMMTransform);
    MeshGMMTransformFilter->Update();
    PolyData_Source->DeepCopy(MeshGMMTransformFilter->GetOutput());

    cout<<"\nMeshGMM done\n" <<endl;

    finish = clock();
    TotalTime = (double)(finish - start);
    char TotalTime_s[15];
#ifdef _WIN32
    sprintf(TotalTime_s, "%10.1lfms", TotalTime);
#endif
#ifdef linux
    sprintf(TotalTime_s, "%10.1lfms", TotalTime / 1000);
#endif
    this->ui->LineEdit_CHGMMTime->setText(TotalTime_s);
    statusBar()->showMessage("CHGMM Done", 2000);

    /* ICP Refine if need */
    if (ui->RadioButton_ADMMeasure->isOn())
        slotICP();

    FlagReset();
    /* Visualization */
    this->slotRender();
}

void SimpleView::slotCHGMMNonRigid()
{
    clock_t start, finish;
    double TotalTime;
    start = clock();

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    if(!this->Flag_ConvexHull)
        this->slotConvexHull();

    vtkPolyData2vnl_matrix(PolyData_Source, GMMTPS.model);
    vtkPolyData2vnl_matrix(PolyData_Target, GMMTPS.scene);
    //vtkPolyData2vnl_matrix(CHM.ConvexHull_Source, GMMTPS.model);
    //vtkPolyData2vnl_matrix(CHM.ConvexHull_Target, GMMTPS.scene);

    vtkPolyData2vnl_matrix(CHM.ConvexHull_Source, GMMTPS.ctrl_pts);

    //gmmreg.b_fix_affine = this->ui->CheckBox_Affined_GMMREG->isChecked();
    GMMTPS.level = this->ui->LineEdit_GMMLevel->text().toInt();
    GMMTPS.v_lambda.clear();
    for(int i = 0;i < 3; i++)
    {
        GMMTPS.v_lambda.push_back(this->ui->LineEdit_GMMLambda->text().toInt());
    }
    //run gmm_tps
    GMMTPS.run(this->ui->LineEdit_GMMMaxEvals->text().toInt());
    //get result
    vnl_matrix2vtkPolyData(PolyData_Source, GMMTPS.transformed_model);
    VTK_CREATE(vtkTransform, Transform);
    VTK_CREATE(vtkTransformPolyDataFilter, TransformFilter);
    TransformFilter->SetInput(PolyData_Source);
    TransformFilter->SetTransform(Transform);
    TransformFilter->Update();
    PolyData_Source->DeepCopy(TransformFilter->GetOutput());

    cout<<"\nCHGMM_TPS_L2 done\n" <<endl;

    finish = clock();
    TotalTime = (double)(finish - start);
    char TotalTime_s[15];
#ifdef _WIN32
    sprintf(TotalTime_s, "%10.1lfms", TotalTime);
#endif
#ifdef linux
    sprintf(TotalTime_s, "%10.1lfms", TotalTime / 1000);
#endif
    this->ui->LineEdit_CHGMMTime->setText(TotalTime_s);
    statusBar()->showMessage("CHGMM NonRigid Done", 2000);

    FlagReset();
    /* Visualization */
    this->slotRender();
}

void SimpleView::slotGMMRigid()
{
    clock_t start, finish;
    double TotalTime;
    start = clock();

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }

    vtkPolyData2vnl_matrix(PolyData_Source, GMM.model);
    vtkPolyData2vnl_matrix(PolyData_Target, GMM.scene);

    GMM.level = this->ui->LineEdit_GMMLevel->text().toInt();
    GMM.v_lambda.clear();
    for(int i = 0;i < 3; i++)
    {
        GMM.v_lambda.push_back(this->ui->LineEdit_GMMLambda->text().toInt());
    }
    //run gmm_tps
    GMM.run(this->ui->LineEdit_GMMMaxEvals->text().toInt());
    //get result
    vnl_matrix2vtkPolyData(PolyData_Source, GMM.transformed_model);
    cout<<"\nRigid done\n" <<endl;

    finish = clock();
    TotalTime = (double)(finish - start);
    char TotalTime_s[15];
#ifdef _WIN32
    sprintf(TotalTime_s, "%10.1lfms", TotalTime);
#endif
#ifdef linux
    sprintf(TotalTime_s, "%10.1lfms", TotalTime / 1000);
#endif
    this->ui->LineEdit_GMMTime->setText(TotalTime_s);
    statusBar()->showMessage("GMMRigid Done", 2000);

    FlagReset();
    /* Visualization */
    this->slotRender();
}

void SimpleView::slotGMMNonRigid()
{
    clock_t start, finish;
    double TotalTime;
    start = clock();

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }

    vtkPolyData2vnl_matrix(PolyData_Source, GMMTPS.model);
    vtkPolyData2vnl_matrix(PolyData_Target, GMMTPS.scene);

    vtkPolyData2vnl_matrix(PolyData_Source, GMMTPS.ctrl_pts);

    //gmmreg.b_fix_affine = this->ui->CheckBox_Affined_GMMREG->isChecked();
    GMMTPS.level = this->ui->LineEdit_GMMLevel->text().toInt();
    GMMTPS.v_lambda.clear();
    for(int i = 0;i < 3; i++)
    {
        GMMTPS.v_lambda.push_back(this->ui->LineEdit_GMMLambda->text().toInt());
    }
    //run gmm_tps
    GMMTPS.run(this->ui->LineEdit_GMMMaxEvals->text().toInt());
    //get result
    vnl_matrix2vtkPolyData(PolyData_Source, GMMTPS.transformed_model);
    cout<<"\nTPS_L2 done\n" <<endl;

    finish = clock();
    TotalTime = (double)(finish - start);
    char TotalTime_s[15];
#ifdef _WIN32
    sprintf(TotalTime_s, "%10.1lfms", TotalTime);
#endif
#ifdef linux
    sprintf(TotalTime_s, "%10.1lfms", TotalTime / 1000);
#endif
    this->ui->LineEdit_GMMTime->setText(TotalTime_s);
    statusBar()->showMessage("GMMNonRigid Done", 2000);

    FlagReset();
    /* Visualization */
    this->slotRender();
}

void SimpleView::slotCPD()
{
    clock_t start, finish;
    double TotalTime;
    start = clock();

    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    if(!this->Flag_ConvexHull)
        this->slotConvexHull();



    cout<<"\nCPD Running..." <<endl;

    vtkPolyData2vnl_matrix(PolyData_Source, CPD.model);
    vtkPolyData2vnl_matrix(PolyData_Target, CPD.scene);

    vtkPolyData2vnl_matrix(CHM.ConvexHull_Source, CPD.ctrl_pts);

    CPD.EMtol = this->ui->LineEdit_CPDEmtol->text().toDouble();
    CPD.anneal = this->ui->LineEdit_CPDAnneal->text().toDouble();
    CPD.beta = this->ui->LineEdit_CPDBeta->text().toDouble();
    CPD.lambda = this->ui->LineEdit_CPDLambda->text().toDouble();
    CPD.outliers = this->ui->LineEdit_CPDOutliers->text().toInt();
    CPD.sigma = this->ui->LineEdit_CPDSigma->text().toDouble();
    CPD.tol = this->ui->LineEdit_CPDTol->text().toDouble();
    //gmmreg.max_iter = this->ui->LineEdit_CPDMaxIter->text().toInt();
    CPD.max_em_iter = this->ui->LineEdit_CPDMaxEMIter->text().toInt();

    CPD.run(this->ui->LineEdit_CPDMaxIter->text().toInt());
    //get result
    vnl_matrix2vtkPolyData(PolyData_Source, CPD.transformed_model);
    cout<<"\nCPD done\n" <<endl;

    finish = clock();
    TotalTime = (double)(finish - start);
    char TotalTime_s[30];
#ifdef _WIN32
    sprintf(TotalTime_s, "%10.1lfms", TotalTime);
#endif
#ifdef linux
    sprintf(TotalTime_s, "%10.1lfms", TotalTime / 1000);
#endif
    this->ui->LineEdit_CPDTime->setText(TotalTime_s);
    statusBar()->showMessage("CPD Done", 2000);

    FlagReset();
    /* Visualization */
    this->slotRender();
}


void SimpleView::slotStatUpdate()
{
    StatUpdate("../Data/");
}

void SimpleView::StatUpdate(char path[])
{
    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    if(!this->Flag_ConvexHull)
        this->slotConvexHull();

    /* Point Errors */
    char filename[100];
    sprintf(filename, "%sStat-%s-points.txt", path, ui->LineEdit_StatOutName->text().ascii());
    FILE *fp;
    fp = fopen(filename, "w");
    fprintf(fp, "\"id\" \"ClosestPointError\"\n");

    VTK_CREATE(vtkKdTree, kdtree);

    kdtree->BuildLocatorFromPoints(PolyData_Target->GetPoints());
    cout<<"KDTree Build Succes"<<endl;

    double sumerror = 0;
    double closestPointDist;
    for (int i = 0; i < PolyData_Source->GetNumberOfPoints(); i++)
    {
        kdtree->FindClosestPoint(PolyData_Source->GetPoint(i), closestPointDist);
        closestPointDist = sqrt(closestPointDist) / Max_Bound(PolyData_Source) * 100;
        fprintf(fp, "%4d\t%10.3lf\n", i, closestPointDist);
        sumerror += closestPointDist;
    }
    fclose(fp);
    char AvrError[15];
    sprintf(AvrError, "%.3lf%%", sumerror/PolyData_Source->GetNumberOfPoints());
    ui->LineEdit_StatClosetPointDistance->setText(AvrError);

    /* Triangle Errors */
    sprintf(filename, "%sStat-%s-triangles.txt",path, ui->LineEdit_StatOutName->text().ascii());
    fp = fopen(filename, "w");
    fprintf(fp, "\"id\" \"ClosestTriangleEucDist\" \"ClosestTriangleCHGMM\"\n");

    //kdtree->BuildLocatorFromPoints(CHM.ConvexHull_Target->GetPoints());
    //cout<<"KDTree Build Succes"<<endl;
    MeshGMM.Alpha = this->ui->LineEdit_CHGMMAlpha->text().toDouble();
    MeshGMM.Beta = this->ui->LineEdit_CHGMMBeta->text().toDouble();
    MeshGMM.Points_Source->DeepCopy(CHM.Points_Source);
    MeshGMM.Points_Target->DeepCopy(CHM.Points_Target);
    MeshGMM.Poly_Source->DeepCopy(CHM.Poly_Source);
    MeshGMM.Poly_Target->DeepCopy(CHM.Poly_Target);
    MeshGMM.ConvexHull_Source->DeepCopy(CHM.ConvexHull_Source);
    MeshGMM.ConvexHull_Target->DeepCopy(CHM.ConvexHull_Target);
    MeshGMM.Register(false);


    sumerror = 0;
    double triangleDist, vd, bestDist, CHGMM = 0;
    for (int i = 0; i < CHM.ConvexHull_Source->GetNumberOfCells(); i++)
    {
        bestDist = inf;
        for (int j = 0; j < CHM.ConvexHull_Target->GetNumberOfCells(); j++)
        {
            triangleDist = 0;
            for (int k = 0; k < CHM.ConvexHull_Source->GetCell(i)->GetNumberOfPoints(); k++)
            {
                vd = inf;
                for (int l = 0; l < CHM.ConvexHull_Target->GetCell(j)->GetNumberOfPoints(); l++)
                {
                    vd = std::min(vd, Euc_Dist(CHM.ConvexHull_Source->GetCell(i)->GetPoints()->GetPoint(k),CHM.ConvexHull_Target->GetCell(j)->GetPoints()->GetPoint(l)));
                }
                triangleDist += vd / Max_Bound(PolyData_Source) * 100;
            }
            bestDist = std::min(bestDist, triangleDist/3);
        }
        fprintf(fp, "%4d\t%10.3lf\t%10.3lf\n", i, bestDist, 100 * MeshGMM.CHGMML2[i])/ Max_Bound(PolyData_Source);
        sumerror += bestDist;
        CHGMM += 100 * MeshGMM.CHGMML2[i] / Max_Bound(PolyData_Source);
    }
    fclose(fp);
    sprintf(AvrError, "%.3lf%%", sumerror/CHM.ConvexHull_Source->GetNumberOfCells());
    ui->LineEdit_StatConvexHullDistance->setText(AvrError);
    sprintf(AvrError, "%.3lf", CHGMM/CHM.ConvexHull_Source->GetNumberOfCells());
    ui->LineEdit_StatCHGMML2->setText(AvrError);

    vtkPolyData2vnl_matrix(PolyData_Source, GMM.model);
    vtkPolyData2vnl_matrix(PolyData_Target, GMM.scene);

    sprintf(AvrError, "%.3lf", GaussTransform(GMM.model, GMM.scene, estimate_scale(GMM.model))/ Max_Bound(PolyData_Source) *1000);
    ui->LineEdit_StatGMML2->setText(AvrError);

    statusBar()->showMessage("Statistic Done", 2000);
}

void SimpleView::DistanceMap()
{
    if (!(Flag_Source && Flag_Target))
    {
        QMessageBox::warning(this, tr("WARNING"),tr("Please read file first!."));
        return;
    }
    VTK_CREATE(vtkCleanPolyData, clean1);
    clean1->SetInputConnection( PolyData_Target->GetProducerPort());
    VTK_CREATE(vtkCleanPolyData, clean2);
    clean2->SetInputConnection( PolyData_Source->GetProducerPort());

    DistanceFilter->SetInputConnection( 0, clean1->GetOutputPort() );
    DistanceFilter->SetInputConnection( 1, clean2->GetOutputPort() );
    //DistanceFilter->SetNegateDistance(1);
    DistanceFilter->SetSignedDistance(0);
    DistanceFilter->Update();
}

void SimpleView::slotTest()
{
    char SourcePath[100], TargetPath[100], ResultPath[100];
    char StatPath[100], OutputName[100], OutPutPath[100];
    int TestNum = ui->LineEdit_TestNum->text().toInt();

    FILE *fp;
    sprintf(StatPath, "%s/Statistic.txt", ui->LineEdit_TestPath->text().ascii());
    fp = fopen(StatPath, "w");

    sprintf(TargetPath, "%s/Target.ply", ui->LineEdit_TestPath->text().ascii());
    LoadModel(PolyData_Target, TargetPath);
    Flag_Target = true;
    for (int i = 0; i < TestNum; i++)
    {
        fprintf(fp, "Source%d:\n", i);
        fprintf(fp, "Method\tClosestPointError\tGMML2\tCovexHullError\tCHGMML2\tRunTime\n");
        sprintf(SourcePath, "%s/Source%d.ply", ui->LineEdit_TestPath->text().ascii(),i);
        sprintf(OutPutPath, "%s/", ui->LineEdit_TestPath->text().ascii());
        Flag_Source = true;

        LoadModel(PolyData_Source, SourcePath);
        FlagReset();
        slotICP();
        sprintf(OutputName, "Source%d-ICP", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        sprintf(ResultPath, "%s/Result%d-ICP.ply", ui->LineEdit_TestPath->text().ascii(),i);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "ICP\t%s\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii(), ui->LineEdit_ICPTime->text().ascii());
        cout << "ICP Done\n";

        LoadModel(PolyData_Source, SourcePath);
        FlagReset();
        slotGMMRigid();
        sprintf(OutputName, "Source%d-GMM", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        sprintf(ResultPath, "%s/Result%d-GMM.ply", ui->LineEdit_TestPath->text().ascii(),i);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "GMM\t%s\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii(), ui->LineEdit_GMMTime->text().ascii());
        cout << "GMM Done\n";

//        LoadModel(PolyData_Source, SourcePath);
//        FlagReset();
//        slotCPD();
//        sprintf(OutputName, "Source%d-CPD", i);
//        ui->LineEdit_StatOutName->setText(OutputName);
//        StatUpdate(OutPutPath);
//        sprintf(ResultPath, "%s/Result%d-CPD.ply", ui->LineEdit_TestPath->text().ascii(),i);
//        SaveModel(PolyData_Source, ResultPath);
//        fprintf(fp, "CPD\t%s\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
//                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
//                ui->LineEdit_StatCHGMML2->text().ascii(), ui->LineEdit_CPDTime->text().ascii());
//        cout << "CPD Done\n";

        LoadModel(PolyData_Source, SourcePath);
        FlagReset();
        slotCHM();
        sprintf(OutputName, "Source%d-CHM", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        sprintf(ResultPath, "%s/Result%d-CHM.ply", ui->LineEdit_TestPath->text().ascii(),i);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "CHM\t%s\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii(), ui->LineEdit_CHMTime->text().ascii());
        cout << "CHM Done\n";

        LoadModel(PolyData_Source, SourcePath);
        FlagReset();
        slotCHGMM();
        sprintf(OutputName, "Source%d-CHGMM", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        sprintf(ResultPath, "%s/Result%d-CHGMM.ply", ui->LineEdit_TestPath->text().ascii(),i);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "CHGMM\t%s\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii(), ui->LineEdit_CHGMMTime->text().ascii());
        cout << "CHGMM Done\n";

        LoadModel(PolyData_Source, SourcePath);
        if (PolyData_Source->GetNumberOfPoints() < 2000)
        {
            FlagReset();
            slotGMMNonRigid();
            sprintf(OutputName, "Source%d-GMMNonRigid", i);
            ui->LineEdit_StatOutName->setText(OutputName);
            StatUpdate(OutPutPath);
            sprintf(ResultPath, "%s/Result%d-GMMNonRigid.ply", ui->LineEdit_TestPath->text().ascii(),i);
            SaveModel(PolyData_Source, ResultPath);
            fprintf(fp, "GMMNonRigid\t%s\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                    ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                    ui->LineEdit_StatCHGMML2->text().ascii(), ui->LineEdit_GMMTime->text().ascii());
            cout << "GMMNonRigid Done\n";
        }
        //        LoadModel(PolyData_Source, SourcePath);
        //        FlagReset();
        //        slotCPD();
        //        sprintf(OutputName, "Source%d-CPDNonRigid", i);
        //        ui->LineEdit_StatOutName->setText(OutputName);
        //        StatUpdate(OutPutPath);
        //        sprintf(ResultPath, "%s/Result%d-CPDNonRigid.ply", ui->LineEdit_TestPath->text().ascii(),i);
        //        SaveModel(PolyData_Source, ResultPath);
        //        fprintf(fp, "CPDNonRigid\t%s\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
        //                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
        //                ui->LineEdit_StatCHGMML2->text().ascii(), ui->LineEdit_CPDTime->text().ascii());
        //        cout << "CPDNonRigid Done\n";

        LoadModel(PolyData_Source, SourcePath);
        FlagReset();
        slotCHM();
        slotCHGMMNonRigid();
        slotCHGMMNonRigid();
        sprintf(OutputName, "Source%d-CHGMMNonRigid", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        sprintf(ResultPath, "%s/Result%d-CHGMMNonRigid.ply", ui->LineEdit_TestPath->text().ascii(),i);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "CHGMMNonRigid\t%s\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii(), ui->LineEdit_CHGMMTime->text().ascii());
        cout << "CHGMMNonRigid Done\n";
    }
    fclose(fp);
}

void SimpleView::slotReTest()
{
    char SourcePath[100], TargetPath[100], ResultPath[100];
    char StatPath[100], OutputName[100], OutPutPath[100];
    int TestNum = ui->LineEdit_TestNum->text().toInt();

    FILE *fp;
    sprintf(StatPath, "%s/Statistic-Refine.txt", ui->LineEdit_TestPath->text().ascii());
    fp = fopen(StatPath, "w");

    sprintf(TargetPath, "%s/Target.ply", ui->LineEdit_TestPath->text().ascii());
    LoadModel(PolyData_Target, TargetPath);
    Flag_Target = true;
    for (int i = 0; i < TestNum; i++)
    {
        fprintf(fp, "Source%d:\n", i);
        fprintf(fp, "Method\tClosestPointError\tGMML2\tCovexHullError\tCHGMML2\n");
        sprintf(SourcePath, "%s/Source%d.ply", ui->LineEdit_TestPath->text().ascii(),i);
        sprintf(OutPutPath, "%s/", ui->LineEdit_TestPath->text().ascii());
        Flag_Source = true;

        sprintf(ResultPath, "%s/Result%d-ICP.ply", ui->LineEdit_TestPath->text().ascii(),i);
        LoadModel(PolyData_Source, ResultPath);
        FlagReset();
        sprintf(OutputName, "Source%d-ICP", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "ICP\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii());
        cout << "ICP Done\n";

        sprintf(ResultPath, "%s/Result%d-GMM.ply", ui->LineEdit_TestPath->text().ascii(),i);
        LoadModel(PolyData_Source, ResultPath);
        FlagReset();
        sprintf(OutputName, "Source%d-GMM", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "GMM\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii());
        cout << "GMM Done\n";

        sprintf(ResultPath, "%s/Result%d-CPD.ply", ui->LineEdit_TestPath->text().ascii(),i);
        LoadModel(PolyData_Source, ResultPath);
        FlagReset();
        sprintf(OutputName, "Source%d-CPD", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "CPD\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii());
        cout << "CPD Done\n";

        sprintf(ResultPath, "%s/Result%d-CHM.ply", ui->LineEdit_TestPath->text().ascii(),i);
        LoadModel(PolyData_Source, ResultPath);
        FlagReset();
        sprintf(OutputName, "Source%d-CHM", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "CHM\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii());
        cout << "CHM Done\n";

        sprintf(ResultPath, "%s/Result%d-CHGMM.ply", ui->LineEdit_TestPath->text().ascii(),i);
        LoadModel(PolyData_Source, ResultPath);
        FlagReset();
        sprintf(OutputName, "Source%d-CHGMM", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "CHGMM\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii());
        cout << "CHGMM Done\n";

        sprintf(ResultPath, "%s/Result%d-GMMNonRigid.ply", ui->LineEdit_TestPath->text().ascii(),i);
        LoadModel(PolyData_Source, ResultPath);
        FlagReset();
        sprintf(OutputName, "Source%d-GMMNonRigid", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "GMMNonRigid\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii());
        cout << "GMMNonRigid Done\n";

        //        sprintf(ResultPath, "%s/Result%d-CPDNonRigid.ply", ui->LineEdit_TestPath->text().ascii(),i);
        //        LoadModel(PolyData_Source, ResultPath);
        //        FlagReset();
        //        sprintf(OutputName, "Source%d-CPDNonRigid", i);
        //        ui->LineEdit_StatOutName->setText(OutputName);
        //        StatUpdate(OutPutPath);
        //        SaveModel(PolyData_Source, ResultPath);
        //        fprintf(fp, "CPDNonRigid\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
        //                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
        //                ui->LineEdit_StatCHGMML2->text().ascii());
        //        cout << "CPDNonRigid Done\n";

        sprintf(ResultPath, "%s/Result%d-CHGMMNonRigid.ply", ui->LineEdit_TestPath->text().ascii(),i);
        LoadModel(PolyData_Source, ResultPath);
        FlagReset();
        sprintf(OutputName, "Source%d-CHGMMNonRigid", i);
        ui->LineEdit_StatOutName->setText(OutputName);
        StatUpdate(OutPutPath);
        SaveModel(PolyData_Source, ResultPath);
        fprintf(fp, "CHGMMNonRigid\t%s\t%s\t%s\t%s\n", ui->LineEdit_StatClosetPointDistance->text().ascii(),
                ui->LineEdit_StatGMML2->text().ascii(), ui->LineEdit_StatConvexHullDistance->text().ascii(),
                ui->LineEdit_StatCHGMML2->text().ascii());
        cout << "CHGMMNonRigid Done\n";
    }
    fclose(fp);
}


void SimpleView::slotMeanModel()
{
    double mind, minid, pt[3];
    nofmean++;
    for (int i = 0; i < PolyData_Source->GetNumberOfPoints(); i++)
    {
        mind = inf;
        for (int j = 0; j < PolyData_Target->GetNumberOfPoints(); j++)
        {
            if (Euc_Dist(PolyData_Source->GetPoint(i), PolyData_Target->GetPoint(j)) < mind)
            {
                mind = Euc_Dist(PolyData_Source->GetPoint(i), PolyData_Target->GetPoint(j));
                minid = j;
            }
        }
        pt[0] = PolyData_Source->GetPoint(i)[0] * nofmean / (nofmean+1) + PolyData_Target->GetPoint(minid)[0] / (nofmean+1);
        pt[1] = PolyData_Source->GetPoint(i)[1] * nofmean / (nofmean+1) + PolyData_Target->GetPoint(minid)[1] / (nofmean+1);
        pt[2] = PolyData_Source->GetPoint(i)[2] * nofmean / (nofmean+1) + PolyData_Target->GetPoint(minid)[2] / (nofmean+1);
        PolyData_Source->GetPoints()->SetPoint(i, pt);
    }
    FlagReset();
    /* Visualization */
    this->slotRender();
}
