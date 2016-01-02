#ifndef application_style_editor_MainWindow_h
#define application_style_editor_MainWindow_h



#include <QWidget>


namespace general_application_style_editor
{

class MainWindow: public QWidget
{

  Q_OBJECT



public:
  //################################################################################################
  MainWindow(QWidget* parent=NULL);

  //################################################################################################
  ~MainWindow();


  //################################################################################################
public slots:
  void handleButtonColors();
  void handleButtonLayouts();
  void handleButtonMain();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
