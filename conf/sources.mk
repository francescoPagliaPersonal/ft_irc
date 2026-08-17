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
SRC_MAN		:=  arg2password.cpp arg2port.cpp\
				Server.cpp Server-CDTOR.cpp Server-Run.cpp Server-Handlers.cpp\
				Server-Clients.cpp Server-Channels.cpp Server-Signals.cpp\
				Server-Commands.cpp Server-procInputBuffer.cpp\
				Epoll.cpp ListeningSocket.cpp ListeningSocket-CDTOR.cpp\
				Client.cpp Client-getMessages.cpp\
				Message.cpp\
				Command.cpp CommandRegistry.cpp\
				CommandRegistry-RegisterCmds.cpp\
				CommandRegistry-handleProtocolErrors.cpp\
				Channel.cpp\
				main.cpp
SRC_BON		:= 

# Pattern for source files in subdirectories. WITH DIR SLASH
#   DIR_PARSER = parser/
#   SRC_PARSER = tokenize.c parse.c
#   SRC_MAN   += $(addprefix $(DIR_PARSER),$(SRC_PARSER))
# Mixed example: SRC_MAN := main.cpp util.c

# Commands
DIR_CMDS = commands/
SRC_CMDS = cmd_cap.cpp cmd_nick.cpp cmd_pass.cpp cmd_user.cpp cmd_ping.cpp
SRC_MAN   += $(addprefix $(DIR_CMDS),$(SRC_CMDS))

# Policies
DIR_PLCY = policies/
SRC_PLCY = AlreadyRegisteredPlcy.cpp ArgsLimitPlcy.cpp 
SRC_MAN   += $(addprefix $(DIR_PLCY),$(SRC_PLCY))
