#ifndef application_style_editor_Editor_h
#define application_style_editor_Editor_h
#include "htl_common_data_types/Object.h"

#include <QWidget>

namespace general_application_style_editor
{
//##################################################################################################
 
class  Editor : public QWidget
{
  Q_OBJECT
public:
  //##############################################################################################
  Editor(QWidget *parent=NULL);

  //##############################################################################################
  ~Editor();

  //!this function will get constructed documents with default style information
  //!But we may not need the default data because we can load documents manualy and also this
  //!Program reads from the config file
  /*!
  \param existingDocumnet- we pass a document to this function and we can update the data
   */
  void loadColorsDocument(htl_common_data_types::Object& existingDocumnet);
void loadDocument();
protected:
  //!We construct documents and we write into the config file as well using this function

  //##############################################################################################

  virtual void paintEvent(QPaintEvent* event);


signals:
  //##############################################################################################
  //!We detect click events using this signal for our buttons
  void clicked();


public slots:
  //!We detect click on save button
  void handleButtonSave();

  //!We detect click on load button
  void handleButtonLoad();

  //!We detect click on new document button
  void handleNewDoc();

  //!We detect if the user adds new rows using the button
  void newRowClicked();

  //!We detect if the user deletes rows using the button
  void delRowClicked();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
