# ---------------------------------------------------------------------------- #
# Makefile template v 3.0                                           2026-08-05 #
# ---------------------------------------------------------------------------- #
#
#							PROJECT BUILD RULES
#
# ---------------------------------------------------------------------------- #

# Linking
$(BIN): $(OBJ)
	@mkdir -p $(@D)
	@printf "$(C_WINTER_BLUE)"
	$(LINKER) $(LINKFLAGS) $(OBJ) $(CLIBS) -o $@
	@printf "$(C_RESET)"
	@if [ "$(MODE)" = "reg" ]; then cp $(BIN) .; fi

# Static patterns bind objects to the sources listed in SRC (not every
# matching file on disk). Needed when both foo.c and foo.cpp exist.
$(addprefix $(OBJ_DIR),$(SRC_C:.c=.o)): $(OBJ_DIR)%.o: $(SRC_ROOT)%.c
	@mkdir -p $(@D)
	@printf "$(C_SUMI_5)"
	$(CC) $(CFLAGS) $(CINCL) -MMD -MP -c $< -o $@
	@printf "$(C_RESET)"

$(addprefix $(OBJ_DIR),$(SRC_CPP:.cpp=.o)): $(OBJ_DIR)%.o: $(SRC_ROOT)%.cpp
	@mkdir -p $(@D)
	@printf "$(C_SUMI_5)"
	$(CXX) $(CXXFLAGS) $(CINCL) -MMD -MP -c $< -o $@
	@printf "$(C_RESET)"

# Ensure libft exists (if enabled)
$(LFT_DIR)$(LFT_NAME): libft

# Auto-generated header dependencies (from -MMD); covers .c and .cpp objects
-include $(OBJ:.o=.d)
