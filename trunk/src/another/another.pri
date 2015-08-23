SOURCES+=src/another/qtsingleapplication.cpp \
         src/another/qtlocalpeer.cpp \
         src/another/qtlockedfile.cpp \
         src/another/qtlockedfile_unix.cpp \
         src/another/qtlockedfile_win.cpp \
         src/another/kinotifywidget.cpp \
         src/another/qxtglobal.cpp \
         src/another/qxtglobalshortcut.cpp
HEADERS+=src/another/qtsingleapplication.h \
         src/another/qtlocalpeer.h \
         src/another/qtlockedfile.h\
         src/another/kinotifywidget.h \
         src/another/qxtglobal.h \
         src/another/qxtglobalshortcut.h \
         src/another/qxtglobalshortcut_p.h

RESOURCES+=src/another/kinotifyresources.qrc

win32 {
  SOURCES += src/another/qxtglobalshortcut_win.cpp
} else:macx {
  SOURCES += src/another/qxtglobalshortcut_mac.cpp
} else {
  SOURCES += src/another/qxtglobalshortcut_x11.cpp
}
