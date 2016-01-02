#include "general_application_style_editor/Editor.h"
#include "htl_common_data_types/ObjectKey.h"
#include "htl_common_data_types/Object.h"
#include "tdp_qt_utils/Serialization.h"
#include "tdp_qt_utils/FileUtils.h"


#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QSettings>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QMessageBox>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QSize>
#include <QFileInfo>




namespace general_application_style_editor
{
//##################################################################################################
struct Editor::Private
{
public:
  Editor* q;
  htl_common_data_types::Object colorsDocument;
  htl_common_data_types::Object exportDocument;
  QGridLayout* layout;
  int  count;
  QLineEdit* labelColor[200];
  QLineEdit* colorField[200];
  QLineEdit* example[200];
  QPushButton* save;
  QPushButton* load;
  QLabel* num[200];
  QRect* colorRect[200];
  QMessageBox* messageBox;
  QLabel* labelName;
  QLabel* saveMessage;
  QLineEdit* documentName;
  QPushButton* button;
  QPushButton* newDocumentButton;
  QPushButton* overWriteButton;
  QPushButton* newRowButton;
  //QPushButton* delRowButton;

  QPushButton* removeRow[200];
  bool newDocClicked;
  int items;
  int newItems;

  QHash<tdp_qt_utils::StringID, QColor> colors;


  //################################################################################################
  Private(Editor* q_):
    q(q_),
    layout(new QGridLayout(q)),
    count(-1),
    newDocClicked(false),
    items(-1),
    newItems(0)
  {

  }


