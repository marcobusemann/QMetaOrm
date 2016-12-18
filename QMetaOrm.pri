INCLUDEPATH += "$$PWD"/include

unix:LIBS += -L"$$PWD"/lib/ -lQMetaOrm
win32:LIBS += "$$PWD"/lib/QMetaOrm.lib
