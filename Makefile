# vi: set tabstop=4 paste nocindent noautoindent: 
#
#

TOPDIR=../JunkBox_Lib

#
CC   = g++
AR   = ar
TH   = touch

#OPENJPEG_VERSION = 12
#OPENJPEG_VERSION = 14
#OPENJPEG_VERSION = 15
#OPENJPEG_VERSION = 20
#OPENJPEG_VERSION = 21
#OPENJPEG_VERSION = 24
OPENJPEG_VERSION = 25

#OPENJPEG_DIR = /usr
OPENJPEG_DIR = /usr/local


ifeq ($(OPENJPEG_VERSION),12)
OPJINC = $(OPENJPEG_DIR)/include
OPJLIB = openjpeg
endif
ifeq ($(OPENJPEG_VERSION),14)
OPJINC = $(OPENJPEG_DIR)/include/openjpeg-1.4
OPJLIB = openjpeg
endif
ifeq ($(OPENJPEG_VERSION),15)
OPJINC = $(OPENJPEG_DIR)/include/openjpeg-1.5
OPJLIB = openjp2
endif
ifeq ($(OPENJPEG_VERSION),20)
OPJINC = $(OPENJPEG_DIR)/include/openjpeg-2.0
OPJLIB = openjp2
endif
ifeq ($(OPENJPEG_VERSION),21)
OPJINC = $(OPENJPEG_DIR)/include/openjpeg-2.1
OPJLIB = openjp2
endif
ifeq ($(OPENJPEG_VERSION),24)
OPJINC = $(OPENJPEG_DIR)/include/openjpeg-2.4
OPJLIB = openjp2
endif
ifeq ($(OPENJPEG_VERSION),25)
OPJINC = $(OPENJPEG_DIR)/include/openjpeg-2.5
OPJLIB = openjp2
endif


SUBJ = oarconv mktree

OBJS = oarconv_help.o OARTool.o TerrainTool.o TreeTool.o MeshMaker.o GeneralTool.o PrimMesher.o PrimShape.o SculptMesher.o TerrainMesher.o

all: $(SUBJ)

#
OARTool.h: LogDocTool.h OARGlobal.h
	$(TH) OARTool.h

TerrainTool.h: LogDocTool.h OARGlobal.h
	$(TH) TerrainTool.h

MeshMaker.h: LogDocTool.h
	$(TH)

PrimShape.h: LogDocTool.h
	$(TH) PrimShape.h

SculptMesher.h: LogDocTool.h
	$(TH) SculptMesher.h

TerrainMesher.h: LogDocTool.h
	$(TH) TerrainMesher.h

mktree.cpp: oarconv.h
	$(TH) mktree.cpp


LIB_DIR=$(TOPDIR)/C++Lib

LIB_BSP_DIR = $(LIB_DIR)/BasicLib
LIB_EXP_DIR = $(LIB_DIR)/ExtendLib
LIB_GRP_DIR = $(LIB_DIR)/GraphLib
LIB_MTP_DIR = $(LIB_DIR)/MathLib
LIB_MSH_DIR = $(LIB_DIR)/MeshLib
LIB_BRP_DIR = $(LIB_DIR)/BREPLib

LIB_BSC_DIR = $(LIB_DIR)/BasicLib/Lib
LIB_EXT_DIR = $(LIB_DIR)/ExtendLib/xLib
LIB_GRF_DIR = $(LIB_DIR)/gLib

LIB_BSP = $(LIB_BSP_DIR)/libbasicpp.a
LIB_EXP = $(LIB_EXP_DIR)/libextendpp.a
LIB_GRP = $(LIB_GRP_DIR)/libgraphpp.a
LIB_MTP = $(LIB_MTP_DIR)/libmathpp.a
LIB_GRF = $(LIB_GRF_DIR)/libgraph.a
LIB_MSH = $(LIB_MSH_DIR)/libmesh.a
LIB_BRP = $(LIB_BRP_DIR)/libbrep.a

