// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SoEventManager.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QShowEvent>
#include <QHideEvent>


#if !defined(FC_OS_MACOSX)
//# include <GL/gl.h>
//# include <GL/glu.h>
//# include <GL/glext.h>
#endif

#include "FdQuarterViewer.H"

static unsigned char fps2dfont[][12] = {
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, //
    {  0,  0, 12, 12,  0,  8, 12, 12, 12, 12, 12,  0 }, // !
    {  0,  0,  0,  0,  0,  0,  0,  0,  0, 20, 20, 20 }, // \"
    {  0,  0, 18, 18, 18, 63, 18, 18, 63, 18, 18,  0 }, // #
    {  0,  8, 28, 42, 10, 10, 12, 24, 40, 42, 28,  8 }, // $
    {  0,  0,  6, 73, 41, 22,  8, 52, 74, 73, 48,  0 }, // %
    {  0, 12, 18, 18, 12, 25, 37, 34, 34, 29,  0,  0 }, // &
    { 12, 12, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // '
    {  0,  6,  8,  8, 16, 16, 16, 16, 16,  8,  8,  6 }, // (
    {  0, 48,  8,  8,  4,  4,  4,  4,  4,  8,  8, 48 }, //)
    {  0,  0,  0,  0,  0,  0,  8, 42, 20, 42,  8,  0 }, // *
    {  0,  0,  0,  8,  8,  8,127,  8,  8,  8,  0,  0 }, // +
    {  0, 24, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0 }, // ,
    {  0,  0,  0,  0,  0,  0,127,  0,  0,  0,  0,  0 }, // -
    {  0,  0, 24, 24,  0,  0,  0,  0,  0,  0,  0,  0 }, // .
    {  0, 32, 32, 16, 16,  8,  8,  8,  4,  4,  2,  2 }, // /
    {  0,  0, 28, 34, 34, 34, 34, 34, 34, 34, 28,  0 }, // 0
    {  0,  0,  8,  8,  8,  8,  8,  8, 40, 24,  8,  0 }, // 1
    {  0,  0, 62, 32, 16,  8,  4,  2,  2, 34, 28,  0 }, // 2
    {  0,  0, 28, 34,  2,  2, 12,  2,  2, 34, 28,  0 }, // 3
    {  0,  0,  4,  4,  4,126, 68, 36, 20, 12,  4,  0 }, // 4
    {  0,  0, 28, 34,  2,  2,  2, 60, 32, 32, 62,  0 }, // 5
    {  0,  0, 28, 34, 34, 34, 60, 32, 32, 34, 28,  0 }, // 6
    {  0,  0, 16, 16, 16,  8,  8,  4,  2,  2, 62,  0 }, // 7
    {  0,  0, 28, 34, 34, 34, 28, 34, 34, 34, 28,  0 }, // 8
    {  0,  0, 28, 34,  2,  2, 30, 34, 34, 34, 28,  0 }, // 9
    {  0,  0, 24, 24,  0,  0,  0, 24, 24,  0,  0,  0 }, // :
    {  0, 48, 24, 24,  0,  0,  0, 24, 24,  0,  0,  0 }, // ;
    {  0,  0,  0,  2,  4,  8, 16,  8,  4,  2,  0,  0 }, // <
    {  0,  0,  0,  0,  0,127,  0,127,  0,  0,  0,  0 }, // =
    {  0,  0,  0, 16,  8,  4,  2,  4,  8, 16,  0,  0 }, // >
    {  0,  0, 16, 16,  0, 16, 28,  2,  2,  2, 60,  0 }, // ?
    {  0,  0, 28, 32, 73, 86, 82, 82, 78, 34, 28,  0 }, // @
    {  0,  0, 33, 33, 33, 63, 18, 18, 18, 12, 12,  0 }, // A
    {  0,  0, 60, 34, 34, 34, 60, 34, 34, 34, 60,  0 }, // B
    {  0,  0, 14, 16, 32, 32, 32, 32, 32, 18, 14,  0 }, // C
    {  0,  0, 56, 36, 34, 34, 34, 34, 34, 36, 56,  0 }, // D
    {  0,  0, 62, 32, 32, 32, 60, 32, 32, 32, 62,  0 }, // E
    {  0,  0, 16, 16, 16, 16, 30, 16, 16, 16, 30,  0 }, // F
    {  0,  0, 14, 18, 34, 34, 32, 32, 32, 18, 14,  0 }, // G
    {  0,  0, 34, 34, 34, 34, 62, 34, 34, 34, 34,  0 }, // H
    {  0,  0, 62,  8,  8,  8,  8,  8,  8,  8, 62,  0 }, // I
    {  0,  0,112,  8,  8,  8,  8,  8,  8,  8, 62,  0 }, // J
    {  0,  0, 33, 33, 34, 36, 56, 40, 36, 34, 33,  0 }, // K
    {  0,  0, 30, 16, 16, 16, 16, 16, 16, 16, 16,  0 }, // L
    {  0,  0, 33, 33, 33, 45, 45, 45, 51, 51, 33,  0 }, // M
    {  0,  0, 34, 34, 38, 38, 42, 42, 50, 50, 34,  0 }, // N
    {  0,  0, 12, 18, 33, 33, 33, 33, 33, 18, 12,  0 }, // O
    {  0,  0, 32, 32, 32, 60, 34, 34, 34, 34, 60,  0 }, // P
    {  3,  6, 12, 18, 33, 33, 33, 33, 33, 18, 12,  0 }, // Q
    {  0,  0, 34, 34, 34, 36, 60, 34, 34, 34, 60,  0 }, // R
    {  0,  0, 60,  2,  2,  6, 28, 48, 32, 32, 30,  0 }, // S
    {  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,127,  0 }, // T
    {  0,  0, 28, 34, 34, 34, 34, 34, 34, 34, 34,  0 }, // U
    {  0,  0, 12, 12, 18, 18, 18, 33, 33, 33, 33,  0 }, // V
    {  0,  0, 34, 34, 34, 54, 85, 73, 73, 73, 65,  0 }, // W
    {  0,  0, 34, 34, 20, 20,  8, 20, 20, 34, 34,  0 }, // X
    {  0,  0,  8,  8,  8,  8, 20, 20, 34, 34, 34,  0 }, // Y
    {  0,  0, 62, 32, 16, 16,  8,  4,  4,  2, 62,  0 }, // Z
    {  0, 14,  8,  8,  8,  8,  8,  8,  8,  8,  8, 14 }, // [
    {  0,  2,  2,  4,  4,  8,  8,  8, 16, 16, 32, 32 }, // [backslash]
    {  0, 56,  8,  8,  8,  8,  8,  8,  8,  8,  8, 56 }, // ]
    {  0,  0,  0,  0,  0, 34, 34, 20, 20,  8,  8,  0 }, // ^
    {  0,127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // _
    {  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 24, 12 }, // `
    {  0,  0, 29, 34, 34, 30,  2, 34, 28,  0,  0,  0 }, // a
    {  0,  0, 60, 34, 34, 34, 34, 50, 44, 32, 32, 32 }, // b
    {  0,  0, 14, 16, 32, 32, 32, 16, 14,  0,  0,  0 }, // c
    {  0,  0, 26, 38, 34, 34, 34, 34, 30,  2,  2,  2 }, // d
    {  0,  0, 28, 34, 32, 62, 34, 34, 28,  0,  0,  0 }, // e
    {  0,  0, 16, 16, 16, 16, 16, 16, 62, 16, 16, 14 }, // f
    { 28,  2,  2, 26, 38, 34, 34, 34, 30,  0,  0,  0 }, // g
    {  0,  0, 34, 34, 34, 34, 34, 50, 44, 32, 32, 32 }, // h
    {  0,  0,  8,  8,  8,  8,  8,  8, 56,  0,  8,  8 }, // i
    { 56,  4,  4,  4,  4,  4,  4,  4, 60,  0,  4,  4 }, // j
    {  0,  0, 33, 34, 36, 56, 40, 36, 34, 32, 32, 32 }, // k
    {  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8, 56 }, // l
    {  0,  0, 73, 73, 73, 73, 73,109, 82,  0,  0,  0 }, // m
    {  0,  0, 34, 34, 34, 34, 34, 50, 44,  0,  0,  0 }, // n
    {  0,  0, 28, 34, 34, 34, 34, 34, 28,  0,  0,  0 }, // o
    { 32, 32, 60, 34, 34, 34, 34, 50, 44,  0,  0,  0 }, // p
    {  2,  2, 26, 38, 34, 34, 34, 34, 30,  0,  0,  0 }, // q
    {  0,  0, 16, 16, 16, 16, 16, 24, 22,  0,  0,  0 }, // r
    {  0,  0, 60,  2,  2, 28, 32, 32, 30,  0,  0,  0 }, // s
    {  0,  0, 14, 16, 16, 16, 16, 16, 62, 16, 16,  0 }, // t
    {  0,  0, 26, 38, 34, 34, 34, 34, 34,  0,  0,  0 }, // u
    {  0,  0,  8,  8, 20, 20, 34, 34, 34,  0,  0,  0 }, // v
    {  0,  0, 34, 34, 34, 85, 73, 73, 65,  0,  0,  0 }, // w
    {  0,  0, 34, 34, 20,  8, 20, 34, 34,  0,  0,  0 }, // x
    { 48, 16,  8,  8, 20, 20, 34, 34, 34,  0,  0,  0 }, // y
    {  0,  0, 62, 32, 16,  8,  4,  2, 62,  0,  0,  0 }, // z
    {  0,  6,  8,  8,  8,  4, 24,  4,  8,  8,  8,  6 }, // {
    {  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8 }, // |
    {  0, 48,  8,  8,  8, 16, 12, 16,  8,  8,  8, 48 }, // }
    {  0,  0,  0,  0,  0,  0, 78, 57,  0,  0,  0,  0 }  // ~
};

const SbVec2s defaultSize(520, 360);

FdQuarterViewer::FdQuarterViewer(QWidget* parent,
                                 const QOpenGLWidget* sharewidget,
                                 Qt::WindowFlags flags)
  : SIM::Coin3D::Quarter::QuarterWidget(parent, sharewidget, flags)
  , matrixaction(SbViewportRegion(defaultSize[0], defaultSize[1]))
{
  init();
}

FdQuarterViewer::FdQuarterViewer(const QSurfaceFormat& format,
                                 QWidget* parent,
                                 const QOpenGLWidget* shareWidget,
                                 Qt::WindowFlags flags)
  : SIM::Coin3D::Quarter::QuarterWidget(format, parent, shareWidget, flags)
  , matrixaction(SbViewportRegion(defaultSize[0], defaultSize[1]))
{
  init();
}

FdQuarterViewer::FdQuarterViewer(QOpenGLContext* context,
                                 QWidget* parent,
                                 const QOpenGLWidget* sharewidget,
                                 Qt::WindowFlags flags)
  : SIM::Coin3D::Quarter::QuarterWidget(context, parent, sharewidget, flags)
  , matrixaction(SbViewportRegion(defaultSize[0], defaultSize[1]))
{
  init();
}

FdQuarterViewer::FdQuarterViewer(QWidget* parent,
                                 const char* name,
                                 SbBool  embed,
                                 FdQuarterViewer::Type,
                                 SbBool  build)
  : SIM::Coin3D::Quarter::QuarterWidget(parent)
  , matrixaction(SbViewportRegion(defaultSize[0], defaultSize[1]))
{
  init();
}

FdQuarterViewer::~FdQuarterViewer()
{
    delete seeksensor;
}

void
FdQuarterViewer::setTitle(const char * const title)
{
  QWidget* toplevel = static_cast<QWidget*>(this);
#if QT_VERSION >= 0x060100
  while (!toplevel->isWindow()) {
#else
  while (!toplevel->isTopLevel()) {
#endif
    toplevel = toplevel->parentWidget();
  }
  if (toplevel) {
#if QT_VERSION >= 0x040000
    toplevel->setWindowTitle(title);
#else
    toplevel->setCaption(title);
#endif
  }
}

void
FdQuarterViewer::redrawOnSelectionChange(SoSelection *selection)
{
  if (this->selection) {
    this->selection->removeChangeCallback(FdQuarterViewer::selection_redraw_cb,
                                          this);
    this->selection->unref();
  }
  this->selection = selection;
  if (selection) {
    selection->addChangeCallback(FdQuarterViewer::selection_redraw_cb,
                                 this);
  }
}

void
FdQuarterViewer::setCamera(SoCamera * cam)
{
  SoCamera* camera = getSoRenderManager()->getCamera();
  if ( camera )
  {
    SoSeparator* root = (SoSeparator*)getSceneGraph();
    int idx = root->findChild(camera);
    if (idx >= 0) {
      root->removeChild(idx);
    }
    camera->unref();
  }

  if ( cam )
  {
    cam->ref();
  }
  
  getSoRenderManager()->setCamera(cam);
}

void
FdQuarterViewer::toggleCameraType()
{
  SoCamera* camera = getCamera();
  if (!camera)
    return;
  if (camera->isOfType(SoOrthographicCamera::getClassTypeId()))
    setCameraType(SoPerspectiveCamera::getClassTypeId());
  else
    setCameraType(SoOrthographicCamera::getClassTypeId());
}

void
FdQuarterViewer::setCameraType(SoType type)
{
  SoCamera* cam = getSoRenderManager()->getCamera();
  if (cam && !cam->isOfType(SoPerspectiveCamera::getClassTypeId()) &&
      !cam->isOfType(SoOrthographicCamera::getClassTypeId())) {
      // TODO: Log error
      return;
  }

  SoType perspectivetype = SoPerspectiveCamera::getClassTypeId();
  SbBool oldisperspective = cam ? cam->getTypeId().isDerivedFrom(perspectivetype) : false;
  SbBool newisperspective = type.isDerivedFrom(perspectivetype);

  // Same old, same old..
  if (oldisperspective == newisperspective) {
    return;
  }

  SoCamera* currentcam = getSoRenderManager()->getCamera();
  SoCamera* newcamera = static_cast<SoCamera*>(type.createInstance());

  // Transfer and convert values from one camera type to the other.
  if(newisperspective) {
    convertOrtho2Perspective(dynamic_cast<SoOrthographicCamera*>(currentcam),
                             dynamic_cast<SoPerspectiveCamera*>(newcamera));
  }
  else {
    convertPerspective2Ortho(dynamic_cast<SoPerspectiveCamera*>(currentcam),
                             dynamic_cast<SoOrthographicCamera*>(newcamera));
  }

  getSoRenderManager()->setCamera(newcamera);
  getSoEventManager()->setCamera(newcamera);

  //if the superscene has a camera we need to replace it too
  auto superscene = dynamic_cast<SoSeparator*>(getSoRenderManager()->getSceneGraph());
  SoSearchAction sa;
  sa.setInterest(SoSearchAction::FIRST);
  sa.setType(SoCamera::getClassTypeId());
  sa.apply(superscene);

  if (sa.getPath()) {
    SoNode* node = sa.getPath()->getTail();
    SoGroup* parent = static_cast<SoGroup*>(sa.getPath()->getNodeFromTail(1));

    if (node && node->isOfType(SoCamera::getClassTypeId())) {
      parent->replaceChild(node, newcamera);
    }
  }
}

void
FdQuarterViewer::setViewing(bool enable)
{
  viewingflag = enable;

  // Turn off the selection indicators when we go back from picking
  // mode into viewing mode.
  if (viewingflag) {
    SoGLRenderAction* action = getSoRenderManager()->getGLRenderAction();

    if (action) {
      SoLocateHighlight::turnOffCurrentHighlight(action);
    }
  }
}

void
FdQuarterViewer::interactiveCountInc()
{
  // Catch problems with missing interactiveCountDec() calls.
  assert(interactionnesting < 100);
  
  if (++interactionnesting == 1) {
    interactionStartCallback.invokeCallbacks(this);
  }
}

void
FdQuarterViewer::interactiveCountDec()
{
  if (--interactionnesting <= 0) {
    interactionEndCallback.invokeCallbacks(this);
    interactionnesting = 0;
  }
}

void
FdQuarterViewer::setSeekMode(bool enable)
{
  if(!enable && seeksensor->isScheduled()) {
    seeksensor->unschedule();
    interactiveCountDec();
  }
  
  inseekmode = enable;
}

bool
FdQuarterViewer::seekToPoint(const SbVec2s& screenpos)
{
  
  SoRayPickAction rpaction(getSoRenderManager()->getViewportRegion());
  rpaction.setPoint(screenpos);
  rpaction.setRadius(pickRadius);
  rpaction.apply(getSoRenderManager()->getSceneGraph());
  
  SoPickedPoint* picked = rpaction.getPickedPoint();
  
  if (!picked) {
    this->interactiveCountInc(); // decremented in setSeekMode(false)
    this->setSeekMode(false);
    return false;
  }
  
  SbVec3f hitpoint;
  hitpoint = picked->getPoint();
  
  this->seekToPoint(hitpoint);
  return true;
}

void
FdQuarterViewer::seekToPoint(const SbVec3f& scenepos)
{
  SbVec3f hitpoint(scenepos);
  
  camerastartposition = getSoRenderManager()->getCamera()->position.getValue();
  camerastartorient = getSoRenderManager()->getCamera()->orientation.getValue();
  
  // move point to the camera coordinate system, consider
  // transformations before camera in the scene graph
  SbMatrix cameramatrix;
  SbMatrix camerainverse;
  getCameraCoordinateSystem(getSoRenderManager()->getCamera(),
                            getSceneGraph(),
                            cameramatrix,
                            camerainverse);
  camerainverse.multVecMatrix(hitpoint, hitpoint);
  
  float fd = seekdistance;
  
  if(!seekdistanceabs) {
    fd *= (hitpoint - getSoRenderManager()->getCamera()->position.getValue()).length()/100.0F;
  }
  
  getSoRenderManager()->getCamera()->focalDistance = fd;
  
  SbVec3f dir = hitpoint - camerastartposition;
  dir.normalize();
  
  // find a rotation that rotates current camera direction into new
  // camera direction.
  SbVec3f olddir;
  getSoRenderManager()->getCamera()->orientation.getValue().multVec(SbVec3f(0, 0, -1), olddir);
  SbRotation diffrot(olddir, dir);
  cameraendposition = hitpoint - fd * dir;
  cameraendorient = getSoRenderManager()->getCamera()->orientation.getValue() * diffrot;
  
  if(seeksensor->isScheduled()) {
    seeksensor->unschedule();
    interactiveCountDec();
  }
  
  seeksensor->setBaseTime(SbTime::getTimeOfDay());
  seeksensor->schedule();
  interactiveCountInc();
}

void
FdQuarterViewer::setSeekTime(const float seconds)
{
  seekperiod = seconds;
}

void
FdQuarterViewer::setSeekDistance(const float distance)
{
  seekdistance = distance;
}

void
FdQuarterViewer::setSeekValueAsPercentage(bool on)
{
  seekdistanceabs = !on;
}

void FdQuarterViewer::setPickRadius(float pickRadius)
{
  this->pickRadius = pickRadius;
  SoEventManager* evm = this->getSoEventManager();
  if (evm){
    SoHandleEventAction* hea = evm->getHandleEventAction();
    if (hea){
      hea->setPickRadius(pickRadius);
    }
  }
}

void
FdQuarterViewer::saveHomePosition()
{
    SoCamera* cam = getSoRenderManager()->getCamera();
    if (!cam) {
        return;
    }

    SoType type = cam->getTypeId();
    assert(type.isDerivedFrom(SoNode::getClassTypeId()));
    assert(type.canCreateInstance());

    if(storedcamera) {
        storedcamera->unref();
    }

    storedcamera = static_cast<SoNode*>(type.createInstance());
    storedcamera->ref();

    storedcamera->copyFieldValues(getSoRenderManager()->getCamera());
}

void
FdQuarterViewer::resetToHomePosition()
{
  SoCamera* cam = getSoRenderManager()->getCamera();
  if (!cam) {
    return;
  }
  
  if(!storedcamera) {
    return;
  }
  
  SoType ttype = getSoRenderManager()->getCamera()->getTypeId();
  SoType stype = storedcamera->getTypeId();
  
  // most common case
  if (ttype == stype) {
    // We copy the field data directly, instead of using
    // SoFieldContainer::copyContents(), for the reason described in
    // detail in So@Gui@Viewer::saveHomePosition().
    getSoRenderManager()->getCamera()->copyFieldValues(storedcamera);
  }
  // handle common case #1
  else if(ttype == SoOrthographicCamera::getClassTypeId() &&
    stype == SoPerspectiveCamera::getClassTypeId()) {
    convertPerspective2Ortho(dynamic_cast<SoPerspectiveCamera*>(storedcamera),
                             dynamic_cast<SoOrthographicCamera*>(getSoRenderManager()->getCamera()));
    }
    // handle common case #2
    else if(ttype == SoPerspectiveCamera::getClassTypeId() &&
      stype == SoOrthographicCamera::getClassTypeId()) {
      convertOrtho2Perspective(dynamic_cast<SoOrthographicCamera*>(storedcamera),
                               dynamic_cast<SoPerspectiveCamera*>(getSoRenderManager()->getCamera()));
      }
      
      // otherwise, cameras have changed in ways we don't understand since
      // the last saveHomePosition() invocation, and so we're just going
      // to ignore the reset request
}

void 
FdQuarterViewer::setAutoClippingStrategy(const AutoClippingStrategy strategy,
                                         const float value,
                                         FdQuarterAutoClippingCB * cb,
                                         void * cbuserdata)
{
  autoclipstrategy = strategy;
  autoclipvalue = value;
  autoclipcb = cb;
  autoclipuserdata = cbuserdata;

  if (autoclipstrategy == VARIABLE_NEAR_PLANE) {
    // normalize the value so that the near plane isn't too near or
    // too far from the projection point.  FIXME: calibrate this
    // normalization, pederb, 2002-04-25
    float v = (std::max)((std::min)(value, 1.f), 0.f); // just in case
    v *= 0.8f;
    v += 0.1f; // v will be in range [0.1, 0.9]

    autoclipvalue = v;
  }
  if (adjustclipplanes) {
    scheduleRedraw();
  }
}

void
FdQuarterViewer::addVisibilityChangeCallback(FdQuarterVisibilityCB * const func,
                                             void * const user)
{
  if (! visibilitychangeCBs)
    visibilitychangeCBs = new SbPList;

  visibilitychangeCBs->append((void *) func);
  visibilitychangeCBs->append(user);
}

void
FdQuarterViewer::removeVisibilityChangeCallback(FdQuarterVisibilityCB * const func,
                                                void * const data)
{
  int idx = visibilitychangeCBs->find((void *) func);
  if (idx != -1) {
    visibilitychangeCBs->remove(idx);
    visibilitychangeCBs->remove(idx);
  }

#if SOQT_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoQtComponent::removeVisibilityChangeCallback",
                              "tried to remove non-existent callback");
    return;
  }
#endif // SOQT_DEBUG
}

bool
FdQuarterViewer::processSoEvent(const SoEvent* event)
{
  const SoType type(event->getTypeId());
  
  constexpr const float delta = 0.1F;
  if(type.isDerivedFrom(SoKeyboardEvent::getClassTypeId())) {
    const SoKeyboardEvent* keyevent = static_cast<const SoKeyboardEvent*>(event);
    
    if(keyevent->getState() == SoButtonEvent::DOWN) {
      switch(keyevent->getKey()) {
        case SoKeyboardEvent::LEFT_ARROW:
          moveCameraScreen(SbVec2f(-delta, 0.0F));
          return true;
        case SoKeyboardEvent::UP_ARROW:
          moveCameraScreen(SbVec2f(0.0F, delta));
          return true;
        case SoKeyboardEvent::RIGHT_ARROW:
          moveCameraScreen(SbVec2f(delta, 0.0F));
          return true;
        case SoKeyboardEvent::DOWN_ARROW:
          moveCameraScreen(SbVec2f(0.0F, -delta));
          return true;
        default:
          break;
      }
    }
  }
  
  return SIM::Coin3D::Quarter::QuarterWidget::processSoEvent(event);
}

void
FdQuarterViewer::paintEvent(QPaintEvent* event)
{
  double start = SbTime::getTimeOfDay().getValue();
  SIM::Coin3D::Quarter::QuarterWidget::paintEvent(event);
  this->framesPerSecond = addFrametime(start);
}

bool
FdQuarterViewer::event(QEvent *e)
{
  // We call to the inherited processEvent, which is eventually redirecting us
  // control on our own processEvent()
  eventHandeled = TRUE;
  processEvent(e);

  if (eventHandeled) {
    if ((e->type() == QEvent::KeyPress) || (e->type() == QEvent::KeyRelease)) {
      QKeyEvent * ke = (QKeyEvent *)e;
      ke->accept();
    }
    return true;
  }

  if ((e->type() == QEvent::Show || e->type() == QEvent::Hide)) {
    if (visibilitychangeCBs) {
      for (int i=0; i < visibilitychangeCBs->getLength() / 2; i++) {
        SoQtComponentVisibilityCB * cb =
          (SoQtComponentVisibilityCB *)(*(visibilitychangeCBs))[i * 2];
        void * userdata = (*(visibilitychangeCBs))[i * 2 + 1];
        cb(userdata, e->type() == QEvent::Show ? true : false);
      }
    }
  }

  return SIM::Coin3D::Quarter::QuarterWidget::event(e);
}

void
FdQuarterViewer::processEvent(QEvent* e)
{
  // The inherited event gave us control, so the event was not handled by it
  eventHandeled = FALSE;
}

void
FdQuarterViewer::init()
{
  interactionnesting = 0;
  seekdistance = 50.0F;
  seekdistanceabs = false;
  seekperiod = 2.0F;
  inseekmode = false;
  storedcamera = nullptr;
  viewingflag = false;
  pickRadius = 5.0;

  seeksensor = new SoTimerSensor(FdQuarterViewer::seeksensorCB, (void*)this);
  getSoEventManager()->setNavigationState(SoEventManager::NO_NAVIGATION);

  resetFrameCounter();
}

void
FdQuarterViewer::convertOrtho2Perspective(const SoOrthographicCamera* in,
                                          SoPerspectiveCamera* out)
{
  if (!in || !out) {
    // TODO: Log error
    return;
  }
  out->aspectRatio.setValue(in->aspectRatio.getValue());
  out->focalDistance.setValue(in->focalDistance.getValue());
  out->orientation.setValue(in->orientation.getValue());
  out->position.setValue(in->position.getValue());
  out->viewportMapping.setValue(in->viewportMapping.getValue());

  SbRotation camrot = in->orientation.getValue();

  float focaldist = float(in->height.getValue() / (2.0*tan(M_PI / 8.0)));

  SbVec3f offset(0,0,focaldist-in->focalDistance.getValue());

  camrot.multVec(offset,offset);
  out->position.setValue(offset+in->position.getValue());

  out->focalDistance.setValue(focaldist);

  // 45° is the default value of this field in SoPerspectiveCamera.
  out->heightAngle = (float)(M_PI / 4.0);
}

void
FdQuarterViewer::convertPerspective2Ortho(const SoPerspectiveCamera* in,
                                          SoOrthographicCamera* out)
{
  out->aspectRatio.setValue(in->aspectRatio.getValue());
  out->focalDistance.setValue(in->focalDistance.getValue());
  out->orientation.setValue(in->orientation.getValue());
  out->position.setValue(in->position.getValue());
  out->viewportMapping.setValue(in->viewportMapping.getValue());

  float focaldist = in->focalDistance.getValue();

  out->height = 2.0F * focaldist * (float)tan(in->heightAngle.getValue() / 2.0);
}

void
FdQuarterViewer::getCameraCoordinateSystem(SoCamera* camera,
                                           SoNode* root,
                                           SbMatrix& matrix,
                                           SbMatrix& inverse)
{
  searchaction.reset();
  searchaction.setSearchingAll(true);
  searchaction.setInterest(SoSearchAction::FIRST);
  searchaction.setNode(camera);
  searchaction.apply(root);
  
  matrix = inverse = SbMatrix::identity();
  
  if(searchaction.getPath()) {
    matrixaction.apply(searchaction.getPath());
    matrix = matrixaction.getMatrix();
    inverse = matrixaction.getInverse();
  }
  
  searchaction.reset();
}

void
FdQuarterViewer::seeksensorCB(void* data, SoSensor* sensor)
{
  FdQuarterViewer* thisp = static_cast<FdQuarterViewer*>(data);
  SbTime currenttime = SbTime::getTimeOfDay();
  
  SoTimerSensor* timer = static_cast<SoTimerSensor*>(sensor);
  
  float par = float((currenttime - timer->getBaseTime()).getValue()) / thisp->seekperiod;
  
  if ((par > 1.0F) || (par + timer->getInterval().getValue() > 1.0F)) {
    par = 1.0F;
  }
  
  bool end = (par == 1.0F);
  
  par = (float)((1.0 - cos(M_PI * par)) * 0.5);
  
  thisp->getSoRenderManager()->getCamera()->position = thisp->camerastartposition +
  (thisp->cameraendposition - thisp->camerastartposition) * par;
  thisp->getSoRenderManager()->getCamera()->orientation =
  SbRotation::slerp(thisp->camerastartorient,
                    thisp->cameraendorient,
                    par);
  
  if (end) {
    thisp->setSeekMode(false);
  }
}

void
FdQuarterViewer::moveCameraScreen(const SbVec2f& screenpos)
{
  SoCamera* cam = getSoRenderManager()->getCamera();
  assert(cam);
  
  
  SbViewVolume vv = cam->getViewVolume(getGLWidget()->width() / getGLWidget()->height());
  SbPlane panplane = vv.getPlane(cam->focalDistance.getValue());
  
  constexpr const float mid = 0.5F;
  SbLine line;
  vv.projectPointToLine(screenpos + SbVec2f(mid, mid), line);
  SbVec3f current_planept;
  panplane.intersect(line, current_planept);
  vv.projectPointToLine(SbVec2f(mid, mid), line);
  SbVec3f old_planept;
  panplane.intersect(line, old_planept);
  
  // Reposition camera according to the vector difference between the
  // projected points.
  cam->position = cam->position.getValue() - (current_planept - old_planept);
}

void
FdQuarterViewer::resetFrameCounter()
{
  this->framecount = 0;
  this->frametime = 0.0F;
  this->drawtime = 0.0F;
  this->starttime = SbTime::getTimeOfDay().getValue();
  this->framesPerSecond = SbVec2f(0, 0);
}

SbVec2f
FdQuarterViewer::addFrametime(double starttime)
{
  constexpr const double FPS_FACTOR = 0.7;
  constexpr const double FIVE_SECS = 5000.0;
  constexpr const float ONE_SEC = 1000.0F;

  this->framecount++;

  double timeofday = SbTime::getTimeOfDay().getValue();

  // draw time is the actual time spent on rendering
  double drawtime = timeofday - starttime;
  this->drawtime = (drawtime*FPS_FACTOR) + this->drawtime*(1.0 - FPS_FACTOR);

  // frame time is the time spent since the last frame. There could an
  // indefinite pause between the last frame because the scene is not
  // changing. So we limit the skew to 5 second.
  double frametime = std::min(timeofday-this->starttime, std::max(drawtime, FIVE_SECS));
  this->frametime = (frametime*FPS_FACTOR) + this->frametime*(1.0 - FPS_FACTOR);

  this->starttime = timeofday;
  return {ONE_SEC * float(this->drawtime), 1.0F / float(this->frametime)};
}

// Position the near and far clipping planes just in front of and
// behind the scene's bounding box. This will give us the optimal
// utilization of the z buffer resolution by shrinking it to its
// minimum depth.
//
// Near and far clipping planes are specified in the camera fields
// nearDistance and farDistance.
void
FdQuarterViewer::setClippingPlanes()
{
  // This is necessary to avoid a crash in case there is no scene
  // graph specified by the user.
  SoCamera* camera = getSoRenderManager()->getCamera();
  if (camera == NULL) return;
  
  if (autoclipbboxaction == NULL)
    autoclipbboxaction =
    new SoGetBoundingBoxAction(getViewportRegion());
  else
    autoclipbboxaction->setViewportRegion(getViewportRegion());
  
  autoclipbboxaction->apply(getSceneGraph());
  
  SbXfBox3f xbox = autoclipbboxaction->getXfBoundingBox();
  
  // Bounding box was calculated in camera space, so we need to "flip"
  // the box (because camera is pointing in the (0,0,-1) direction
  // from origo.
  float nearval = camera->nearDistance.getValue();
  float farval = camera->farDistance.getValue();
  if ( !xbox.isEmpty() ) {
    SbMatrix cammat;
    SbMatrix inverse;
    getCameraCoordinateSystem(camera, getSceneGraph(), cammat, inverse);
    xbox.transform(inverse);
    
    SbMatrix mat;
    mat.setTranslate(- camera->position.getValue());
    xbox.transform(mat);
    mat = camera->orientation.getValue().inverse();
    xbox.transform(mat);
    SbBox3f box = xbox.project();
    
    nearval = -box.getMax()[2];
    farval = -box.getMin()[2];
  }
  
  // FIXME: what if we have a weird scale transform in the scene graph?
  // Could we end up with nearval > farval then? Investigate, then
  // either use an assert() (if it can't happen) or an So@Gui@Swap()
  // (to handle it). 20020116 mortene.
  
  // Check if scene is completely behind us.
  // Do NOT check for orthographic cameras.
  if (farval <= 0.0f && !camera->isOfType(SoOrthographicCamera::getClassTypeId())) { return; }
  
  if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
    // Disallow negative and small near clipping plane distance.
    
    float nearlimit; // the smallest value allowed for nearval
    if (autoclipstrategy == FdQuarterViewer::CONSTANT_NEAR_PLANE) {
      nearlimit = autoclipvalue;
    }
    else {
      assert(autoclipstrategy == FdQuarterViewer::VARIABLE_NEAR_PLANE);
      // From glFrustum() documentation: Depth-buffer precision is
      // affected by the values specified for znear and zfar. The
      // greater the ratio of zfar to znear is, the less effective the
      // depth buffer will be at distinguishing between surfaces that
      // are near each other. If r = far/near, roughly log (2) r bits
      // of depth buffer precision are lost. Because r approaches
      // infinity as znear approaches zero, you should never set znear
      // to zero.
      
      GLint depthbits[1];
      glGetIntegerv(GL_DEPTH_BITS, depthbits);
      
      int use_bits = (int) (float(depthbits[0]) * (1.0f - autoclipvalue)); 
      float r = (float) pow(2.0, (double) use_bits);
      nearlimit = farval / r;
    }
    
    if (nearlimit >= farval) {
      // (The "5000" magic constant was found by fiddling around a bit
      // on an OpenGL implementation with a 16-bit depth-buffer
      // resolution, adjusting to find something that would work well
      // with both a very "stretched" / deep scene and a more compact
      // single-model one.)
      nearlimit = farval / 5000.0f;
    }
    
    // adjust the near plane if the the value is too small.
    if (nearval < nearlimit) { nearval = nearlimit; }
  }
  
  // Some slack around the bounding box, in case the scene fits
  // exactly inside it. This is done to minimize the chance of
  // artifacts caused by the limitation of the z-buffer
  // resolution. One common artifact if this is not done is that the
  // near clipping plane cuts into the corners of the model as it's
  // rotated.
  const float SLACK = 0.001f;
  
  // FrustumCamera can be found in the SmallChange CVS module. We
  // should not change the nearDistance for this camera, as this will
  // modify the frustum.
  //
  // FIXME: quite the hack that So@Gui@ needs to know about the
  // FrustumCamera class. Wouldn't it be better if FrustumCamera
  // instead registered a callback with setAutoClippingStrategy() and
  // handled this itself?  20040908 mortene.
  if (camera->getTypeId().getName() == "FrustumCamera") {
    nearval = camera->nearDistance.getValue();
    farval *= (1.0f + SLACK); 
    if (farval <= nearval) {
      // nothing is visible, so just set farval to some value > nearval.
      farval = nearval + 10.0f;
    }
  }
  else {
    // For orthographic cameras also negative values nearval/farval
    // are reasonable.
    nearval *= (1.0f - (nearval>0?1:-1)*SLACK);
    farval *= (1.0f + (farval>0?1:-1)*SLACK);
  }
  
  if (autoclipcb) {
    SbVec2f nearfar(nearval, farval);
    nearfar = autoclipcb(autoclipuserdata, nearfar);
    
    nearval = nearfar[0];
    farval = nearfar[1]; 
  }
  
  if (nearval != camera->nearDistance.getValue()) {
    camera->nearDistance = nearval;
  }
  if (farval != camera->farDistance.getValue()) {
    camera->farDistance = farval;
  }
}

void
FdQuarterViewer::draw2DString(const char* str,
                              SbVec2s glsize,
                              SbVec2f position)
{
  // Store GL state.
  glPushAttrib(GL_ENABLE_BIT|GL_CURRENT_BIT);
  
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, glsize[0], 0.0, glsize[1], -1, 1);
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  glColor3f(1.0, 1.0, 0.0);
  glRasterPos2f(position[0], position[1]);
  printString(str);
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  glPopAttrib();
}

void
FdQuarterViewer::printString(const char* str)
{
  std::size_t len = strlen(str);
  
  for(std::size_t i = 0; i < len; i++) {
    glBitmap(8, 12, 0.0, 2.0, 10.0, 0.0, fps2dfont[str[i] - 32]);
  }
}
