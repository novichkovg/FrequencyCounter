SRCDIR := src
OBJDIR := obj

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst %,$(OBJDIR)/%.o,$(basename $(SRCS)))
OBJS := $(subst src,,$(OBJS))

CXX := g++
CXXFLAGS := -std=c++17 -O3 -I$(SRCDIR)

$(shell mkdir -p $(dir $(OBJS)) >/dev/null)

freq: $(OBJS)
	@$(CXX) $^ -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@$(RM) -rf $(OBJDIR) freq
