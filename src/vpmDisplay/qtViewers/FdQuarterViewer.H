// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FD_QUARTERVIEWER_H
#define FD_QUARTERVIEWER_H

#include <Inventor/SoType.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/lists/SoCallbackList.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Quarter/QuarterWidget.h>


class SbViewportRegion;
class SoCamera;
class SoOrthographicCamera;
class SoPerspectiveCamera;
class SoSelection;
class SoGetBoundingBoxAction;

class FdQuarterViewer;
using FdQuarterViewerCB = void (void* data, FdQuarterViewer* viewer);
using FdQuarterAutoClippingCB = SbVec2f (void* data, const SbVec2f& nearfar);
using FdQuarterVisibilityCB = void (void* user, SbBool visible);

class FdQuarterViewer :  public SIM::Coin3D::Quarter::QuarterWidget {

public:
  enum Type {
    BROWSER,
    EDITOR
  };

  enum AutoClippingStrategy {
    VARIABLE_NEAR_PLANE,
    CONSTANT_NEAR_PLANE
  };

  // This constructors are not really needed, but it is good to have them
  // already overloaded for future usage
  FdQuarterViewer(QWidget* parent = NULL,
                  const QOpenGLWidget* sharewidget = NULL,
                  Qt::WindowFlags flags = Qt::WindowFlags());
  FdQuarterViewer(const QSurfaceFormat& format,
                  QWidget* parent = NULL,
                  const QOpenGLWidget* shareWidget = NULL,
                  Qt::WindowFlags flags = Qt::WindowFlags());
  FdQuarterViewer(QOpenGLContext* context,
                  QWidget* parent = NULL,
                  const QOpenGLWidget* sharewidget = NULL,
                  Qt::WindowFlags flags = Qt::WindowFlags());
  // The SoQtViewer impersonation constructor
  FdQuarterViewer(QWidget* parent = NULL,
                  const char* name = NULL,
                  SbBool  embed = TRUE,
                  FdQuarterViewer::Type = FdQuarterViewer::Type::BROWSER,
                  SbBool  build = TRUE);
  ~FdQuarterViewer() override;

  // Missing functions
  inline QWidget* getWidget() { return this; }
  inline QWidget* getGLWidget() { return this; }
  inline QWidget* getWidget() const
  {
    return const_cast<FdQuarterViewer*>(this);
  }
  QWidget* getGLWidget() const
  {
    return const_cast<FdQuarterViewer*>(this);
  }

  void setTitle(const char * const title);

  virtual void setCamera(SoCamera * cam);
  virtual void toggleCameraType();
  virtual void setCameraType(SoType type);
  inline SoCamera* getCamera() const
  {
    return getSoRenderManager()->getCamera();
  }

  inline const SbViewportRegion & getViewportRegion() const
  {
    return getSoRenderManager()->getViewportRegion();
  }
  inline void setViewportRegion( const SbViewportRegion &newRegion )
  {
    getSoRenderManager()->setViewportRegion(newRegion);
  }

  void redrawOnSelectionChange(SoSelection *selection);

  virtual void setViewing(bool enable);
  inline bool isViewing() const { return viewingflag; }

  void interactiveCountInc();
  void interactiveCountDec();
  inline int getInteractiveCount() const { return interactionnesting; }

  inline void addStartCallback(FdQuarterViewerCB* func, void* data = NULL)
  {
    interactionStartCallback.addCallback((SoCallbackListCB*)func, data);
  }
  inline void addFinishCallback(FdQuarterViewerCB* func, void* data = NULL)
  {
    interactionStartCallback.removeCallback((SoCallbackListCB*)func, data);
  }
  inline void removeStartCallback(FdQuarterViewerCB* func, void* data = NULL)
  {
    interactionEndCallback.addCallback((SoCallbackListCB*)func, data);
  }
  inline void removeFinishCallback(FdQuarterViewerCB* func, void* data = NULL)
  {
    interactionEndCallback.removeCallback((SoCallbackListCB*)func, data);
  }

  virtual void setSeekMode(bool enable);
  inline bool isSeekMode() const { return inseekmode; }
  bool seekToPoint(const SbVec2s& screenpos);
  void seekToPoint(const SbVec3f& scenepos);
  void setSeekTime(float seconds);
  inline float getSeekTime() const { return seekperiod; }
  void setSeekDistance(float distance);
  inline float getSeekDistance() const { return seekdistance; }
  void setSeekValueAsPercentage(bool on);
  inline bool isSeekValuePercentage() const { return !seekdistanceabs; }

  inline virtual float getPickRadius() const { return this->pickRadius; }
  virtual void setPickRadius(float pickRadius);

  virtual void saveHomePosition();
  virtual void resetToHomePosition();
  virtual bool hasHomePosition() const
  {
    return storedcamera != NULL;
  }

  inline SbBool isAutoRedraw() const {
    return getSoRenderManager()->isAutoRedraw();
  }

  inline void setAutoRedraw(SbBool enable) {
    if (enable)
      getSoRenderManager()->setRenderCallback(FdQuarterViewer::rendercb, this);
    else
      getSoRenderManager()->setRenderCallback(NULL, this);
  }

  inline void setBackgroundColor(const SbColor &color)
  {
    float r, g, b;
    color.getValue(r, g, b);
    SIM::Coin3D::Quarter::QuarterWidget::setBackgroundColor(QColor(
      (int)(r * 255), (int)(g * 255), (int)(b * 255)));
  }

