TARGET = project1

CFLAGS += -MMD -MP $(CCOMMONFLAGS)
CXXFLAGS += -MMD -MP $(CCOMMONFLAGS)

DEBUG ?= 0
DEBUG_GLIBCXX ?= 1
DEBUGOPT ?= 0
ASAN ?= 1
ifeq ($(DEBUG), 1)
	CCOMMONFLAGS += -DDEBUG=1 -fsanitize=undefined -fno-sanitize-recover=all -fstack-protector-all -Weverything -Wno-missing-variable-declarations -Wno-sign-conversion -pipe -g3 -march=native -Wno-padded -Wno-reserved-id-macro -Wno-disabled-macro-expansion
	CC = clang
	CFLAGS += -std=gnu18
	CXX = clang++
	CXXFLAGS += -Wno-global-constructors -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -std=c++17
	ifeq ($(DEBUG_GLIBCXX), 1)
		CXXFLAGS += -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
	endif
	ifeq ($(DEBUGOPT), 1)
		CCOMMONFLAGS += -D_FORTIFY_SOURCE=2 -Og -fno-omit-frame-pointer
	endif
	ifeq ($(ASAN), 1)
		CCOMMONFLAGS += -fsanitize=address -fsanitize-address-use-after-scope
	endif
	ifeq ($(COVERAGE), 1)
		CCOMMONFLAGS +=  -fprofile-instr-generate -fcoverage-mapping
	endif
	LDFLAGS = -fPIE -pie -Wl,-z,relro,-z,now
	LEXFLAGS += --debug
else
	CCOMMONFLAGS += -O3 -flto -s -W -Wall -Wextra -fprofile-use
	CC = gcc
	CFLAGS += -std=gnu18
	CXX = g++
	CXXFLAGS += -std=c++17
endif

SRCS = $(filter-out $(BLACKLIST), $(wildcard *.c) $(wildcard *.cc))
OBJS = $(addsuffix .o,$(basename $(SRCS)))
DEPS = $(OBJS:.o=.d)
DEPEND_NO_HEADER = $(filter-out $(wildcard *.h) $(wildcard *.hh), $^)
TAR_EXCL = $(foreach f, $(OUTPUT) $(DEPS) $(TAR_NAME) $(TAR_EXCL_EXTRA), --exclude=../$(shell basename ${PWD})/$(f))

### Custom Variables Begin ###

BLACKLIST =
TAR_EXCL_EXTRA = 
OBJS += 
OUTPUT = $(OBJS) $(TARGET)
STU_ID = b07902134
TAR_NAME = $(STU_ID)_ver$(VERSION).tar.bz2

### Custom Variables End ###

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(DEPEND_NO_HEADER)

#%.o: %.c
#	$(CC) $(CFLAGS) -c -o $@ $(DEPEND_NO_HEADER)
#
#%.o: %.cc
#	$(CXX) $(CXXFLAGS) -c -o $@ $(DEPEND_NO_HEADER)

tar:
	tar $(TAR_EXCL) -cpjf ../$(TAR_NAME) ../$(shell basename ${PWD})
	mv ../$(TAR_NAME) .

-include $(DEPS)

.PHONY: clean
clean:
	rm -f $(OUTPUT)

.PHONY: cleandep
cleandep:
	rm -f $(DEPS)

.PHONY: env
env:
	echo "export MALLOC_CHECK_=3 ASAN_OPTIONS=strict_init_order=true,detect_stack_use_after_return=true"

### Custom Rules Begin ###

### Custom Rules End ###
