# ---------------------------------------------------------------------------- #
# Makefile template v 3.1                                           2026-09-12 #
# ---------------------------------------------------------------------------- #
#
#							   PROJECT BUILD TARGETS
#
# ---------------------------------------------------------------------------- #

all:		MODE := reg
all:		PART := man
all:		build

nc:			EXEC_STR = nc -C $(HOST) $(PORT)
nc:
	$(EXEC)

chat:		EXEC_STR = irssi -c $(HOST) -p $(PORT) -w $(PASSWORD) -n $(NICK)
chat:
	$(EXEC)

bonus:		MODE := reg
bonus:		PART := bon
bonus:		build

run:		MODE := reg
run:		PART := man
run:		build
	$(EXEC)

runb:		MODE := reg
runb:		PART := bon
runb:		build
	$(EXEC)

bot:		runb

asan:		MODE := asan
asan:		PART := man
asan:		build
	$(EXEC)

asanb:		MODE := asan
asanb:		PART := bon
asanb:		build
	$(EXEC)

val:		MODE := val
val:		PART := man
val:		build
	$(EXEC_VALG)

valb:		MODE := val
valb:		PART := bon
valb:		build
	$(EXEC_VALG)

both:
	@$(MAKE) build MODE=$(MODE) PART=man SRCLANG=$(SRCLANG) --no-print-directory
	@$(MAKE) build MODE=$(MODE) PART=bon SRCLANG=$(SRCLANG) --no-print-directory

both-run:	MODE := reg
both-run:	both
	$(EXEC_BOTH)

both-asan:	MODE := asan
both-asan:	both
	$(EXEC_BOTH)

both-val:	MODE := val
both-val:	both
	$(EXEC_BOTH_VALG)

print:
	@printf "$(C_SAKURA_BLOSSOM)"
	@printf '%s\n' '---BUILDING---'
	@printf '%s\n' "$(PROG_NAME)"
	@printf '%s\n' '---SRC ROOT---'
	@printf '%s\n' "$(SRC_ROOT)"
	@printf '%s\n' '---SRC_MAN---'
	@printf '%s\n' "$(SRC_MAN)"
	@printf '%s\n' '---SRC_BON---'
	@printf '%s\n' "$(SRC_BON)"
	@printf '%s\n' '---SRCLANG---'
	@printf '%s\n' "$(SRCLANG) -> $(SRCLANG_RESOLVED)"
	@printf '%s\n' '---LINKER---'
	@printf '%s\n' "$(LINKER)"
	@printf '%s\n' '---OBJ_DIR---'
	@printf '%s\n' "$(OBJ_DIR)"
	@printf '%s\n' '---CFLAGS---'
	@printf '%s\n' "$(CFLAGS)"
	@printf '%s\n' '---CXXFLAGS---'
	@printf '%s\n' "$(CXXFLAGS)"
	@printf '%s\n' '---CLIBS---'
	@printf '%s\n' "$(CLIBS)"
	@printf '$(C_RESET)'
	$(LFT_PRINT)

clean:
	@rm -rf build
	@printf "$(C_BAMBOO_GREEN)$(PROG_NAME) clean complete$(C_RESET)\n"
	$(LFT_CLEAN)

fclean:		clean
	@rm -rf bin $(PROG_NAME) $(PROG_NAME_BONUS)
	@printf "$(C_BAMBOO_GREEN)$(PROG_NAME) fclean complete$(C_RESET)\n"
	$(LFT_FCLEAN)

re:		fclean all

# the second make call assures that MODE and PART change affect SRC and OBJ
# (make expands dependencies right away and does not adapt them on later change)
build: prt $(LFT_DEP)
	@printf "$(C_BAMBOO_GREEN)"
	@$(MAKE) -j$(JOBS) -Otarget $(BIN) MODE=$(MODE) PART=$(PART) SRCLANG=$(SRCLANG) --no-print-directory
	@printf "$(C_RESET)"
	@printf '\n'

prt: 
	$(CHECK_LANG)
	$(WARN_LIBFT)
	$(PRT_BUILD)

# this target ensures that change in libft is detected
ifeq ($(USE_LIBFT),1)
libft:
	@if [ "$(SRCLANG_RESOLVED)" = "c" ]; then \
		$(MAKE) -C $(LFT_ROOT) MODE=$(MODE) --no-print-directory; \
	else \
		printf "$(C_AUTUMN_ORANGE)warning: USE_LIBFT=1 ignored "; \
		printf "(libft is pure-C only; SRCLANG=$(SRCLANG_RESOLVED))$(C_RESET)\n"; \
	fi
else
libft:
	@printf "$(C_FUJI_WHITE) There is $(C_AUTUMN_RED)$(C_BOLD)no libft "
	@printf "$(C_RESET_BOLD)$(C_FUJI_WHITE)support enabled.$(C_RESET)\n"
endif

norm:
#	$(LFT_NORM)
	@norminette -R CheckForForbiddenHeader \
	$(addprefix $(SRC_ROOT),$(filter %.c,$(SRC_MAN) $(SRC_BON))) \
	$(NORM_HEADERS)

.PHONY:	all bonus clean fclean re \
		run runb asan asanb val valb bot \
		both both-run both-asan both-val \
		print build libft norm \
		nc chat