  inline SbColor getBackgroundColor() const
  {
    SbColor color;
    SbColor4f colora = getSoRenderManager()->getBackgroundColor();
    colora.getRGB(color);
    return color;
  }

  inline void setHeadlight(SbBool on) { getHeadlight()->on = on; }
  inline SbBool isHeadlight() { return getHeadlight()->on.getValue(); }

  inline void render()
  {
    setClippingPlanes();
    redraw();
  }

  inline SoGLRenderAction* getGLRenderAction() const
  {
    return getSoRenderManager()->getGLRenderAction();
  }

  inline void setAntialiasing(SbBool smoothing, int numPasses)
  {
    getSoRenderManager()->setAntialiasing(smoothing, numPasses);
  }

  inline void setSize(const SbVec2s size)
  {
    resize(size[0], size[1]);
  }

  inline SbVec2s getGlxSize() const
  {
    SoRenderManager* rm = getSoRenderManager();
    if (!rm)
      return SbVec2s(0, 0);
    return rm->getWindowSize();
  }

  inline void setAutoClipping(SbBool enable)
  {
    adjustclipplanes = enable;
    if (enable) { this->scheduleRedraw(); }
  }
  inline SbBool isAutoClipping(void) const { return adjustclipplanes; }

  void setAutoClippingStrategy(const AutoClippingStrategy strategy,
                               const float value = 0.6f,
                               FdQuarterAutoClippingCB * cb = NULL,
                               void * cbuserdata = NULL);

  inline void scheduleRedraw(void) { getSoRenderManager()->scheduleRedraw(); }

  void addVisibilityChangeCallback(FdQuarterVisibilityCB * const func,
                                   void * const user);
  void removeVisibilityChangeCallback(FdQuarterVisibilityCB * const func,
                                      void * const data);

  // Missing events
  bool processSoEvent(const SoEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

  // Missing functions that we are not porting
  virtual void setDrawToFrontBufferEnable( const SbBool enable )
  {}
  inline SbBool isDetailSeek(void) const { return TRUE; }

  // To trick FdDB::viewer->getSceneManager()->getSceneGraph()
  inline SIM::Coin3D::Quarter::QuarterWidget * getSceneManager()
  {
    return this;
  }

  inline void setSceneGraph(SoNode* root) override
  {
    SIM::Coin3D::Quarter::QuarterWidget::setSceneGraph(root);
  }

  // To convert the Qt events to the SoQt ones
  virtual bool event(QEvent *e);
  virtual void processEvent(QEvent* e);
private:
  void init();
  static void convertPerspective2Ortho(const SoPerspectiveCamera* in,  SoOrthographicCamera* out);
  static void convertOrtho2Perspective(const SoOrthographicCamera* in, SoPerspectiveCamera* out);
  void getCameraCoordinateSystem(SoCamera * camera, SoNode * root, SbMatrix & matrix, SbMatrix & inverse);
  static void seeksensorCB(void * data, SoSensor * sensor);
  void moveCameraScreen(const SbVec2f & screenpos);
  void resetFrameCounter();
  SbVec2f addFrametime(double ft);
  void setClippingPlanes(void);
  static void rendercb(void *userdata, SoRenderManager *)
  {
    FdQuarterViewer* thisp = static_cast<FdQuarterViewer*>(userdata);
    thisp->render();
  }
  static void selection_redraw_cb(void * data, SoSelection * sel)
  {
    FdQuarterViewer * viewer = (FdQuarterViewer *) data;
    viewer->getSoRenderManager()->scheduleRedraw();
  }

  bool viewingflag = false;
  int  interactionnesting = 0;
  SoCallbackList interactionStartCallback;
  SoCallbackList interactionEndCallback;

  double frametime = 0.0;
  double drawtime = 0.0;
  double starttime = 0.0;
  int framecount = 0.0;

  // Seek functionality
  SoTimerSensor* seeksensor = NULL;
  float seekperiod = 0.0F;
  bool inseekmode = false;
  SbVec3f camerastartposition, cameraendposition;
  SbRotation camerastartorient, cameraendorient;
  float seekdistance = 0.0F;
  bool seekdistanceabs = false;
  SoSearchAction searchaction;
  SoGetMatrixAction matrixaction;
  float pickRadius = 0.0F;
  // Home position storage.
  SoNode * storedcamera = NULL;
  // Selection storage
  SoSelection* selection = NULL;

  // auto clipping parameters
  FdQuarterViewer::AutoClippingStrategy autoclipstrategy = FdQuarterViewer::VARIABLE_NEAR_PLANE;
  float autoclipvalue = 0.6;
  FdQuarterAutoClippingCB * autoclipcb = NULL;
  void * autoclipuserdata = NULL;
  SbBool adjustclipplanes = TRUE;
  SoGetBoundingBoxAction * autoclipbboxaction = NULL;

  // visibility callback
  SbPList * visibilitychangeCBs = NULL;

  // To know if processEvent has handled the event
  SbBool eventHandeled;

protected:
  static void draw2DString(const char * str, SbVec2s glsize, SbVec2f position);
  static void printString(const char * str);
  SbVec2f framesPerSecond;
};

typedef FdQuarterViewerCB SoQtViewerCB;
typedef FdQuarterAutoClippingCB SoQtAutoClippingCB;
typedef FdQuarterVisibilityCB SoQtComponentVisibilityCB;
typedef FdQuarterViewer SoQtViewer;

#define SOQT_OBJECT_HEADER(classname, parentname) 
 
#define SOQT_OBJECT_SOURCE(classname) 

#endif // FD_QUARTERVIEWER_H
