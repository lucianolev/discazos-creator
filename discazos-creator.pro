# The application version
VERSION = 0.3.0

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

LIBS += /usr/lib/libtag.so

HEADERS += \
    src/gui/discazoscreator.h \
    src/gui/intropage.h \
    src/gui/albuminfopage.h \
    src/gui/discform.h \
    src/models/mp3tracksmodel.h \
    src/models/mp3track.h \
    src/models/albuminfo.h \
    src/models/disc.h \
    src/gui/savediscazopage.h \
    src/libmp3basics/mp3_frame.h \
    src/libmp3basics/mp3_file_reader.h \
    src/libmp3basics/mp3_file.h

SOURCES += \
    src/gui/discazoscreator.cpp \
    src/gui/intropage.cpp \
    src/gui/albuminfopage.cpp \
    src/gui/discform.cpp \
    src/models/mp3tracksmodel.cpp \
    src/models/mp3track.cpp \
    src/models/albuminfo.cpp \
    src/main.cpp \
    src/models/disc.cpp \
    src/gui/savediscazopage.cpp \
    src/libmp3basics/mp3_file.cpp \
    src/libmp3basics/mp3_frame.cpp \
    src/libmp3basics/mp3_file_reader.cpp

FORMS += \
    src/gui/intropage.ui \
    src/gui/albuminfopage.ui \
    src/gui/discform.ui \
    src/gui/savediscazopage.ui

TARGET = discazos-creator
target.path = /usr/bin
INSTALLS += target
