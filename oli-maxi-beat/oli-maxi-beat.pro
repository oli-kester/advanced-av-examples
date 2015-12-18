TEMPLATE = app
CONFIG += console g++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += maximilian.h \
    libs/fft.h \
    libs/maxiAtoms.h \
    libs/maxiBark.h \
    libs/maxiFFT.h \
    libs/maxiGrains.h \
    libs/maxim.h \
    libs/maxiMFCC.h \
    libs/sineTable.h \
    player.h \
    portaudio.h \
    RtAudio.h \
    RtError.h \
    stb_vorbis.h

SOURCES += main.cpp \
    libs/fft.cpp \
    libs/maxiAtoms.cpp \
    libs/maxiBark.cpp \
    libs/maxiFFT.cpp \
    libs/maxiGrains.cpp \
    libs/maxiMFCC.cpp \
    maximilian.cpp \
    player.cpp \
    RtAudio.cpp \
    stb_vorbis.c

DISTFILES += \
    README.txt
