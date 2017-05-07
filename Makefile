CXXFLAGS     =	-O2 -g -Wall -Werror -fmessage-length=0
LIBS         =

BUILD_FOLDER := ./build/

##########################################################

all:
	mkdir -p $(BUILD_FOLDER)
	@echo "\n========== pre_compile ==========\n"
	$(MAKE) pre_compile
	@echo "\n========== compile ==========\n"
	$(MAKE) compile
	@echo "\n========== post_compile ==========\n"
	$(MAKE) post_compile
	@echo "\n========== pre_compile_test ==========\n"
	$(MAKE) pre_compile_test
	@echo "\n========== compile_test ==========\n"
	$(MAKE) compile_test
	@echo "\n========== post_compile_test ==========\n"
	$(MAKE) post_compile_test
	@echo "\n========== pre_do_test ==========\n"
	$(MAKE) pre_do_test
	@echo "\n========== do_test ==========\n"
	$(MAKE) do_test
	@echo "\n========== post_do_test ==========\n"
	$(MAKE) post_do_test

##########################################################

SOURCES_INCLUDE  := ./src/include
SOURCES_HPP      := $(shell find src/include -name "*.hpp")
SOURCES_CPP      := $(shell find src/cpp -name "*.cpp")
OBJS             := $(subst src, $(BUILD_FOLDER)/src, $(SOURCES_CPP:.cpp=.o))

pre_compile:

compile: $(OBJS)

post_compile:

$(BUILD_FOLDER)/src/%.o: src/%.cpp $(SOURCES_HPP)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(SOURCES_INCLUDE) -c $< -o $@


##########################################################

GTEST_DIR    := test/googletest/
GMOCK_DIR    := test/googlemock/
TEST_CPP     := $(shell find test/ -type f -name "*.cpp" ! -path "*$(GTEST_DIR)*" ! -path "*$(GMOCK_DIR)*")
TEST_OBJS    := $(subst test, $(BUILD_FOLDER)/test, $(TEST_CPP:.cpp=.o))

pre_compile_test: 
	mkdir -p $(BUILD_FOLDER)/test/

compile_test: $(BUILD_FOLDER)/test/unit_test

post_compile_test:

$(BUILD_FOLDER)/test/unit_test: $(TEST_OBJS) $(BUILD_FOLDER)/test/gtest-all.o $(BUILD_FOLDER)/test/gmock-all.o
	$(CXX) $(CXXFLAGS) -I$(SOURCES_INCLUDE) -I$(GTEST_DIR)/include -I$(GMOCK_DIR)/include -c $(GMOCK_DIR)/src/gmock_main.cc -o $(BUILD_FOLDER)/test/gmock_main.o
	$(CXX) $(CXXFLAGS) -o $@ -pthread \
		$(OBJS) $(TEST_OBJS) \
		$(BUILD_FOLDER)/test/gmock_main.o \
		$(BUILD_FOLDER)/test/gtest-all.o \
		$(BUILD_FOLDER)/test/gmock-all.o \
		$(LIBS)

$(BUILD_FOLDER)/test/%.o: test/%.cpp $(SOURCES_HPP) $(OBJS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(SOURCES_INCLUDE) -I$(GTEST_DIR)/include -I$(GMOCK_DIR)/include -c $< -o $@


$(BUILD_FOLDER)/test/gtest-all.o: $(shell find $(GTEST_DIR) -name "*")
	$(CXX) $(CXXFLAGS) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc \
		-o $(BUILD_FOLDER)/test/gtest-all.o

$(BUILD_FOLDER)/test/gmock-all.o: $(shell find $(GTEST_DIR) -name "*") $(shell find $(GMOCK_DIR) -name "*")
	$(CXX) $(CXXFLAGS) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc \
		-o $(BUILD_FOLDER)/test/gmock-all.o
	

##########################################################

pre_do_test:

do_test:
	@cd $(BUILD_FOLDER)/test/ && \
		./unit_test --gtest_color=yes --gtest_output=xml \
		--gtest_filter=ExpressionGrammarTest._all \
		| tee unit_test.stdout

post_do_test:

##########################################################

clean:
	rm -r $(BUILD_FOLDER)
