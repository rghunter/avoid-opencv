GTEST_VERSION = 1.7.0

GTEST_DIR = gtest-$(GTEST_VERSION)
LIBS = $(addprefix $(GTEST_DIR)/lib/, (libgtest.la,libgtest_main.la))

all : $(LIBS)

$(LIBS) : $(GTEST_DIR)/config.log
	$(MAKE) -j12 -C $(GTEST_DIR)

$(GTEST_DIR)/config.log : $(GTEST_DIR)
	cd $(GTEST_DIR); \
		./configure

$(GTEST_DIR) : gtest-$(GTEST_VERSION).zip
	unzip gtest-1.7.0.zip
