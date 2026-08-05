# ---------------------------------------------------------------------------- #
# Makefile template v 3.0                                           2026-08-05 #
# ---------------------------------------------------------------------------- #
#
#									LIBFT
#
# ---------------------------------------------------------------------------- #

# libft (pure C only — ignored when SRCLANG_RESOLVED is cpp or mixed)
LFT_DIR		= $(LFT_ROOT)lib/$(MODE)/
LFT_NAME	:= libft.a

# Construct or remove libft dependency references.
# USE_LIBFT enables the feature; SRCLANG_RESOLVED=c activates it at build time.
ifeq ($(USE_LIBFT),1)
	LFT_DEP		= $(if $(filter c,$(SRCLANG_RESOLVED)),$(LFT_DIR)$(LFT_NAME),)
	CLIBS		= $(if $(filter c,$(SRCLANG_RESOLVED)),$(LFT_DIR)$(LFT_NAME) $(LIBS),$(LIBS))
	INC_PATHS	+= $(LFT_ROOT)include/
	NORM_HEADERS	+= $(LFT_ROOT)include/libft.h
	LFT_CLEAN	= $(if $(filter c,$(SRCLANG_RESOLVED)),@$(MAKE) -C $(LFT_ROOT) clean --no-print-directory,)
	LFT_FCLEAN	= $(if $(filter c,$(SRCLANG_RESOLVED)),@$(MAKE) -C $(LFT_ROOT) fclean --no-print-directory,)
	LFT_PRINT	= $(if $(filter c,$(SRCLANG_RESOLVED)),@$(MAKE) -C $(LFT_ROOT) print --no-print-directory,)
	LFT_NORM	= $(if $(filter c,$(SRCLANG_RESOLVED)),@$(MAKE) -C $(LFT_ROOT) norm --no-print-directory,)
else
	CLIBS	:= $(LIBS)
endif

# Warn when USE_LIBFT is on but language is not pure C
define WARN_LIBFT
$(if $(and $(filter 1,$(USE_LIBFT)),$(filter-out c,$(SRCLANG_RESOLVED))),\
	@printf '$(C_AUTUMN_ORANGE)warning: USE_LIBFT=1 ignored (libft is pure-C only; SRCLANG=$(SRCLANG_RESOLVED))$(C_RESET)\n')
endef
