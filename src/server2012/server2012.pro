TEMPLATE = app
TARGET = server2012
QT += core \
    xml \
    xmlpatterns \
    network
HEADERS += xmlwrite.h \
    netrepository.h \
    xmlParse.h \
    server.h \
    simulation.h
SOURCES += xmlwrite.cpp \
    netrepository.cpp \
    xmlParse.cpp \
    server.cpp \
    simulation.cpp \
    main.cpp
FORMS += 
RESOURCES += 
