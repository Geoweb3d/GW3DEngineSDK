#pragma once

#include "../StdAfx.h"

#include "geoweb3d/engine/IGW3DVectorLayerStream.h"
#include "geoweb3d/common/GW3DCommon.h"

#include <QtCore/QObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QTimer>
#include <QtCore/QThread>

#include "../../tmp/uic/QtDockWindow.h"
#include "core/GW3DGUIInterFace.h"

class MyCompanyExtension;

namespace QtWidgetHelper
{
	static QWidget* GetWidget(OSWinHandle handle)
	{
		return QWidget::find((WId)handle);
	}
}

class EntityDockWindow : public QFrame
{
    Q_OBJECT

public:
	EntityDockWindow (QWidget* parent, MyCompanyExtension* app);
	~EntityDockWindow ();

	void LogMessage(const char *message);
	void LogMessage(QString);

	void setCurrent3dView (Geoweb3d::GUI::IGW3DGUIPreDrawContext*); 

protected:
	void showEvent (QShowEvent *event);
	void paintEvent (QPaintEvent *e);

private slots:
	void SlotButtonASelected_ ();
	void SlotButtonBSelected_ ();
	void SlotButtonCSelected_ ();
	void SlotButtonDSelected_ (); 

	void SlotFlashFeatureSelected_ (bool);

private:
	void InitializeWidgets_ ();

	Ui::obj_NewDockWindow* entityDockWidget_; 
	Geoweb3d::GUI::IGW3DGUIPreDrawContext* ctx_;

	MyCompanyExtension* app_;
	QTimer* timer;

};

