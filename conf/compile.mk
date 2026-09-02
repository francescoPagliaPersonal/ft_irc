# ---------------------------------------------------------------------------- #
# Makefile template v 3.1                                           2026-09-02 #
# ---------------------------------------------------------------------------- #
#
#							PROJECT COPMILE DATA
#
# ---------------------------------------------------------------------------- #

CWWW	:= -Wall -Werror -Wextra
CXXWWW	:= -Wall -Wextra -Werror -Wpedantic -Wshadow -std=c++98
CINCL	= $(addprefix -I,$(INC_PATHS))

# Mode-specific flags (shared by C and C++)
C_FLAGS_reg		:=
C_FLAGS_asan	:= -g -fsanitize=address -fno-omit-frame-pointer -O0 -DDEBUG=1
C_FLAGS_val		:= -g -O0 -DDEBUG=2

# Bonus flag: automatically added when PART=bon
BONUS_DEFINE := $(if $(filter bon,$(PART)),-DBONUS=1)

# Final flags used for compilation/linking (recursive; MODE is target-specific)
# -fPIE: required so C objects link cleanly when LINKER is c++ (mixed)
CFLAGS		= $(CWWW) $(CFLAGS_OTHER) $(C_FLAGS_$(MODE)) $(BONUS_DEFINE) \
			  $(if $(filter mixed,$(SRCLANG_RESOLVED)),-fPIE,)
CXXFLAGS	= $(CXXWWW) $(CXXFLAGS_OTHER) $(C_FLAGS_$(MODE)) $(BONUS_DEFINE)
