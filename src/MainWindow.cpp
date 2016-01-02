#include "general_application_style_editor/MainWindow.h"
#include "htl_common_data_types/ObjectKey.h"
#include "general_application_style_editor/Editor.h"

#include <QResizeEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>



namespace general_application_style_editor
{


struct MainWindow::Private
{


public:
  MainWindow* q;
  Editor* colorEditor;
  QPushButton* colorEditorButton;
  QPushButton* layoutEditorButton;
  QPushButton* loadMainButton;
  QVBoxLayout * layout;
  QScrollArea* scroll;

  //################################################################################################
  Private(MainWindow* q_):
    q(q_),
    colorEditor(NULL),
    layout(new QVBoxLayout(q)),
    scroll(new QScrollArea(q))


  {

  }
  //################################################################################################
  ~Private()
  {

  }

};


//##################################################################################################
MainWindow::MainWindow(QWidget* parent):
  QWidget(parent),
  d(new Private(this))
{
  d->colorEditorButton= new QPushButton();
  d->colorEditorButton->setText("Colour Editor");

  d->layoutEditorButton= new QPushButton();
  d->layoutEditorButton->setText("Layout Editor");

  d->loadMainButton= new QPushButton();
  d->loadMainButton->setText("Back To Main Menu");

  htl_common_data_types::Object colorsDocument;
  d->layout->addWidget(d->colorEditorButton);
  d->layout->addWidget(d->layoutEditorButton);
  d->layout->addWidget(d->loadMainButton);

  d->loadMainButton->hide();

  d->colorEditor = new Editor(this);

  d->colorEditor->loadColorsDocument(colorsDocument);

  //d->layout ->addWidget(d->scroll);
  d->scroll->resize(1200,1200);
  d->layout->addWidget(d->scroll);
  d->colorEditor->hide();
  connect(d->colorEditorButton, SIGNAL(clicked()), this, SLOT(handleButtonColors()));
  connect(d->layoutEditorButton, SIGNAL(clicked()), this, SLOT(handleButtonLayouts()));
  connect(d->loadMainButton, SIGNAL(clicked()), this, SLOT(handleButtonMain()));


}

//##################################################################################################
MainWindow::~MainWindow()
{
  delete d;
}

void MainWindow::handleButtonColors()
{


  d->colorEditorButton->hide();
  d->layoutEditorButton->hide();
  d->loadMainButton->show();
  d->colorEditor->show();

  //d->scroll->setBackgroundRole(QPalette::Dark);

  d->scroll->setWidget(d->colorEditor);
  d->colorEditor->resize(contentsRect().width()/1.01,d->colorEditor->contentsRect().height());
}
void MainWindow::handleButtonLayouts()
{

}

void MainWindow::handleButtonMain()
{
  d->colorEditorButton->show();
  d->layoutEditorButton->show();
  d->colorEditor->hide();
  d->loadMainButton->hide();

}


}
