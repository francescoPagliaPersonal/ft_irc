# ---------------------------------------------------------------------------- #
# Standalone C++98 test runner for ft_irc
# ---------------------------------------------------------------------------- #
#   make          compile -g -O0 and run
#   make asan     AddressSanitizer build and run
#   make val      Valgrind build and run
#   make clean    remove tests/unit/build/
#   make fclean   also remove test_irc
# ---------------------------------------------------------------------------- #

CXX			:= c++
PROG_NAME	:= ircserv
JOBS		?= $(shell nproc --ignore=1)

include ../../conf/sources.mk

SRC_ROOT	:= ../../source/
INC_PATHS	:= ../../include/

PROD_SRC	:= $(filter-out main.cpp,$(SRC_MAN))

TEST_SRC	:= main.cpp \
				test_args.cpp \
				test_message.cpp \
				test_policies.cpp \
				test_commands.cpp \
				test_irc_helpers.cpp \
				test_channel.cpp \
				test_join.cpp \
				test_invite.cpp \
				test_mode.cpp \
				test_quit.cpp \
				test_topic.cpp \
				test_framing.cpp

MODE		?= reg
OBJ_DIR		:= build/$(MODE)/
BIN			:= test_irc

PROD_OBJ	:= $(addprefix $(OBJ_DIR)prod/,$(PROD_SRC:.cpp=.o))
TEST_OBJ	:= $(addprefix $(OBJ_DIR)harness/,$(TEST_SRC:.cpp=.o))

CXXWWW		:= -Wall -Wextra -Werror -Wpedantic -Wshadow -std=c++98
CINCL		:= $(addprefix -I,$(INC_PATHS)) -I.
CXXFLAGS_reg	:= -g -O0
CXXFLAGS_asan	:= -g -fsanitize=address -fno-omit-frame-pointer -O0
CXXFLAGS_val	:= -g -O0
CXXFLAGS	:= $(CXXWWW) $(CXXFLAGS_$(MODE)) $(CINCL)

LDFLAGS_asan	:= -fsanitize=address
LDFLAGS		:= $(LDFLAGS_$(MODE))

VALGRIND_FLAGS	:= --leak-check=full \
				--show-leak-kinds=all --errors-for-leak-kinds=all \
				--show-error-list=yes \
				--track-origins=yes --track-fds=yes

.PHONY: all asan val clean fclean run runval build

all:
	@$(MAKE) run MODE=reg

asan:
	@$(MAKE) run MODE=asan

val:
	@$(MAKE) runval MODE=val

# Second make call so MODE is set before OBJ/flags expand, and compile in parallel.
build:
	@$(MAKE) -j$(JOBS) -Otarget $(BIN) MODE=$(MODE) --no-print-directory

$(BIN): $(PROD_OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(PROD_OBJ) $(TEST_OBJ) -o $@

$(OBJ_DIR)prod/%.o: $(SRC_ROOT)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)harness/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: build
	./$(BIN)

runval: build
	valgrind $(VALGRIND_FLAGS) ./$(BIN)

clean:
	rm -rf build

fclean: clean
	rm -f $(BIN)

-include $(PROD_OBJ:.o=.d)
-include $(TEST_OBJ:.o=.d)
