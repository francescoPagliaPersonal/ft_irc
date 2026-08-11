# ---------------------------------------------------------------------------- #
# Makefile template v 3.0                                           2026-08-05 #
# ---------------------------------------------------------------------------- #
#
#								PROJECT COLORS
#
# ---------------------------------------------------------------------------- #

#  RESET / BASE

C_RESET       := \033[0m
C_BOLD         := \033[1m
C_DIM          := \033[2m
C_ITALIC       := \033[3m
C_UNDERLINE    := \033[4m
C_BLINK        := \033[5m
C_REVERSE      := \033[7m
C_HIDDEN       := \033[8m
C_STRIKE       := \033[9m
C_DUNDERLINE   := \033[21m
C_RESET_BOLD       := \033[22m
C_RESET_DIM        := \033[22m
C_RESET_ITALIC     := \033[23m
C_RESET_UNDERLINE  := \033[24m
C_RESET_BLINK      := \033[25m
C_RESET_REVERSE    := \033[27m
C_RESET_HIDDEN     := \033[28m
C_RESET_STRIKE     := \033[29m

#  SUMI INK (background tones from darkest to lightest)

C_SUMI_0      := \033[38;2;17;17;27m
C_SUMI_1      := \033[38;2;22;22;29m
C_SUMI_2      := \033[38;2;36;40;59m
C_SUMI_3      := \033[38;2;54;58;79m
C_SUMI_4      := \033[38;2;65;69;89m
C_SUMI_5      := \033[38;2;74;78;100m

#  FUJI (foreground text from brightest to darkest)

C_FUJI_WHITE  := \033[38;2;235;219;178m
C_FUJI_GRAY   := \033[38;2;198;188;167m
C_FUJI_GRAY2  := \033[38;2;169;159;139m
C_FUJI_GRAY3  := \033[38;2;147;137;117m

#  WAVE BLUES (signature from darker to brighter)

C_WAVE_BLUE1  := \033[38;2;38;66;86m
C_WAVE_BLUE2  := \033[38;2;65;90;119m
C_DRAGON_BLUE  := \033[38;2;72;103;131m
C_WINTER_BLUE  := \033[38;2;110;134;161m

#  WARM ACCENTS

C_AUTUMN_RED    := \033[38;2;223;113;113m
C_AUTUMN_YELLOW := \033[38;2;224;175;104m
C_AUTUMN_GREEN  := \033[38;2;152;195;121m
C_AUTUMN_ORANGE := \033[38;2;255;158;100m
C_AUTUMN_PURPLE := \033[38;2;198;160;246m

#  SAKURA / LOTUS ACCENTS

C_SAKURA_PINK     := \033[38;2;223;142;155m
C_SAKURA_BLOSSOM  := \033[38;2;255;204;212m

#  NATURAL GREENS

C_BAMBOO_GREEN  := \033[38;2;133;153;0m
C_SPRING_GREEN  := \033[38;2;152;195;121m
C_MOSS_GREEN    := \033[38;2;106;153;85m
