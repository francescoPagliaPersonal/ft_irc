# ---------------------------------------------------------------------------- #
# Makefile template v 3.1                                           2026-08-25 #
# ---------------------------------------------------------------------------- #
# Features:
# - C, C++, and mixed projects (SRCLANG=auto|c|cpp|mixed)
# - Compiler-generated dependency files (-MMD -MP)
# - Multiple mode support (reg, asan, val)
# - Only one mode per invocation allowed						 __(°)<
# - Separate mandatory vs bonus part builds						 \_)_)
# - Easy to use: make run/runb/asan/asanb/val/valb				~~~~~~
# - Supports nested source subdirectories
# - Optional libft (pure C only)
# ---------------------------------------------------------------------------- #
#
#							PROJECT DATA INPUT
#
# ---------------------------------------------------------------------------- #

# Name of the executable
PROG_NAME		:= ircserv

# Makefile configuration
MK_DIR			:= conf/
JOBS			?= $(shell nproc --ignore=1)

include $(MK_DIR)sources.mk

# ---------------------------------------------------------------------------- #

# Source language: auto | c | cpp | mixed (do not use LANG — that is the locale)
SRCLANG			?= auto

# Compiler
CC				:= cc
CXX				:= c++

# only other special flags that are not -W..., sanitizer or valgrind!
CFLAGS_OTHER	:=
CXXFLAGS_OTHER	:=

# standard libraries (NOT LIBFT!) go here: -lname -lname
LIBS			:= 
# to use libft set to 1 otherwise to 0 (pure C projects only)
USE_LIBFT		?= 0
LFT_ROOT		:= lib/libft/

# ----------------------------------------------------------------------------
# arguments for executing the program in the various modes
PORT			:= 6669
PASSWORD		:= 1o.0
HOST			:= localhost
NICK			:= BugDetector

RUN_ARGS_reg	:= $(PORT) $(PASSWORD)
RUN_ARGS_asan	:= $(PORT) $(PASSWORD)
RUN_ARGS_val	:= $(PORT) $(PASSWORD)
# ----------------------------------------------------------------------------
VALGRIND_FLAGS	:=	--leak-check=full \
					--show-leak-kinds=all --errors-for-leak-kinds=all\
					--show-error-list=yes\
					--track-origins=yes --track-fds=yes

#	--errors-for-leak-kinds=all		
#	--show-error-list=yes			
#	--leak-check=full				
#	--show-leak-kinds=all			catch still reachable
#	--track-fds=yes					
#	--track-origins=yes				uninitialized reads, slow
#	--trace-children=yes			
#	--trace-children-skip=""		
#	--suppressions=					
#	--num-callers=20				longer stack traces
#	--quiet
#	--log-file=valgrind.log
#	--read-var-info=yes				slow
#	--tool=helgrind					threads data race 

# ----------------------------------------------------------------------------

include $(MK_DIR)colors.mk
include $(MK_DIR)control.mk
include $(MK_DIR)selection.mk
include $(MK_DIR)libft.mk
include $(MK_DIR)compile.mk
include $(MK_DIR)targets.mk
include $(MK_DIR)rules.mk
include $(MK_DIR)printing.mk

# ---------------------------------------------------------------------------- #
# Assignment operators (quick reference)
#   :=   expand once (defaults / constants)
#   =    re-expand on use (MODE/PART/SRCLANG-dependent values)
#   ?=   set only if undefined (here: USE_LIBFT, SRCLANG)
#   +=   append (keeps existing := or = flavor)
# ---------------------------------------------------------------------------- #
