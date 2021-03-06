# Makefile for the mqttpp sample applications

PAHO_C_LIB ?= /usr/local/lib

all: example

CXXFLAGS += -Wall -std=c++0x lib/mqtt_client.cpp lib/mqtt_client_config.cpp lib/jsoncpp.cpp
CPPFLAGS += -I.. -I$(PAHO_C_LIB)/src

ifdef DEBUG
  CPPFLAGS += -DDEBUG
  CXXFLAGS += -g -O0
else
  CPPFLAGS += -D_NDEBUG
  CXXFLAGS += -O2
endif

LDLIBS += -L../../lib -L$(PAHO_C_LIB) -lmqttpp -lpaho-mqtt3as

.PHONY: clean
clean:
	rm -f example

.PHONY: distclean
distclean: clean
