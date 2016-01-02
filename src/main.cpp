#include "general_application_style_editor/MainWindow.h"

#include "tdp_qt_utils/FileUtils.h"
#include "tdp_qt_utils/TimeUtils.h"

#include <QApplication>
#include <QFontDatabase>
#include <QProcess>
#include <QStringList>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  tdp_qt_utils::initializeTimer();

  //Used for saving settings
  QCoreApplication::setOrganizationName("Hutchison Technologies Ltd.");
  QCoreApplication::setOrganizationDomain("http://www.hutchison-t.com/");
  QCoreApplication::setApplicationName("CAD Mobile Application");

  general_application_style_editor::MainWindow w;
  w.resize(1200,1200);
  w.show();


  return a.exec();
}






