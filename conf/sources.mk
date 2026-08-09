# ---------------------------------------------------------------------------- #
# Makefile template v 3.0                                           2026-08-05 #
# ---------------------------------------------------------------------------- #
#
#								PROJECT SOURCES
#
# ---------------------------------------------------------------------------- #

# Root directory that contains the source files.
SRC_ROOT		:= source/
# Include directories (space-separated). Used to build CINCL flags.
INC_PATHS		:= include/

# Optional headers for norminette only (rebuilds use compiler .d files).
# libft header is added automatically when USE_LIBFT=1
NORM_HEADERS	:= include/$(PROG_NAME).hpp

# Give File names relative to SRC_ROOT directory (.c and/or .cpp; mixed OK)
SRC_MAN		:= main.cpp Server.cpp
SRC_BON		:= 

# Pattern for source files in subdirectories. WITH DIR SLASH
#   DIR_PARSER = parser/
#   SRC_PARSER = tokenize.c parse.c
#   SRC_MAN   += $(addprefix $(DIR_PARSER),$(SRC_PARSER))
# Mixed example: SRC_MAN := main.cpp util.c
