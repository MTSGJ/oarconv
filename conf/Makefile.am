# vi: set tabstop=4 nocindent: 

LIB_TOP_DIR = ..

LIB_BSIC_DIR = $(LIB_TOP_DIR)/C++Lib/BasicLib
LIB_EXTD_DIR = $(LIB_TOP_DIR)/C++Lib/ExtendLib
LIB_GRPH_DIR = $(LIB_TOP_DIR)/C++Lib/GraphLib
LIB_BREP_DIR = $(LIB_TOP_DIR)/C++Lib/BREPLib
LIB_MATH_DIR = $(LIB_TOP_DIR)/C++Lib/MathLib
LIB_MESH_DIR = $(LIB_TOP_DIR)/C++Lib/MeshLib

LIB_BSIC = $(LIB_BSIC_DIR)/libbasicpp.a
LIB_EXTD = $(LIB_EXTD_DIR)/libextendpp.a
LIB_GRPH = $(LIB_GRPH_DIR)/libgraphpp.a
LIB_BREP = $(LIB_BREP_DIR)/libbrep.a
LIB_MATH = $(LIB_MATH_DIR)/libmathpp.a
LIB_MESH = $(LIB_MESH_DIR)/libmesh.a


BSIC_LIB = -L$(LIB_BSIC_DIR) -lbasicpp -lm
EXTD_LIB = -L$(LIB_EXTD_DIR) -lextendpp
GRPH_LIB = -L$(LIB_GRPH_DIR) -lgraphpp -ljpeg 
BREP_LIB = -L$(LIB_BREP_DIR) -lbrep
MATH_LIB = -L$(LIB_MATH_DIR) -lmathpp
MESH_LIB = -L$(LIB_MESH_DIR) -lmesh


SUBJ = oarconv 

bin_PROGRAMS  = $(SUBJ)

oarconv_SOURCES = oarconv.cpp oarconv_help.cpp OARTool.cpp TerrainTool.cpp TreeTool.cpp MeshMaker.cpp PrimMesher.cpp PrimShape.cpp SculptMesher.cpp TerrainMesher.cpp
oarconv_LDADD = $(MESH_LIB) $(MATH_LIB) $(BREP_LIB) $(GRPH_LIB) $(EXTD_LIB) $(BSIC_LIB) $(OPJLIB)

#
#xmlcollada_SOURCES = xmlcollada.cpp
#xmlcollada_LDADD = $(MESH_LIB) $(MATH_LIB) $(BREP_LIB) $(GRPH_LIB) $(EXTD_LIB) $(BSIC_LIB)

#mktree_SOURCES = mktree.cpp
#mktree_LDADD = $(MESH_LIB) $(MATH_LIB) $(BREP_LIB) $(GRPH_LIB) $(EXTD_LIB) $(BSIC_LIB)



AM_CFLAGS  = -fPIC -I$(LIB_BSIC_DIR) -I$(LIB_BSIC_DIR)/Lib -I$(LIB_EXTD_DIR) -I$(LIB_EXTD_DIR)/xLib -I$(LIB_GRPH_DIR) -I$(LIB_BREP_DIR) -I$(LIB_MATH_DIR) -I$(LIB_MESH_DIR)
AM_CFLAGS += @ZLIB_INC_DIR@ @DEBUG@
AM_CFLAGS += -W -Wall

AM_CXXFLAGS = $(AM_CFLAGS)



################################################################################

$(LIB_BSIC):
	(cd  $(LIB_BSIC_DIR) && make)

$(LIB_EXTD):
	(cd  $(LIB_EXTD_DIR) && make)

$(LIB_GRPH):
	(cd  $(LIB_GRPH_DIR) && make)

$(LIB_BREP):
	(cd  $(LIB_BREP_DIR) && make)

$(LIB_MATH):
	(cd  $(LIB_MATH_DIR) && make)

$(LIB_MESH):
	(cd  $(LIB_MESH_DIR) && make)




################################################################################

install: $(SUBJ)
	$(INSTALL_PROGRAM) -m 0755 $(SUBJ) /usr/local/bin
	mkdir -p /usr/local/share/oarconv/assets
	chmod 0755 -R /usr/local/share/oarconv
	[ -d ./assets ] && $(INSTALL_PROGRAM) -m 0644 ./assets/* /usr/local/share/oarconv/assets
clean:
	rm -f *.o *~ $(SUBJ)  
 
clean_deps:
	-rm -rf .deps


