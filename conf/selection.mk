# ---------------------------------------------------------------------------- #
# Makefile template v 3.0                                           2026-08-05 #
# ---------------------------------------------------------------------------- #
#
#								BUILD SELECTION
#
# ---------------------------------------------------------------------------- #

# MODE affects flags + output directories; regular, sanitizer, valgrind
MODE	:= reg
# PART affects which sources are compiled; mandatory or bonus
PART	:= man
# SRCLANG (not LANG — that is the locale env var): auto | c | cpp | mixed
SRCLANG	?= auto

# Output layout to isolate objects and binaries per mode & part.
# NOTE: must be recursive (=), because MODE/PART are target-specific and change.
OBJ_DIR	= build/$(MODE)/$(PART)/
BIN_DIR	= bin/$(MODE)/$(PART)/
BIN		= $(BIN_DIR)$(PROG_NAME)

# Choose sources based on PART.
# NOTE: cannot use make directives (ifeq)
#		because they are set while reading not when builing
#		=> PART is target specific and can change
SRC	= $(if $(filter bon,$(PART)),$(SRC_BON),$(SRC_MAN))

# Objects from .c or .cpp (basename strips either extension)
OBJ	= $(addprefix $(OBJ_DIR),$(addsuffix .o,$(basename $(SRC))))

# Language detection from current SRC (depends on PART)
SRC_C		= $(filter %.c,$(SRC))
SRC_CPP		= $(filter %.cpp,$(SRC))
SRC_OTHER	= $(filter-out %.c %.cpp,$(SRC))

SRCLANG_DETECTED = $(strip \
	$(if $(SRC_OTHER),unknown,\
	$(if $(and $(SRC_C),$(SRC_CPP)),mixed,\
	$(if $(SRC_CPP),cpp,\
	$(if $(SRC_C),c,empty)))))

SRCLANG_RESOLVED = $(if $(filter auto,$(SRCLANG)),$(SRCLANG_DETECTED),$(SRCLANG))

# Link with cc only for pure C; otherwise c++
LINKER		= $(if $(filter c,$(SRCLANG_RESOLVED)),$(CC),$(CXX))
LINKFLAGS	= $(if $(filter c,$(SRCLANG_RESOLVED)),$(CFLAGS),$(CXXFLAGS))

# Validate SRCLANG after PART/SRC are known (expanded from recipes)
define CHECK_LANG
$(if $(filter empty,$(SRCLANG_DETECTED)),$(error \
No source files in SRC for PART=$(PART)))
$(if $(filter unknown,$(SRCLANG_DETECTED)),$(error \
Unsupported source extension(s) in SRC: $(SRC_OTHER) (use .c or .cpp)))
$(if $(filter-out auto,$(SRCLANG)),\
$(if $(filter $(SRCLANG),$(SRCLANG_DETECTED)),,\
$(error SRCLANG=$(SRCLANG) does not match sources ($(SRCLANG_DETECTED)))))
endef
