
#The  style_editor.pro may be included from subdirs, this check is to prevent these
#dependencies bring brought into sub projects. The reason that this is included is to make global
#defines available in those sub projects.
isEmpty(IS_SUB_MODULE){

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += tdp_qt_utils
SUBDIRS += htl_common_data_types


SUBDIRS += general_application_style_editor


}
