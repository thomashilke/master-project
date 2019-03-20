.SUFFIX:

include config.mk

OBJECTS = $(patsubst %.cpp,build/%.o,$(SOURCES))
DEPS = $(patsubst %.cpp,build/%.deps,$(SOURCES))

.PHONY = all deps clean install
.DEFAULT_GOAL = all

all: $(BIN) $(LIB) $(HEADERS)

-include $(DEPS)

$(HEADERS): include/lexer/%: src/%
	@echo "[INST]" $(<:src/%=%)
	@install -m 0644 -D $< $@

$(OBJECTS): build/%.o: %.cpp
	@echo "[CXX] " $@
	@$(MKDIR) $(MKDIRFLAGS) $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DEPS): build/%.deps: %.cpp
	@echo "[DEPS]" $@
	@$(MKDIR) $(MKDIRFLAGS) $(dir $@)
	@$(DEPS_BIN) $(DEPSFLAGS) -std=c++11 -MM -MT build/$*.o $< > $@
	@$(DEPS_BIN) $(DEPSFLAGS) -std=c++11 -MM -MT build/$*.deps $< >> $@

$(BIN): bin/%:
	@echo "[LD]  " $@
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIB)

$(LIB): lib/%:
	@echo "[AR]  " $@
	@$(AR) $(ARFLAGS) $@ $^

deps: $(DEPS)

clean:
	@rm -f $(OBJECTS)
	@rm -f $(DEPS)
	@rm -f $(BIN)
	@rm -rf build/*
	@rm -rf include/*
	@rm -f $(LIB)

install: $(BIN) $(HEADERS) $(LIB)
	cp $(BIN) $(PREFIX)/$(BIN_DIR)/
	cp -r include/* $(PREFIX)/$(INCLUDE_DIR)/
	cp $(LIB) $(PREFIX)/$(LIB_DIR)/

print-%:
	@echo $*=$($*)
