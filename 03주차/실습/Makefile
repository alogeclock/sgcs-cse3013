cc = g++ # c++로 컴파일
cflags = -W -g # 오류 출력하도록 하고, 디버깅 가능하게 함
target = main
objects = main.o Array.o RangeArray.o

$(target): $(objects) Array.h RangeArray.h # main 파일 디버깅 가능하도록 생성
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.cpp Array.h RangeArray.h # 두 헤더 파일에 의존하는 모든 목적 파일을 차례차례 생성
	$(cc) $(cflags) -c -o $@ $<

.PHONY: clean # PHONY 파일 가상화
clean: # 생성한 실행 가능 목적 파일 및 목적 파일 삭제 명령어
	rm $(objects) $(target)