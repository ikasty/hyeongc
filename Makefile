OBJS = hyeong.o parser.o parser_state.o token.o interpreter.o debug.o value.o
TARGET = hyeong

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

hyeong.o: hyeong.c parser.h interpreter.h debug.h
parser.o: parser.c code.h token.h parser.h parser_state.h
parser_state.o: parser_state.c parser_state.h token.h parser.h
token.o: token.c token.h 
interpreter.o: interpreter.c interpreter.h code.h debug.h
value.o: value.c value.h
debug.o: debug.c debug.h
