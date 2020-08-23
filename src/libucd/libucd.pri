INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += $$PWD/src/CharDistribution.cpp \
    $$PWD/src/JpCntx.cpp \
    $$PWD/src/LangBulgarianModel.cpp \
    $$PWD/src/LangCyrillicModel.cpp \
    $$PWD/src/LangCzechModel.cpp \
    $$PWD/src/LangFinnishModel.cpp \
    $$PWD/src/LangFrenchModel.cpp \
    $$PWD/src/LangGermanModel.cpp \
    $$PWD/src/LangGreekModel.cpp \
    $$PWD/src/LangHebrewModel.cpp \
    $$PWD/src/LangHungarianModel.cpp \
    $$PWD/src/LangPolishModel.cpp \
    $$PWD/src/LangSpanishModel.cpp \
    $$PWD/src/LangSwedishModel.cpp \
    $$PWD/src/LangThaiModel.cpp \
    $$PWD/src/LangTurkishModel.cpp \
    $$PWD/src/nsBig5Prober.cpp \
    $$PWD/src/nsCharSetProber.cpp \
    $$PWD/src/nsEscCharsetProber.cpp \
    $$PWD/src/nsEscSM.cpp \
    $$PWD/src/nsEUCJPProber.cpp \
    $$PWD/src/nsEUCKRProber.cpp \
    $$PWD/src/nsEUCTWProber.cpp \
    $$PWD/src/nsGB2312Prober.cpp \
    $$PWD/src/nsHebrewProber.cpp \
    $$PWD/src/nsLatin1Prober.cpp \
    $$PWD/src/nsMBCSGroupProber.cpp \
    $$PWD/src/nsMBCSSM.cpp \
    $$PWD/src/nsSBCharSetProber.cpp \
    $$PWD/src/nsSBCSGroupProber.cpp \
    $$PWD/src/nsSJISProber.cpp \
    $$PWD/src/nsUniversalDetector.cpp \
    $$PWD/src/nsUTF8Prober.cpp \
    $$PWD/src/ucdapi.cpp

DISTFILES += \
    $$PWD/src/Big5Freq.tab \
    $$PWD/src/EUCKRFreq.tab \
    $$PWD/src/EUCTWFreq.tab \
    $$PWD/src/GB2312Freq.tab \
    $$PWD/src/JISFreq.tab \
    $$PWD/src/Makefile.am

HEADERS += \
    $$PWD/src/CharDistribution.h \
    $$PWD/src/JpCntx.h \
    $$PWD/src/nsBig5Prober.h \
    $$PWD/src/nsCharSetProber.h \
    $$PWD/src/nsCodingStateMachine.h \
    $$PWD/src/nscore.h \
    $$PWD/src/nsDebug.h \
    $$PWD/src/nsEscCharsetProber.h \
    $$PWD/src/nsEUCJPProber.h \
    $$PWD/src/nsEUCKRProber.h \
    $$PWD/src/nsEUCTWProber.h \
    $$PWD/src/nsGB2312Prober.h \
    $$PWD/src/nsHebrewProber.h \
    $$PWD/src/nsLatin1Prober.h \
    $$PWD/src/nsMBCSGroupProber.h \
    $$PWD/src/nsPkgInt.h \
    $$PWD/src/nsSBCharSetProber.h \
    $$PWD/src/nsSBCSGroupProber.h \
    $$PWD/src/nsSJISProber.h \
    $$PWD/src/nsUniversalDetector.h \
    $$PWD/src/nsUTF8Prober.h \
    $$PWD/src/prmem.h \
    $$PWD/src/libucd.h
