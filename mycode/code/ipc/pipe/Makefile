VER=1.0.0
PTRADE_VERSION=PTRADE_1_61 ## PTRADE_1_59 | PTRADE_1_61 | PTRADE_NG_1_61

EXE1=pipe_run
DFLAGS += -DDEBUG
LIBS = -lpthread 

#FRAME=../frame
#DIR_LIST += $(FRAME)/interface $(FRAME)/protocol $(FRAME)/net $(FRAME)/mdb $(FRAME)/base $(FRAME)/field
DIR_LIST += ./

###############################################################
################# common frame ################################
###############################################################
OS=$(shell uname)$(shell uname -a | sed -n '1p' | perl -nle 'print $$1 if /\s+([0-9]\.\d+)/')
GCC=$(shell gcc --version | sed -n '1p' | perl -nle 'print $$1 if /\s+([0-9]\.\d+)/')
VER_PT=$(shell bit=`getconf LONG_BIT`;if [ $$bit -eq 64 ];  then echo 'X64'; else echo 'X32'; fi;)
CC=g++

CODE_LEN_PATH=./src
OutPut=build/self/uni_test

#SOURCE CODE
CC_SRC=$(shell find  $(DIR_LIST)    -name "*.cc" )
CC_SRC2=$(shell find  $(DIR_LIST)   -name "*.c" )
CC_SRC3=$(shell find  $(DIR_LIST)   -name "*.cpp" )

#OBJECTS
CC_OBJS=$(patsubst %.cc,./$(OutPut)/%.o,$(CC_SRC))
CC_OBJS2=$(patsubst %.c,./$(OutPut)/%.o,$(CC_SRC2))
CC_OBJS3=$(patsubst %.cpp,./$(OutPut)/%.o,$(CC_SRC3))
OBJS=$(CC_OBJS) $(CC_OBJS2) $(CC_OBJS3)

define OBJ_MKDIR
  OBJ_DIRS+=./$(OutPut)/$(1)
endef

CC_DIRS=$(shell find $(DIR_LIST) -type d|sed -e '/.svn/d')
$(foreach dir,$(CC_DIRS),$(eval $(call OBJ_MKDIR,$(dir))))

#DEPS
DEPS=$(patsubst %.o,%.d,$(OBJS))

define SAFE_MKDIR
  INC_DIR+=-I $(1)
endef
$(foreach dir,$(DIR_LIST),$(eval $(call SAFE_MKDIR,$(dir))))


ifeq "$(OS)" "Darwin"
	DFLAGS += -DMAC
	CFLAGS=-Wno-invalid-source-encoding -Wno-deprecated-declarations -Wno-undefined-var-template
else
	DFLAGS  += -DLINUX
	LDFLAGS =  -lrt -ldl
	#--export-dynamic
	CFLAGS  = -msse4
endif

DFLAGS += -DTIXML_USE_STL -DGCC
DFLAGS += -D$(PTRADE_VERSION)
LDFLAGS+=$(LIB_DIR) $(LIBS)
#-Wl
CPPFLAGS+=$(INC_DIR) $(DFLAGS) -std=c++0x
CFLAGS += -g -O3 -fPIC -Wall -Wno-unused-function -Wno-strict-aliasing


all:$(EXE1)
$(shell mkdir -p $(sort $(OBJ_DIRS)))
-include $(DEPS)

$(EXE1):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


./$(OutPut)/%.o:%.cc
	$(CC) -o $@ $(CFLAGS) -c $< $(CPPFLAGS)
./$(OutPut)/%.d:%.cc
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(patsubst %.d,%.o,$@) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

./$(OutPut)/%.o:%.cpp
	$(CC) -o $@ $(CFLAGS) -c $< $(CPPFLAGS)
./$(OutPut)/%.d:%.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(patsubst %.d,%.o,$@) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

./$(OutPut)/%.o:%.c
	$(CC) -o $@ $(CFLAGS) -c $< $(CPPFLAGS)
./$(OutPut)/%.d:%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(patsubst %.d,%.o,$@) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	rm -rf build
	rm -rf $(EXE1)
codelen:
	find $(CODE_LEN_PATH) \( -name "*.cc" -o  -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -exec cat {} \;|sed -e 's/\"/\n\"\n/g;s/\([^\/]\)\(\/\*\)/\1\n\2\n/g;'|sed  -e '/^\"/{:a;N;/\".*\"/!ba;s/\".*\".*//;N;/\"/!d;b}' -e '/^\/\*/{s/\/\*.*\*\///;/\/\*/{:b;N;/\/\*.*\*\//!bb;s/\/\*.*\*\///}}' -e 's/\/\/.*//g' |sed -e '/^[[:space:]]*$$/d'|wc -l
srczip:
	zip -r ./$(EXE1)_src_$(VER).zip * -x *.o *.d *.svn *.zip *.a *.so $(EXE1) *.svn-work *.svn-base *.so.* *.d.* *.svn

binzip:
	if [ ! -d "release" ];  then mkdir release; fi;
	mkdir -p ./release/plugin/iv_algorithm;
	mkdir -p ./release/plugin/spot_algorithm;
	mkdir -p ./release/plugin/theoretical_algorithm;
	cp $(EXE1) ./release/
	cp ./readme.md ./release/
	cp ./config.xml ./release/

	ldd $(EXE1)>sys_so_111_232_876_23;cp `awk  '{if(substr($$3,1,4)!="/lib"&&substr($$3,1,8)!="/usr/lib")print $$3}' sys_so_111_232_876_23` ./release/lib/;rm -rf sys_so_111_232_876_23
	cd release; zip -r ../$(EXE1)_$(VER)_$(OS)_GCC$(GCC)_$(VER_PT).zip *
	rm -rf release