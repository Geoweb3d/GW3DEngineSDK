#include "../stdafx.h"
#include "QtDockWindow.h"

#include "../Mouse3DQuery.h"
#include "../MyCompanyExtension.h"

#include "core/GW3DGUIInterFace.h"

#include "gui/GW3DGUICommon.h"
#include "gui/IGW3DGUIStatusOverlay.h"
#include "gui/IGW3DGUIView3d.h"

using namespace Geoweb3d;


// Inherets from a QFrame
EntityDockWindow::EntityDockWindow (QWidget* parent, MyCompanyExtension* app)
	: QFrame (parent)  
	, app_ (app)
{

	QLayout* layout = parent->layout();
	layout->addWidget(this);

    setAttribute(Qt::WA_NativeWindow);

	setAutoFillBackground (true);

	// Attach the ui code (designer)
	entityDockWidget_ = new Ui::obj_NewDockWindow ();   
	entityDockWidget_->setupUi (this);
	 
	InitializeWidgets_ ();  
	entityDockWidget_->label->setText (QString ("Example label\n"));

	show ();

}



EntityDockWindow::~EntityDockWindow () 
{
	delete entityDockWidget_; 
} 


void EntityDockWindow::paintEvent (QPaintEvent *e)
{
	// not needed, example only
	QFrame::paintEvent(e);
}


void EntityDockWindow::showEvent (QShowEvent *event)
{
	// not needed, example only
}
	
	
	
void EntityDockWindow::setCurrent3dView (Geoweb3d::GUI::IGW3DGUIPreDrawContext* ctx)
{
	ctx_ = ctx;
}
	
	
void EntityDockWindow::LogMessage(const char *message)
{
	QString msgOut(message);
	LogMessage(msgOut);
}
	
	
void EntityDockWindow::LogMessage(QString message)
{
	entityDockWidget_->txtDebugOut->appendPlainText(QString(message));
	QTextCursor c = entityDockWidget_->txtDebugOut->textCursor();
	c.movePosition(QTextCursor::End);
	entityDockWidget_->txtDebugOut->setTextCursor(c);
}
	
	
void EntityDockWindow::InitializeWidgets_ ()
{
	connect (entityDockWidget_->buttonA, SIGNAL (clicked()), this, SLOT (SlotButtonASelected_()));
	connect (entityDockWidget_->buttonB, SIGNAL (clicked()), this, SLOT (SlotButtonBSelected_()));
	connect (entityDockWidget_->buttonC, SIGNAL (clicked()), this, SLOT (SlotButtonCSelected_()));
	connect (entityDockWidget_->buttonD, SIGNAL (clicked()), this, SLOT (SlotButtonDSelected_()));
	
	connect(entityDockWidget_->flashFeaturePB, SIGNAL (clicked (bool)), this, SLOT (SlotFlashFeatureSelected_ (bool)));
	
	for (int i=0; i<10; i++) {
		QTreeWidgetItem *entities = new QTreeWidgetItem (entityDockWidget_->treeWidget);
		entities->setText(0, tr ("Entity %1").arg (i));
	}

	entityDockWidget_->toolBox->setCurrentIndex(0);

	entityDockWidget_->txtDebugOut->setWordWrapMode(QTextOption::WordWrap);
	entityDockWidget_->txtDebugOut->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
	
	
void EntityDockWindow::SlotButtonASelected_ ()
{
	entityDockWidget_->label->setText (QString ("top left button\n"));
}
	
	
void EntityDockWindow::SlotButtonBSelected_ ()
{
	entityDockWidget_->label->setText (QString ("top right button\n"));
}
	
	
void EntityDockWindow::SlotButtonCSelected_ ()
{
	entityDockWidget_->label->setText (QString ("bottom left button\n"));
}
	
	
void EntityDockWindow::SlotButtonDSelected_ ()
{
	entityDockWidget_->label->setText (QString ("bottom right button\n"));
}
	
	
void EntityDockWindow::SlotFlashFeatureSelected_ (bool isChecked)
{
app_->GetMouseQuery()->SetFlashOnHover(isChecked);
}
	