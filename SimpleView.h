/*=========================================================================
Copyright: Dec.5 2013
BIT OTISLAB
Medical Group
Jingfan Fan
=========================================================================*/

#ifndef SimpleView_H
#define SimpleView_H

/* Include VTK header files*/
#include <vtkCleanPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkAxesActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkPolyVertex.h>
#include <vtkPolyData.h>
#include <vtkDistancePolyDataFilter.h>
#include <vtkScalarBarActor.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkActor.h>
#include <vtkPlane.h>
#include <vtkLookupTable.h>
#include <vtkPlaneSource.h>
#include <vtkProperty.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPLYWriter.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkBMPWriter.h>
#include <vtkPolyDataNormals.h>
#include <vtkTexture.h>
#include <vtkTextureMapToPlane.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkMassProperties.h>

/* Include QT header files*/
#include <QFileDialog>
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QTextEdit>
#include <QTextCursor>
#include <QFileInfo>
#include <QTextDocumentWriter>
#include <QtGui>
#include <QMainWindow>

// This is mul/mbl/tests/test_thin_plate_spline_3d.cxx
#include <vcl_iostream.h>
#include <mbl/mbl_thin_plate_spline_3d.h>
#include <vnl/vnl_random.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_distance.h>
#include <testlib/testlib_test.h>

/* Include GMM_TPS head files*/
/////////////////////////////tps heads//////////////////////////////
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "gmmreg_tps.h"
#include "gmmreg_utils.h"
#include "gmmreg_base.h"
#include "gmmreg_cpd.h"
#include "gmmreg_grbf.h"
#include "gmmreg_rigid.h"
using std::cerr;
using std::cout;
using std::endl;
// Note that auto_ptr is made deprecated by the current standard C++11.
//typedef std::auto_ptr<gmmreg_tps_L2> gmmreg_tps_L2_Ptr;

/* Include CHM header files */
#include "CHM.h"
#include "ProjMatch.h"
#include "MeshGMM.h"

#define arc(x) (3.141592653*x/180)

/* Include C++ header files*/
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "ui_SimpleView.h"

// Forward Qt class declarations
class Ui_SimpleView;
class QTextEdit;

// Forward VTK class declarations
class vtkQtTableView;


class SimpleView : public QMainWindow
{
    Q_OBJECT

public:
    SimpleView();
    ~SimpleView();

    /* slots */
public slots:
    virtual void slotOpenSource();
    virtual void slotOpenTarget();
    virtual void slotSaveSource();
    virtual void slotSaveTarget();
    virtual void slotSaveMixed();
    virtual void slotSetSourcePath();
    virtual void slotSetTargetPath();
    virtual void slotTransform();
    virtual void slotICP();
    virtual void slotNormalize();
    virtual void slotConvexHull();
    virtual void slotRandomProj();
    virtual void slotProj();
    virtual void slotProjOneImg();
    virtual void slotProjMatch();
    virtual void slotShow3DMatches();
    virtual void slotProjRegister();
    virtual void slotCHM();
    virtual void slotCHGMM();
    virtual void slotCHGMMNonRigid();
    virtual void slotGMMRigid();
    virtual void slotGMMNonRigid();
    virtual void slotCPD();
    virtual void slotExit();
    virtual void slotRandomT();
    virtual void slotNoising();
    virtual void slotTPSNoising();
    virtual void slotRender();
    virtual void slotStatUpdate();
    virtual void slotTest();
    virtual void slotReTest();
    virtual void slotTestPath();
    virtual void slotMeanModel();

private:
    // Designer form
    Ui_SimpleView *ui;

    /* Variables */
    MeshGMM_Register MeshGMM;
    CHM_Register CHM;
    ProjMatch PM;
    gmmreg_rigid GMM;
    gmmreg_cpd_tps CPD;
    gmmreg_tps_L2 GMMTPS;

    vtkSmartPointer<vtkQtTableView> TableView;

    bool PS_in[10000], PT_in[10000];

    vtkSmartPointer<vtkPoints> GMPoints_Source;
    vtkSmartPointer<vtkPoints> GMPoints_Target;

    vtkSmartPointer<vtkPolyData> PolyData_Source;
    vtkSmartPointer<vtkPolyData> PolyData_Target;
    vtkSmartPointer<vtkPolyData> SparsePoints_Source;
    vtkSmartPointer<vtkPolyData> SparsePoints_Target;

    vtkSmartPointer<vtkDistancePolyDataFilter> DistanceFilter;
    vtkSmartPointer <vtkLookupTable> lut;

    vtkSmartPointer<vtkPolyDataMapper> PolyDataMapper_Source;
    vtkSmartPointer<vtkPolyDataMapper> PolyDataMapper_Target;
    vtkSmartPointer<vtkPolyDataMapper> SparsePointsMapper_Source;
    vtkSmartPointer<vtkPolyDataMapper> SparsePointsMapper_Target;
    vtkSmartPointer<vtkPolyDataMapper> PolyDataMapper_ConvexHullSource;
    vtkSmartPointer<vtkPolyDataMapper> PolyDataMapper_ConvexHullTarget;
    vtkSmartPointer<vtkPolyDataMapper> DistanceMapper;

    vtkSmartPointer<vtkActor> Actor_Source;
    vtkSmartPointer<vtkActor> Actor_Target;
    vtkSmartPointer<vtkActor> Actor_SparsePointsSource;
    vtkSmartPointer<vtkActor> Actor_SparsePointsTarget;
    vtkSmartPointer<vtkActor> Actor_ConvexHullSource;
    vtkSmartPointer<vtkActor> Actor_ConvexHullTarget;
    vtkSmartPointer<vtkActor> Actor_DistanceMap;

    vtkSmartPointer<vtkAxesActor> Actor_axes;
    vtkSmartPointer<vtkScalarBarActor> Actor_scalarBar;

    vtkSmartPointer<vtkRenderer> ren;

    bool Flag_Source, Flag_Target, Flag_ConvexHull, Flag_Proj, Flag_ProjMatch;

    int sizeofimg;
    int nofmean;

    /* Functions */
    void VTKVariablesInit();
    void QTVariablesInit();
    void DistanceMap();
    void FlagReset();
    void StatUpdate(char path[]);
    void LoadModel(vtkPolyData *PolyData, const char *path);
    void SaveModel(vtkPolyData *PolyData, const char *path);
    void PolyDataProject(vtkPolyData *PolyData, vtkPolyData *ProjectedPolyData, vtkPlaneSource *sourceplane);
    void PolyDataBackProject(vtkPolyData *ProjectedPolyData, vtkPolyData *PolyData, vtkPlaneSource *sourceplane, vector<bool> &p_in);
    void ProjOneFace(vtkPolyData *PolyData, vtkPolyData *PolyConvexHull, int Pid, char path[]);
};



#endif // SimpleView_H