BLIB = -L$(LIB_BSP_DIR) -lbasicpp -lm 
MLIB = -L$(LIB_MTP_DIR) -lmathpp
ELIB = -L$(LIB_EXP_DIR) -lextendpp
RLIB = -L$(LIB_GRP_DIR) -lgraphpp
GLIB = -L$(LIB_GRF_DIR) -lgraph
PLIB = -L$(LIB_BRP_DIR) -lbrep
HLIB = -L$(LIB_MSH_DIR) -lmesh


#DEBUG = -DEBUG

CFLAGS = -fPIC -DHAVE_CONFIG_H -I.. -I/usr/local/include -I$(OPJINC) -I$(TOPDIR) -I$(LIB_BSC_DIR) -I$(LIB_BSP_DIR) -I$(LIB_MTP_DIR) -I$(LIB_GRP_DIR) -I$(LIB_GRF_DIR) -I$(LIB_EXP_DIR) -I$(LIB_EXT_DIR) -I$(LIB_BRP_DIR) -I$(LIB_MSH_DIR) $(DEBUG)

CFLAGS += -DENABLE_OPENJPEG -DOPENJPEG_VERSION=$(OPENJPEG_VERSION) -DHAVE_OPENJPEG_H -DENABLE_PNG

#
#
#
.h.cpp:
	$(TH) $@

.h.c:
	$(TH) $@


.cpp.o: $(LIB_MSH)
	$(CC) $< $(CFLAGS) -c -O2 

.c.o:
	$(CC) $< $(CFLAGS) -c -O2 


clean:
	rm -f *.o *~ $(SUBJ) *.dae *.obj *.mtl *.stl


#
#
#
#
#export: export.o $(LIB_BSP) $(LIB_GRP) $(LIB_EXP) 
#	$(CC) $(@).o $(PLIB) $(ELIB) $(RLIB) $(MLIB) $(BLIB) -O2 -o $@  -lz  
#

oarconv: oarconv.o $(OBJS) $(LIB_BSP) $(LIB_GRP) $(LIB_EXP) $(LIB_BRP) $(LIB_MSH) $(LIB_MTP)
	$(CC) $(@).o $(OBJS) $(HLIB) $(ELIB) $(PLIB) $(RLIB) $(MLIB) $(BLIB) -O2 -o $@  -lz -L/usr/local/lib -ljpeg -lpng -l$(OPJLIB)

xmlcollada: xmlcollada.o $(OBJS) $(LIB_BSP) $(LIB_GRP) $(LIB_EXP) $(LIB_BRP) $(LIB_MSH) $(LIB_MTP)
	$(CC) $(@).o $(OBJS) $(HLIB) $(ELIB) $(PLIB) $(RLIB) $(MLIB) $(BLIB) -O2 -o $@  -lz -L/usr/local/lib -ljpeg -lpng -l$(OPJLIB)

mktree: mktree.o $(OBJS) $(LIB_BSP) $(LIB_GRP) $(LIB_EXP) $(LIB_BRP) $(LIB_MSH) $(LIB_MTP)
	$(CC) $(@).o $(OBJS) $(HLIB) $(ELIB) $(PLIB) $(RLIB) $(MLIB) $(BLIB) -O2 -o $@  -lz -L/usr/local/lib -ljpeg -lpng -l$(OPJLIB)



$(LIB_BSP):
	(cd $(LIB_BSP_DIR) && make)

$(LIB_GRP):
	(cd $(LIB_GRP_DIR) && make)

$(LIB_GRF):
	(cd $(LIB_GRF_DIR) && make)

$(LIB_EXP):
	(cd $(LIB_EXP_DIR) && make)

$(LIB_MTP):
	(cd $(LIB_MTP_DIR) && make)

$(LIB_MSH):
	(cd $(LIB_MSH_DIR) && make)

$(LIB_BRP):
	(cd $(LIB_BRP_DIR) && make)

