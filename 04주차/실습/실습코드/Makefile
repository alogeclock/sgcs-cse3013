.SUFFIXES : .cpp .o # .cpp, .o 확장자 파일들이 확장자 규칙에 따라 처리되도록 함
CXX = g++  # 컴파일러를 g++로 사용
CPPFLAGS = -g # 디버깅이 가능하도록 컴파일

HEDRS = LinkedList.h Stack.h # header 모음
SRCS = main.cpp # main cpp file
OBJS = $(SRCS:.cpp=.o)

TARGET = a.out

all : $(TARGET)

$(TARGET) : $(OBJS) $(HDRS)
	$(CXX) -o $@ $(OBJS)

clean :
	rm -rf $(OBJS) $(TARGET)	
