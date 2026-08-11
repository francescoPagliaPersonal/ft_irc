# ---------------------------------------------------------------------------- #
# Makefile template v 3.0                                           2026-08-05 #
# ---------------------------------------------------------------------------- #
#
#								PROJECT CONTROL
#
# ---------------------------------------------------------------------------- #

GNU_RED := $(subst \033,$(shell printf '\033'),$(C_AUTUMN_RED))
GNU_WHITE := $(subst \033,$(shell printf '\033'),$(C_FUJI_WHITE))
GNU_ORANGE := $(subst \033,$(shell printf '\033'),$(C_AUTUMN_ORANGE))
GNU_BOLD := $(subst \033,$(shell printf '\033'),$(C_BOLD))
GNU_UL := $(subst \033,$(shell printf '\033'),$(C_UNDERLINE))
GNU_RESUL := $(subst \033,$(shell printf '\033'),$(C_RESET_UNDERLINE))
GNU_RESET := $(subst \033,$(shell printf '\033'),$(C_RESET))

# Ensure PROG_NAME is filled
ifeq ($(words $(PROG_NAME)),0)
$(strip $(error $(GNU_ORANGE)\
You must $(GNU_RED)set the $(GNU_BOLD)PROG_NAME$(GNU_RESET) \
$(GNU_ORANGE)variable))
endif

# Enforce one goal per invocation; prevents `make clean asan`
# There is no support for switching multiple modes at the same time.
ifneq ($(words $(MAKECMDGOALS)),0)
ifneq ($(words $(MAKECMDGOALS)),1)
$(strip $(error $(GNU_RED)\
$(GNU_ORANGE)Run with $(GNU_RED)$(GNU_UL)none$(GNU_RESET) \
$(GNU_ORANGE)or $(GNU_RED)$(GNU_UL)one$(GNU_RESUL) \
$(GNU_BOLD)target$(GNU_RESET) $(GNU_ORANGE)per invocation. \
$(GNU_RESET)Got: $(MAKECMDGOALS)))
endif
endif

# for executable targets
EXEC_STR	= ./$(BIN) $(RUN_ARGS_$(MODE))
EXEC_V_STR	= valgrind $(VALGRIND_FLAGS) ./$(BIN) $(RUN_ARGS_$(MODE))
END_STR		:= printf '$(C_FUJI_GRAY3)::::::::::::\n$(C_RESET)'

define EXEC
	$(PRT_EXEC)
	@$(EXEC_STR)
	@$(END_STR)
endef
define EXEC_VALG
	$(PRT_VALG)
	@$(EXEC_V_STR)
	@$(END_STR)
endef