  //################################################################################################
  void update()
  {
    q->update();
  }



};

//##################################################################################################
Editor::Editor(QWidget *parent):
  QWidget(parent),
  d(new Private(this))
{

  d->layout->setSpacing(0);
  d->layout->setMargin(30);

  d->newDocumentButton=new QPushButton();
  d->newDocumentButton->setText("New Document");

  d->newRowButton=new QPushButton();
  d->newRowButton->setText("+");

  d->newRowButton->setStyleSheet("Background-color:gray;font-size:20px ");



  d->layout->setRowStretch(0,20);

  d->save=new QPushButton();
  d->save->setText("Save");

  d->load=new QPushButton();
  d->load->setText("Load Document");

  connect(d->save, SIGNAL(clicked()), this, SLOT(handleButtonSave()));
  connect(d->load, SIGNAL(clicked()), this, SLOT(handleButtonLoad()));
  connect(d->newDocumentButton, SIGNAL(clicked()), this, SLOT(handleNewDoc()));
  connect(d->newRowButton, SIGNAL(clicked()), this, SLOT(newRowClicked()));




}

//##################################################################################################
Editor::~Editor()
{
  delete d;
}
//##################################################################################################
void Editor::loadDocument()
{
  d-> newItems=0;

  d-> exportDocument.fields.clear();

  d-> colors.clear();

  QSettings settings;
  if( d->newDocClicked==false)
  {
    if(! d->layout->isEmpty())
    {
      QLayoutItem *child;
      while ((child = d->layout->takeAt(5)) != 0)
      {
        child->widget()->deleteLater();
        delete child;
      }
      // delete layout;
      //  layout = new QGridLayout(q);
      //layout->setSpacing(0);
    }

    d->layout->addWidget(  d->save,0,3);
    d->layout->addWidget(  d->load,0,2);
    d->layout->addWidget(  d->newDocumentButton,0,1);

    d->layout->removeWidget( d->newRowButton);
    d->layout->addWidget( d->newRowButton,0,4);
  }

  d->newDocClicked=false;

  if( d->colorsDocument.fields.values().isEmpty())
  {
    d->items=settings.allKeys().count()-1;
    int i=-1;
    while(i<settings.allKeys().count() -1)
    {
      i++;
      d->removeRow[i]=new QPushButton();
      d->removeRow[i]->setText("-");
      d->removeRow[i]->setStyleSheet("Background-color:gray;font-size:20px");

      if( d->colorField[i]==NULL)
      {
        d->colorField[i] =new QLineEdit();
        d->labelColor[i] =new QLineEdit();
        d->num[i] =new QLabel();
        d->example[i] =new QLineEdit();
        d->example[i]->setText("CLICK FOR PREVIEW");
        d->example[i]->setDisabled(true);

        if(settings.value(settings.allKeys().at(i))=="transparent")
        {
          d->colorField[i]->setText("transparent");

        }
        else
        {
          if( settings.value(settings.allKeys().at(i)).value<QColor>().isValid())
          {
            d->colorField[i]->setText((settings.value(settings.allKeys().at(i)).value<QColor>().name()) );
          }
        }

        d->colorField[i]->setAlignment(Qt::AlignRight);
        d->colorField[i]->setStyleSheet("background-color:white");

        d->labelColor[i]->setText(settings.allKeys().at(i));
        d->labelColor[i]->setStyleSheet("background-color:white");
        d->labelColor[i]->setAlignment(Qt::AlignRight);
      }
      if(i<10)
      {
        d->num[i]->setText(QString::number(i)+"   | ");
      }
      else
      {
        d->num[i]->setText(QString::number(i)+" | ");
      }
      if( settings.value(settings.allKeys().at(i)).value<QColor>().isValid())
      {
        d->layout->addWidget( d->num[i],i+1,0);
        d->layout->addWidget( d->labelColor[i],i+1,1);
        d->layout->addWidget( d->colorField[i],i+1,2);
        d->layout->addWidget( d->example[i],i+1,3);

        d->layout->addWidget( d->removeRow[i],i+1,4);
        connect(d->removeRow[i], SIGNAL(clicked()), this, SLOT(delRowClicked()));
        d->removeRow[i]->setObjectName(QString::number(i));
      }

    }

    if(settings.allKeys().count()>2)
    {
      d->q->resize( d->q->contentsRect().width(), settings.allKeys().count()*50);
    }
    else
    {
      d-> q->resize( d->q->contentsRect().width(), 200);
    }
  }
  else
  {
    d-> save->show();

    int i=-1;
    d->items= d->colorsDocument.fields.keys().count()-1;
    while(i< d->colorsDocument.fields.keys().count()-1)
    {
      i++;

      d->removeRow[i]=new QPushButton();
      d->removeRow[i]->setText("-");
      d->removeRow[i]->setStyleSheet("Background-color:gray;font-size:20px");

      d->colors.insert( d->colorsDocument.fields.keys().at(i),  d->colorsDocument.fields.values().at(i).value);
      //if(colorField[i]==NULL){
      d->colorField[i] =new QLineEdit();
      d->labelColor[i] =new QLineEdit();
      d->num[i] =new QLabel();
      d->example[i] =new QLineEdit();
      d->example[i]->setText("CLICK FOR PREVIEW");
      d->example[i]->setDisabled(true);
      d->colorField[i]->setText( d->colorsDocument.fields.values().at(i).value);
      d->colorField[i]->setAlignment(Qt::AlignRight);

      d->example[i]->setStyleSheet("height:20px");
      d->colorField[i]->setStyleSheet("background-color:white");
      d->labelColor[i]->setText( d->colorsDocument.fields.keys().at(i).keyString());
      //labelColor[i]->setDisabled(true);
      d->labelColor[i]->setStyleSheet("background-color:white");
      d->labelColor[i]->setAlignment(Qt::AlignRight);
      if(i<10)
      {
        d->num[i]->setText(QString::number(i)+"   | ");
      }
      else
      {
        d->num[i]->setText(QString::number(i)+" | ");
      }

      d->layout->addWidget( d->num[i],i+1,0);
      d->layout->addWidget( d->labelColor[i],i+1,1);
      d->layout->addWidget( d->colorField[i],i+1,2);
      d->layout->addWidget( d->example[i],i+1,3);

      d->layout->addWidget( d->removeRow[i],i+1,4);
      connect(d->removeRow[i], SIGNAL(clicked()), this, SLOT(delRowClicked()));
      d->removeRow[i]->setObjectName(QString::number(i));
    }

    if(  d->colorsDocument.fields.keys().count()>2)
    {
      d->q->resize( d->q->contentsRect().width(), d->colorsDocument.fields.keys().count()*40);
    }
    else
    {
      d->q->resize( d->q->contentsRect().width(), 200);
    }
  }

  update();
}
//##################################################################################################
void Editor::newRowClicked()
{
  d-> items++;

  d-> removeRow[d->items]=new QPushButton();
  d-> removeRow[d->items]->setText("-");
  d-> removeRow[d->items]->setStyleSheet("Background-color:gray;font-size:20px");

  d-> colorField[d->items] =new QLineEdit();
  d-> colorField[d->items]->setPlaceholderText("Enter Color");
  d-> colorField[d->items]->setAlignment(Qt::AlignRight);

  d-> labelColor[d->items] =new QLineEdit();
  d-> labelColor[d->items]->setPlaceholderText("Element Name");
  d-> labelColor[d->items]->setAlignment(Qt::AlignRight);

  d-> num[d->items] =new QLabel();

  d-> example[d->items] =new QLineEdit();
  d-> example[d->items]->setText("CLICK FOR PREVIEW");
  d-> example[d->items]->setDisabled(true);
  d-> example[d->items]->hide();


  if(d->items<10)
  {
    d-> num[d->items]->setText(QString::number(d->items)+"   | ");
  }
  else
  {
    d-> num[d->items]->setText(QString::number(d->items)+" | ");
  }
  if(d->items>1){
    //Second click
    if(d->newItems==1)
    {
      d->layout->removeWidget(   d->num[d->items-1]);
      d->layout->removeWidget(   d->labelColor[d->items-1]);
      d->layout->removeWidget(   d->colorField[d->items-1]);
      d->layout->removeWidget(   d->example[d->items-1]);
      d->layout->removeWidget( d->removeRow[d->items-1]);

      d->layout->addWidget(   d->num[d->items-1],d->items+1,0);
      d->layout->addWidget(   d->labelColor[d->items-1],d->items+1,1);
      d->layout->addWidget(   d->colorField[d->items-1],d->items+1,2);
      d->layout->addWidget(   d->example[d->items-1], d->items+1,3);
      d->layout->addWidget(   d->removeRow[d->items-1],d->items+1,4);

      d->layout->addWidget(   d->num[d->items],1,0);
      d->layout->addWidget(   d->labelColor[d->items], 1,1);
      d->layout->addWidget(   d->colorField[d->items], 1,2);
      d->layout->addWidget(   d->example[d->items], 1,3);
      d->layout->addWidget( d->removeRow[d->items], 1,4);

      connect(d->removeRow[d->items], SIGNAL(clicked()), this, SLOT(delRowClicked()));
      d->removeRow[d->items]->setObjectName(QString::number(d->items));
    }
    //first click
    if(d->newItems==0)
    {
      d->layout->removeWidget(   d->num[0]);
      d->layout->removeWidget(   d->labelColor[0]);
      d->layout->removeWidget(   d->colorField[0]);
      d->layout->removeWidget(   d->example[0]);
      d->layout->removeWidget(   d->removeRow[0]);

      d->layout->addWidget(   d->num[0],d->items+1,0);
      d->layout->addWidget(   d->labelColor[0],d->items+1,1);
      d->layout->addWidget(   d->colorField[0],d->items+1,2);
      d->layout->addWidget(   d->example[0], d->items+1,3);
      d->layout->addWidget(   d->removeRow[0],d->items+1,4);

      d->layout->addWidget(   d->num[d->items],1,0);
      d->layout->addWidget(   d->labelColor[d->items], 1,1);
      d->layout->addWidget(   d->colorField[d->items], 1,2);
      d->layout->addWidget(   d->example[d->items], 1,3);
      d->layout->addWidget(   d->removeRow[d->items], 1,4);

      connect(d->removeRow[d->items], SIGNAL(clicked()), this, SLOT(delRowClicked()));
      d->removeRow[d->items]->setObjectName(QString::number(d->items));

      d->newItems=1;
    }


  }
  //new doc
  else
  {
    d->layout->addWidget(   d->num[d->items],d->items+1,0);
    d->layout->addWidget(   d->labelColor[d->items],d->items+1,1);
    d->layout->addWidget(   d->colorField[d->items],d->items+1,2);
    d->layout->addWidget(   d->example[d->items],d->items+1,3);
    d->layout->addWidget(   d->removeRow[d->items],d->items+1,4);
    connect(d->removeRow[d->items], SIGNAL(clicked()), this, SLOT(delRowClicked()));
    d->removeRow[d->items]->setObjectName(QString::number(d->items));
  }

  if(d->items>0)
  {
    if((d->labelColor[d->items-1]->text().isEmpty()==true || d->colorField[d->items-1]->text().isEmpty()==true))
    {
      d->layout->removeWidget(d->num[d->items-1]);
      d->layout->removeWidget(d->labelColor[d->items-1]);
      d->layout->removeWidget(d->colorField[d->items-1]);
      d->layout->removeWidget(d->example[d->items-1]);

      d->num[d->items-1] ->hide();
      d->labelColor[d->items-1] ->hide();
      d->example[d->items-1] ->hide();
    }
  }
  if(d->items>0)
  {
    this->resize(contentsRect().width(),(d->items+2)*50);
  }
}

//##################################################################################################
void Editor::delRowClicked()
{
  int deleteRow=sender()->objectName().toInt();

  d->layout->removeWidget(d->num[deleteRow]);
  d->layout->removeWidget(d->labelColor[deleteRow]);
  d->layout->removeWidget(d->colorField[deleteRow]);
  d->layout->removeWidget(d->example[deleteRow]);


  d->num[deleteRow]  ->hide();
  d->labelColor[deleteRow] ->hide();
  d->colorField[deleteRow]->setText("t");
  d->colorField[deleteRow]->hide();
  d->removeRow[deleteRow]->hide();
  d->example[deleteRow]->hide();


  if(d->items>0)
  {
    d->items--;

    if(d->colors.isEmpty()!=true)
    {

      d->colors.remove(d->labelColor[deleteRow]->text());

      d->exportDocument.fields.clear();
      d->update();

    }
  }
  if(d->items<1)
  {
    //  d->delRowButton->hide();
  }

  if(d->items>1)
  {
    this->resize(contentsRect().width(),(d->items+2)*50);
  }
}

//##################################################################################################
void Editor::handleNewDoc()
{
  d->newItems=0;
  d->items=-1;
  this->resize(contentsRect().width(),(d->items+2)*1000);


  d->exportDocument.fields.clear();
  if(d->newDocClicked==false)
  {
    d-> colors.clear();
    if(! d-> layout->isEmpty())
    {
      QLayoutItem *child;
      while ((child = d-> layout->takeAt(5)) != 0)
      {
        child->widget()->deleteLater();
        delete child;
      }
    }

    d->newDocClicked=true;
  }
}
//##################################################################################################
void Editor::handleButtonSave()
{
  QFileDialog* qD;
  qD= new QFileDialog;


  QString fileName = qD->getSaveFileName(this, "Save File", QSettings().value("file-directory").toString(), "*.clr");
  if(fileName.endsWith(".clr"))
  {
    QByteArray requestData = tdp_qt_utils::serializeStandAlone(d->exportDocument);
    tdp_qt_utils::writeBinaryFile(fileName,requestData);
  }
  else
  {
    QByteArray requestData = tdp_qt_utils::serializeStandAlone(d->exportDocument);
    tdp_qt_utils::writeBinaryFile(fileName+".clr",requestData);
  }
}


//##################################################################################################
void Editor::handleButtonLoad()
{
  QString fileName = QFileDialog::getOpenFileName(this, "Load File", QSettings().value("file-directory").toString(), "*.clr");

  if(fileName.isEmpty())
    return;

  QByteArray requestData = tdp_qt_utils::readBinaryFile(fileName);
  htl_common_data_types::Object test;
  tdp_qt_utils::deserializeStandAlone(requestData, test);

  d->colorsDocument.fields.clear();
  d->colorsDocument=test;
  loadDocument();
}

//##################################################################################################
void Editor::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event);

  QSettings settings;

  if(d->items>-1)
  {
    if(d->labelColor[d->items]->text().isEmpty()!=true && QColor(d->colorField[d->items]->text()).isValid())
    {
      //d->colorField[d->items]->show();
      // d->update();
    }
    else
    {
      // d->colorField[d->items]->hide();
      d->example[d->items]->hide();
      // d->newRowButton->hide();
    }

    int k=-1;
    //d->colors.clear();

    while(k<d->items)
    {
      k++;
      if(QColor(d->colorField[k]->text()).isValid())
      {
        if(d->example[k]->isHidden() /*&& QColor(d->colorField[d->count]->text()).isValid()*/ )
        {
          d->example[k]->show();
        }

        d->colors.insert(d->labelColor[k]->text(),QColor(d->colorField[k]->text()));
      }
    }
    d->count=-1;
    settings.clear();
    d->exportDocument.fields.clear();
    while(d->count <d->colors.count()-1)
    {
      d->count++;
      //if(QColor(d->colorField[d->count]->text()).isValid())
      //{

      settings.setValue(  d->colors.keys().at(d->count).keyString() , QColor(d->colors.values().at(d->count)) );
      QPalette palette;

      if(d->colors.values().at(d->count)=="transparent")
      {
        d->exportDocument.fields.insert(d->colors.keys().at(d->count),htl_common_data_types::Field(d->colors.keys().at(d->count),("transparent")));
      }
      else
      {
        d->exportDocument.fields.insert(d->colors.keys().at(d->count),htl_common_data_types::Field(d->colors.keys().at(d->count),(   d->colors.values().at(d->count).name()   )));
        //qWarning()<< d->exportDocument.fields.count()<<"exp" <<d->colors.count()<<"col";
      }
      palette.setColor(QPalette::Base,QColor(d->colorField[d->count]->text()));
      d->example[d->count]->setPalette(palette);
      //}
    }
  }
}


void Editor::loadColorsDocument(htl_common_data_types::Object &existingDocument)
{
  d->colorsDocument=existingDocument;
  loadDocument();
}
}
