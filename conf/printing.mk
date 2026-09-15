# ---------------------------------------------------------------------------- #
# Makefile template v 3.0                                           2026-09-12 #
# ---------------------------------------------------------------------------- #
#
#								PROJECT PRINTOUTS
#
# ---------------------------------------------------------------------------- #

# MODE_TXT ?= $(if $(filter reg, $(MODE)),regular,$(if $(filter asan,$(MODE)),sanitizer,valgrind))
MODE_TXT = $(strip \
  $(if $(filter reg,$(MODE)),regular, \
  $(if $(filter asan,$(MODE)),sanitizer,valgrind)) \
)

PART_TXT = $(if $(filter man,$(PART)),server,bot)

SRCLANG_TXT = $(SRCLANG_RESOLVED)

define PRT_BUILD
	@printf '\n'
 	@printf '$(C_AUTUMN_ORANGE)   building: '
	@printf '$(C_FUJI_WHITE)binary $(C_BOLD)$(PNAME)$(C_RESET_BOLD) '
	@printf '($(C_ITALIC)$(SRCLANG_TXT)$(C_RESET_ITALIC)) '
	@printf 'for $(C_ITALIC)$(MODE_TXT)$(C_RESET_ITALIC) use '
	@printf 'with $(C_ITALIC)$(PART_TXT)$(C_RESET_ITALIC) files.$(C_RESET)\n'
	@printf '$(C_FUJI_GRAY3)°°°°°°°°°°°°\n$(C_RESET)'
endef

define PRT_EXEC
	@printf '$(C_AUTUMN_ORANGE)  executing: '
	@printf '$(C_FUJI_WHITE)$(EXEC_STR)$(C_RESET)\n'
	@printf '$(C_FUJI_GRAY3)°°°°°°°°°°°°\n$(C_RESET)'
endef

# 	@printf '\n'
define PRT_VALG
	@printf '$(C_AUTUMN_ORANGE)  executing: '
	@printf '$(C_FUJI_WHITE)$(EXEC_V_STR)$(C_RESET)\n'
	@printf '$(C_FUJI_GRAY3)°°°°°°°°°°°°\n$(C_RESET)'
endef

define PRT_BOTH
	@printf '$(C_AUTUMN_ORANGE)  executing: '
	@printf '$(C_FUJI_WHITE)$(SERVER_BIN) $(PORT) $(PASSWORD) & '
	@printf '$(BOT_BIN) $(HOST) $(PORT) $(PASSWORD)$(C_RESET)\n'
	@printf '$(C_FUJI_GRAY3)°°°°°°°°°°°°\n$(C_RESET)'
endef

define PRT_BOTH_VALG
	@printf '$(C_AUTUMN_ORANGE)  executing: '
	@printf '$(C_FUJI_WHITE)valgrind ... $(SERVER_BIN) $(PORT) $(PASSWORD) & '
	@printf 'valgrind ... $(BOT_BIN) $(HOST) $(PORT) $(PASSWORD)$(C_RESET)\n'
	@printf '$(C_FUJI_GRAY3)°°°°°°°°°°°°\n$(C_RESET)'
endef
